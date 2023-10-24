#ifndef __SETUP_BRK__
#define __SETUP_BRK__

extern void *brkv;

void setup_brk();

void *dismiss_brk(void **ret);

void *memory_alloc(unsigned long bytes);

unsigned long int memory_free(void *pointer);

#endif
