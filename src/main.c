#include "main.h"

typedef struct heap_block {
	unsigned long used;
	unsigned long size;
	void *blk;
} tHeapBlock;

tHeapBlock getHeapBlock(void *p) { 
	tHeapBlock hb = {
		.used = ((long*) p)[-2],
		.size = ((long*) p)[-1],
		.blk = p
	};

	return hb;
}

void printHeapBlock(tHeapBlock *hb) {
	printf("BLK: %p\nUsed: %s\nSize: %lu\n", hb->blk, hb->used == 1 ? "yes" : "no", hb->size);
}

int main() {
	long *p, *q;
	tHeapBlock hb;
	setup_brk();
	printf("brk: %p\n", brkv);
	p = memory_alloc(10);
	q = memory_alloc(20);
	printf("p: %p\n", p);
	hb = getHeapBlock(p);
	printHeapBlock(&hb);
	// hb = getHeapBlock(q);
	// printHeapBlock(&hb);

	return 0;
}
