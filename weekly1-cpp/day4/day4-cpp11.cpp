#include <stdio.h>
#include <stddef.h>

struct B {
    double d;
    char c;
    short s;
};

int main() {
    printf("Sizeof(struct B) = %zu\n", sizeof(struct B));
    printf("Offset of d: %zu\n", offsetof(struct B, d));
    printf("Offset of c: %zu\n", offsetof(struct B, c));
    printf("Offset of s: %zu\n", offsetof(struct B, s));
    return 0;
}
/*
Sizeof(struct B) = 16
Offset of d: 0
Offset of c: 8
Offset of s: 10
*/

111111
22222
33333
