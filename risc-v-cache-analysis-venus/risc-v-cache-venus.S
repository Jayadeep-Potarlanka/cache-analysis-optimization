#define size 1024

.data
    size_val: .word size
    a: .space size
.text
    #.globl main
main:
    # initialize registers
    li  t0, 10      # reps=10
    li  t1, 2       # step=2
    li  t2, 0       # option=1
    li  t3, 0       # i=0
    la  t5, a       # load address of array a into t5
    lw  t6, size_val # load size of array a into t6

loop_outer:
    # check loop condition for reps
    bge   t3, t0, exit_outer

    # initialize j
    li  t4, 0       # j=0

loop_inner:
    # check loop condition for j
    bge t4, t6, exit_inner

    # check option
    beqz t2, else   # option == 0, set a[0] to 0
    lw   s8, 0(t5)
    add  s7, t4, t4
    add s7, s7, s7
    add  s7, s7, t5
    addi s8, s8, 1
    sw   s8, 0(s7)
    j    continue_loop

else:              # option != 0, increment a[j]
    add  s7, t4, t4
    add s7, s7, s7
    add  s7, s7, t5
    sw   x0, 0(s7)

continue_loop:
    # increment j by step
    add    t4, t4, t1

    j   loop_inner

exit_inner:
    # increment i
    addi    t3, t3, 1

    j   loop_outer

exit_outer:
    # return 1
    li  a0, 1
    ecall