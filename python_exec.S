.intel_syntax noprefix
.section .text
.global _start

_start:
    /* execve("/usr/bin/python3", ["python3", "-c", "<script>"], NULL)
       Всі вказівники обчислюються відносно rip і будуються на стеку
       під час виконання — так само, як у Завданні 1, щоб уникнути
       помилки з "зашитими" абсолютними адресами в .data. */

    lea     rdi, [rip + path]
    lea     r10, [rip + arg0]
    lea     r11, [rip + arg1]
    lea     r12, [rip + arg2]

    xor     rax, rax
    push    rax                 /* argv[3] = NULL */
    push    r12                 /* argv[2] = script */
    push    r11                 /* argv[1] = "-c" */
    push    r10                 /* argv[0] = "python3" */
    mov     rsi, rsp            /* argv */

    xor     rdx, rdx            /* envp = NULL */
    xor     rax, rax
    mov     al, 59              /* execve */
    syscall

    /* fallback: exit(1), якщо execve не вдався */
    mov     rax, 60
    mov     rdi, 1
    syscall

.section .data
path:
    .asciz "/usr/bin/python3"
arg0:
    .asciz "python3"
arg1:
    .asciz "-c"
arg2:
    .asciz "print('Hello from shellcode-launched Python! No files were created on disk.')"