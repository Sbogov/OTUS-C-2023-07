#include <stdio.h>

void f1() {
    printf("Inside f1\n");
}

void f2() {
    printf("Inside f2\n");
	f1();
}

void f3() {
    printf("Inside f3\n");
}

void f4() {
    printf("Inside f4\n");
}


void f5(void (*func1)(), void (*func2)()) {
    printf("Inside f5 %p, %p\n", func1, func2);
	func1();
	func2();
	printf("f5 end\n");
}

int main() {
    f1();
    f2();
	f3();
	f4();
	f5(&f3, &f4);
    return 0;
}
