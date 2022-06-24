; ModuleID = 'ChocoPy code'
source_filename = "../tests/pa3/sample/list_concat_2.py"

%$union.type = type {
  %$int$dispatchTable_type*,
  %$bool$dispatchTable_type*,
  %$str$dispatchTable_type*,
  %$object$dispatchTable_type*
}

%$union.len = type {
  %$.list$prototype_type*,
  %$str$prototype_type*
}

%$union.put = type {
  %$int$prototype_type*,
  %$bool$prototype_type*,
  %$str$prototype_type*
}

%$union.conslist = type {
  i32,
  i1,
  %$str$prototype_type*,
  %$.list$prototype_type*
}

%$object$prototype_type  = type  {
  i32,
  i32,
  %$object$dispatchTable_type*
}
@$object$prototype  = global %$object$prototype_type{
  i32 0,
  i32 3,
  %$object$dispatchTable_type* @$object$dispatchTable
}
%$object$dispatchTable_type = type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)*
}
@$object$dispatchTable = global %$object$dispatchTable_type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)* @$object.__init__
}

%$int$prototype_type  = type  {
  i32,
  i32,
  %$int$dispatchTable_type*,
  i32 
}
@$int$prototype  = global %$int$prototype_type{
  i32 1,
  i32 4,
  %$int$dispatchTable_type* @$int$dispatchTable,
  i32 0
}
%$int$dispatchTable_type = type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)*
}
@$int$dispatchTable = global %$int$dispatchTable_type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)* @$object.__init__
}

%$bool$prototype_type  = type  {
  i32,
  i32,
  %$bool$dispatchTable_type*,
  i1 
}
@$bool$prototype  = global %$bool$prototype_type{
  i32 2,
  i32 4,
  %$bool$dispatchTable_type* @$bool$dispatchTable,
  i1 0
}
%$bool$dispatchTable_type = type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)*
}
@$bool$dispatchTable = global %$bool$dispatchTable_type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)* @$object.__init__
}

%$str$prototype_type  = type  {
  i32,
  i32,
  %$str$dispatchTable_type*,
  i32 ,
  i8* 
}
@$str$prototype  = global %$str$prototype_type{
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 0,
  i8* inttoptr (i32 0 to i8*)
}
%$str$dispatchTable_type = type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)*
}
@$str$dispatchTable = global %$str$dispatchTable_type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)* @$object.__init__
}

%$.list$prototype_type  = type  {
  i32,
  i32,
  %$union.type ,
  i32 ,
  %$union.conslist* 
}
@$.list$prototype  = global %$.list$prototype_type{
  i32 -1,
  i32 5,
  %$union.type {%$int$dispatchTable_type* undef,  %$bool$dispatchTable_type* undef,  %$str$dispatchTable_type* undef,  %$object$dispatchTable_type* undef},
  i32 0,
  %$union.conslist* inttoptr (i32 0 to %$union.conslist*)
}

@const_0 = external global %$bool$prototype_type
@const_1 = external global %$bool$prototype_type
@const_2 = external global %$str$prototype_type
@const_3 = external global %$str$prototype_type
@const_4 = external global %$str$prototype_type
@const_5 = external global %$str$prototype_type
@const_6 = external global %$str$prototype_type
@const_7 = external global %$str$prototype_type
@z = global [0 x i32] undef
@i = global i32 0
declare %$object$dispatchTable_type @$object.__init__(%$object$dispatchTable_type)
declare void @heap.init()
declare %$str$dispatchTable_type* @initchars(i8*)
declare %$int$dispatchTable_type* @noconv()
declare void @error.OOB()
declare void @error.None()
declare void @error.Div()
declare %$.list$prototype_type* @concat(%$.list$prototype_type*, %$.list$prototype_type*)
declare %$.list$prototype_type* @conslist(i32, %$union.conslist, ...)
declare i32 @$len(%$union.len*)
declare void @print(%$union.put*)
declare %$bool$prototype_type* @makebool(i1)
declare %$int$prototype_type* @makeint(i32)
declare %$str$prototype_type* @makestr(%$str$prototype_type*)
declare %$str$prototype_type* @$input()
declare i1 @streql(%$str$prototype_type*)
declare i1 @strneql(%$str$prototype_type*)
declare %$str$prototype_type* @strcat(%$str$prototype_type*, %$str$prototype_type*)
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @before_main, i8* null }]
define void @before_main() {

label0:
  tail call void asm sideeffect "lui a0, 8192\0A	add s11, zero, a0", ""()
  call void @heap.init()
  tail call void asm sideeffect "mv s10, gp\0A	add s11, s11, s10\0A	mv fp, zero\0A	lw ra, 12(sp)\0A	addi sp, sp, 16\0A	ret", ""()

unreachable
}
define void @main() {

label0:
  tail call void asm sideeffect "addi fp, sp, 0", ""()
  %op2 = alloca i32
  store i32 1, i32* %op2
  %op3 = bitcast i32* %op2 to %$union.conslist*
  %op4 = load %$union.conslist, %$union.conslist* %op3
  %op5 = alloca i32
  store i32 2, i32* %op5
  %op6 = bitcast i32* %op5 to %$union.conslist*
  %op7 = load %$union.conslist, %$union.conslist* %op6
  %op8 = alloca i32
  store i32 3, i32* %op8
  %op9 = bitcast i32* %op8 to %$union.conslist*
  %op10 = load %$union.conslist, %$union.conslist* %op9
  %op11 = call %$.list$prototype_type* @conslist(3, %$union.conslist %op4, %$union.conslist %op7, %$union.conslist %op10)
  %op12 = alloca i32
  store i32 4, i32* %op12
  %op13 = bitcast i32* %op12 to %$union.conslist*
  %op14 = load %$union.conslist, %$union.conslist* %op13
  %op15 = alloca i32
  store i32 5, i32* %op15
  %op16 = bitcast i32* %op15 to %$union.conslist*
  %op17 = load %$union.conslist, %$union.conslist* %op16
  %op18 = alloca i32
  store i32 6, i32* %op18
  %op19 = bitcast i32* %op18 to %$union.conslist*
  %op20 = load %$union.conslist, %$union.conslist* %op19
  %op21 = call %$.list$prototype_type* @conslist(3, %$union.conslist %op14, %$union.conslist %op17, %$union.conslist %op20)
  %op22 = call %$.list$prototype_type* @concat(%$.list$prototype_type* %op11, %$.list$prototype_type* %op21)
  %op23 = alloca i32
  store i32 7, i32* %op23
  %op24 = bitcast i32* %op23 to %$union.conslist*
  %op25 = load %$union.conslist, %$union.conslist* %op24
  %op26 = alloca i32
  store i32 8, i32* %op26
  %op27 = bitcast i32* %op26 to %$union.conslist*
  %op28 = load %$union.conslist, %$union.conslist* %op27
  %op29 = alloca i32
  store i32 9, i32* %op29
  %op30 = bitcast i32* %op29 to %$union.conslist*
  %op31 = load %$union.conslist, %$union.conslist* %op30
  %op32 = call %$.list$prototype_type* @conslist(3, %$union.conslist %op25, %$union.conslist %op28, %$union.conslist %op31)
  %op33 = call %$.list$prototype_type* @concat(%$.list$prototype_type* %op22, %$.list$prototype_type* %op32)
  br label %label34

label34:                                                ; preds = %label0, %label41
  %op35 = load i32, i32*@i
  %op36 = load [0 x i32], [0 x i32]*@z
  %op37 = call %$.list$prototype_type* @conslist(0)
  %op38 = bitcast %$.list$prototype_type* %op37 to %$union.len*
  %op39 = call i32 @$len(%$union.len* %op38)
  %op40 = icmp slt i32 %op35, %op39
  br  i1 %op40, label %label41, label %label49

label41:                                                ; preds = %label34
  %op42 = load i32, i32*@i
  %op43 = getelementptr [0 x i32], [0 x i32]* @z, i32 0, i32 %op42
  %op44 = load i32, i32* %op43
  %op45 = call %$int$prototype_type* @makeint(i32 %op44)
  %op46 = bitcast %$int$prototype_type* %op45 to %$union.put*
  call void @print(%$union.put* %op46)
  %op47 = load i32, i32*@i
  %op48 = add i32 %op47, 1
  store i32 %op48, i32*@i
  br label %label34

label49:                                                ; preds = %label34
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
