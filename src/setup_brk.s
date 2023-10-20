.section .data
	heap_start: .quad 0
	brkv: .quad 0

.section .text
.global setup_brk
.type setup_brk, @function
.global dismiss_brk
.type dismiss_brk, @function
.global memory_alloc
.type memory_alloc, @function
.global brkv

setup_brk:
	movq $12, %rax			# syscall brk
	movq $0, %rdi			# %rdi = 0 para pegar o brk atual
	syscall
	movq %rax, heap_start
	movq %rax, brkv
	ret

dismiss_brk:
	movq $12, %rax			# syscall brk
	movq heap_start, %rdi 	# %rdi = valor inicial de brk
	syscall					# restaura o valor de brk para o valor inicial
	ret

# void *memory_alloc(unsigned int bytes);
# bytes é um parâmetro passado como argumento no registrador %rdi
memory_alloc:
	movq heap_start, %r8	# %r8 = heap_start
	movq brkv, %r9			# %r9 = brkv
	movq $0, %r10			# %r10 = NULL
	# Retorna o endereço do registro primeiro bloco com tamanho suficiente para a 
	# alocação no registrador %r10. Caso %r10 seja NULL, nenhum bloco disponível é 
	# grande o suficiente.
	_first_fit_i:
		cmp %r8, %r9		# if curr_blk >= brkv -> endloop
		jge _first_fit_e
		cmpq $1, (%r8)		# if blk_unavailable -> next iter
		je _next_block
		cmpq %rdi, 8(%r8)	# if blk_size < bytes -> next iter
		jg _next_block
		movq %r8, %r10		
		jmp _first_fit_e
		_next_block:
		addq 8(%r8), %r8 	# curr_blk += blk_size
		jmp _first_fit_i
	_first_fit_e:
	cmp $0, %r10			# if %r10 == NULL
	je _create_new_blk	
	movq $1, (%r10)			# marca o bloco como usado
	movq 8(%r10), %r8		# %r8 = blk_size
	subq %rdi, %r8			# %r8 = blk_size - bytes
	cmp $16, %r8
	jge _skip_blk_break
	movq %rdi, %rsi			# %rsi = bytes
	movq brkv, %rdx			# %rdx = brkv
	addq %rdx, %rdi			# %rdi = brkv + bytes
