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
subq $24, %rsp
movq $5, %r10
movq %r10, 8(%rsp)
movq  8(%rsp), %r10
salq $1, %r10
movq %r10, 8(%rsp)
movq  8(%rsp), %r10
addq  $1, %r10
movq %r10, 8(%rsp)
movq  8(%rsp), %r10
movq %r10, %rdi
movq $3, %rsi
call allocate
movq %rax, %r10
movq %r10, 16(%rsp)
movq  16(%rsp), %r10
movq %r10, %r10
addq  $8, %r10
movq $5, 0(%r10)
movq  16(%rsp), %r10
movq %r10, %r10
addq  $16, %r10
movq $7, 0(%r10)
movq  16(%rsp), %r10
movq %r10, %rdi
call print
movq  8(%rsp), %r10
movq %r10, %rdi
movq $3, %rsi
call allocate
movq %rax, %r10
movq %r10, 0(%rsp)
movq $_main_ret3, -8(%rsp)
movq  0(%rsp), %r10
movq %r10, %rdi
movq $3, %rsi
movq $1, %rdx
subq $8, %rsp
jmp _storeValueToArray
_main_ret3:
movq  0(%rsp), %r10
movq %r10, %rdi
call print
movq $_main_ret5, -8(%rsp)
movq  16(%rsp), %r10
movq %r10, %rdi
movq  0(%rsp), %r10
movq %r10, %rsi
subq $8, %rsp
jmp _isArraySame
_main_ret5:
movq %rax, %r10
movq $_main_ret6, -8(%rsp)
movq %r10, %rdi
subq $8, %rsp
jmp _encode
_main_ret6:
movq %rax, %r10
movq %r10, %rdi
call print
addq $24, %rsp
retq
_isAddress:
movq %rdi, %r10
movq %r10, %r10
andq  $1, %r10
cmpq $0, %r10
sete %r10b
movzbq %r10b, %r10
cmpq $1, %r10
je _isAddress_isAddress
jmp _isAddress_isNumber
_isAddress_isAddress:
movq $1, %rax
retq
_isAddress_isNumber:
movq $0, %rax
retq
_encode:
movq %rdi, %r10
_encode_entry:
salq $1, %r10
addq  $1, %r10
movq %r10, %rax
retq
_increaseOne:
movq %rdi, %r10
movq %r10, %r10
addq  $1, %r10
movq %r10, %rax
retq
_isEqual:
movq %rdi, %r11
movq %rsi, %r10
cmpq %r10, %r11
sete %r10b
movzbq %r10b, %r10
cmpq $1, %r10
je _isEqual_equal
jmp _isEqual_not_equal
_isEqual_equal:
movq $1, %rax
retq
_isEqual_not_equal:
movq $0, %rax
retq
_getSamller:
subq $16, %rsp
movq %rdi, %r10
movq %r10, 0(%rsp)
movq %rsi, %r10
movq %r10, 8(%rsp)
movq $_getSamller_ret0, -8(%rsp)
movq  0(%rsp), %r10
movq %r10, %rdi
movq  8(%rsp), %r10
movq %r10, %rsi
subq $8, %rsp
jmp _isSmallerAndEqual
_getSamller_ret0:
movq %rax, %r10
cmpq $1, %r10
je _getSamller_p1_smaller
jmp _getSamller_p2_smaller
_getSamller_p1_smaller:
movq  0(%rsp), %r10
movq %r10, %rax
addq $16, %rsp
retq
_getSamller_p2_smaller:
movq  8(%rsp), %r10
movq %r10, %rax
addq $16, %rsp
retq
_isSmaller:
movq %rdi, %r11
movq %rsi, %r10
cmpq $1, %r11
setl %r10b
movzbq %r10b, %r10
cmpq $1, %r10
je _isSmaller_smaller
jmp _isSmaller_larger
_isSmaller_smaller:
movq $1, %rax
retq
_isSmaller_larger:
movq $0, %rax
retq
_isSmallerAndEqual:
movq %rdi, %r11
movq %rsi, %r10
cmpq $1, %r11
setle %r10b
movzbq %r10b, %r10
cmpq $1, %r10
je _isSmallerAndEqual_smaller
jmp _isSmallerAndEqual_larger
_isSmallerAndEqual_smaller:
movq $1, %rax
retq
_isSmallerAndEqual_larger:
movq $0, %rax
retq
_getArrayElementByIndex:
subq $8, %rsp
movq %rdi, %r10
movq %r10, 0(%rsp)
movq %rsi, %r10
movq $_getArrayElementByIndex_ret0, -8(%rsp)
movq %r10, %rdi
subq $8, %rsp
jmp _increaseOne
_getArrayElementByIndex_ret0:
movq %rax, %r10
movq $8, %r11
imulq  %r10, %r11
movq  0(%rsp), %r10
movq %r10, %r10
addq  %r11, %r10
movq  0(%r10), %r10
movq %r10, %rax
addq $8, %rsp
retq
_storeValueToArray:
subq $40, %rsp
movq %rdi, %r10
movq %r10, 16(%rsp)
movq %rsi, %r10
movq %r10, 24(%rsp)
movq %rdx, %r10
movq %r10, 8(%rsp)
movq $_storeValueToArray_ret0, -8(%rsp)
movq  16(%rsp), %r10
movq %r10, %rdi
subq $8, %rsp
jmp _isAddress
_storeValueToArray_ret0:
movq %rax, %r10
cmpq $1, %r10
je _storeValueToArray_is_array_label
jmp _storeValueToArray_end_label
_storeValueToArray_is_array_label:
movq  16(%rsp), %r10
movq  0(%r10), %r10
movq %r10, 32(%rsp)
movq $_storeValueToArray_ret1, -8(%rsp)
movq $3, %rdi
movq  32(%rsp), %r10
movq %r10, %rsi
subq $8, %rsp
jmp _getSamller
_storeValueToArray_ret1:
movq %rax, %r10
movq %r10, 32(%rsp)
_storeValueToArray_loop_start:
movq  8(%rsp), %r10
movq  32(%rsp), %r11
cmpq %r11, %r10
setl %r10b
movzbq %r10b, %r10
cmpq $1, %r10
je _storeValueToArray_loop_next
jmp _storeValueToArray_end_label
_storeValueToArray_loop_next:
movq $_storeValueToArray_ret2, -8(%rsp)
movq  8(%rsp), %r10
movq %r10, %rdi
subq $8, %rsp
jmp _increaseOne
_storeValueToArray_ret2:
movq %rax, %r10
movq $8, %r11
imulq  %r10, %r11
movq  16(%rsp), %r10
movq %r10, %r10
movq %r10, 0(%rsp)
movq  0(%rsp), %r10
addq  %r11, %r10
movq %r10, 0(%rsp)
movq  8(%rsp), %r10
movq  24(%rsp), %r11
addq  %r10, %r11
movq %r11, 24(%rsp)
movq $_storeValueToArray_ret3, -8(%rsp)
movq  24(%rsp), %r10
movq %r10, %rdi
subq $8, %rsp
jmp _encode
_storeValueToArray_ret3:
movq %rax, %r10
movq  0(%rsp), %r11
movq %r10, 0(%r11)
movq  8(%rsp), %r10
addq  $1, %r10
movq %r10, 8(%rsp)
jmp _storeValueToArray_loop_start
_storeValueToArray_end_label:
addq $40, %rsp
retq
_isArraySame:
subq $48, %rsp
movq %rdi, %r10
movq %r10, 40(%rsp)
movq %rsi, %r10
movq %r10, 32(%rsp)
movq $_isArraySame_ret0, -8(%rsp)
movq  40(%rsp), %r10
movq %r10, %rdi
subq $8, %rsp
jmp _isAddress
_isArraySame_ret0:
movq %rax, %r10
movq %r10, 0(%rsp)
movq $_isArraySame_ret1, -8(%rsp)
movq  32(%rsp), %r10
movq %r10, %rdi
subq $8, %rsp
jmp _isAddress
_isArraySame_ret1:
movq %rax, %r10
movq  0(%rsp), %r10
cmpq $1, %r10
je _isArraySame_go_ahead_label
jmp _isArraySame_end_label
cmpq $1, %r10
je _isArraySame_go_ahead_label
jmp _isArraySame_end_label
_isArraySame_end_label:
movq $0, %rax
addq $48, %rsp
retq
_isArraySame_go_ahead_label:
movq  40(%rsp), %r10
movq  0(%r10), %r10
movq %r10, 24(%rsp)
movq  32(%rsp), %r10
movq  0(%r10), %r11
movq $_isArraySame_ret2, -8(%rsp)
movq  24(%rsp), %r10
movq %r10, %rdi
movq %r11, %rsi
subq $8, %rsp
jmp _isEqual
_isArraySame_ret2:
movq %rax, %r10
cmpq $1, %r10
je _isArraySame_chek_elements_label
jmp _isArraySame_end_label
_isArraySame_chek_elements_label:
movq $0, %r10
movq %r10, 16(%rsp)
_isArraySame_loop_start:
movq  16(%rsp), %r10
movq  24(%rsp), %r11
cmpq %r11, %r10
setl %r10b
movzbq %r10b, %r10
cmpq $1, %r10
je _isArraySame_loop_next
jmp _isArraySame_loop_end
_isArraySame_loop_next:
movq $_isArraySame_ret3, -8(%rsp)
movq  40(%rsp), %r10
movq %r10, %rdi
movq  16(%rsp), %r10
movq %r10, %rsi
subq $8, %rsp
jmp _getArrayElementByIndex
_isArraySame_ret3:
movq %rax, %r10
movq %r10, 8(%rsp)
movq $_isArraySame_ret4, -8(%rsp)
movq  32(%rsp), %r10
movq %r10, %rdi
movq  16(%rsp), %r10
movq %r10, %rsi
subq $8, %rsp
jmp _getArrayElementByIndex
_isArraySame_ret4:
movq %rax, %r11
movq $_isArraySame_ret5, -8(%rsp)
movq  8(%rsp), %r10
movq %r10, %rdi
movq %r11, %rsi
subq $8, %rsp
jmp _isEqual
_isArraySame_ret5:
movq %rax, %r10
cmpq $1, %r10
je _isArraySame_continue_checking_label
jmp _isArraySame_end_label
_isArraySame_continue_checking_label:
movq  16(%rsp), %r10
addq  $1, %r10
movq %r10, 16(%rsp)
jmp _isArraySame_loop_start
_isArraySame_loop_end:
movq $1, %rax
addq $48, %rsp
retq
