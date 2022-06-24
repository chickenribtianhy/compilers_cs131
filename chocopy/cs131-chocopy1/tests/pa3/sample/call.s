	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_c2p0"
	.file	"call.py"
	.globl	before_main                     # -- Begin function before_main
	.p2align	1
	.type	before_main,@function
before_main:                            # @before_main
	.cfi_startproc
# %bb.0:                                # %label0
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	#APP
	lui a0, 8192
	add s11, zero, a0
	#NO_APP
	call	heap.init
	#APP
	mv s10, gp
	add s11, s11, s10
	mv fp, zero
	lw ra, 12(sp)
	addi sp, sp, 16
	ret
	#NO_APP
.Lfunc_end0:
	.size	before_main, .Lfunc_end0-before_main
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	1
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %label0
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	#APP
	addi fp, sp, 0
	#NO_APP
	call	($f)
	call	makeint
	call	print
	#APP
	li a7, 93 #exit system call
ecall
	#NO_APP
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.globl	$f                              # -- Begin function $f
	.p2align	1
	.type	$f,@function
$f:                                     # @"$f"
	.cfi_startproc
# %bb.0:                                # %label0
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	lui	a0, %hi(const_9)
	addi	a0, a0, %lo(const_9)
	call	print
	call	($g)
	lui	a0, %hi(const_10)
	addi	a0, a0, %lo(const_10)
	call	print
	addi	a0, zero, 42
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end2:
	.size	$f, .Lfunc_end2-($f)
	.cfi_endproc
                                        # -- End function
	.globl	$g                              # -- Begin function $g
	.p2align	1
	.type	$g,@function
$g:                                     # @"$g"
	.cfi_startproc
# %bb.0:                                # %label0
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	lui	a0, %hi(const_11)
	addi	a0, a0, %lo(const_11)
	call	print
	call	($h)
	lui	a0, %hi(const_12)
	addi	a0, a0, %lo(const_12)
	call	print
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end3:
	.size	$g, .Lfunc_end3-($g)
	.cfi_endproc
                                        # -- End function
	.globl	$h                              # -- Begin function $h
	.p2align	1
	.type	$h,@function
$h:                                     # @"$h"
	.cfi_startproc
# %bb.0:                                # %label0
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sw	ra, 12(sp)
	.cfi_offset ra, -4
	lui	a0, %hi(const_13)
	addi	a0, a0, %lo(const_13)
	call	print
	lui	a0, %hi(const_14)
	addi	a0, a0, %lo(const_14)
	call	print
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end4:
	.size	$h, .Lfunc_end4-($h)
	.cfi_endproc
                                        # -- End function
	.type	$object$prototype,@object       # @"$object$prototype"
	.data
	.globl	$object$prototype
	.p2align	3
$object$prototype:
	.word	0                               # 0x0
	.word	3                               # 0x3
	.word	($object$dispatchTable)
	.size	$object$prototype, 12

	.type	$object$dispatchTable,@object   # @"$object$dispatchTable"
	.section	.sdata,"aw",@progbits
	.globl	$object$dispatchTable
	.p2align	3
$object$dispatchTable:
	.word	($object.__init__)
	.size	$object$dispatchTable, 4

	.type	$int$prototype,@object          # @"$int$prototype"
	.data
	.globl	$int$prototype
	.p2align	3
$int$prototype:
	.word	1                               # 0x1
	.word	4                               # 0x4
	.word	($int$dispatchTable)
	.word	0                               # 0x0
	.size	$int$prototype, 16

	.type	$int$dispatchTable,@object      # @"$int$dispatchTable"
	.section	.sdata,"aw",@progbits
	.globl	$int$dispatchTable
	.p2align	3
$int$dispatchTable:
	.word	($object.__init__)
	.size	$int$dispatchTable, 4

	.type	$bool$prototype,@object         # @"$bool$prototype"
	.data
	.globl	$bool$prototype
	.p2align	3
$bool$prototype:
	.word	2                               # 0x2
	.word	4                               # 0x4
	.word	($bool$dispatchTable)
	.byte	0                               # 0x0
	.zero	3
	.size	$bool$prototype, 16

	.type	$bool$dispatchTable,@object     # @"$bool$dispatchTable"
	.section	.sdata,"aw",@progbits
	.globl	$bool$dispatchTable
	.p2align	3
$bool$dispatchTable:
	.word	($object.__init__)
	.size	$bool$dispatchTable, 4

	.type	$str$prototype,@object          # @"$str$prototype"
	.data
	.globl	$str$prototype
	.p2align	4
$str$prototype:
	.word	3                               # 0x3
	.word	5                               # 0x5
	.word	($str$dispatchTable)
	.word	0                               # 0x0
	.word	0
	.size	$str$prototype, 20

	.type	$str$dispatchTable,@object      # @"$str$dispatchTable"
	.section	.sdata,"aw",@progbits
	.globl	$str$dispatchTable
	.p2align	3
$str$dispatchTable:
	.word	($object.__init__)
	.size	$str$dispatchTable, 4

	.type	$.list$prototype,@object        # @"$.list$prototype"
	.data
	.globl	$.list$prototype
	.p2align	4
$.list$prototype:
	.word	4294967295                      # 0xffffffff
	.word	5                               # 0x5
	.zero	16
	.word	0                               # 0x0
	.word	0
	.size	$.list$prototype, 32

	.type	const_9,@object                 # @const_9
	.globl	const_9
	.p2align	4
const_9:
	.word	3                               # 0x3
	.word	6                               # 0x6
	.word	($str$dispatchTable)
	.word	7                               # 0x7
	.word	.Lstr.const_9
	.size	const_9, 20

	.type	.Lstr.const_9,@object           # @str.const_9
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr.const_9:
	.asciz	"start f"
	.size	.Lstr.const_9, 8

	.type	const_10,@object                # @const_10
	.data
	.globl	const_10
	.p2align	4
const_10:
	.word	3                               # 0x3
	.word	6                               # 0x6
	.word	($str$dispatchTable)
	.word	5                               # 0x5
	.word	.Lstr.const_10
	.size	const_10, 20

	.type	.Lstr.const_10,@object          # @str.const_10
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr.const_10:
	.asciz	"end f"
	.size	.Lstr.const_10, 6

	.type	const_11,@object                # @const_11
	.data
	.globl	const_11
	.p2align	4
const_11:
	.word	3                               # 0x3
	.word	6                               # 0x6
	.word	($str$dispatchTable)
	.word	7                               # 0x7
	.word	.Lstr.const_11
	.size	const_11, 20

	.type	.Lstr.const_11,@object          # @str.const_11
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr.const_11:
	.asciz	"start g"
	.size	.Lstr.const_11, 8

	.type	const_12,@object                # @const_12
	.data
	.globl	const_12
	.p2align	4
const_12:
	.word	3                               # 0x3
	.word	6                               # 0x6
	.word	($str$dispatchTable)
	.word	5                               # 0x5
	.word	.Lstr.const_12
	.size	const_12, 20

	.type	.Lstr.const_12,@object          # @str.const_12
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr.const_12:
	.asciz	"end g"
	.size	.Lstr.const_12, 6

	.type	const_13,@object                # @const_13
	.data
	.globl	const_13
	.p2align	4
const_13:
	.word	3                               # 0x3
	.word	6                               # 0x6
	.word	($str$dispatchTable)
	.word	7                               # 0x7
	.word	.Lstr.const_13
	.size	const_13, 20

	.type	.Lstr.const_13,@object          # @str.const_13
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr.const_13:
	.asciz	"start h"
	.size	.Lstr.const_13, 8

	.type	const_14,@object                # @const_14
	.data
	.globl	const_14
	.p2align	4
const_14:
	.word	3                               # 0x3
	.word	6                               # 0x6
	.word	($str$dispatchTable)
	.word	5                               # 0x5
	.word	.Lstr.const_14
	.size	const_14, 20

	.type	.Lstr.const_14,@object          # @str.const_14
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr.const_14:
	.asciz	"end h"
	.size	.Lstr.const_14, 6

	.section	.init_array,"aw",@init_array
	.p2align	2
	.word	before_main
	.section	".note.GNU-stack","",@progbits
