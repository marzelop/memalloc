#include "main.h"

#define PTR_NUM 10



int blockAvailable(void *p) {
	return ((long*) p)[-2] == 0;
}

unsigned long getBlockSize(void *p) {
	return ((unsigned long*) p)[-1];
}

void printHeapBlockInfo(void *p) {
	printf("BLK: %p\nUsed: %s\nSize: %lu\n", p, !blockAvailable(p) ? "yes" : "no", getBlockSize(p));
}

int main() {
	void *p[PTR_NUM], *q, *r;
	int i;
	setup_brk();
	printf("brk: %p\n", brkv);
	p[0] = memory_alloc(10);
	for (i = 1; i < PTR_NUM; i++) {
		p[i] = memory_alloc(10*(i + 1));
		printHeapBlockInfo(p[i - 1]);
		printf("Distancia até o próximo bloco: %lu\nEsperado: %lu\n%s\n\n", p[i] - p[i - 1], (unsigned long) 10*i + 16, 
		p[i] - p[i - 1] == (unsigned long) 10*i + 16 ? "Correto" : "Errado");
	}
	printHeapBlockInfo(p[i - 1]);
	memory_free(p[0]);
	memory_free(p[PTR_NUM-1]);
	printf("\n");

	printf("Liberado p[0] e p[%d].\n", PTR_NUM-1);
	printHeapBlockInfo(p[0]);
	printf("\n");
	printHeapBlockInfo(p[PTR_NUM-1]);
	printf("Ambos os blocos estão disponíveis? %s\n\n", blockAvailable(p[0]) && blockAvailable(p[PTR_NUM-1]) ? "Sim, está correto." : "Não, está errado.");


	printf("Alocando 5 bytes.\n");
	q = memory_alloc(5);
	printHeapBlockInfo(q);
	printf("Alocou no mesmo lugar de p[0]? %s\n", p[0] == q ? "Sim, está correto." : "Não, está errado.");
	printf("O bloco deve conter 10 bytes, porque não é possível quebrar em dois blocos. %s\n\n", getBlockSize(q) == 10 ? "Correto" : "Errado");

	printf("Alocando 50 bytes.\n");
	r = memory_alloc(50);
	printHeapBlockInfo(r);
	printf("Alocou no mesmo lugar de p[%d]? %s\n", PTR_NUM-1, p[PTR_NUM-1] == r ? "Sim, está correto." : "Não, está errado.");
	printf("O bloco deve conter 50 bytes, porque é possível quebrar em dois blocos. %s\n\n", getBlockSize(r) == 50 ? "Correto" : "Errado");

	return 0;
}
