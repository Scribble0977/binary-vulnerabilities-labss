#!/usr/bin/env python3
import socket
import sys

fname = sys.argv[1] if len(sys.argv) > 1 else "hello_payload"
PORT = 7993

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind(("0.0.0.0", PORT))
s.listen(1)
print(f"[server] waiting on 0.0.0.0:{PORT} ...")
conn, addr = s.accept()
with open(fname, "rb") as f:
    data = f.read()
conn.sendall(data)
conn.shutdown(socket.SHUT_WR)
conn.close()
print(f"[server] sent {len(data)} bytes")