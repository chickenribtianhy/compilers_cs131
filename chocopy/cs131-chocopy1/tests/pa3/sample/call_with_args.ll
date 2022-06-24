; ModuleID = 'ChocoPy code'
source_filename = "../tests/pa3/sample/call_with_args.py"

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
  %$object$prototype_type(%$object$prototype_type)*
}
@$object$dispatchTable = global %$object$dispatchTable_type {
  %$object$prototype_type(%$object$prototype_type)* @$object.__init__
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
  %$object$prototype_type(%$object$prototype_type)*
}
@$int$dispatchTable = global %$int$dispatchTable_type {
  %$object$prototype_type(%$object$prototype_type)* @$object.__init__
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
  %$object$prototype_type(%$object$prototype_type)*
}
@$bool$dispatchTable = global %$bool$dispatchTable_type {
  %$object$prototype_type(%$object$prototype_type)* @$object.__init__
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
  %$object$prototype_type(%$object$prototype_type)*
}
@$str$dispatchTable = global %$str$dispatchTable_type {
  %$object$prototype_type(%$object$prototype_type)* @$object.__init__
}

%$.list$prototype_type  = type  {
  i32,
  i32,
  %union.list,
  i32 
}
%union.list = type {
  i32,
  %$str$dispatchTable_type*,
  %$int$dispatchTable_type*,
  %$bool$dispatchTable_type*
}
@$.list$prototype  = global %$.list$prototype_type{
  i32 -1,
  i32 4,
  %union.list {i32 0, %$str$dispatchTable_type* undef,%$int$dispatchTable_type* undef,%$bool$dispatchTable_type* undef },
  i32 0
}

@const_9 = global %$str$prototype_type {
  i32 3,
  i32 6,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 7,
  i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.const_9, i32 0, i32 0) 
}
@str.const_9 = private unnamed_addr constant [7 x i8] c"start f", align 1

@const_10 = global %$str$prototype_type {
  i32 3,
  i32 6,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 5,
  i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.const_10, i32 0, i32 0) 
}
@str.const_10 = private unnamed_addr constant [5 x i8] c"end f", align 1

@const_11 = global %$str$prototype_type {
  i32 3,
  i32 6,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 7,
  i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.const_11, i32 0, i32 0) 
}
@str.const_11 = private unnamed_addr constant [7 x i8] c"start g", align 1

@const_12 = global %$str$prototype_type {
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 1,
  i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.const_12, i32 0, i32 0) 
}
@str.const_12 = private unnamed_addr constant [1 x i8] c"h", align 1

@const_13 = global %$str$prototype_type {
  i32 3,
  i32 6,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 5,
  i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.const_13, i32 0, i32 0) 
}
@str.const_13 = private unnamed_addr constant [5 x i8] c"end g", align 1

declare %$object$prototype_type @$object.__init__(%$object$prototype_type)
declare void @print_int(%$int$prototype_type*)
declare void @print_str(%$str$prototype_type*)
declare void @print_bool(%$bool$prototype_type*)
declare %$str$prototype_type @$input()
declare i32 @$len_str(%$str$prototype_type*)
declare i32 @$len_list(%$.list$prototype_type*)
declare i1 @streql(%$str$prototype_type)
declare i1 @strneql(%$str$prototype_type)
declare %$str$prototype_type @strcat(%$str$prototype_type, %$str$prototype_type)
declare %$.list$prototype_type @concat(%$.list$prototype_type, %$.list$prototype_type)
declare %$.list$prototype_type @conslist_int(i32, i32)
declare %$.list$prototype_type @conslist_list(i32*, i32*)
declare %$bool$prototype_type* @makebool(i1)
declare %$int$prototype_type* @makeint(i32)
declare %$str$prototype_type* @makestr(%$str$prototype_type*)
declare void @heap.init()
declare i1 @initchars()
declare i32 @noconv()
declare void @error.OOB()
declare void @error.None()
declare void @error.Div()
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @before_main, i8* null }]
define void @before_main() {

label0:
  tail call void asm sideeffect "lui a0, 8192\0A	add s11, zero, a0", ""()
  call void @heap.init()
  tail call void asm sideeffect "mv gp, a0\0A	mv s10, gp\0A	add s11, s11, s10\0A	mv fp, zero\0A	lw ra, 12(sp)\0A	addi sp, sp, 16\0A	ret", ""()

unreachable
}
define void @main() {

label0:
  %op1 = call i32 @$f(4)
  %op2 = call %$int$prototype_type* @makeint(i32 %op1)
  call void @print_int(%$int$prototype_type* %op2)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
define i32 @$f(i32 %arg0) {

label1:
  %op2 = load i32, i32 %arg0
  %op3 = call %$str$prototype_type* @makestr(%$str$prototype_type* @const_9)
  call void @print_str(%$str$prototype_type* %op3)
  %op4 = call %$int$prototype_type* @makeint(i32 %op2)
  call void @print_int(%$int$prototype_type* %op4)
  call void @$g(1, i32 %op2)
  %op5 = call %$str$prototype_type* @makestr(%$str$prototype_type* @const_10)
  call void @print_str(%$str$prototype_type* %op5)
  ret i32 %op2
}
define void @$g(i32 %arg0, i32 %arg1) {

label2:
  %op3 = load i32, i32 %arg0
  %op4 = load i32, i32 %arg1
  %op5 = call %$str$prototype_type* @makestr(%$str$prototype_type* @const_11)
  call void @print_str(%$str$prototype_type* %op5)
  %op6 = call %$int$prototype_type* @makeint(i32 %op3)
  call void @print_int(%$int$prototype_type* %op6)
  %op7 = call %$int$prototype_type* @makeint(i32 %op4)
  call void @print_int(%$int$prototype_type* %op7)
  call void @$h(%$str$prototype_type* @const_12)
  %op8 = call %$str$prototype_type* @makestr(%$str$prototype_type* @const_13)
  call void @print_str(%$str$prototype_type* %op8)
  ret void
}
define void @$h(%$str$prototype_type %arg0) {

label1:
  %op2 = load %$str$prototype_type, %$str$prototype_type %arg0
  call void @print_str(%$str$prototype_type* %op2)
  ret void
}
