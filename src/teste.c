#include "teste.h"

#define PTR_NUM 16
#define R_SIZE       64
#define R_SIZE_TEXT "64"

int blockAvailable(void *p) {
	return ((long*) p)[-2] == 0; 
}

unsigned long getBlockSize(void *p) {
	return ((unsigned long*) p)[-1];
}

void printHeapBlockInfo(void *p) {
	printf("BLK: %p\nUsed: %s\nSize: %p\n", p, !blockAvailable(p) ? "yes" : "no", getBlockSize(p));
}

int main() {
	void *p[PTR_NUM], *q, *r, *nb, *s, *t, *brki;
	int i;
	setup_brk();
	brki = brkv;
	printf("brk inicial: %p\n", brkv);
	printf("Alocando um vetor de ponteiros, onde cada ponteiro aponta para um bloco de tamanho 16(i+1), onde i é o índice do ponteiro no vetor.\n");
	p[0] = memory_alloc(16);
	for (i = 1; i < PTR_NUM; i++) {
		p[i] = memory_alloc(16*(i + 1));
		printHeapBlockInfo(p[i - 1]);
		printf("Distancia até o próximo bloco: %lu\nEsperado: %lu\n%s\n\n", p[i] - p[i - 1], (unsigned long) 16*i + 16, 
		p[i] - p[i - 1] == (unsigned long) 16*i + 16 ? "Correto" : "Errado");
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

	printf("Alocando "R_SIZE_TEXT" bytes.\n");
	r = memory_alloc(R_SIZE);
	printHeapBlockInfo(r);
	printf("Alocou no mesmo lugar de p[%d]? %s\n", PTR_NUM-1, p[PTR_NUM-1] == r ? "Sim, está correto." : "Não, está errado.");
	printf("O bloco deve conter "R_SIZE_TEXT" bytes, porque é possível quebrar em dois blocos. %s\n\n", getBlockSize(r) == R_SIZE ? "Correto" : "Errado");

	nb = r + R_SIZE + 16;
	printf("Novo bloco livre:\n");
	printHeapBlockInfo(nb);
	printf("O novo bloco vazio deve conter %u bytes. Que seria %u - "R_SIZE_TEXT" - 16 ((tamanho do bloco antigo completo) - (tamanho do novo bloco utilizado) - (tamanho do registro do bloco)).\n%s\n", 16*i - R_SIZE - 16, 16*i, getBlockSize(nb) == 16*i - R_SIZE - 16 ? "Correto" : "Errado");
	printf("O novo bloco não deve estar ocupado. %s\n\n", blockAvailable(nb) ? "Correto" : "Errado");
	printf("brkv: %p\n", brkv);

	
	// Libera toda a memória alocada dinamicamente (q, r, s estão inclusos no vetor p e nb é um bloco livre)
	printf("Liberando toda a memória utilizada no momento.\n");
	for (i = 0; i < PTR_NUM - 2; i++) {
		memory_free(p[i]);
	}

	return 0;
}
