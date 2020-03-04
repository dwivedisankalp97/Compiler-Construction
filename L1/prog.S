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
subq $8, %rsp
_main_newLabel0:
movq $21, %r10
movq %r10, 0(%rsp)
jmp _main_NewLabelLB1
_main_NewLabelLB1:
movq  0(%rsp), %r10
movq %r10, %r10
sarq $1, %r10
cmpq $0, %r10
setg %r10b
movzbq %r10b, %r10
salq $1, %r10
addq  $1, %r10
movq %r10, %r10
sarq $1, %r10
cmpq $1, %r10
je _main_new_label0
jmp _main_new_label1
_main_new_label0:
movq  0(%rsp), %r10
movq %r10, %rdi
call print
movq  0(%rsp), %r10
movq %r10, %r10
sarq $1, %r10
movq %r10, %r10
movq %r10, 0(%rsp)
movq  0(%rsp), %r10
subq  $1, %r10
movq %r10, 0(%rsp)
movq  0(%rsp), %r10
salq $1, %r10
movq %r10, 0(%rsp)
movq  0(%rsp), %r10
addq  $1, %r10
movq %r10, 0(%rsp)
movq  0(%rsp), %r10
movq %r10, %r10
sarq $1, %r10
cmpq $2, %r10
setg %r10b
movzbq %r10b, %r10
salq $1, %r10
addq  $1, %r10
movq %r10, %r10
sarq $1, %r10
cmpq $1, %r10
je _main_true_label
jmp _main_false_label
_main_true_label:
jmp _main_new_label1
_main_new_label1:
addq $8, %rsp
retq
_main_false_label:
movq  0(%rsp), %r10
movq %r10, %r10
sarq $1, %r10
movq %r10, %r10
movq %r10, 0(%rsp)
movq  0(%rsp), %r10
subq  $1, %r10
movq %r10, 0(%rsp)
movq  0(%rsp), %r10
salq $1, %r10
movq %r10, 0(%rsp)
movq  0(%rsp), %r10
addq  $1, %r10
movq %r10, 0(%rsp)
jmp _main_NewLabelLB1
