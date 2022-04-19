#include <Interpreter.h>

Interpreter::Interpreter()
{
    namefields.push_back(NameField());
}

Interpreter::~Interpreter()
{

}

void Interpreter::run(const Node* node)
{
    switch(node->type)
    {
    case Node::SEQUENCE:
        run_sequence((const SequenceNode*)node);
        break;
    case Node::DECLVAR:
        decl_var((const DeclareVariableNode*)node);
        break;
    case Node::ASSIGN:
        run_assign((const AssignNode*)node);
        break;
    case Node::PRINT:
        run_print((const PrintNode*)node);
        break;
    case Node::BRANCHING:
        run_branching((const BranchingNode*)node);
        break;
    case Node::FORCYCLE:
        run_for_cycle((const ForCycleNode*)node);
        break;
    case Node::WHILECYCLE:
        run_while_cycle((const WhileCycleNode*)node);
        break;
    }
}

void Interpreter::run_sequence(const SequenceNode* sq)
{
    namefields.push_back(get_current_name_field());

    for (int i = 0; i < sq->nodes.size(); i++)
    {
        run(sq->nodes.at(i).get());
    }

    namefields.pop_back();
}

void Interpreter::decl_var(const DeclareVariableNode* dvn)
{
    std::shared_ptr<Node> expression = dvn->expression;
    Type type = dvn->vartype;
    std::string name = dvn->name;

    init_var(name);

    if (expression.get())
    {
        switch(type.type)
        {
        case Type::INTEGER:
            set_var_int_val(name, int(calc_expr(expression.get())));
            break;
        case Type::FLOAT:
            set_var_float_val(name, calc_expr(expression.get()));
            break;
        }
    }
    else
    {
        switch(type.type)
        {
        case Type::INTEGER:
            set_var_int_val(name, 0);
            break;
        case Type::FLOAT:
            set_var_float_val(name, 0.f);
            break;
        }
    }
}

float Interpreter::calc_expr(const Node* node)
{
    switch(node->type)
    {
    case Node::INTEGER:
        return static_cast<const IntNumNode*>(node)->val;
        break;
    case Node::FLOAT:
        return static_cast<const FloatNumNode*>(node)->val;
        break;
    case Node::SUM:
        return calc_expr(static_cast<const SumNode*>(node)->left.get()) + calc_expr(static_cast<const SumNode*>(node)->right.get());
        break;
    case Node::SUBTRACT:
        return calc_expr(static_cast<const SubtractNode*>(node)->left.get()) - calc_expr(static_cast<const SubtractNode*>(node)->right.get());
        break;
    case Node::MULTIPLICATION:
        return calc_expr(static_cast<const MultiplicationNode*>(node)->left.get()) * calc_expr(static_cast<const MultiplicationNode*>(node)->right.get());
        break;
    case Node::DIVISION:
    {
        float rval = calc_expr(static_cast<const DivisionNode*>(node)->right.get());
        if (rval == 0.f)
        {
            throw ZeroDivisionError();
        }
        return calc_expr(static_cast<const SumNode*>(node)->left.get()) / rval;
        break;
    }
    case Node::VAR:
    {
        const VariableNode* vn = static_cast<const VariableNode*>(node);
        Variable v = get_var(vn->name);

        if (vn->vartype.type == Type::INTEGER)
        {
            return v.ival;
        }
        else if (vn->vartype.type == Type::FLOAT)
        {
            return v.fval;
        }
        else
        {
            throw InterpreterException("Not number variable");
        }
    }
        break;
    default:
        throw InterpreterException("Unknown exception");
        break;
    }
}

bool Interpreter::calc_logic(const Node* node)
{
    switch(node->type)
    {
    case Node::AND:
    {
        const ANDNode* n = static_cast<const ANDNode*>(node);

        return calc_logic(n->left.get()) && calc_logic(n->right.get());
    }
        break;
    case Node::OR:
    {
        const ORNode* n = static_cast<const ORNode*>(node);

        return calc_logic(n->left.get()) || calc_logic(n->right.get());
    }
        break;
    case Node::EQUALS:
    {
        const EqualsNode* n = static_cast<const EqualsNode*>(node);

        return calc_expr(n->left.get()) == calc_expr(n->right.get());
    }
        break;
    case Node::GREATER:
    {
        const GreaterNode* n = static_cast<const GreaterNode*>(node);

        return calc_expr(n->left.get()) > calc_expr(n->right.get());
    }
        break;
    case Node::GOQ:
    {
        const GOQNode* n = static_cast<const GOQNode*>(node);

        return calc_expr(n->left.get()) >= calc_expr(n->right.get());
    }
        break;
    case Node::LESSER:
    {
        const LesserNode* n = static_cast<const LesserNode*>(node);

        return calc_expr(n->left.get()) < calc_expr(n->right.get());
    }
        break;
    case Node::LOQ:
    {
        const LOQNode* n = static_cast<const LOQNode*>(node);

        return calc_expr(n->left.get()) <= calc_expr(n->right.get());
    }
        break;
    default:
        throw InterpreterException("Unknown exception: calc_logic()");
        break;
    }
}

void Interpreter::run_print(const PrintNode* pn)
{
    std::string out;

    for (int i = 0; i < pn->expressions.size(); i++)
    {
        Node* n = pn->expressions.at(i).get();

        switch(n->type)
        {
        case Node::INTEGER:
        case Node::FLOAT:
        case Node::SUM:
        case Node::SUBTRACT:
        case Node::MULTIPLICATION:
        case Node::DIVISION:
            out += std::to_string(calc_expr(n));
            break;
        case Node::VAR:
        {
            VariableNode* vn = static_cast<VariableNode*>(n);

            if(vn->vartype.type == Type::INTEGER || vn->vartype.type == Type::FLOAT)
            {
                out += std::to_string(calc_expr(vn));
            }
            else
            {
                throw InterpreterException("Not implemented err::run_print()");
            }
        }
            break;
        }

        out += ' ';
    }

    out += '\n';

    std::cout << out;
}

void Interpreter::run_assign(const AssignNode* an)
{
    Node* l = an->left.get();

    switch (l->type)
    {
    case Node::VAR:
    {
        const VariableNode* vn = static_cast<const VariableNode*>(l);
        const Variable& v = get_var(vn->name);

        if (vn->vartype.type == Type::INTEGER)
        {
            set_var_int_val(vn->name, int(calc_expr(an->right.get())));
        }
        else if (vn->vartype.type == Type::FLOAT)
        {
            set_var_float_val(vn->name, calc_expr(an->right.get()));
        }
        else
        {
            throw InterpreterException("Not number variable");
        }
        break;
    }
    default:
        throw InterpreterException("not implemented err::run_assign()");
        break;
    }
}

void Interpreter::run_branching(const BranchingNode* bn)
{
    bool val = calc_logic(bn->statement.get());

    if (val)
    {
        run(bn->if_body.get());
    }
    else
    {
        if (bn->else_body.get())
        {
            run(bn->else_body.get());
        }
    }
}

void Interpreter::run_for_cycle(const ForCycleNode* fcn)
{
    run(fcn->init.get());

    while(calc_logic(fcn->condition.get()))
    {
        run(fcn->body.get());
        run(fcn->step.get());
    }
}

void Interpreter::run_while_cycle(const WhileCycleNode* wcn)
{
    while(calc_logic(wcn->condition.get()))
    {
        run(wcn->body.get());
    }
}