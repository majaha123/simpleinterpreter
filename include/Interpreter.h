#ifndef INTERPRETER_H
#define INTERPRETER_H

#include<Parser.h>
#include<map>

class InterpreterException
{
public:
    InterpreterException(const std::string& message) : message(message) {}

    const std::string& what() const { return message; }
private:
    std::string message;
};

class ZeroDivisionError : public InterpreterException
{
public:
    ZeroDivisionError() : InterpreterException("Zero division error") {}
};

class Variable
{
public:
    Variable() {}
    Variable(int ival) : ival(ival) {}
    Variable(float fval) : fval(fval) {}
    Variable(const std::string& sval) : sval(sval) {}

    Variable(const Variable& other) { this->ival = other.ival; this->fval = other.fval; this->sval = other.sval; }

    Variable operator=(const Variable& other)
    {
        Variable out;

        out.ival = this->ival = other.ival;
        out.fval = this->fval = other.fval;
        out.sval = this->sval = other.sval;

        return out;
    }

    int ival;
    float fval;
    std::string sval;
};

typedef std::unordered_map<std::string, std::shared_ptr<Variable>> NameField;

class Interpreter
{
public:
    Interpreter();
    ~Interpreter();

    void run(const Node* node);
    void run_sequence(const SequenceNode* sn);
    void run_print(const PrintNode* pn);
    void run_assign(const AssignNode* an);
    void run_branching(const BranchingNode* bn);
    void run_for_cycle(const ForCycleNode* fcn);
    void run_while_cycle(const WhileCycleNode* wcn);

    void decl_var(const DeclareVariableNode* dvn);
    float calc_expr(const Node* node);
    bool calc_logic(const Node* node);

    const NameField& get_current_name_field() const { return namefields.front(); }
    NameField& get_current_name_field() { return namefields.front(); }

    const Variable& get_var(const std::string& name) const { return *get_current_name_field().at(name).get(); }

    void init_var(const std::string& name) { get_current_name_field()[name] = std::shared_ptr<Variable>(new Variable()); }
    void set_var_int_val(const std::string& name, int val) { get_current_name_field().at(name)->ival = val; };
    void set_var_float_val(const std::string& name, float val) { get_current_name_field().at(name)->fval = val; };
    void set_var_string_val(const std::string& name, const std::string& val) { get_current_name_field().at(name)->sval = val; };
private:
    std::vector<NameField> namefields;
};


#endif /* INTERPRETER_H */