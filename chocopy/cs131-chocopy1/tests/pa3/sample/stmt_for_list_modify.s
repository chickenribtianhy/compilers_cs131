	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0_m2p0_a2p0_c2p0"
	.file	"stmt_for_list_modify.py"
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
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	sw	ra, 28(sp)
	.cfi_offset ra, -4
	#APP
	addi fp, sp, 0
	#NO_APP
	lui	a0, %hi(x)
	lw	a0, %lo(x)(a0)
	sw	a0, 24(sp)
	j	.LBB1_1
.LBB1_1:                                # %label3
                                        # =>This Inner Loop Header: Depth=1
	lw	a0, 24(sp)
	addi	a1, a0, -2
	snez	a1, a1
	lui	a2, %hi(z)
	addi	a2, a2, %lo(z)
	slli	a3, a0, 2
	add	a2, a2, a3
	addi	a0, a0, 1
	lw	a2, 0(a2)
	sw	a1, 20(sp)
	sw	a0, 16(sp)
	sw	a2, 12(sp)
	j	.LBB1_2
.LBB1_2:                                # %label9
                                        #   in Loop: Header=BB1_1 Depth=1
	lui	a0, %hi(z)
	addi	a0, a0, %lo(z)
	lw	a1, 16(sp)
	slli	a2, a1, 2
	add	a0, a0, a2
	lw	a2, 12(sp)
	sw	a2, 0(a0)
	add	a0, zero, a2
	call	makeint
	call	print
	lw	a0, 16(sp)
	lw	a1, 20(sp)
	sw	a0, 24(sp)
	bnez	a1, .LBB1_1
	j	.LBB1_3
.LBB1_3:                                # %label13
	#APP
	li a7, 93 #exit system call
ecall
	#NO_APP
	lw	ra, 28(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
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

	.type	x,@object                       # @x
	.section	.sbss,"aw",@nobits
	.globl	x
	.p2align	2
x:
	.word	0                               # 0x0
	.size	x, 4

	.type	z,@object                       # @z
	.data
	.globl	z
	.p2align	2
z:
	.word	1                               # 0x1
	.word	2                               # 0x2
	.word	1                               # 0x1
	.size	z, 12

	.section	.init_array,"aw",@init_array
	.p2align	2
	.word	before_main
	.section	".note.GNU-stack","",@progbits
