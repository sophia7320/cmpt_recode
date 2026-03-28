#include <stdio.h>
#include <string.h>

struct A {
    char data[60];

    void (*print)(struct A);
};

void __print(struct A a) {
    printf("%s", a.data);
    return;
}

struct A createA() {
    struct A t;
    t.print = __print;
    return t;
}

int main() {
    struct A a = createA();
    strcpy(a.data, "abc");
    a.print(a);

    scanf("%d", &a);
    return 0;
}