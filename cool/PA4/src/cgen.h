#include <assert.h>
#include <stdio.h>
#include <unordered_set>
#include <unordered_map>
#include <deque>

#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"

enum Basicness
{
   Basic,
   NotBasic
};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

std::unordered_map<Symbol, std::unordered_map<Symbol, int> *> func_name_offset;
std::unordered_map<Symbol, std::unordered_map<Symbol, method_class *> *> func_name_method;
std::unordered_map<Symbol, std::unordered_map<Symbol, int> *> attribute_name_offset;
std::unordered_map<Symbol, int> class_name;

struct location
{
   int offset;
   char *reg;
};
SymbolTable<Symbol, location> location_table;

class CgenClassTable : public SymbolTable<Symbol, CgenNode>
{
private:
   List<CgenNode> *nds, *parent_nds;
   ostream &str;
   int stringclasstag;
   int intclasstag;
   int boolclasstag;
   std::unordered_set<Symbol> basic_classes;
   int tag_table = 4;

   std::unordered_map<Symbol, Class_> class_map;
   std::unordered_map<Symbol, std::deque<Class_> *> classes_map;

   // The following methods emit code for
   // constants and global declarations.

   void code_global_data();
   void code_global_text();
   void code_bools(int);
   void code_select_gc();
   void code_constants();
   void _inverse_();
   void class_table();
   void _dispatch_table_();
   void _prototype_();

   void object_init();
   void class_methods();
   // The following creates an inheritance graph from
   // a list of classes.  The graph is implemented as
   // a tree of `CgenNode', and class names are placed
   // in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);
   void build_inheritance_tree();
   void set_relations(CgenNodeP nd);

public:
   CgenClassTable(Classes, ostream &str);
   void code();
   CgenNodeP root();
};

class CgenNode : public class__class
{
private:
   CgenNodeP parentnd;       // Parent of class
   List<CgenNode> *children; // Children of class
   Basicness basic_status;   // `Basic' if class is basic
                             // `NotBasic' otherwise

public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }
};

class BoolConst
{
private:
   int val;

public:
   BoolConst(int);
   void code_def(ostream &, int boolclasstag);
   void code_ref(ostream &) const;
};
