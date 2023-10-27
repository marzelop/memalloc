.section .data
	heap_start: .quad 1
	brkv: .quad 1

.section .text
.global setup_brk
.type setup_brk, @function
.global dismiss_brk
.type dismiss_brk, @function
.global memory_alloc
.type memory_alloc, @function
.global memory_free
.type memory_free, @function
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
	movq %rdi, brkv
	syscall					# restaura o valor de brk para o valor inicial
	ret

# void *memory_alloc(unsigned int bytes);
# bytes é um parâmetro passado como argumento no registrador %rdi
memory_alloc:
	cmpq $0, %rdi			# if bytes == 0 -> return NULL
	jne _non_zero_bytes
	movq $0, %rax
	ret
	_non_zero_bytes:
	movq heap_start, %r8	# %r8 = heap_start
	movq brkv, %r9			# %r9 = brkv
	movq $0, %r10			# %r10 = NULL
	# Retorna o endereço do registro primeiro bloco com tamanho suficiente para a 
	# alocação no registrador %r10. Caso %r10 seja NULL, nenhum bloco disponível é 
	# grande o suficiente.
	_first_fit_i:
		cmp %r8, %r9		# if curr_blk >= brkv -> endloop
		jle _first_fit_e
		cmpq $1, (%r8)		# if blk_unavailable -> next iter
		je _next_block
		cmpq %rdi, 8(%r8)	# if blk_size < bytes -> next iter
		jl _next_block
		movq %r8, %r10		
		jmp _first_fit_e
		_next_block:
		addq 8(%r8), %r8 	# curr_blk += blk_size
		addq $16, %r8		# curr_blk += reg_size
		jmp _first_fit_i
	_first_fit_e:
	cmp $0, %r10			# if %r10 == NULL
	je _create_new_blk	
	movq $1, (%r10)			# marca o bloco como usado
	movq 8(%r10), %r8		# %r8 = blk_size
	subq %rdi, %r8			# %r8 = blk_size - bytes
	cmp $16, %r8			# if blk_size - bytes <= 16 -> don't break block
	jle _skip_blk_break
	movq %rdi, 8(%r10)		# blk_size = bytes
	movq %r10, %r9			# %r9 = blk_reg
	addq $16, %r9			# %r9 += 16
	addq %rdi, %r9			# %r9 += bytes -> %r11 = new_blk_reg
	movq $0, 0(%r9)			# Marca o novo bloco como disponível
	subq $16, %r8			# subtrai o tamanho de um registro em %r8
	movq %r8, 8(%r9)		# new_blk_size = blk_size - bytes - 16
	_skip_blk_break:
	movq %r10, %rax			# %rax = new_blk_reg
	addq $16, %rax			# %rax += reg_size -> new_blk
	ret
	_create_new_blk:
	movq %rdi, %r8			# %r8 = bytes
	addq $16, %r9			# %rdi += reg_size (16)
	addq %r9, %rdi			# %rdi = brkv + bytes + reg_size -> %rdi = new_brkv
	movq $12, %rax			# syscall brk
	syscall
	cmp %rdi, %rax			# if brk fail -> handle error
	jne _new_blk_fail
	movq %rdi, brkv
	movq %rdi, %rax			# %rax = brkv + bytes + reg_size
	subq $16, %rax			# %rax = brkv + bytes
	subq %r8, %rax			# %rax = brkv
	movq $1, (%rax)			# Marca novo bloco como usado
	movq %r8, 8(%rax)		# Atribui o valor do tamanho do bloco
	addq $16, %rax			# %rax = brkv + 16 -> new_blk
	ret
	_new_blk_fail:
	movq $0, %rax			# %rax = NULL
	ret

# Marca um bloco como livre
# Recebe o endereço do bloco que deve ser liberado em %rdi
memory_free:
	cmp $0, %rdi			# if addr == NULL -> ret 0
	je _free_null
	movq $0, -16(%rdi)		# marca o bloco como disponível
	_free_null:
	movq $0, %rax			# retorna 0
	ret
