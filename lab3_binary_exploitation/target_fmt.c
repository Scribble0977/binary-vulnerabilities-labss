#include <stdio.h>

int main() {
    char buf[256];

    while (1) {
        gets(buf);
        printf(buf);
        fflush(stdout);
    }
}
