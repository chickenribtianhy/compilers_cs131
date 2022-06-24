; ModuleID = 'ChocoPy code'
source_filename = "../tests/pa3/sample/stmt_for_list_return.py"

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
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
define void @$print_list(%$.list$prototype_type* %arg0) {

label1:
  %op2 = alloca i32
  store i32 0, i32* %op2
  %op3 = bitcast %$.list$prototype_type* %arg0 to %$union.len*
  %op4 = call i32 @$len(%$union.len* %op3)
  br label %label5

label5:                                                ; preds = %label1, %label21
  %op6 = phi i32 [ %op4, %label1 ], [ %op9, %label14 ]
  %op7 = icmp ne i32 %op4, %op6
  %op8 = getelementptr %$.list$prototype_type, %$.list$prototype_type* %arg0, i32 0, i32 4
  %op9 = add i32 %op6, 1
  %op10 = load %$union.conslist*, %$union.conslist** %op8
  %op11 = bitcast %$union.conslist* %op10 to %$int$prototype_type*
  %op12 = getelementptr %$int$prototype_type, %$int$prototype_type* %op11, i32 0, i32 4
  %op13 = load %$union.conslist*, %$union.conslist** %op12
  br label %label14

label14:                                                ; preds = %label5
  %op15 = call %$int$prototype_type* @makeint(%$union.conslist* %op13)
  %op16 = bitcast %$int$prototype_type* %op15 to %$union.put*
  call void @print(%$union.put* %op16)
  %op17 = icmp sge %$union.conslist* %op13, i32 30
  br  i1 %op17, label %label19, label %label21

label18:                                                ; preds = %label21
  ret void

label19:                                                ; preds = %label14
  %op20 = call %$int$dispatchTable_type* @noconv()
  ret %$int$dispatchTable_type* %op20

label21:                                                ; preds = %label14
  call void @$print_list(40)
  br  i1 %op7, label %label5, label %label18
}
declare void @$print_list(%$.list$prototype_type*)
