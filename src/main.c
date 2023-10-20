#include "main.h"

int main() {
	long *p;
	long used;
	long size;
	setup_brk();
	printf("%p\n", brkv);
	printf("%d\n", p = memory_alloc(8));
	*p = 4;
	used = p[-2];
	size = p[-1];
	printf("%ld %ld %ld\n", used, size, *p);

	printf("%p %p\n", p, brkv);
	
	return 0;
}
