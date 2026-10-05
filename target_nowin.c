#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* win() ВИДАЛЕНО — треба отримати шелл через ret2libc і ret2shellcode.
   Вразливий код винесено в окрему функцію vuln(), як у звіті
   (скрипти звертаються до elf.symbols['vuln']). */

void xjlokjlx() { puts("Kitty says xjlokjlx!"); }
void cimbtsgm() { puts("Kitty says cimbtsgm!"); }
void oodbuxxt() { puts("Kitty says oodbuxxt!"); }
void imauzenj() { puts("Kitty says imauzenj!"); }

void vuln() {
    int pwd[16] = { 0 };
    char buf[16] = { 0 };

    gets(buf);
    if(pwd[0] != 1337)
        exit(1);
    else
        puts("ACCESS GRANTED!");
}

int main() {
    vuln();
    return 0;
}