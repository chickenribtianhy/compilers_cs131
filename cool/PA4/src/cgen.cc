
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"
#include <algorithm>
#include <unordered_map>
#include <vector>

struct edge
{
  Symbol target;
  int pre;
};

std::vector<Symbol> name_table;
std::vector<edge> way;
std::unordered_map<Symbol, int> head, left_hand, right_hand;

/* initialize the edge */
void add_edge(Symbol a, Symbol b)
{
  if (!head.count(a))
    head[a] = -1;
  if (!head.count(b))
    head[b] = -1;
  way.emplace_back((edge){b, head[a]});
  head[a] = way.size() - 1;
}

int df_num = -1;
void dfs(Symbol x)
{
  left_hand[x] = ++df_num;
  name_table.emplace_back(x);
  for (int y = head[x]; y != -1; y = way[y].pre)
  {
    dfs(way[y].target);
  }
  right_hand[x] = df_num;
  class_name[x] = left_hand[x];
}
void dfs_gen(Symbol x, std::unordered_map<Symbol, int> &type_set, std::vector<Symbol> &list)
{
  for (int y = head[x]; y != -1; y = way[y].pre)
  {
    dfs_gen(way[y].target, type_set, list);
  }
  if (type_set.count(x))
    list.emplace_back(x);
}

extern void emit_string_constant(ostream &str, char *s);
extern int cgen_debug;
method_class *is_method(Feature x)
{
  return dynamic_cast<method_class *>(x);
}
attr_class *is_attr(Feature x)
{
  return dynamic_cast<attr_class *>(x);
}
//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
  arg = idtable.add_string("arg");
  arg2 = idtable.add_string("arg2");
  Bool = idtable.add_string("Bool");
  concat = idtable.add_string("concat");
  cool_abort = idtable.add_string("abort");
  copy = idtable.add_string("copy");
  Int = idtable.add_string("Int");
  in_int = idtable.add_string("in_int");
  in_string = idtable.add_string("in_string");
  IO = idtable.add_string("IO");
  length = idtable.add_string("length");
  Main = idtable.add_string("Main");
  main_meth = idtable.add_string("main");
  //   _no_class is a symbol that can't be the name of any
  //   user-defined class.
  No_class = idtable.add_string("_no_class");
  No_type = idtable.add_string("_no_type");
  Object = idtable.add_string("Object");
  out_int = idtable.add_string("out_int");
  out_string = idtable.add_string("out_string");
  prim_slot = idtable.add_string("_prim_slot");
  self = idtable.add_string("self");
  SELF_TYPE = idtable.add_string("SELF_TYPE");
  Str = idtable.add_string("String");
  str_field = idtable.add_string("_str_field");
  substr = idtable.add_string("substr");
  type_name = idtable.add_string("type_name");
  val = idtable.add_string("_val");
}

static char *gc_init_names[] =
    {"_NoGC_Init", "_GenGC_Init", "_ScnGC_Init"};
static char *gc_collect_names[] =
    {"_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect"};

//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream &os)
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  CgenClassTable *codegen_classtable = new CgenClassTable(classes, os);

  os << "\n# end of generated code\n";
}

//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_load(char *dest_reg, int offset, char *source_reg, ostream &s)
{
  s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")" << endl;
}

static void emit_store(char *source_reg, int offset, char *dest_reg, ostream &s)
{
  s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")" << endl;
}

static void emit_load_imm(char *dest_reg, int val, ostream &s)
{
  s << LI << dest_reg << " " << val << endl;
}

static void emit_load_address(char *dest_reg, char *address, ostream &s)
{
  s << LA << dest_reg << " " << address << endl;
}

static void emit_partial_load_address(char *dest_reg, ostream &s)
{
  s << LA << dest_reg << " ";
}

static void emit_load_bool(char *dest, const BoolConst &b, ostream &s)
{
  emit_partial_load_address(dest, s);
  b.code_ref(s);
  s << endl;
}

static void emit_load_string(char *dest, StringEntry *str, ostream &s)
{
  emit_partial_load_address(dest, s);
  str->code_ref(s);
  s << endl;
}

static void emit_load_int(char *dest, IntEntry *i, ostream &s)
{
  emit_partial_load_address(dest, s);
  i->code_ref(s);
  s << endl;
}

static void emit_move(char *dest_reg, char *source_reg, ostream &s)
{
  s << MOVE << dest_reg << " " << source_reg << endl;
}

static void emit_neg(char *dest, char *src1, ostream &s)
{
  s << NEG << dest << " " << src1 << endl;
}

static void emit_add(char *dest, char *src1, char *src2, ostream &s)
{
  s << ADD << dest << " " << src1 << " " << src2 << endl;
}

static void emit_addu(char *dest, char *src1, char *src2, ostream &s)
{
  s << ADDU << dest << " " << src1 << " " << src2 << endl;
}

static void emit_addiu(char *dest, char *src1, int imm, ostream &s)
{
  s << ADDIU << dest << " " << src1 << " " << imm << endl;
}

static void emit_div(char *dest, char *src1, char *src2, ostream &s)
{
  s << DIV << dest << " " << src1 << " " << src2 << endl;
}

static void emit_mul(char *dest, char *src1, char *src2, ostream &s)
{
  s << MUL << dest << " " << src1 << " " << src2 << endl;
}

static void emit_sub(char *dest, char *src1, char *src2, ostream &s)
{
  s << SUB << dest << " " << src1 << " " << src2 << endl;
}

static void emit_sll(char *dest, char *src1, int num, ostream &s)
{
  s << SLL << dest << " " << src1 << " " << num << endl;
}

static void emit_jalr(char *dest, ostream &s)
{
  s << JALR << "\t" << dest << endl;
}

static void emit_jal(char *address, ostream &s)
{
  s << JAL << address << endl;
}

static void emit_return(ostream &s)
{
  s << RET << endl;
}

static void emit_gc_assign(ostream &s)
{
  s << JAL << "_GenGC_Assign" << endl;
}

static void emit_disptable_ref(Symbol sym, ostream &s)
{
  s << sym << DISPTAB_SUFFIX;
}

static void emit_init_ref(Symbol sym, ostream &s)
{
  s << sym << CLASSINIT_SUFFIX;
}

static void emit_label_ref(int l, ostream &s)
{
  s << "label" << l;
}

static void emit_protobj_ref(Symbol sym, ostream &s)
{
  s << sym << PROTOBJ_SUFFIX;
}

static void emit_method_ref(Symbol classname, Symbol methodname, ostream &s)
{
  s << classname << METHOD_SEP << methodname;
}

static void emit_label_def(int l, ostream &s)
{
  emit_label_ref(l, s);
  s << ":" << endl;
}

static void emit_beqz(char *source, int label, ostream &s)
{
  s << BEQZ << source << " ";
  emit_label_ref(label, s);
  s << endl;
}

static void emit_beq(char *src1, char *src2, int label, ostream &s)
{
  s << BEQ << src1 << " " << src2 << " ";
  emit_label_ref(label, s);
  s << endl;
}

static void emit_bne(char *src1, char *src2, int label, ostream &s)
{
  s << BNE << src1 << " " << src2 << " ";
  emit_label_ref(label, s);
  s << endl;
}

static void emit_bleq(char *src1, char *src2, int label, ostream &s)
{
  s << BLEQ << src1 << " " << src2 << " ";
  emit_label_ref(label, s);
  s << endl;
}

static void emit_blt(char *src1, char *src2, int label, ostream &s)
{
  s << BLT << src1 << " " << src2 << " ";
  emit_label_ref(label, s);
  s << endl;
}

static void emit_blti(char *src1, int imm, int label, ostream &s)
{
  s << BLT << src1 << " " << imm << " ";
  emit_label_ref(label, s);
  s << endl;
}

static void emit_bgti(char *src1, int imm, int label, ostream &s)
{
  s << BGT << src1 << " " << imm << " ";
  emit_label_ref(label, s);
  s << endl;
}

static void emit_branch(int l, ostream &s)
{
  s << BRANCH;
  emit_label_ref(l, s);
  s << endl;
}

int counter = 0;
/* Push a register on the stack. The stack grows towards smaller addresses. */
static void emit_push(char *reg, ostream &str)
{
  emit_store(reg, 0, SP, str);
  emit_addiu(SP, SP, -4, str);
}
static void emit_pop(char *reg, ostream &str)
{
  emit_load(reg, 1, SP, str);
  emit_addiu(SP, SP, 4, str);
}
//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
static void emit_fetch_int(char *dest, char *source, ostream &s)
{
  emit_load(dest, DEFAULT_OBJFIELDS, source, s);
}

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(char *source, char *dest, ostream &s)
{
  emit_store(source, DEFAULT_OBJFIELDS, dest, s);
}

static void emit_test_collector(ostream &s)
{
  emit_push(ACC, s);
  emit_move(ACC, SP, s);  // stack end
  emit_move(A1, ZERO, s); // allocate nothing
  s << JAL << gc_collect_names[cgen_Memmgr] << endl;
  emit_addiu(SP, SP, 4, s);
  emit_load(ACC, 0, SP, s);
}

static void emit_gc_check(char *source, ostream &s)
{
  if (source != (char *)A1)
    emit_move(A1, source, s);
  s << JAL << "_gc_check" << endl;
}

static void emit_copy(ostream &s)
{
  s << JAL << "Object.copy" << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream &s)
{
  s << STRCONST_PREFIX << index;
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream &s, int stringclasstag)
{
  IntEntryP lensym = inttable.add_int(len);

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);
  s << LABEL                                                              // label
    << WORD << stringclasstag << endl                                     // tag
    << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len + 4) / 4) << endl // size
    << WORD;

  /***** Add dispatch information for class String ******/

  s << STRINGNAME << DISPTAB_SUFFIX << endl; // dispatch table
  s << WORD;
  lensym->code_ref(s);
  s << endl;                    // string length
  emit_string_constant(s, str); // ascii string
  s << ALIGN;                   // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the
// stringtable.
//
void StrTable::code_string_table(ostream &s, int stringclasstag)
{
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s, stringclasstag);
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
  s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);
  s << LABEL                                           // label
    << WORD << intclasstag << endl                     // class tag
    << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl // object size
    << WORD;

  /***** Add dispatch information for class Int ******/

  s << INTNAME << DISPTAB_SUFFIX << endl; // dispatch table
  s << WORD << str << endl;               // integer value
}

//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag)
{
  for (List<IntEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s, intclasstag);
}

//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream &s) const
{
  s << BOOLCONST_PREFIX << val;
}

//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream &s, int boolclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);
  s << LABEL                                            // label
    << WORD << boolclasstag << endl                     // class tag
    << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl // object size
    << WORD;

  /***** Add dispatch information for class Bool ******/

  s << BOOLNAME << DISPTAB_SUFFIX << endl; // dispatch table
  s << WORD << val << endl;                // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data()
{
  Symbol main = idtable.lookup_string(MAINNAME);
  Symbol string = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc = idtable.lookup_string(BOOLNAME);

  str << "\t.data\n"
      << ALIGN;
  //
  // The following global names must be defined first.
  //
  str << GLOBAL << CLASSNAMETAB << endl;
  str << GLOBAL;
  emit_protobj_ref(main, str);
  str << endl;
  str << GLOBAL;
  emit_protobj_ref(integer, str);
  str << endl;
  str << GLOBAL;
  emit_protobj_ref(string, str);
  str << endl;
  str << GLOBAL;
  falsebool.code_ref(str);
  str << endl;
  str << GLOBAL;
  truebool.code_ref(str);
  str << endl;
  str << GLOBAL << INTTAG << endl;
  str << GLOBAL << BOOLTAG << endl;
  str << GLOBAL << STRINGTAG << endl;

  //
  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  //
  str << INTTAG << LABEL
      << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL
      << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL
      << WORD << stringclasstag << endl;
}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
  str << GLOBAL << HEAP_START << endl
      << HEAP_START << LABEL
      << WORD << 0 << endl
      << "\t.text" << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Int"), str);
  str << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("String"), str);
  str << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"), str);
  str << endl
      << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << endl;
}

void CgenClassTable::code_bools(int boolclasstag)
{
  falsebool.code_def(str, boolclasstag);
  truebool.code_def(str, boolclasstag);
}

void CgenClassTable::code_select_gc()
{
  //
  // Generate GC choice constants (pointers to GC functions)
  //
  str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
  str << "_MemMgr_INITIALIZER:" << endl;
  str << WORD << gc_init_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
  str << "_MemMgr_COLLECTOR:" << endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_TEST" << endl;
  str << "_MemMgr_TEST:" << endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}

//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants()
{
  //
  // Add constants that are required by the code generator.
  //
  stringtable.add_string("");
  inttable.add_string("0");

  stringtable.code_string_table(str, stringclasstag);
  inttable.code_string_table(str, intclasstag);
  code_bools(boolclasstag);
}

CgenClassTable::CgenClassTable(Classes classes, ostream &s) : nds(NULL), parent_nds(NULL), str(s)
{

  enterscope();
  if (cgen_debug)
    cout << "Building CgenClassTable" << endl;
  install_basic_classes();
  install_classes(classes);
  build_inheritance_tree();

  /* assigning basic class tags */
  stringclasstag = class_name[Str];
  intclasstag = class_name[Int];
  boolclasstag = class_name[Bool];

  code();
  exitscope();
}

void CgenClassTable::install_basic_classes()
{

  // The tree package uses these globals to annotate the classes built below.
  // curr_lineno  = 0;
  Symbol filename = stringtable.add_string("<basic class>");

  //
  // A few special class names are installed in the lookup table but not
  // the class list.  Thus, these classes exist, but are not part of the
  // inheritance hierarchy.
  // No_class serves as the parent of Object and the other special classes.
  // SELF_TYPE is the self class; it cannot be redefined or inherited.
  // prim_slot is a class known to the code generator.
  //
  addid(No_class,
        new CgenNode(class_(No_class, No_class, nil_Features(), filename),
                     Basic, this));
  addid(SELF_TYPE,
        new CgenNode(class_(SELF_TYPE, No_class, nil_Features(), filename),
                     Basic, this));
  addid(prim_slot,
        new CgenNode(class_(prim_slot, No_class, nil_Features(), filename),
                     Basic, this));

  //
  // The Object class has no parent class. Its methods are
  //        cool_abort() : Object    aborts the program
  //        type_name() : Str        returns a string representation of class name
  //        copy() : SELF_TYPE       returns a copy of the object
  //
  // There is no need for method bodies in the basic classes---these
  // are already built in to the runtime system.
  //
  install_class(
      new CgenNode(
          class_(Object,
                 No_class,
                 append_Features(
                     append_Features(
                         single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
                         single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
                     single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
                 filename),
          Basic, this));

  //
  // The IO class inherits from Object. Its methods are
  //        out_string(Str) : SELF_TYPE          writes a string to the output
  //        out_int(Int) : SELF_TYPE               "    an int    "  "     "
  //        in_string() : Str                    reads a string from the input
  //        in_int() : Int                         "   an int     "  "     "
  //
  install_class(
      new CgenNode(
          class_(IO,
                 Object,
                 append_Features(
                     append_Features(
                         append_Features(
                             single_Features(method(out_string, single_Formals(formal(arg, Str)),
                                                    SELF_TYPE, no_expr())),
                             single_Features(method(out_int, single_Formals(formal(arg, Int)),
                                                    SELF_TYPE, no_expr()))),
                         single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
                     single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
                 filename),
          Basic, this));

  //
  // The Int class has no methods and only a single attribute, the
  // "val" for the integer.
  //
  install_class(
      new CgenNode(
          class_(Int,
                 Object,
                 single_Features(attr(val, prim_slot, no_expr())),
                 filename),
          Basic, this));

  //
  // Bool also has only the "val" slot.
  //
  install_class(
      new CgenNode(
          class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())), filename),
          Basic, this));

  //
  // The class Str has a number of slots and operations:
  //       val                                  ???
  //       str_field                            the string itself
  //       length() : Int                       length of the string
  //       concat(arg: Str) : Str               string concatenation
  //       substr(arg: Int, arg2: Int): Str     substring
  //
  install_class(
      new CgenNode(
          class_(Str,
                 Object,
                 append_Features(
                     append_Features(
                         append_Features(
                             append_Features(
                                 single_Features(attr(val, Int, no_expr())),
                                 single_Features(attr(str_field, prim_slot, no_expr()))),
                             single_Features(method(length, nil_Formals(), Int, no_expr()))),
                         single_Features(method(concat,
                                                single_Formals(formal(arg, Str)),
                                                Str,
                                                no_expr()))),
                     single_Features(method(substr,
                                            append_Formals(single_Formals(formal(arg, Int)),
                                                           single_Formals(formal(arg2, Int))),
                                            Str,
                                            no_expr()))),
                 filename),
          Basic, this));
  basic_classes.insert(Object);
  basic_classes.insert(IO);
  basic_classes.insert(Int);
  basic_classes.insert(Bool);
  basic_classes.insert(Str);
}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd)
{
  Symbol name = nd->get_name();

  if (probe(name))
  {
    return;
  }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<CgenNode>(nd, nds);
  addid(name, nd);
  class_map[name] = nd;
}

void CgenClassTable::install_classes(Classes cs)
{
  for (int i = cs->first(); cs->more(i); i = cs->next(i))
  {
    class_name[cs->nth(i)->get_name()] = ++tag_table;
    install_class(new CgenNode(cs->nth(i), NotBasic, this));
  }
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree()
{
  for (List<CgenNode> *l = nds; l; l = l->tl())
    set_relations(l->hd());
  for (auto entry : class_map)
  {
    auto name_0 = entry.first;
    auto &chain = *(classes_map[entry.first] = new std::deque<Class_>);
    add_edge(class_map[name_0]->get_father(), name_0);
    for (auto i = name_0; i != Object; i = class_map[i]->get_father())
      chain.push_front(class_map[i]);
    chain.push_front(class_map[Object]);
  }
  dfs(Object);
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd)
{
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n)
{
  children = new List<CgenNode>(n, children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}

/* inverse cgenclasstable */
void CgenClassTable::_inverse_()
{
  for (List<CgenNode> *i = nds; i; i = i->tl())
  {
    parent_nds = new List<CgenNode>(i->hd(), parent_nds);
  }
}

/*  Emits class name table and class object table */
void CgenClassTable::class_table()
{
  str << CLASSNAMETAB << ":" << endl;
  for (auto name_0 : name_table)
  {
    str << WORD;
    stringtable.lookup_string(name_0->get_string())->code_ref(str);
    str << endl;
  }

  str << CLASSOBJTAB << ":" << endl;
  for (auto name_0 : name_table)
  {
    str << WORD;
    str << name_0->get_string() << PROTOBJ_SUFFIX;
    str << endl;
    str << WORD;
    str << name_0->get_string() << CLASSINIT_SUFFIX;
    str << endl;
  }
}

/* Emits dispatch table */
void CgenClassTable::_dispatch_table_()
{
  for (List<CgenNode> *i = parent_nds; i; i = i->tl())
  {
    auto name_0 = i->hd()->get_name();
    str << name_0 << DISPTAB_SUFFIX << ":" << endl;
    std::unordered_map<Symbol, Symbol> func_class;
    auto func_offset = new std::unordered_map<Symbol, int>;
    auto func_method = new std::unordered_map<Symbol, method_class *>;
    func_name_offset[name_0] = func_offset;
    func_name_method[name_0] = func_method;
    int offset = 0;
    for (auto class_1 : *classes_map[name_0])
    {
      auto features = class_1->get_features();
      // for (auto _feature_ : *class_1->get_features())
      for (int i = features->first(); features->more(i); i = features->next(i))
      {
        auto _feature_ = features->nth(i);
        if (is_method(_feature_))
        {
          func_class[_feature_->get_name()] = class_1->get_name();
        }
      }
    }
    for (auto class_1 : *classes_map[name_0])
    {
      auto features = class_1->get_features();
      // for (auto _feature_ : *class_1->get_features())
      for (int i = features->first(); features->more(i); i = features->next(i))
      {
        auto _feature_ = features->nth(i);
        auto _name = _feature_->get_name();
        if (is_method(_feature_))
          if (func_class.count(_name) != 0)
          {
            str << WORD << func_class[_name] << METHOD_SEP << _name << endl;
            func_class.erase(_name);
            (*func_offset)[_name] = offset++;
            (*func_method)[_name] = is_method(_feature_);
          }
      }
    }
  }
}

/* _prototype_ */

void CgenClassTable::_prototype_()
{
  for (List<CgenNode> *i = parent_nds; i; i = i->tl())
  {
    str << WORD << -1 << endl;
    auto name_0 = i->hd()->get_name();
    str << name_0 << PROTOBJ_SUFFIX << ":" << endl;
    str << WORD << class_name[name_0] << endl;
    int attrsize = 0;
    auto attr_offset = new std::unordered_map<Symbol, int>;
    attribute_name_offset[name_0] = attr_offset;
    for (auto class_1 : *classes_map[name_0])
    {
      auto features = class_1->get_features();
      // for (auto _feature_ : *class_1->get_features())
      for (int i = features->first(); features->more(i); i = features->next(i))
      {
        auto _feature_ = features->nth(i);
        if (is_attr(_feature_))
        {
          auto _name = _feature_->get_name();
          (*attr_offset)[_name] = attrsize;
          attrsize++;
        }
      }
    }
    str << WORD << DEFAULT_OBJFIELDS + attrsize << endl;
    str << WORD << name_0 << DISPTAB_SUFFIX << endl;
    for (auto class_1 : *classes_map[name_0])
    {
      auto features = class_1->get_features();
      // for (auto _feature_ : *class_1->get_features())
      for (int i = features->first(); features->more(i); i = features->next(i))
      {
        auto _feature_ = features->nth(i);
        if (is_attr(_feature_))
        {
          str << "# " << is_attr(_feature_)->name << endl;
          str << WORD;
          if (is_attr(_feature_)->type_decl == Bool)
          {
            falsebool.code_ref(str);
          }
          else if (is_attr(_feature_)->type_decl == Str)
          {
            stringtable.lookup_string("")->code_ref(str);
          }
          else if (is_attr(_feature_)->type_decl == Int)
          {
            inttable.lookup_string("0")->code_ref(str);
          }
          else
          {
            str << EMPTYSLOT;
          }
          str << endl;
        }
      }
    }
  }
}

/* callee_begin */
void callee_begin(ostream &s, int var = 0)
{
  emit_addiu(SP, SP, -12 - var * 4, s);
  emit_store(FP, 3 + var, SP, s);
  emit_store(SELF, 2 + var, SP, s);
  emit_store(RA, 1 + var, SP, s);
  emit_addiu(FP, SP, 4, s);
  emit_move(SELF, ACC, s);
}

/* callee end */
void callee_end(ostream &s, int var = 0, int clean = 0)
{
  emit_load(FP, 3 + var, SP, s);
  emit_load(SELF, 2 + var, SP, s);
  emit_load(RA, 1 + var, SP, s);
  emit_addiu(SP, SP, 12 + var * 4 + clean * 4, s);
  emit_return(s);
}

char buff[2000];

/* concatenate strings */
void cat(char *lft, char *rgt)
{
  strcpy(buff, lft);
  strcat(buff, rgt);
}

std::unordered_map<Symbol, StringEntryP> ssep;
Symbol global_class;

int depth = 0;
int md = 0;
void depth_increase()
{
  depth++;
  md = std::max(md, depth);
}
void depth_decrease() { depth--; }

std::unordered_map<Symbol, std::unordered_map<Symbol, int> *> class_methods_done;

/* object initialization */
void CgenClassTable::object_init()
{
  for (List<CgenNode> *l = parent_nds; l; l = l->tl())
  {
    auto name_0 = l->hd()->get_name();
    global_class = name_0;
    class_methods_done[name_0] = new std::unordered_map<Symbol, int>;
    str << name_0 << CLASSINIT_SUFFIX << ":" << endl;

    location_table.enterscope();
    md = 0;
    auto offset = 0;

    /* add attributes to symbol table and allocate stack */
    for (auto class_1 : *classes_map[name_0])
    {
      auto features = class_1->get_features();
      // for (auto _feature_ : *class_1->get_features())
      for (int i = features->first(); features->more(i); i = features->next(i))
      {
        auto _feature_ = features->nth(i);
        if (is_attr(_feature_))
        {
          location_table.addid(
              _feature_->get_name(),
              new location({DEFAULT_OBJFIELDS + (*attribute_name_offset[global_class])[_feature_->get_name()], SELF}));
          is_attr(_feature_)->init->path_height();
          if (class_1->get_name() != name_0)
            offset++;
        }
      }
    }
    cout << "# " << md << " variables\n";
    callee_begin(str, md);

    if (name_0 != Object)
    {
      auto dadname = class_map[name_0]->get_father()->get_string();
      cat(dadname, CLASSINIT_SUFFIX);
      emit_jal(buff, str);
    }

    // for (auto _feature_ : *class_map[name_0]->get_features())
    // {
    Features features = class_map[name_0]->get_features();
    for (int i = features->first(); features->more(i); i = features->next(i))
    // for (auto _feature_ : *name2class[name_0]->get_features())
    {
      auto _feature_ = features->nth(i);
      auto xattr = is_attr(_feature_);
      if (xattr)
      {
        if (!dynamic_cast<no_expr_class *>(xattr->init))
        {
          cout << "# " << xattr->name << endl;
          location_table.enterscope();

          xattr->init->code(str);
          emit_store(ACC, DEFAULT_OBJFIELDS + offset, SELF, str);

          location_table.exitscope();
        }
        offset++;
      }
    }
    emit_move(ACC, SELF, str);
    callee_end(str, md);

    location_table.exitscope();
  }
}

/* class method */

void CgenClassTable::class_methods()
{
  for (List<CgenNode> *l = parent_nds; l; l = l->tl())
  {
    auto name_0 = l->hd()->get_name();
    global_class = name_0;
    if (basic_classes.count(name_0))
      continue;

    location_table.enterscope();
    for (auto class_1 : *classes_map[name_0])
    {
      auto features = class_1->get_features();
      // for (auto _feature_ : *class_1->get_features())
      for (int i = features->first(); features->more(i); i = features->next(i))
      {
        auto _feature_ = features->nth(i);
        if (is_attr(_feature_))
        {
          location_table.addid(
              _feature_->get_name(),
              new location({DEFAULT_OBJFIELDS + (*attribute_name_offset[global_class])[_feature_->get_name()], SELF}));
        }
      }
    }

    for (auto class_1 : *classes_map[name_0])
    {
      auto features = class_1->get_features();
      // for (auto _feature_ : *class_1->get_features())
      for (int i = features->first(); features->more(i); i = features->next(i))
      {
        auto _feature_ = features->nth(i);
        if (is_method(_feature_))
        {
          if (basic_classes.count(class_1->get_name()))
            continue;
          if ((*class_methods_done[class_1->get_name()]).count(_feature_->get_name()))
            continue;

          md = 0;
          is_method(_feature_)->expr->path_height();
          cout << "# " << md << " variables\n";

          location_table.enterscope();
          int count = is_method(_feature_)->formals->len();
          auto formals = is_method(_feature_)->formals;
          // for (auto xformal : *is_method(_feature_)->formals)
          for (int i = formals->first(); formals->more(i); i = formals->next(i))
          {
            auto xformal = formals->nth(i);
            location_table.addid(dynamic_cast<formal_class *>(xformal)->name,
                                 new location({3 + md + --count, FP}));
          }

          str << class_1->get_name() << "." << _feature_->get_name() << ":" << endl;
          callee_begin(str, md);
          is_method(_feature_)->expr->code(str);
          callee_end(str, md, is_method(_feature_)->formals->len());

          location_table.exitscope();
          auto _name_0 = class_1->get_name();
          auto _name_1 = _feature_->get_name();
          (*class_methods_done[_name_0])[_name_1] = 1;
        }
      }
    }

    location_table.exitscope();
  }
}

/* path_height check the deepest hight to open the corresponding stack. */

void assign_class::path_height()
{
  expr->path_height();
}

void static_dispatch_class::path_height()
{
  expr->path_height();
  for (int i = actual->first(); actual->more(i); i = actual->next(i))
  // for (auto actual_0 : *actual)
  {
    auto actual_0 = actual->nth(i);
    actual_0->path_height();
  }
}

void dispatch_class::path_height()
{
  expr->path_height();
  for (int i = actual->first(); actual->more(i); i = actual->next(i))
  // for (auto actual_0 : *actual)
  {
    auto actual_0 = actual->nth(i);
    actual_0->path_height();
  }
}

void cond_class::path_height()
{
  pred->path_height();
  then_exp->path_height();
  else_exp->path_height();
}

void loop_class::path_height()
{
  pred->path_height();
  body->path_height();
}

void typcase_class::path_height()
{
  expr->path_height();
  depth_increase();
  for (int i = cases->first(); cases->more(i); i = cases->next(i))
  {
    auto case_0 = cases->nth(i);
    dynamic_cast<branch_class *>(case_0)->expr->path_height();
  }
  depth_decrease();
}

void block_class::path_height()
{
  // for (auto exp_0 : *body)
  for (int i = body->first(); body->more(i); i = body->next(i))
  {
    auto exp_0 = body->nth(i);
    exp_0->path_height();
  }
}

void let_class::path_height()
{
  depth_increase();
  init->path_height();
  body->path_height();
  depth_decrease();
}

void plus_class::path_height()
{
  e1->path_height();
  e2->path_height();
}

void sub_class::path_height()
{
  e1->path_height();
  e2->path_height();
}

void mul_class::path_height()
{
  e1->path_height();
  e2->path_height();
}

void divide_class::path_height()
{
  e1->path_height();
  e2->path_height();
}

void neg_class::path_height() { e1->path_height(); }

void lt_class::path_height()
{
  e1->path_height();
  e2->path_height();
}

void eq_class::path_height()
{
  e1->path_height();
  e2->path_height();
}

void leq_class::path_height()
{
  e1->path_height();
  e2->path_height();
}

void comp_class::path_height()
{
  e1->path_height();
}

void int_const_class::path_height() {}

void string_const_class::path_height() {}

void bool_const_class::path_height() {}

void new__class::path_height() {}

void isvoid_class::path_height()
{
  e1->path_height();
}

void no_expr_class::path_height() {}
void object_class::path_height() {}

void CgenClassTable::code()
{
  if (cgen_debug)
    cout << "coding global data" << endl;
  code_global_data();

  if (cgen_debug)
    cout << "choosing gc" << endl;
  code_select_gc();

  if (cgen_debug)
    cout << "coding constants" << endl;
  code_constants();

  _inverse_();
  class_table();
  _dispatch_table_();
  _prototype_();

  if (cgen_debug)
    cout << "coding global text" << endl;
  code_global_text();

  object_init();
  class_methods();
}

CgenNodeP CgenClassTable::root() { return probe(Object); }

///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct)
    : class__class((const class__class &)*nd), parentnd(NULL), children(NULL), basic_status(bstatus)
{
  stringtable.add_string(name->get_string()); // Add class name to string table
}

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

void assign_class::code(ostream &s)
{
  expr->code(s);
  auto where = location_table.lookup(name);
  emit_store(ACC, where->offset, where->reg, s);
}

void static_dispatch_class::code(ostream &s)
{
  // for (auto actual_0 : *actual)
  for (int i = actual->first(); actual->more(i); i = actual->next(i))
  {
    auto actual_0 = actual->nth(i);
    actual_0->code(s);
    emit_push(ACC, s);
  }
  expr->code(s);
  auto acclabel = counter++;
  emit_bne(ACC, ZERO, acclabel, s);
  cat(STRCONST_PREFIX, "0");
  emit_load_address(ACC, buff, s);
  emit_load_imm(T1, line_number, s);
  emit_jal("_dispatch_abort", s);
  emit_label_def(acclabel, s);
  auto type = type_name;
  if (type == SELF_TYPE)
    type = global_class;
  cat(type->get_string(), DISPTAB_SUFFIX);
  emit_load_address(T1, buff, s);
  emit_load(T1, (*func_name_offset[type])[name], T1, s);
  emit_jalr(T1, s);
}

void dispatch_class::code(ostream &s)
{
  // for (auto actual_0 : *actual)
  for (int i = actual->first(); actual->more(i); i = actual->next(i))
  {
    auto actual_0 = actual->nth(i);
    actual_0->code(s);
    emit_push(ACC, s);
  }
  expr->code(s);
  auto acclabel = counter++;
  emit_bne(ACC, ZERO, acclabel, s);
  cat(STRCONST_PREFIX, "0");
  emit_load_address(ACC, buff, s);
  emit_load_imm(T1, line_number, s);
  emit_jal("_dispatch_abort", s);
  emit_label_def(acclabel, s);
  emit_load(T1, 2, ACC, s);
  auto type = expr->type;
  if (type == SELF_TYPE)
    type = global_class;
  emit_load(T1, (*func_name_offset[type])[name], T1, s);
  emit_jalr(T1, s);
}

void cond_class::code(ostream &s)
{
  pred->code(s);
  emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
  auto elselabel = counter++;
  auto endlabel = counter++;
  emit_beqz(T1, elselabel, s);
  then_exp->code(s);
  emit_branch(endlabel, s);
  emit_label_def(elselabel, s);
  else_exp->code(s);
  emit_label_def(endlabel, s);
}

void loop_class::code(ostream &s)
{
  auto looplabel = counter++;
  auto endlabel = counter++;
  emit_label_def(looplabel, s);
  pred->code(s);
  emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
  emit_beq(T1, ZERO, endlabel, s);
  body->code(s);
  emit_branch(looplabel, s);
  emit_label_def(endlabel, s);
  emit_move(ACC, ZERO, s);
}

void typcase_class::code(ostream &s)
{
  expr->code(s);
  auto endlabel = counter++;
  std::vector<int> labels;
  std::unordered_map<Symbol, int> typeset; // types of branches
  // for (auto _branch_1 : *cases)
  for (int i = cases->first(); cases->more(i); i = cases->next(i))
  {
    auto _branch_1 = cases->nth(i);
    labels.emplace_back(counter++);
    auto ty = dynamic_cast<branch_class *>(_branch_1)->type_decl;
    typeset[ty] = 1;
  }
  auto ab = counter++;
  labels.emplace_back(ab);
  auto count = 0;
  cout << "# type begin: current pos = " << depth << "\n";
  emit_store(ACC, depth, FP, s);
  emit_bne(ACC, ZERO, labels[0], s);
  cat(STRCONST_PREFIX, "0");
  emit_load_address(ACC, buff, s);
  emit_load_imm(T1, line_number, s);
  emit_jal("_case_abort2", s);

  std::vector<Symbol> todo; // specific order of types of branches

  dfs_gen(Object, typeset, todo); // using sub-tree structure to judge lowest class type
  bool first = true;
  for (auto type_0 : todo)
  {
    // for (auto _branch_1 : *cases)
    for (int i = cases->first(); cases->more(i); i = cases->next(i))
    {
      auto _branch_1 = cases->nth(i);
      auto _branch_2 = dynamic_cast<branch_class *>(_branch_1);
      if (_branch_2->type_decl != type_0)
        continue;
      emit_label_def(labels[count++], s);
      if (first)
      {
        emit_load(T2, 0, ACC, s);
        first = false;
      }
      // decide the sub-tree range
      emit_blti(T2, left_hand[_branch_2->type_decl], labels[count], s);
      emit_bgti(T2, right_hand[_branch_2->type_decl], labels[count], s);
      location_table.enterscope();
      location_table.addid(_branch_2->name, new location({depth, FP}));
      _branch_2->expr->code(s);
      location_table.exitscope();
      emit_branch(endlabel, s);
    }
  }
  cout << "# type end: current pos = " << depth << "\n";
  emit_label_def(ab, s);
  emit_jal("_case_abort", s);
  emit_label_def(endlabel, s);
}

void block_class::code(ostream &s)
{
  // for (auto exp_0 : *body)
  for (int i = body->first(); body->more(i); i = body->next(i))
  {
    auto exp_0 = body->nth(i);
    exp_0->code(s);
  }
}

void let_class::code(ostream &s)
{
  location_table.enterscope();
  if (dynamic_cast<no_expr_class *>(init))
  {
    if (type_decl == Bool)
    {
      emit_load_bool(ACC, BoolConst(0), s);
    }
    else if (type_decl == Int)
    {
      emit_load_int(ACC, inttable.lookup_string("0"), s);
    }
    else if (type_decl == Str)
    {
      emit_load_string(ACC, stringtable.lookup_string(""), s);
    }

    else
    {
      emit_move(ACC, ZERO, s);
    }
  }
  else
  {
    init->code(s);
  }
  emit_store(ACC, depth, FP, s);
  location_table.addid(identifier, new location({depth, FP}));
  cout << "# let begin: current pos = " << depth << "\n";
  depth++;
  body->code(s);
  depth--;
  cout << "# let end: current pos = " << depth << "\n";
  location_table.exitscope();
}

void plus_class::code(ostream &s)
{
  e1->code(s);
  emit_push(ACC, s);
  e2->code(s);
  emit_copy(s);
  emit_pop("$s1", s);
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
  emit_load(T1, DEFAULT_OBJFIELDS, "$s1", s);
  emit_add(T1, T1, T2, s);
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
}

void sub_class::code(ostream &s)
{
  e1->code(s);
  emit_push(ACC, s);
  e2->code(s);
  emit_copy(s);
  emit_pop("$s1", s);
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
  emit_load(T1, DEFAULT_OBJFIELDS, "$s1", s);
  emit_sub(T1, T1, T2, s);
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
}

void mul_class::code(ostream &s)
{
  e1->code(s);
  emit_push(ACC, s);
  e2->code(s);
  emit_copy(s);
  emit_pop("$s1", s);
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
  emit_load(T1, DEFAULT_OBJFIELDS, "$s1", s);
  emit_mul(T1, T1, T2, s);
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
}

void divide_class::code(ostream &s)
{
  e1->code(s);
  emit_push(ACC, s);
  e2->code(s);
  emit_copy(s);
  emit_pop("$s1", s);
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
  emit_load(T1, DEFAULT_OBJFIELDS, "$s1", s);
  emit_div(T1, T1, T2, s);
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
}

void neg_class::code(ostream &s)
{
  e1->code(s);
  emit_copy(s);
  emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
  emit_neg(T1, T1, s);
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s);
}

void lt_class::code(ostream &s)
{
  e1->code(s);
  emit_push(ACC, s);
  e2->code(s);
  emit_pop("$s1", s);
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
  emit_load(T1, DEFAULT_OBJFIELDS, "$s1", s);
  emit_load_bool(ACC, BoolConst(1), s);
  auto truelabel = counter++;
  emit_blt(T1, T2, truelabel, s);
  emit_load_bool(ACC, BoolConst(0), s);
  emit_label_def(truelabel, s);
}

void eq_class::code(ostream &s)
{
  e1->code(s);
  emit_push(ACC, s);
  e2->code(s);
  emit_pop(T1, s);
  emit_move(T2, ACC, s);
  emit_load_bool(ACC, BoolConst(1), s);
  auto truelabel = counter++;
  emit_beq(T1, T2, truelabel, s);
  emit_load_bool(A1, BoolConst(0), s);
  emit_jal("equality_test", s);
  emit_label_def(truelabel, s);
}

void leq_class::code(ostream &s)
{
  e1->code(s);
  emit_push(ACC, s);
  e2->code(s);
  emit_pop("$s1", s);
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s);
  emit_load(T1, DEFAULT_OBJFIELDS, "$s1", s);
  emit_load_bool(ACC, BoolConst(1), s);
  auto truelabel = counter++;
  emit_bleq(T1, T2, truelabel, s);
  emit_load_bool(ACC, BoolConst(0), s);
  emit_label_def(truelabel, s);
}

void comp_class::code(ostream &s)
{
  e1->code(s);
  emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);
  emit_load_bool(ACC, BoolConst(1), s);
  auto truelabel = counter++;
  emit_beqz(T1, truelabel, s);
  emit_load_bool(ACC, BoolConst(0), s);
  emit_label_def(truelabel, s);
}

void int_const_class::code(ostream &s)
{
  emit_load_int(ACC, inttable.lookup_string(token->get_string()), s);
}

void string_const_class::code(ostream &s)
{
  emit_load_string(ACC, stringtable.lookup_string(token->get_string()), s);
}

void bool_const_class::code(ostream &s)
{
  emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(ostream &s)
{
  if (type_name == SELF_TYPE)
  {
    emit_load_address(T1, CLASSOBJTAB, s);
    emit_load(T2, 0, SELF, s);
    emit_sll(T2, T2, 3, s);
    emit_addu(T1, T1, T2, s);
    emit_move("$s1", T1, s);
    emit_load(ACC, 0, T1, s);
    emit_jal("Object.copy", s);
    emit_load(T1, 1, "$s1", s);
    emit_jalr(T1, s);
    return;
  }
  cat(type_name->get_string(), PROTOBJ_SUFFIX);
  emit_load_address(ACC, buff, s);
  emit_jal("Object.copy", s);
  cat(type_name->get_string(), CLASSINIT_SUFFIX);
  emit_jal(buff, s);
}

void isvoid_class::code(ostream &s)
{
  e1->code(s);
  emit_move(T1, ACC, s);
  emit_load_bool(ACC, BoolConst(1), s);
  auto truelabel = counter++;
  emit_beqz(T1, truelabel, s);
  emit_load_bool(ACC, BoolConst(0), s);
  emit_label_def(truelabel, s);
}

void no_expr_class::code(ostream &s)
{
  cout << "# no_expr_class"
       << "\n";
}

void object_class::code(ostream &s)
{
  if (type == SELF_TYPE)
  {
    emit_move(ACC, SELF, s);
  }
  else
  {
    auto location = location_table.lookup(name);
    emit_load(ACC, location->offset, location->reg, s);
  }
}
