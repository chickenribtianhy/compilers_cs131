#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>
#include <map>
#include <unordered_map>
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"

#define TRUE 1
#define FALSE 0

class ClassTable;
typedef ClassTable *ClassTableP;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.
typedef int Status;
#define Visit_yes 1
#define Visit_no 2

class ClassTable
{
private:
  int semant_errors;
  void install_basic_classes();
  ostream &error_stream;

  /* added */
  Symbol temp;

public:
  ClassTable(Classes);
  int errors() { return semant_errors; }
  ostream &semant_error();
  ostream &semant_error(Class_ c);
  ostream &semant_error(Symbol filename, tree_node *t);

  /* added */
  std::map<Symbol, Class_> class_maping;
  std::unordered_map<Symbol, Status> class_status;
  std::unordered_map<Symbol, std::deque<Class_> *> chain_maping;
  bool dfs(Symbol);
  void check_all();
  void check_main();
  void check_method();
  ostream &semant_error(Class_ c, tree_node *t);
};

#endif
