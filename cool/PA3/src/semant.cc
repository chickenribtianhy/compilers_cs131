

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol
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
/* dfs to root */
// bool ClassTable::dfs(Symbol x)
// {
//     class_status[x] = Visit_no;
//     // if (class_maping.count(class_maping[x]->get_father()))
//     auto _class_ = class_maping[x];
//     auto x_father = _class_->get_father();
//     if (class_maping.count(x_father) == 0)
//     {
//         class_status[x] = Visit_yes;
//         return false; /* can't fine father */
//     }
//     if (class_status.count(x_father) == 1 && class_status[x_father] == Visit_no)
//     {
//         semant_error(_class_) << "Class " << _class_->get_name() << " is involved in an inheritance cycle.\n";
//         temp = x_father;
//         return true;
//     }
//     else if (class_status.count(x_father) == 0)
//     {
//         if (dfs(x_father))
//         {
//             if (temp != No_class)
//                 semant_error(_class_) << "Class " << _class_->get_name() << " is involved in an inheritance cycle.\n";
//             if (temp == x)
//                 temp = No_class;
//             return true;
//         }
//     }

//     // class_status[x] = 1;
//     // return false;
// }

bool ClassTable::dfs(Symbol x)
{
    class_status[x] = Visit_no;
    if (class_maping.count(class_maping[x]->get_father()))
    {
        if (class_status.count(class_maping[x]->get_father()) && class_status[class_maping[x]->get_father()] == Visit_no)
        {
            // semant_error(class_maping[x]) << "Class " << class_maping[x]->get_name() << ", or an ancestor of " << class_maping[x]->get_name()
            //                               << ", is involved in an inheritance cycle.\n";
            temp = class_maping[x]->get_father();
            return true;
        }
        else if (class_status.count(class_maping[x]->get_father()) == 0)
        {
            if (dfs(class_maping[x]->get_father()))
            {
                // semant_error(class_maping[x]) << "faq "
                //                               << " is involved in an inheritance cycle.\n";
                if (temp != No_class)
                {
                }
                // semant_error(class_maping[x]) << "Class " << class_maping[x]->get_name() << ", or an ancestor of " << class_maping[x]->get_name() << " is involved in an inheritance cycle.\n";
                if (temp == x)
                    temp = No_class;
                return true;
            }
        }
    }
    class_status[x] = 1;
    return false;
}

ClassTable::ClassTable(Classes classes) : semant_errors(0), error_stream(cerr)
{
    /* initial basic classes */
    install_basic_classes();

    for (int i = classes->first(); classes->more(i); i = classes->next(i))
    // for (auto _class_ : *classes)
    {
        auto _class_ = classes->nth(i);
        auto x_name = _class_->get_name();     // symbol
        auto x_father = _class_->get_father(); // symbol
        if (x_name == Object || x_name == SELF_TYPE || x_name == IO || x_name == Int || x_name == Bool || x_name == Str)
            semant_error(_class_) << "Redefinition of basic class " << x_name << ".\n";
        else if (class_maping.count(x_name) == 1)
            semant_error(_class_) << "Class " << x_name << " was previously defined.\n";
        /* class_maping.count(x_name) == 0 */
        else if (x_father == SELF_TYPE || x_father == Int || x_father == Bool || x_father == Str)
            semant_error(_class_) << "Class " << x_name << " cannot inherit class " << x_father << ".\n";
        else
            /* success */
            class_maping[x_name] = _class_;
    }
    /* traverse the map */
    for (auto entry : class_maping)
    {
        auto _class_ = entry.second;
        auto x_name = _class_->get_name();     // symbol
        auto x_father = _class_->get_father(); // symbol
        // if (x_father == No_class || class_maping.count(x_father) == 0)
        if (x_father != No_class && class_maping.count(x_father) == 0)
            semant_error(_class_) << "Class " << x_name << " inherits from an undefined class " << x_father << ".\n";
    }

    /* check inheritence error */

    // /* reverse */
    std::map<Symbol, Class_>::reverse_iterator iter;
    for (iter = class_maping.rbegin(); iter != class_maping.rend(); iter++)
    // std::map<Symbol, Class_>::iterator iter;
    // for (iter = class_maping.begin(); iter != class_maping.end(); iter++)
    {
        auto entry = *iter;
        auto x_name = entry.first;
        // semant_error(entry.second) << "class_maping, " << x_name << ".\n";
        // if (class_status.count(x_name) == 0)
        {
            if (dfs(x_name))
            {
                semant_error(entry.second) << "Class " << entry.second->get_name() << ", or an ancestor of " << entry.second->get_name() << ", is involved in an inheritance cycle.\n";
            }
            // break;
        }
    }
}

void ClassTable::install_basic_classes()
{

    // The tree package uses these globals to annotate the classes built below.
    // curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");

    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.

    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    //
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    Class_ Object_class =
        class_(Object,
               No_class,
               append_Features(
                   append_Features(
                       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
                       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
                   single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
               filename);

    class_maping[Object_class->get_name()] = Object_class;

    //
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    Class_ IO_class =
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
               filename);
    class_maping[IO_class->get_name()] = IO_class;

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer.
    //
    Class_ Int_class =
        class_(Int,
               Object,
               single_Features(attr(val, prim_slot, no_expr())),
               filename);
    class_maping[Int_class->get_name()] = Int_class;

    //
    // Bool also has only the "val" slot.
    //
    Class_ Bool_class =
        class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())), filename);
    class_maping[Bool_class->get_name()] = Bool_class;

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //
    Class_ Str_class =
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
               filename);
    class_maping[Str_class->get_name()] = Str_class;
}

void ClassTable::check_main()
{
    // Check Main class
    if (!class_maping.count(Main))
        semant_error() << "Class Main is not defined.\n";
}

method_class *is_method(Feature x)
{
    /*  */
    return dynamic_cast<method_class *>(x);
}

// list_node refine

template <class Elem>
class list_iter : public std::iterator<std::random_access_iterator_tag, Elem>
{
private:
    int p;
    list_node<Elem> *cp;

public:
    list_iter(int x, list_node<Elem> *y) : p(x), cp(y) {}
    friend bool operator!=(list_iter<Elem> a, list_iter<Elem> b) { return a.p != b.p; }
    list_iter<Elem> operator++()
    {
        p++;
        return *this;
    }
    Elem operator*() { return cp->nth(p); }
};

void ClassTable::check_method()
{
    bool ra_method = false;
    for (auto entry : class_maping)
    {
        auto _class_ = entry.second;
        auto &methods = *(_class_->methods = new std::unordered_map<Symbol, Feature>);
        auto &attrs = *(_class_->attrs = new std::unordered_map<Symbol, Feature>);
        auto features = _class_->get_features();
        for (int i = features->first(); features->more(i); i = features->next(i))
        // for (auto feature : *_class_->get_features())
        {
            auto feature = features->nth(i);
            if (is_method(feature))
            {
                if (methods.count(feature->get_name()))
                    semant_error(_class_, feature) << "Method " << feature->get_name() << " is multiply defined.\n";
                methods[feature->get_name()] = feature;
                ra_method |= _class_->get_name() == Main && feature->get_name() == main_meth;
            }
            else /* attribute */
            {
                if (attrs.count(feature->get_name()))
                    semant_error(_class_, feature) << "Attribute " << feature->get_name() << " is multiply defined in class.\n";
                attrs[feature->get_name()] = feature; /* employ the last one */
            }
        }
        // Check Main method
        if (_class_->get_name() == Main && ra_method == false)
            semant_error(class_maping[Main]) << "No 'main' method in class Main.\n";
    }
}
Class_ __class;
ClassTable *classtable;

SymbolTable<Symbol, Symbol> attribtable;
SymbolTable<Symbol, method_class *> methodtable;

void ClassTable::check_all()
{
    check_method();
    check_main();
    for (auto entry : class_maping)
    {
        auto xname = entry.first;
        auto &chain = *(chain_maping[entry.first] = new std::deque<Class_>);
        for (auto i = xname; i != Object; i = class_maping[i]->get_father())
            chain.push_front(class_maping[i]);
        chain.push_front(class_maping[Object]);
    }
    for (auto entry : class_maping)
    {
        auto xname = entry.first;
        auto _class_ = entry.second;
        __class = _class_;
        auto &chain = *chain_maping[xname];
        if (xname == Object || xname == IO || xname == Int || xname == Bool || xname == Str)
            continue;
        for (auto _class_2 : chain)
        {
            if (_class_2 == chain.back())
                break;
            methodtable.enterscope();
            attribtable.enterscope();
            // for (auto feature : *_class_2->get_features())
            auto features = _class_2->get_features();
            for (int i = features->first(); features->more(i); i = features->next(i))
            {
                auto feature = features->nth(i);
                auto feature_name = feature->get_name();
                if (is_method(feature))
                {
                    if (!methodtable.lookup(feature_name))
                    {
                        methodtable.addid(feature_name, new (method_class *)(is_method(feature)));
                    }
                }
                else
                {
                    if (feature_name != self && !attribtable.lookup(feature_name))
                    {
                        attribtable.addid(feature_name, new Symbol(feature->get_type()));
                    }
                }
            }
        }
        // check redefined method and attr
        methodtable.enterscope();
        attribtable.enterscope();
        // for (auto feature : *_class_->get_features())
        // {
        auto features = _class_->get_features();
        for (int i = features->first(); features->more(i); i = features->next(i))
        {
            auto feature = features->nth(i);
            if (is_method(feature))
            {
                auto _method_ptr = methodtable.lookup(feature->get_name());
                if (_method_ptr)
                {
                    auto _method_1 = *_method_ptr;
                    auto _method_0 = is_method(feature);
                    if (_method_1->get_type() != _method_0->get_type())
                    {
                        semant_error(_class_, _method_0) << "In redefined method " << _method_0->get_name() << ", return type " << _method_0->get_type() << " is different from original return type " << _method_1->get_type() << ".\n";
                    }
                    // auto i = _method_1->get_formals()->begin();
                    auto i = _method_1->get_formals()->first();
                    // auto j = _method_0->get_formals()->begin();
                    auto j = _method_0->get_formals()->first();
                    // if ((i != _method_1->get_formals()->end()) ^ (j != _method_0->get_formals()->end()))
                    if ((_method_1->get_formals()->more(i)) ^ (_method_0->get_formals()->more(j)))
                    {
                        semant_error(_class_, _method_0) << "Incompatible number of formal parameters in redefined method " << _method_0->get_name() << ".\n";
                    }
                    // semant_error(_class_, _method_0) << "faq\t" << _method_0->get_name() << ", " << _method_1->get_name() << ".\n";
                    // semant_error(_class_, _method_0) << "faq\t" << _method_0->get_formals()->len() << ", " << _method_1->get_formals()->len() << ".\n";
                    while (_method_1->get_formals()->more(i) && _method_0->get_formals()->more(j))
                    {
                        if (_method_0->get_formals()->nth(j)->get_type() != _method_1->get_formals()->nth(i)->get_type())
                            semant_error(_class_, _method_0) << "In redefined method " << _method_0->get_name() << ", parameter type " << _method_0->get_formals()->nth(j)->get_type() << " is different from original type " << _method_1->get_formals()->nth(i)->get_type() << "\n";
                        ++i;
                        ++j;
                        if ((_method_1->get_formals()->more(i)) ^ (_method_0->get_formals()->more(j)))
                        {
                            semant_error(_class_, _method_0) << "Incompatible number of formal parameters in redefined method " << _method_0->get_name() << ".\n";
                        }
                    }
                }
                else
                    methodtable.addid(feature->get_name(), new (method_class *)(is_method(feature)));
            }
            else
            {
                auto _method_ptr = attribtable.lookup(feature->get_name());
                if (feature->get_name() == self)
                {
                    semant_error(_class_, feature) << "'self' cannot be the name of an attribute.\n";
                }
                else if (_method_ptr) /* attribute found */
                {
                    auto _method = *_method_ptr;
                    // if(_method)
                    // semant_error(_class_, feature) << "faq\t" << class_maping[_method] << " ? " << class_maping[feature->get_name()] << " \n";
                    semant_error(_class_, feature) << "Attribute " << feature->get_name() << " is an attribute of an inherited class.\n";
                }
                else
                    attribtable.addid(feature->get_name(), new Symbol(feature->get_type()));
            }
        }
        // for (auto feature : *_class_->get_features())
        // {
        features = _class_->get_features();
        for (int i = features->first(); features->more(i); i = features->next(i))
        {
            auto feature = features->nth(i);
            if (is_method(feature))
            {

                if (_class_->get_name() == Main && feature->get_name() == main_meth)
                {
                    if (is_method(feature)->get_formals()->len())
                        semant_error(_class_) << "'main' method in class Main should have no arguments.\n";
                }
                feature->_get_type();
            }
            else
            {
                feature->_get_type();
            }
        }
        for (auto _class_2 : chain)
        {
            attribtable.exitscope();
            methodtable.exitscope();
        }
    }
}

bool undefined_class(Symbol _class_)
{
    if (_class_ == SELF_TYPE)
        return false;
    if (classtable->class_maping.count(_class_) == 1)
        return false;
    return true;
    // return _class_ != SELF_TYPE && !classtable->class_maping.count(_class_);
}

bool is_less_equal(Symbol left, Symbol right)
{
    // cout << left << "<" << right << endl;
    if (left == right)
        return true;
    if (right == SELF_TYPE && left != SELF_TYPE)
        return false;

    Symbol new_left;
    Symbol new_right;
    if (left == SELF_TYPE)
        new_left = __class->get_name();
    else
        new_left = left;
    if (right == SELF_TYPE)
        new_right = __class->get_name();
    else
        new_right = right;

    if (classtable->class_maping.count(new_left) == 0)
        return false; /* not found */
    if (classtable->class_maping.count(new_right) == 0)
        return false;
    for (auto _class_ : *classtable->chain_maping[new_left])
    {
        if (_class_->get_name() == new_right)
            return true;
    }
    return false;
}

void method_class::_get_type()
{
    attribtable.enterscope();
    // for (auto formal : *formals)
    // {
    for (int i = formals->first(); formals->more(i); i = formals->next(i))
    {
        auto formal = formals->nth(i);
        bool intact = true;
        if (formal->get_type() == SELF_TYPE)
        {
            intact = false;
            classtable->semant_error(__class, formal) << "Formal parameter " << formal->get_name() << " cannot have type SELF_TYPE.\n";
        }
        if (formal->get_name() == self)
        {
            intact = false;
            classtable->semant_error(__class, formal) << "'self' cannot be the name of a formal parameter.\n";
        }
        if (undefined_class(formal->get_type()))
        {
            intact = false;
            classtable->semant_error(__class, formal) << "Class " << formal->get_type() << " of formal parameter " << formal->get_name() << " is undefined.\n";
        }
        if (attribtable.probe(formal->get_name()))
        {
            intact = false;
            classtable->semant_error(__class, formal) << "Formal parameter " << formal->get_name() << " is multiply defined.\n";
        }
        if (formal->get_type() == SELF_TYPE)
            attribtable.addid(formal->get_name(), new Symbol(Object));
        if (intact)
            attribtable.addid(formal->get_name(), new Symbol(formal->get_type()));
    }
    if (undefined_class(get_type()))
    {
        classtable->semant_error(__class, this) << "Undefined return type " << get_type() << " in method " << name << ".\n";
        auto _type = expr->_get_type();
    }
    else
    {
        auto _type = expr->_get_type();
        if (!is_less_equal(_type, get_type()))
            classtable->semant_error(__class, this) << "Inferred return type " << _type << " of method " << name << " does not conform to declared return type " << return_type << ".\n";
    }
    attribtable.exitscope();
}

void attr_class::_get_type()
{
    if (undefined_class(type_decl))
        classtable->semant_error(__class, this) << "Class " << type_decl << " of attribute " << name << " is undefined.\n";
    else
    {
        auto _type = init->_get_type();
        if (_type != No_type && !is_less_equal(_type, type_decl))
            classtable->semant_error(__class, this) << "Inferred type " << _type << " of initialization of attribute " << name << " does not conform to declared type " << type_decl << ".\n";
    }
}

Symbol branch_class::_get_type()
{
    attribtable.enterscope();
    if (undefined_class(type_decl))
        attribtable.addid(name, new Symbol(Object));
    else
        attribtable.addid(name, new Symbol(type_decl));
    auto type = expr->_get_type();
    attribtable.exitscope();
    return type;
}

Symbol assign_class::_get_type()
{
    if (name == self)
    {
        classtable->semant_error(__class, this) << "Cannot assign to 'self'.\n";
        return type = expr->_get_type();
    }
    if (!attribtable.lookup(name))
    {
        classtable->semant_error(__class, this) << "Assignment to undeclared variable " << name << ".\n";
        return type = expr->_get_type();
    }
    auto _type = expr->_get_type();
    if (!is_less_equal(_type, *attribtable.lookup(name)))
    {
        classtable->semant_error(__class, this) << "Type " << _type << " of assigned expression does not conform to declared type " << *attribtable.lookup(name) << " of identifier " << name << ".\n";
        return type = *attribtable.lookup(name);
    }
    return type = _type;
}
Symbol static_dispatch_class::_get_type()
{
    auto _type = expr->_get_type();
    if (type_name == SELF_TYPE)
    {
        classtable->semant_error(__class, this) << "Static dispatch to SELF_TYPE.\n";
        return type = Object;
    }
    if (undefined_class(type_name))
    {
        classtable->semant_error(__class, this) << "Static dispatch to undefined class " << type_name << ".\n";
        return type = Object;
    }
    if (undefined_class(_type))
        return type = Object;
    bool obj = false;
    if (!is_less_equal(_type, type_name))
    {
        classtable->semant_error(__class, this) << "Expression type " << _type << " does not conform to declared static dispatch type " << type_name << ".\n";
        obj = true;
    }
    method_class *target = 0;
    for (auto _class_ : *classtable->chain_maping[type_name])
    {
        if (_class_->methods->count(name))
            target = is_method((*_class_->methods)[name]);
    }
    if (!target)
    {
        classtable->semant_error(__class, this) << "Static dispatch to undefined method " << name << ".\n";
        return type = Object;
    }
    auto i = target->get_formals()->first();
    // auto i = target->get_formals()->begin();
    auto j = actual->first();
    // auto j = actual->begin();
    // if ((i != target->get_formals()->end()) ^ (j != actual->end()))
    if ((target->get_formals()->more(i)) ^ (actual->more(j)))
    {
        classtable->semant_error(__class, this)
            << "Method " << name << " invoked with wrong number of arguments.\n";
    }
    while (target->get_formals()->more(i) && (actual->more(j)))
    {
        auto realtype = actual->nth(j)->_get_type();
        if (!is_less_equal(realtype, target->get_formals()->nth(i)->get_type()))
            classtable->semant_error(__class, this) << "In call of method " << name << ", type " << realtype << " of parameter " << target->get_formals()->nth(i)->get_name() << " does not conform to declared type " << target->get_formals()->nth(i)->get_type() << ".\n";
        ++i;
        ++j;
        if ((target->get_formals()->more(i)) ^ (actual->more(j)))
        {
            classtable->semant_error(__class, this) << "Method " << name << " invoked with wrong number of arguments.\n";
        }
    }
    if (undefined_class(target->get_type()) || obj)
        return type = Object;
    if (target->get_type() == SELF_TYPE)
        return type = type_name;
    return type = target->get_type();
}

Symbol dispatch_class::_get_type()
{
    auto _type = expr->_get_type();
    if (undefined_class(_type))
    {
        classtable->semant_error(__class, this) << "Dispatch on undefined class " << _type << ".\n";
        return type = Object;
    }
    if (_type == SELF_TYPE)
        _type = __class->get_name();
    method_class *target = 0;
    for (auto _class_ : *classtable->chain_maping[_type])
    {
        if (_class_->methods->count(name))
            target = is_method((*_class_->methods)[name]);
    }
    if (!target)
    {
        classtable->semant_error(__class, this) << "Dispatch to undefined method " << name << ".\n";
        return type = Object;
    }

    auto i = target->get_formals()->first();
    // auto i = target->get_formals()->begin();
    auto j = actual->first();
    // auto j = actual->begin();
    if ((target->get_formals()->more(i)) ^ (actual->more(j)))
    // if ((i != target->get_formals()->end()) ^ (j != actual->end()))
    {
        classtable->semant_error(__class, this) << "Method " << name << " called with wrong number of arguments.\n";
    }
    while (target->get_formals()->more(i) && (actual->more(j)))
    // while (i != target->get_formals()->end() && j != actual->end())
    {
        auto realtype = actual->nth(j)->_get_type();
        // auto realtype = (*j)->_get_type();
        if (!is_less_equal(realtype, target->get_formals()->nth(i)->get_type()))
            // if (!is_less_equal(realtype, (*i)->get_type()))
            classtable->semant_error(__class, this) << "In call of method " << name << ", type " << realtype << " of parameter " << target->get_formals()->nth(i)->get_name() << " does not conform to declared type " << target->get_formals()->nth(i)->get_type() << ".\n";
        ++i;
        ++j;
        if ((target->get_formals()->more(i)) ^ (actual->more(j)))
        // if ((i != target->get_formals()->end()) ^ (j != actual->end()))
        {
            classtable->semant_error(__class, this)
                << "Method " << name << " called with wrong number of arguments.\n";
        }
    }
    if (undefined_class(target->get_type()))
        return type = Object;
    if (target->get_type() == SELF_TYPE)
    {
        if (_type == __class->get_name())
            return type = SELF_TYPE;
        else
            return type = _type;
    }
    return type = target->get_type();
}

Symbol lowest(Symbol a, Symbol b)
{
    if (a == b)
        return a;
    if (a == SELF_TYPE)
        a = __class->get_name();
    if (b == SELF_TYPE)
        b = __class->get_name();

    auto ret = Object;
    // for (auto i = classtable->chain_maping[a]->begin(), j = classtable->chain_maping[b]->begin();
    //      i != classtable->chain_maping[a]->end() && j != classtable->chain_maping[b]->end() &&
    //      (*i)->get_name() == (*j)->get_name();
    //      i++, j++)
    auto i = classtable->chain_maping[a]->begin();
    auto j = classtable->chain_maping[b]->begin();

    while (i != classtable->chain_maping[a]->end() && j != classtable->chain_maping[b]->end() && (*i)->get_name() == (*j)->get_name())
    {
        ret = (*i)->get_name();
        ++i;
        ++j;
    }
    return ret;
}

Symbol cond_class::_get_type()
{
    if (pred->_get_type() != Bool)
        classtable->semant_error(__class, this) << "Predicate of 'if' does not have type Bool.\n";
    return type = lowest(then_exp->_get_type(), else_exp->_get_type());
}
Symbol loop_class::_get_type()
{
    if (pred->_get_type() != Bool)
        classtable->semant_error(__class, this) << "Loop condition does not have type Bool.\n";
    body->_get_type();
    return type = Object;
}
Symbol typcase_class::_get_type()
{
    auto exptype = expr->_get_type();
    std::unordered_set<Symbol> types;
    type = No_type;

    // for (auto xbranch : *cases)
    for (int i = cases->first(); cases->more(i); i = cases->next(i))
    {
        auto xbranch = cases->nth(i);
        bool intact = true;
        auto branch = dynamic_cast<branch_class *>(xbranch);
        if (branch->get_name() == self)
        {
            classtable->semant_error(__class, branch) << "'self' bound in 'case'.\n";
        }
        if (branch->getdecltype() == SELF_TYPE) //faq
        {
            classtable->semant_error(__class, branch) << "Identifier " << branch->get_name() << " declared with type SELF_TYPE in case branch.\n";
            intact = false;
        }
        if (undefined_class(branch->getdecltype()))
        {
            classtable->semant_error(__class, branch) << "Class " << branch->getdecltype() << " of case branch is undefined.\n";
            intact = false;
        }
        if (types.count(branch->getdecltype()))
        {
            classtable->semant_error(__class, branch) << "Duplicate branch " << branch->getdecltype() << " in case statement.\n";
            intact = false;
        }
        if (intact)
            types.insert(branch->getdecltype());
        if (type == No_type)
            type = branch->_get_type();
        else
            type = lowest(type, branch->_get_type());
    }
    return type;
}
Symbol block_class::_get_type()
{
    // for (auto i : *body)
    for (int i = body->first(); body->more(i); i = body->next(i))

    {
        auto _block = body->nth(i);
        type = _block->_get_type();
    }
    return type;
}
// let
Symbol let_class::_get_type()
{
    attribtable.enterscope();
    // auto _type = init->_get_type();
    if (identifier == self)
    {
        classtable->semant_error(__class, this) << "'self' cannot be bound in a 'let' expression.\n";
    }
    else
    {
        attribtable.addid(identifier, new Symbol(type_decl));
    }

    if (undefined_class(type_decl))
    {
        classtable->semant_error(__class, this) << "Class " << type_decl << " of let-bound identifier " << identifier << " is undefined.\n";
        auto _type = init->_get_type();
    }
    else
    {
        auto _type = init->_get_type();
        if (_type != No_type && !is_less_equal(_type, type_decl))
        {
            classtable->semant_error(__class, this) << "Inferred type " << _type << " of initialization of " << identifier << " does not conform to identifier's declared type " << type_decl << ".\n";
        }
    }
    type = body->_get_type();
    attribtable.exitscope();
    return type;
}
// operator
Symbol plus_class::_get_type()
{
    auto _type1 = e1->_get_type();
    auto _type2 = e2->_get_type();
    if (_type1 != Int || _type2 != Int)
        classtable->semant_error(__class, this) << "non-Int arguments: " << _type1 << " + " << _type2 << endl;
    type = Int;
    return type;
}
Symbol sub_class::_get_type()
{
    auto _type1 = e1->_get_type();
    auto _type2 = e2->_get_type();
    if (_type1 != Int || _type2 != Int)
        classtable->semant_error(__class, this) << "non-Int arguments: " << _type1 << " - " << _type2 << endl;
    type = Int;
    return type;
}
Symbol mul_class::_get_type()
{
    auto _type1 = e1->_get_type();
    auto _type2 = e2->_get_type();
    if (_type1 != Int || _type2 != Int)
        classtable->semant_error(__class, this) << "non-Int arguments: " << _type1 << " * " << _type2 << endl;
    type = Int;
    return type;
}
Symbol divide_class::_get_type()
{
    auto _type1 = e1->_get_type();
    auto _type2 = e2->_get_type();
    if (_type1 != Int || _type2 != Int)
        classtable->semant_error(__class, this) << "non-Int arguments: " << _type1 << " / " << _type2 << endl;
    return type = Int;
}
Symbol neg_class::_get_type()
{
    auto _type = e1->_get_type();
    if (_type != Int)
        classtable->semant_error(__class, this) << "Argument of '~' has type " << _type << " instead of Int.\n";
    type = Int;
    return type;
}
Symbol lt_class::_get_type()
{
    auto _type1 = e1->_get_type();
    auto _type2 = e2->_get_type();
    if (_type1 != Int || _type2 != Int)
        classtable->semant_error(__class, this) << "non-Int arguments: " << _type1 << " < " << _type2 << endl;
    type = Bool;
    return type;
}
Symbol eq_class::_get_type() //=========
{
    auto _type1 = e1->_get_type();
    auto _type2 = e2->_get_type();
    if (_type1 == Int || _type2 == Int || _type1 == Bool || _type2 == Bool || _type1 == Str || _type2 == Str)
        if (_type1 != _type2)
        {
            // =============
            classtable->semant_error(__class, this) << "Illegal comparison with a basic type.\n";
        }
    type = Bool;
    return type;
}
Symbol leq_class::_get_type()
{
    auto _type1 = e1->_get_type();
    auto _type2 = e2->_get_type();
    if (_type1 != Int || _type2 != Int)
    {
        classtable->semant_error(__class, this) << "non-Int arguments: " << _type1 << " <= " << _type2 << endl;
    }
    type = Bool;
    return type;
}
Symbol comp_class::_get_type()
{
    auto _type = e1->_get_type();
    if (_type != Bool)
    {
        classtable->semant_error(__class, this) << "Argument of 'not' has type " << _type << " instead of Bool.\n";
    }
    type = Bool;
    return type;
}
Symbol int_const_class::_get_type()
{
    type = Int;
    return type;
}
Symbol bool_const_class::_get_type()
{
    type = Bool;
    return type;
}
Symbol string_const_class::_get_type()
{
    type = Str;
    return type;
}
Symbol new__class::_get_type()
{
    if (undefined_class(type_name))
    {
        classtable->semant_error(__class, this) << "'new' used with undefined class " << type_name << ".\n";
        type = Object;
        return type;
    }
    type = type_name;
    return type;
}
Symbol isvoid_class::_get_type()
{
    e1->_get_type();
    type = Bool;
    return type;
}

Symbol no_expr_class::_get_type()
{
    type = No_type;
    return No_type;
}

Symbol object_class::_get_type()
{
    if (name == self)
    {
        type = SELF_TYPE;
        return type;
    }
    Symbol *temp = NULL;
    temp = attribtable.lookup(name);
    if (temp != NULL)
    {
        // return type = *temp;
        type = *temp;
        return type;
    }
    else
    {
        type = Object;
        classtable->semant_error(__class, this) << "Undeclared identifier " << name << ".\n";
        return Object;
    }
}

/* ==============================line=================================== */

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
//       print a line number and filename
//
///////////////////////////////////////////////////////////////////

ostream &ClassTable::semant_error(Class_ c)
{
    error_stream << c->get_filename() << ":" << c->get_line_number() << ": ";
    ++semant_errors;
    return error_stream;
    // return semant_error(c->get_filename(), c);
}

ostream &ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    ++semant_errors;
    return error_stream;
}

ostream &ClassTable::semant_error(Class_ c, tree_node *t)
{
    error_stream << c->get_filename() << ":" << t->get_line_number() << ": ";
    ++semant_errors;
    return error_stream;
    // return semant_error(c->get_filename(), t);
}

ostream &ClassTable::semant_error()
{
    ++semant_errors;
    return error_stream;
}
/*   This is the entry point to the semantic checker.

     Your checker should do the following two things:

     1) Check that the program is semantically correct
     2) Decorate the abstract syntax tree with type information
        by setting the `type' field in each Expression node.
        (see `tree.h')

     You are free to first do 1), make sure you catch all semantic
     errors. Part 2) can be done in a second stage, when you want
     to build mycoolc.
 */
void program_class::semant()
{
    initialize_constants();

    /* ClassTable constructor may do some semantic analysis */
    classtable = new ClassTable(classes);

    /* some semantic analysis code may go here */

    if (classtable->errors())
    {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
    classtable->check_all(); //

    if (classtable->errors())
    {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}
