#include <Nodes.h>

bool SequenceNode::is_same(const Node* other) {
    if (!Node::is_same(other))
    {
        return false;
    }

    SequenceNode* ot = (SequenceNode*)(other);

    if (ot->nodes.size() != this->nodes.size())
    {
        return false;
    }

    for (int i = 0; i < nodes.size(); i++)
    {
        if (!this->nodes[i]->is_same(ot->nodes[i].get()))
        {
            return false;
        }
    }

    return true;
}

std::string SequenceNode::to_string()
{
    std::string out = "";

    if (nodes.empty())
    {
        return out;
    }

    out += nodes.front()->to_string();

    if (nodes.size() == 1)
    {
        return out;
    }

    for (int i = 1; i < nodes.size(); i++)
    {
        out += "\n" + nodes.at(i)->to_string();
    }

    return out;
}

bool DeclareVariableNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    DeclareVariableNode* ot = (DeclareVariableNode*)other;

    if(!(this->vartype == ot->vartype && this->name == ot->name))
    {
        return false;
    }
    
    if(!this->expression.get() || !ot->expression.get())
    {
        if (this->expression.get() || ot->expression.get())
        {
            return false;
        }

        return true;
    }
    
    return this->expression->is_same(ot->expression.get());
}

std::string DeclareVariableNode::to_string()
{
    std::string out = "";
    
    switch(vartype.type)
    {
    case Type::INTEGER:
        out = "int";
        break;
    case Type::FLOAT:
        out = "float";
        break;
    case Type::BOOL:
        out = "bool";
        break;
    case Type::VOID:
        out = "void";
        break;
    }

    out += std::string(" ") + name;

    if (!expression.get())
    {
        return out;
    }

    return out + std::string(" = ") + expression->to_string();
}

bool VariableNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    VariableNode* ot = (VariableNode*)other;

    return this->name == ot->name && this->vartype == ot->vartype;
}

std::string VariableNode::to_string()
{
    return name;
}

bool IntNumNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    IntNumNode* ot = (IntNumNode*)other;

    return this->val == ot->val;
}

std::string IntNumNode::to_string()
{
    return std::to_string(val);
}

bool FloatNumNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    FloatNumNode* ot = (FloatNumNode*)other;

    return fabs(this->val - ot->val) < 0.0001f;
}

std::string FloatNumNode::to_string()
{
    return std::to_string(val);
}

bool BinaryNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    BinaryNode* ot = (BinaryNode*)other;

    if (!this->left.get())
    {
        return !ot->left.get();
    }

    if (!this->right.get())
    {
        return !ot->right.get();
    }

    return this->left->is_same(ot->left.get()) && this->right->is_same(ot->right.get());
}

std::string BinaryNode::to_string()
{
    return std::string("(") + left->to_string() + std::string(" ") + op + std::string(" ") + right->to_string() + std::string(")");
}

bool SumNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    SumNode* ot = (SumNode*)other;

    if (!this->left.get())
    {
        return !ot->left.get();
    }

    if (!this->right.get())
    {
        return !ot->right.get();
    }

    return (this->left->is_same(ot->left.get()) && this->right->is_same(ot->right.get())) || (this->left->is_same(ot->right.get()) && this->right->is_same(ot->left.get()));
}

bool MultiplicationNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    MultiplicationNode* ot = (MultiplicationNode*)other;

    if (!this->left.get())
    {
        return !ot->left.get();
    }

    if (!this->right.get())
    {
        return !ot->right.get();
    }

    return (this->left->is_same(ot->left.get()) && this->right->is_same(ot->right.get())) || (this->left->is_same(ot->right.get()) && this->right->is_same(ot->left.get()));
}

bool PrintNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    PrintNode* ot = (PrintNode*)other;

    if (ot->expressions.size() != this->expressions.size())
    {
        return false;
    }

    for (int i = 0; i < this->expressions.size(); i++)
    {
        if(!this->expressions[i]->is_same(ot->expressions[i].get()))
        {
            return false;
        }
    }
    
    return true;
}

std::string PrintNode::to_string()
{
    std::string out = "print(";

    if (expressions.empty())
    {
        return out + std::string(")");
    }

    out += expressions.front()->to_string();

    if (expressions.size() == 1)
    {
        return out + std::string(")");
    }

    for (int i = 1; i < expressions.size(); i++)
    {
        out += std::string(", ") + expressions.at(i)->to_string();
    }

    return out + std::string(")");
}

bool BranchingNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    BranchingNode* bn = (BranchingNode*)other;

    if (!this->statement->is_same(bn->statement.get()))
    {
        return false;
    }

    if (!this->if_body->is_same(bn->if_body.get()))
    {
        return false;
    }

    if (!this->else_body.get() && !bn->else_body.get())
    {
        return true;
    }

    if (this->else_body.get() && bn->else_body.get())
    {
        return this->else_body->is_same(bn->else_body.get());
    }

    return false;
}

std::string BranchingNode::to_string()
{
    std::string out = "if (";

    out += statement->to_string() + std::string(")\n{\n") + if_body->to_string() + std::string("\n}");

    if (!else_body.get())
    {
        return out;
    }

    return out + std::string("\nelse\n{\n") + else_body->to_string() + std::string("\n}");
}

bool ForCycleNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    ForCycleNode* fcn = (ForCycleNode*)other;

    if (!this->init->is_same(fcn->init.get()))
    {
        return false;
    }

    if (!this->condition->is_same(fcn->condition.get()))
    {
        return false;
    }

    if (!this->step->is_same(fcn->step.get()))
    {
        return false;
    }

    if (!this->body->is_same(fcn->body.get()))
    {
        return false;
    }

    return true;
}

std::string ForCycleNode::to_string()
{
    std::string out = "for (";
    
    out += init->to_string() + "; " + condition->to_string() + "; " + step->to_string() + ")\n";
    out += "{\n" + body->to_string() + "\n}";

    return out;
}

bool WhileCycleNode::is_same(const Node* other)
{
    if (!Node::is_same(other))
    {
        return false;
    }

    WhileCycleNode* wn = (WhileCycleNode*)other;

    if (!this->condition->is_same(wn->condition.get()))
    {
        return false;
    }

    if (!this->body->is_same(wn->body.get()))
    {
        return false;
    }

    return true;
}

std::string WhileCycleNode::to_string()
{
    std::string out = "while (";
    
    out += condition->to_string() + ")\n";
    out += "{\n" + body->to_string() + "\n}";

    return out;
}