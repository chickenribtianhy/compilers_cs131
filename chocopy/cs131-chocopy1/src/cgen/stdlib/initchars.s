 sw a0, 4(sp)
 la a0, $str$prototype
 lw t0, 0(a0)
 lw t1, 4(a0)
 lw t2, 8(a0)
 li t3, 1
 la a0, allChars
 lw t4, 4(sp)
initchars_1:
 sw t0, 0(a0)
 sw t1, 4(a0)
 sw t2, 8(a0)
 sw t3, 12(a0)
 sw t4, 16(a0)
 addi a0, a0, 20
 jr  ra
 .data
 .align 2
 .globl allChars
allChars:
 .space 5120
 .text