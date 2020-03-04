.text
.globl go
go:
pushq %rbx
pushq %rbp
pushq %r12
pushq %r13
pushq %r14
pushq %r15
call _main
popq %r15
popq %r14
popq %r13
popq %r12
popq %rbp
popq %rbx
retq
_main:
_main_entry:

jmp _main_FALSE
_main_TRUE:
movq $3, %rdi
call print
retq
_main_FALSE:
movq $5, %rdi
call print
retq
