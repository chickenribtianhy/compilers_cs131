 addi sp, sp, -8
 sw ra, 4(sp)
 sw a0, 0(sp)
 la a0, $int$prototype
 lw t0, 0(a0)
 add a0, a0, t0
 jr ra
