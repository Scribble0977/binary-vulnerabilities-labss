#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    const char *fn = "download_exec.bin";
    if (argc > 1) fn = argv[1];

    int fd = open(fn, O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    struct stat st;
    if (fstat(fd, &st) < 0) { perror("fstat"); close(fd); return 1; }
    size_t size = (size_t)st.st_size;

    void *mem = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    read(fd, mem, size);
    close(fd);

    printf("[+] loaded %zu bytes from %s -> %p\n", size, fn, mem);
    __builtin___clear_cache((char*)mem, (char*)mem + size);

    void (*sc)() = mem;
    sc();
    return 0;
}