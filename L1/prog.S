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
subq $64, %rsp
_call_label0:
movq $601, %rdi
movq %rdi, 40(%rsp)
movq  40(%rsp), %rdi
movq %rdi, %rdi
sarq $1, %rdi
movq %rdi, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
sarq $1, %rdi
imulq  %rdi, %rsi
salq $1, %rsi
addq  $1, %rsi
addq  $6, %rsi
movq %rsi, %rdi
movq $1, %rsi
call allocate
movq %rax, %rdi
movq %rdi, 56(%rsp)
movq  56(%rsp), %rdi
movq $5, 8(%rdi)
movq  40(%rsp), %rsi
movq  56(%rsp), %rdi
movq %rsi, 16(%rdi)
movq  40(%rsp), %rsi
movq  56(%rsp), %rdi
movq %rsi, 24(%rdi)
movq  40(%rsp), %rdi
movq %rdi, %rdi
sarq $1, %rdi
movq %rdi, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
sarq $1, %rdi
imulq  %rdi, %rsi
salq $1, %rsi
addq  $1, %rsi
addq  $6, %rsi
movq %rsi, %rdi
movq $1, %rsi
call allocate
movq %rax, %rdi
movq %rdi, 48(%rsp)
movq  48(%rsp), %rdi
movq $5, 8(%rdi)
movq  40(%rsp), %rdi
movq  48(%rsp), %rsi
movq %rdi, 16(%rsi)
movq  40(%rsp), %rsi
movq  48(%rsp), %rdi
movq %rsi, 24(%rdi)
movq  40(%rsp), %rdi
movq %rdi, %rdi
sarq $1, %rdi
movq %rdi, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
sarq $1, %rdi
imulq  %rdi, %rsi
salq $1, %rsi
addq  $1, %rsi
addq  $6, %rsi
movq %rsi, %rdi
movq $1, %rsi
call allocate
movq %rax, %rdi
movq %rdi, 32(%rsp)
movq  32(%rsp), %rdi
movq $5, 8(%rdi)
movq  32(%rsp), %rsi
movq  40(%rsp), %rdi
movq %rdi, 16(%rsi)
movq  32(%rsp), %rdi
movq  40(%rsp), %rsi
movq %rsi, 24(%rdi)
movq $9, %rdi
movq %rdi, 8(%rsp)
movq $9, %rdi
movq %rdi, 24(%rsp)
movq $1, %rdi
movq %rdi, 16(%rsp)
jmp _call_label1
_call_label1:
movq  16(%rsp), %rdi
movq %rdi, %rsi
sarq $1, %rsi
movq  8(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rsi
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rsi
sarq $1, %rsi
cmpq $1, %rsi
je _call_label2
jmp _call_label6
_call_label2:
movq $1, %rdi
movq %rdi, 0(%rsp)
jmp _call_label3
_call_label3:
movq  0(%rsp), %rdi
movq %rdi, %rsi
sarq $1, %rsi
movq  24(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rsi
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rsi
sarq $1, %rsi
cmpq $1, %rsi
je _call_label4
jmp _call_label5
_call_label4:
movq  56(%rsp), %rdi
movq %rdi, %rdi
movq  48(%rsp), %rsi
movq %rsi, %rsi
movq  32(%rsp), %rdx
movq %rdx, %rdx
movq  16(%rsp), %rcx
movq %rcx, %rcx
movq  0(%rsp), %r8
movq %r8, %r8
movq $_call_label7, -8(%rsp)
subq $8, %rsp
jmp _computeAndPrint
_call_label7:
movq  0(%rsp), %rdi
movq %rdi, %rsi
sarq $1, %rsi
movq $3, %rdx
sarq $1, %rdx
movq %rsi, %rdi
movq %rdi, 0(%rsp)
movq  0(%rsp), %rdi
addq  %rdx, %rdi
movq %rdi, 0(%rsp)
movq  0(%rsp), %rdi
salq $1, %rdi
movq %rdi, 0(%rsp)
movq  0(%rsp), %rdi
addq  $1, %rdi
movq %rdi, 0(%rsp)
jmp _call_label3
_call_label5:
movq  16(%rsp), %rdi
movq %rdi, %rsi
sarq $1, %rsi
movq $3, %rdx
sarq $1, %rdx
movq %rsi, %rdi
movq %rdi, 16(%rsp)
movq  16(%rsp), %rdi
addq  %rdx, %rdi
movq %rdi, 16(%rsp)
movq  16(%rsp), %rdi
salq $1, %rdi
movq %rdi, 16(%rsp)
movq  16(%rsp), %rdi
addq  $1, %rdi
movq %rdi, 16(%rsp)
jmp _call_label1
_call_label6:
addq $64, %rsp
retq
_computeAndPrint:
subq $32, %rsp
movq %rdi, %rdi
movq %rdi, 8(%rsp)
movq %rsi, %rdi
movq %rdi, 24(%rsp)
movq %rdx, %rdi
movq %rdi, 16(%rsp)
movq %rcx, %rsi
movq %r8, %rdi
movq %rdi, 0(%rsp)
_call_label_ciao_8:
movq  8(%rsp), %rdi
movq %rdi, %rdi
movq %rsi, %rsi
movq $_call_label_ciao_9, -8(%rsp)
subq $8, %rsp
jmp _initMatrix
_call_label_ciao_9:
movq  24(%rsp), %rdi
movq %rdi, %rdi
movq  0(%rsp), %rsi
movq %rsi, %rsi
movq $_call_label_ciao_10, -8(%rsp)
subq $8, %rsp
jmp _initMatrix
_call_label_ciao_10:
movq  8(%rsp), %rdi
movq %rdi, %rdi
movq  24(%rsp), %rsi
movq %rsi, %rsi
movq  16(%rsp), %rdx
movq %rdx, %rdx
movq $_call_label_ciao_11, -8(%rsp)
subq $8, %rsp
jmp _matrixMultiplication
_call_label_ciao_11:
movq  8(%rsp), %rdi
movq %rdi, %rdi
movq $_call_label_ciao_12, -8(%rsp)
subq $8, %rsp
jmp _totalSum
_call_label_ciao_12:
movq %rax, %rdi
movq %rdi, %rdi
call print
movq  24(%rsp), %rdi
movq %rdi, %rdi
movq $_call_label_ciao_13, -8(%rsp)
subq $8, %rsp
jmp _totalSum
_call_label_ciao_13:
movq %rax, %rdi
movq %rdi, %rdi
call print
movq  16(%rsp), %rdi
movq %rdi, %rdi
movq $_call_label_ciao_14, -8(%rsp)
subq $8, %rsp
jmp _totalSum
_call_label_ciao_14:
movq %rax, %rdi
movq %rdi, %rdi
call print
addq $32, %rsp
retq
_initMatrix:
subq $32, %rsp
movq %rdi, %rdi
movq %rdi, 24(%rsp)
movq %rsi, %rdi
movq %rdi, 16(%rsp)
_call_label_ciao__ciao_15:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  24(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
movq %rdi, 8(%rsp)
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  24(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %r11
movq $1, %rdi
movq %rdi, 0(%rsp)
jmp _call_label_ciao__ciao_16
_call_label_ciao__ciao_16:
movq  0(%rsp), %rdi
movq %rdi, %rsi
sarq $1, %rsi
movq  8(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rsi
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rsi
sarq $1, %rsi
cmpq $1, %rsi
je _call_label_ciao__ciao_17
jmp _call_label_ciao__ciao_26
_call_label_ciao__ciao_17:
movq $1, %r10
jmp _call_label_ciao__ciao_18
_call_label_ciao__ciao_18:
movq %r10, %rsi
sarq $1, %rsi
movq %r11, %rdx
sarq $1, %rdx
cmpq %rdx, %rsi
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rsi
sarq $1, %rsi
cmpq $1, %rsi
je _call_label_ciao__ciao_19
jmp _call_label_ciao__ciao_25
_call_label_ciao__ciao_19:
movq  16(%rsp), %rdi
movq %rdi, %rsi
sarq $1, %rsi
movq  0(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
movq %rsi, %r9
addq  %rdx, %r9
salq $1, %r9
addq  $1, %r9
movq %r9, %rsi
sarq $1, %rsi
movq %r10, %rdx
sarq $1, %rdx
movq %rsi, %r9
addq  %rdx, %r9
salq $1, %r9
addq  $1, %r9
movq  0(%rsp), %rdi
movq %rdi, %rcx
sarq $1, %rcx
movq %r10, %r8
sarq $1, %r8
movq $0, %rsi
movq  24(%rsp), %rdi
cmpq $0, %rdi
sete %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao_23
jmp _call_label_ciao__ciao_20
_call_label_ciao__ciao_20:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rdi
addq  $16, %rdi
movq  24(%rsp), %rsi
movq %rsi, %rsi
addq  %rdi, %rsi
movq  0(%rsi), %rdi
sarq $1, %rdi
movq %rcx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rcx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao_21
jmp _call_label_ciao__ciao_23
_call_label_ciao__ciao_21:
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rdi
addq  $16, %rdi
movq  24(%rsp), %rsi
movq %rsi, %rsi
addq  %rdi, %rsi
movq  0(%rsi), %rdi
sarq $1, %rdi
movq %r8, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %r8
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao_22
jmp _call_label_ciao__ciao_23
_call_label_ciao__ciao_22:
jmp _call_label_ciao__ciao_24
_call_label_ciao__ciao_23:
movq  24(%rsp), %rdi
movq %rdi, %rdi
movq %rsi, %rsi
call array_error
jmp _call_label_ciao__ciao_24
_call_label_ciao__ciao_24:
movq $0, %rdx
movq  24(%rsp), %rdi
movq  24(%rdi), %rdi
movq $1, %rsi
movq %rdi, %rdi
sarq $1, %rdi
imulq  %rdi, %rsi
movq %rcx, %rdi
imulq  %rsi, %rdi
addq  %rdi, %rdx
addq  %r8, %rdx
movq %rdx, %rsi
imulq  $8, %rsi
addq  $32, %rsi
movq  24(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq %r9, 0(%rdi)
movq %r10, %rsi
sarq $1, %rsi
movq $3, %rdx
sarq $1, %rdx
movq %rsi, %r10
addq  %rdx, %r10
salq $1, %r10
addq  $1, %r10
jmp _call_label_ciao__ciao_18
_call_label_ciao__ciao_25:
movq  0(%rsp), %rdi
movq %rdi, %rsi
sarq $1, %rsi
movq $3, %rdx
sarq $1, %rdx
movq %rsi, %rdi
movq %rdi, 0(%rsp)
movq  0(%rsp), %rdi
addq  %rdx, %rdi
movq %rdi, 0(%rsp)
movq  0(%rsp), %rdi
salq $1, %rdi
movq %rdi, 0(%rsp)
movq  0(%rsp), %rdi
addq  $1, %rdi
movq %rdi, 0(%rsp)
jmp _call_label_ciao__ciao_16
_call_label_ciao__ciao_26:
addq $32, %rsp
retq
_matrixMultiplication:
subq $64, %rsp
movq %rdi, %rdi
movq %rdi, 56(%rsp)
movq %rsi, %rdi
movq %rdi, 48(%rsp)
movq %rdx, %rdi
movq %rdi, 40(%rsp)
_call_label_ciao__ciao__ciao_27:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  56(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
movq %rdi, 32(%rsp)
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  56(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
movq %rdi, 24(%rsp)
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  48(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdx
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  48(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
movq %rdi, 16(%rsp)
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rsi
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rcx
addq  $16, %rcx
movq  40(%rsp), %rdi
movq %rdi, %rdi
addq  %rcx, %rdi
movq  0(%rdi), %rdi
movq  24(%rsp), %rcx
movq %rcx, %rcx
sarq $1, %rcx
movq %rdx, %rdx
sarq $1, %rdx
cmpq %rdx, %rcx
sete %dl
movzbq %dl, %rdx
salq $1, %rdx
addq  $1, %rdx
movq %rdx, %rcx
sarq $1, %rcx
cmpq $1, %rcx
je _call_label_ciao__ciao__ciao_28
jmp _call_label_ciao__ciao__ciao_73
_call_label_ciao__ciao__ciao_28:
movq %rsi, %rcx
sarq $1, %rcx
movq  32(%rsp), %rsi
movq %rsi, %rdx
sarq $1, %rdx
cmpq %rdx, %rcx
sete %sil
movzbq %sil, %rsi
salq $1, %rsi
addq  $1, %rsi
movq %rsi, %rcx
sarq $1, %rcx
cmpq $1, %rcx
je _call_label_ciao__ciao__ciao_29
jmp _call_label_ciao__ciao__ciao_73
_call_label_ciao__ciao__ciao_29:
movq %rdi, %rcx
sarq $1, %rcx
movq  16(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rcx
sete %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rcx
sarq $1, %rcx
cmpq $1, %rcx
je _call_label_ciao__ciao__ciao_30
jmp _call_label_ciao__ciao__ciao_73
_call_label_ciao__ciao__ciao_30:
movq $1, %r11
jmp _call_label_ciao__ciao__ciao_31
_call_label_ciao__ciao__ciao_31:
movq %r11, %rcx
sarq $1, %rcx
movq  32(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rcx
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rcx
sarq $1, %rcx
cmpq $1, %rcx
je _call_label_ciao__ciao__ciao_32
jmp _call_label_ciao__ciao__ciao_44
_call_label_ciao__ciao__ciao_32:
movq $1, %r10
jmp _call_label_ciao__ciao__ciao_33
_call_label_ciao__ciao__ciao_33:
movq %r10, %rcx
sarq $1, %rcx
movq  16(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rcx
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rcx
sarq $1, %rcx
cmpq $1, %rcx
je _call_label_ciao__ciao__ciao_34
jmp _call_label_ciao__ciao__ciao_43
_call_label_ciao__ciao__ciao_34:
movq $1, %r9
jmp _call_label_ciao__ciao__ciao_35
_call_label_ciao__ciao__ciao_35:
movq %r9, %rcx
sarq $1, %rcx
movq  24(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rcx
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rcx
sarq $1, %rcx
cmpq $1, %rcx
je _call_label_ciao__ciao__ciao_36
jmp _call_label_ciao__ciao__ciao_42
_call_label_ciao__ciao__ciao_36:
movq %r11, %rcx
sarq $1, %rcx
movq %r10, %r8
sarq $1, %r8
movq $0, %rsi
movq  40(%rsp), %rdi
cmpq $0, %rdi
sete %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_40
jmp _call_label_ciao__ciao__ciao_37
_call_label_ciao__ciao__ciao_37:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rdi
addq  $16, %rdi
movq  40(%rsp), %rsi
movq %rsi, %rsi
addq  %rdi, %rsi
movq  0(%rsi), %rdi
sarq $1, %rdi
movq %rcx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rcx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_38
jmp _call_label_ciao__ciao__ciao_40
_call_label_ciao__ciao__ciao_38:
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rdi
addq  $16, %rdi
movq  40(%rsp), %rsi
movq %rsi, %rsi
addq  %rdi, %rsi
movq  0(%rsi), %rdi
sarq $1, %rdi
movq %r8, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %r8
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_39
jmp _call_label_ciao__ciao__ciao_40
_call_label_ciao__ciao__ciao_39:
jmp _call_label_ciao__ciao__ciao_41
_call_label_ciao__ciao__ciao_40:
movq  40(%rsp), %rdi
movq %rdi, %rdi
movq %rsi, %rsi
call array_error
jmp _call_label_ciao__ciao__ciao_41
_call_label_ciao__ciao__ciao_41:
movq $0, %rdx
movq  40(%rsp), %rdi
movq  24(%rdi), %rdi
movq $1, %rsi
movq %rdi, %rdi
sarq $1, %rdi
imulq  %rdi, %rsi
movq %rcx, %rdi
imulq  %rsi, %rdi
addq  %rdi, %rdx
addq  %r8, %rdx
movq %rdx, %rsi
imulq  $8, %rsi
addq  $32, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq $1, 0(%rdi)
movq %r9, %rcx
sarq $1, %rcx
movq $3, %rdx
sarq $1, %rdx
movq %rcx, %r9
addq  %rdx, %r9
salq $1, %r9
addq  $1, %r9
jmp _call_label_ciao__ciao__ciao_35
_call_label_ciao__ciao__ciao_42:
movq %r10, %rcx
sarq $1, %rcx
movq $3, %rdx
sarq $1, %rdx
movq %rcx, %r10
addq  %rdx, %r10
salq $1, %r10
addq  $1, %r10
jmp _call_label_ciao__ciao__ciao_33
_call_label_ciao__ciao__ciao_43:
movq %r11, %rcx
sarq $1, %rcx
movq $3, %rdx
sarq $1, %rdx
movq %rcx, %r11
addq  %rdx, %r11
salq $1, %r11
addq  $1, %r11
jmp _call_label_ciao__ciao__ciao_31
_call_label_ciao__ciao__ciao_44:
movq $1, %rdi
movq %rdi, 8(%rsp)
jmp _call_label_ciao__ciao__ciao_45
_call_label_ciao__ciao__ciao_45:
movq  8(%rsp), %rdi
movq %rdi, %rcx
sarq $1, %rcx
movq  32(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rcx
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rcx
sarq $1, %rcx
cmpq $1, %rcx
je _call_label_ciao__ciao__ciao_46
jmp _call_label_ciao__ciao__ciao_73
_call_label_ciao__ciao__ciao_46:
movq $1, %rdi
movq %rdi, 0(%rsp)
jmp _call_label_ciao__ciao__ciao_47
_call_label_ciao__ciao__ciao_47:
movq  0(%rsp), %rdi
movq %rdi, %rcx
sarq $1, %rcx
movq  16(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rcx
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rcx
sarq $1, %rcx
cmpq $1, %rcx
je _call_label_ciao__ciao__ciao_48
jmp _call_label_ciao__ciao__ciao_72
_call_label_ciao__ciao__ciao_48:
movq $1, %r10
jmp _call_label_ciao__ciao__ciao_49
_call_label_ciao__ciao__ciao_49:
movq %r10, %rcx
sarq $1, %rcx
movq  24(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
cmpq %rdx, %rcx
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rcx
sarq $1, %rcx
cmpq $1, %rcx
je _call_label_ciao__ciao__ciao_50
jmp _call_label_ciao__ciao__ciao_71
_call_label_ciao__ciao__ciao_50:
movq  8(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
movq %r10, %rcx
sarq $1, %rcx
movq $0, %rsi
movq  56(%rsp), %rdi
cmpq $0, %rdi
sete %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_54
jmp _call_label_ciao__ciao__ciao_51
_call_label_ciao__ciao__ciao_51:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  56(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
sarq $1, %rdi
movq %rdx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rdx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_52
jmp _call_label_ciao__ciao__ciao_54
_call_label_ciao__ciao__ciao_52:
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  56(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
sarq $1, %rdi
movq %rcx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rcx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_53
jmp _call_label_ciao__ciao__ciao_54
_call_label_ciao__ciao__ciao_53:
jmp _call_label_ciao__ciao__ciao_55
_call_label_ciao__ciao__ciao_54:
movq  56(%rsp), %rdi
movq %rdi, %rdi
movq %rsi, %rsi
call array_error
jmp _call_label_ciao__ciao__ciao_55
_call_label_ciao__ciao__ciao_55:
movq $0, %rsi
movq  56(%rsp), %rdi
movq  24(%rdi), %r8
movq $1, %rdi
movq %r8, %r8
sarq $1, %r8
imulq  %r8, %rdi
movq %rdx, %rdx
imulq  %rdi, %rdx
addq  %rdx, %rsi
addq  %rcx, %rsi
movq %rsi, %rsi
imulq  $8, %rsi
addq  $32, %rsi
movq  56(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %r8
movq %r10, %rcx
sarq $1, %rcx
movq  0(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
movq $0, %rsi
movq  48(%rsp), %rdi
cmpq $0, %rdi
sete %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_59
jmp _call_label_ciao__ciao__ciao_56
_call_label_ciao__ciao__ciao_56:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  48(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
sarq $1, %rdi
movq %rcx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rcx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_57
jmp _call_label_ciao__ciao__ciao_59
_call_label_ciao__ciao__ciao_57:
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  48(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
sarq $1, %rdi
movq %rdx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rdx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_58
jmp _call_label_ciao__ciao__ciao_59
_call_label_ciao__ciao__ciao_58:
jmp _call_label_ciao__ciao__ciao_60
_call_label_ciao__ciao__ciao_59:
movq  48(%rsp), %rdi
movq %rdi, %rdi
movq %rsi, %rsi
call array_error
jmp _call_label_ciao__ciao__ciao_60
_call_label_ciao__ciao__ciao_60:
movq $0, %rsi
movq  48(%rsp), %rdi
movq  24(%rdi), %r9
movq $1, %rdi
movq %r9, %r9
sarq $1, %r9
imulq  %r9, %rdi
movq %rcx, %rcx
imulq  %rdi, %rcx
addq  %rcx, %rsi
addq  %rdx, %rsi
movq %rsi, %rsi
imulq  $8, %rsi
addq  $32, %rsi
movq  48(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
movq %r8, %rcx
sarq $1, %rcx
movq %rdi, %rdx
sarq $1, %rdx
movq %rcx, %r9
imulq  %rdx, %r9
salq $1, %r9
addq  $1, %r9
movq  8(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
movq  0(%rsp), %rdi
movq %rdi, %rcx
sarq $1, %rcx
movq $0, %rsi
movq  40(%rsp), %rdi
cmpq $0, %rdi
sete %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_64
jmp _call_label_ciao__ciao__ciao_61
_call_label_ciao__ciao__ciao_61:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
sarq $1, %rdi
movq %rdx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rdx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_62
jmp _call_label_ciao__ciao__ciao_64
_call_label_ciao__ciao__ciao_62:
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
sarq $1, %rdi
movq %rcx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rcx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_63
jmp _call_label_ciao__ciao__ciao_64
_call_label_ciao__ciao__ciao_63:
jmp _call_label_ciao__ciao__ciao_65
_call_label_ciao__ciao__ciao_64:
movq  40(%rsp), %rdi
movq %rdi, %rdi
movq %rsi, %rsi
call array_error
jmp _call_label_ciao__ciao__ciao_65
_call_label_ciao__ciao__ciao_65:
movq $0, %rsi
movq  40(%rsp), %rdi
movq  24(%rdi), %r8
movq $1, %rdi
movq %r8, %r8
sarq $1, %r8
imulq  %r8, %rdi
movq %rdx, %rdx
imulq  %rdi, %rdx
addq  %rdx, %rsi
addq  %rcx, %rsi
movq %rsi, %rsi
imulq  $8, %rsi
addq  $32, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %r8
movq %r8, %rcx
sarq $1, %rcx
movq $9, %rdx
sarq $1, %rdx
movq %rcx, %r8
imulq  %rdx, %r8
salq $1, %r8
addq  $1, %r8
movq %r8, %rcx
sarq $1, %rcx
movq %r9, %rdx
sarq $1, %rdx
movq %rcx, %r8
addq  %rdx, %r8
salq $1, %r8
addq  $1, %r8
movq  8(%rsp), %rdi
movq %rdi, %rcx
sarq $1, %rcx
movq  0(%rsp), %rdi
movq %rdi, %rdx
sarq $1, %rdx
movq $0, %rsi
movq  40(%rsp), %rdi
cmpq $0, %rdi
sete %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_69
jmp _call_label_ciao__ciao__ciao_66
_call_label_ciao__ciao__ciao_66:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
sarq $1, %rdi
movq %rcx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rcx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_67
jmp _call_label_ciao__ciao__ciao_69
_call_label_ciao__ciao__ciao_67:
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
sarq $1, %rdi
movq %rdx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rdx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao_68
jmp _call_label_ciao__ciao__ciao_69
_call_label_ciao__ciao__ciao_68:
jmp _call_label_ciao__ciao__ciao_70
_call_label_ciao__ciao__ciao_69:
movq  40(%rsp), %rdi
movq %rdi, %rdi
movq %rsi, %rsi
call array_error
jmp _call_label_ciao__ciao__ciao_70
_call_label_ciao__ciao__ciao_70:
movq $0, %rsi
movq  40(%rsp), %rdi
movq  24(%rdi), %r9
movq $1, %rdi
movq %r9, %r9
sarq $1, %r9
imulq  %r9, %rdi
movq %rcx, %rcx
imulq  %rdi, %rcx
addq  %rcx, %rsi
addq  %rdx, %rsi
movq %rsi, %rsi
imulq  $8, %rsi
addq  $32, %rsi
movq  40(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq %r8, 0(%rdi)
movq %r10, %rcx
sarq $1, %rcx
movq $3, %rdx
sarq $1, %rdx
movq %rcx, %r10
addq  %rdx, %r10
salq $1, %r10
addq  $1, %r10
jmp _call_label_ciao__ciao__ciao_49
_call_label_ciao__ciao__ciao_71:
movq  0(%rsp), %rdi
movq %rdi, %rcx
sarq $1, %rcx
movq $3, %rdx
sarq $1, %rdx
movq %rcx, %rdi
movq %rdi, 0(%rsp)
movq  0(%rsp), %rdi
addq  %rdx, %rdi
movq %rdi, 0(%rsp)
movq  0(%rsp), %rdi
salq $1, %rdi
movq %rdi, 0(%rsp)
movq  0(%rsp), %rdi
addq  $1, %rdi
movq %rdi, 0(%rsp)
jmp _call_label_ciao__ciao__ciao_47
_call_label_ciao__ciao__ciao_72:
movq  8(%rsp), %rdi
movq %rdi, %rcx
sarq $1, %rcx
movq $3, %rdx
sarq $1, %rdx
movq %rcx, %rdi
movq %rdi, 8(%rsp)
movq  8(%rsp), %rdi
addq  %rdx, %rdi
movq %rdi, 8(%rsp)
movq  8(%rsp), %rdi
salq $1, %rdi
movq %rdi, 8(%rsp)
movq  8(%rsp), %rdi
addq  $1, %rdi
movq %rdi, 8(%rsp)
jmp _call_label_ciao__ciao__ciao_45
_call_label_ciao__ciao__ciao_73:
addq $64, %rsp
retq
_totalSum:
subq $16, %rsp
movq %rdi, %rdi
movq %rdi, 8(%rsp)
_call_label_ciao__ciao__ciao__ciao_74:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  8(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
movq %rdi, 0(%rsp)
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rsi
addq  $16, %rsi
movq  8(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %r11
movq $1, %r10
movq $1, %rax
jmp _call_label_ciao__ciao__ciao__ciao_75
_call_label_ciao__ciao__ciao__ciao_75:
movq %r10, %rsi
sarq $1, %rsi
movq  0(%rsp), %rdi
movq %rdi, %rdi
sarq $1, %rdi
cmpq %rdi, %rsi
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rsi
sarq $1, %rsi
cmpq $1, %rsi
je _call_label_ciao__ciao__ciao__ciao_76
jmp _call_label_ciao__ciao__ciao__ciao_85
_call_label_ciao__ciao__ciao__ciao_76:
movq $1, %r9
jmp _call_label_ciao__ciao__ciao__ciao_77
_call_label_ciao__ciao__ciao__ciao_77:
movq %r9, %rsi
sarq $1, %rsi
movq %r11, %rdi
sarq $1, %rdi
cmpq %rdi, %rsi
setl %dil
movzbq %dil, %rdi
salq $1, %rdi
addq  $1, %rdi
movq %rdi, %rsi
sarq $1, %rsi
cmpq $1, %rsi
je _call_label_ciao__ciao__ciao__ciao_78
jmp _call_label_ciao__ciao__ciao__ciao_84
_call_label_ciao__ciao__ciao__ciao_78:
movq %r10, %rcx
sarq $1, %rcx
movq %r9, %r8
sarq $1, %r8
movq $0, %rsi
movq  8(%rsp), %rdi
cmpq $0, %rdi
sete %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao__ciao_82
jmp _call_label_ciao__ciao__ciao__ciao_79
_call_label_ciao__ciao__ciao__ciao_79:
movq $0, %rdi
imulq  $8, %rdi
movq %rdi, %rdi
addq  $16, %rdi
movq  8(%rsp), %rsi
movq %rsi, %rsi
addq  %rdi, %rsi
movq  0(%rsi), %rdi
sarq $1, %rdi
movq %rcx, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %rcx
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao__ciao_80
jmp _call_label_ciao__ciao__ciao__ciao_82
_call_label_ciao__ciao__ciao__ciao_80:
movq $1, %rdi
imulq  $8, %rdi
movq %rdi, %rdi
addq  $16, %rdi
movq  8(%rsp), %rsi
movq %rsi, %rsi
addq  %rdi, %rsi
movq  0(%rsi), %rdi
sarq $1, %rdi
movq %r8, %rsi
salq $1, %rsi
addq  $1, %rsi
cmpq %rdi, %r8
setl %dil
movzbq %dil, %rdi
cmpq $1, %rdi
je _call_label_ciao__ciao__ciao__ciao_81
jmp _call_label_ciao__ciao__ciao__ciao_82
_call_label_ciao__ciao__ciao__ciao_81:
jmp _call_label_ciao__ciao__ciao__ciao_83
_call_label_ciao__ciao__ciao__ciao_82:
movq  8(%rsp), %rdi
movq %rdi, %rdi
movq %rsi, %rsi
call array_error
jmp _call_label_ciao__ciao__ciao__ciao_83
_call_label_ciao__ciao__ciao__ciao_83:
movq $0, %rdx
movq  8(%rsp), %rdi
movq  24(%rdi), %rdi
movq $1, %rsi
movq %rdi, %rdi
sarq $1, %rdi
imulq  %rdi, %rsi
movq %rcx, %rdi
imulq  %rsi, %rdi
addq  %rdi, %rdx
addq  %r8, %rdx
movq %rdx, %rsi
imulq  $8, %rsi
addq  $32, %rsi
movq  8(%rsp), %rdi
movq %rdi, %rdi
addq  %rsi, %rdi
movq  0(%rdi), %rdi
movq %rax, %rsi
sarq $1, %rsi
movq %rdi, %rdi
sarq $1, %rdi
movq %rsi, %rax
addq  %rdi, %rax
salq $1, %rax
addq  $1, %rax
movq %r9, %rsi
sarq $1, %rsi
movq $3, %rdi
sarq $1, %rdi
movq %rsi, %r9
addq  %rdi, %r9
salq $1, %r9
addq  $1, %r9
jmp _call_label_ciao__ciao__ciao__ciao_77
_call_label_ciao__ciao__ciao__ciao_84:
movq %r10, %rsi
sarq $1, %rsi
movq $3, %rdi
sarq $1, %rdi
movq %rsi, %r10
addq  %rdi, %r10
salq $1, %r10
addq  $1, %r10
jmp _call_label_ciao__ciao__ciao__ciao_75
_call_label_ciao__ciao__ciao__ciao_85:
movq %rax, %rax
addq $16, %rsp
retq
