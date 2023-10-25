#include "main.h"

#define PTR_NUM 10
#define R_SIZE       56
#define R_SIZE_TEXT "56"

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
	void *p[PTR_NUM], *q, *r, *nb, *s, *big, *t, *brki;
	int i;
	setup_brk();
	brki = brkv;
	printf("brk inicial: %p\n", brkv);
	printf("Alocando um vetor de ponteiros, onde cada ponteiro aponta para um bloco de tamanho 10(i+1), onde i é o índice do ponteiro no vetor.\n");
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

	printf("Alocando "R_SIZE_TEXT" bytes.\n");
	r = memory_alloc(R_SIZE);
	printHeapBlockInfo(r);
	printf("Alocou no mesmo lugar de p[%d]? %s\n", PTR_NUM-1, p[PTR_NUM-1] == r ? "Sim, está correto." : "Não, está errado.");
	printf("O bloco deve conter "R_SIZE_TEXT" bytes, porque é possível quebrar em dois blocos. %s\n\n", getBlockSize(r) == R_SIZE ? "Correto" : "Errado");

	nb = r + R_SIZE + 16;
	printf("Novo bloco livre:\n");
	printHeapBlockInfo(nb);
	printf("O novo bloco vazio deve conter %u bytes. Que seria %u - "R_SIZE_TEXT" - 16 ((tamanho do bloco antigo completo) - (tamanho do novo bloco utilizado) - (tamanho do registro do bloco)).\n%s\n", 10*i - R_SIZE - 16, 10*i, getBlockSize(nb) == 10*i - R_SIZE - 16 ? "Correto" : "Errado");
	printf("O novo bloco não deve estar ocupado. %s\n\n", blockAvailable(nb) ? "Correto" : "Errado");
	printf("brkv: %p\n", brkv);

	printf("Liberando p[%d].\n", PTR_NUM-2);
	memory_free(p[PTR_NUM-2]);
	printf("Alocando 30 bytes. Que cabem no último bloco livre e no penúltimo bloco livre.\n");
	s = memory_alloc(30);
	printHeapBlockInfo(s);
	printf("Bloco deve estar alocado na mesma posição de p[%d] (penúltimo bloco livre/first-fit). %s\n\n", PTR_NUM-2, 
	s == p[PTR_NUM-2] ? "Correto" : "Errado");
	
	// Libera toda a memória alocada dinamicamente (q, r, s estão inclusos no vetor p e nb é um bloco livre)
	printf("Liberando toda a memória utilizada no momento.\n");
	for (i = 0; i < PTR_NUM - 2; i++) {
		memory_free(p[i]);
	}
	
	// printf("Alocando 1GB.\n");
	// big = memory_alloc(1*1024*1024*1024); // 1GB
	// printHeapBlockInfo(big);
	// printf("Bloco deve ter 1GB de tamanho. %s\n", getBlockSize(big) == 1*1024*1024*1024 ? "Correto" : "Errado");
	// printf("Liberando 1GB.\n\n");
	// memory_free(big);

	printf("Alocando 200 bytes.\n");
	t = memory_alloc(200);
	printHeapBlockInfo(t);
	printf("O bloco está depois de p[%d]? %s\n", PTR_NUM-1, t > p[PTR_NUM-1] ? "Sim, isso acontece porque o alocador não junta dois blocos vazios em sequência, ou seja, os blocos podem diminuir de tamanho, mas nunca aumentar." : "NÃO? Algo deve ter dado muito errado.\n");
	printf("brk atual: %p\nRestaurando brk.\n", brkv);
	printf("brki: %p\n", brki);
	printf("&heap_start: %p\n", &brkv - 8);
	dismiss_brk();
	//printf("AAAAAA\n");
	//printf("%p\n", brki);
	// printf("brk atual: %p\n", brkv);
	// printf("brk atual: %p\nbrk inicial: %p\nAmbos devem ser iguais. %s\n", brkv, brki, brkv == brki ? "Correto" : "Errado");

	return 0;
}
