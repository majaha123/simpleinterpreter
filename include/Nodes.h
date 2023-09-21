#ifndef NODES_H
#define NODES_H

#include <memory>
#include <iostream>
#include <cmath> // for abs()
#include <vector>
#include <unordered_map>

#define DECL_TYPE(enumval) const Type enumval##_TYPE(Type::enumval)

struct Type
{
    Type(const Type& other) { this->is_const = other.is_const; this->type = other.type; }
    Type() : is_const(false), type() {}
    explicit Type(int vartype, bool is_const = false) : type(vartype), is_const(is_const) {}

    bool operator==(const Type& other) { return this->is_const == other.is_const && this->type == other.type; }
    const Type& operator=(const Type& other)
    {
        this->is_const = other.is_const;
        this->type = other.type;

        return other;
    }

    bool is_num() const
    {
        return type == Type::INTEGER || type == Type::FLOAT;
    }

    bool is_const;
    int type;

    enum Types
    {
        INTEGER,
        FLOAT,
        BOOL,

        VOID
    };
};

DECL_TYPE(INTEGER);
DECL_TYPE(FLOAT);
DECL_TYPE(BOOL);
DECL_TYPE(VOID);

class Node 
{
public:
    Node(int type) : type(type) {}

    virtual bool is_same(const Node* other) 
    { 
        if (!other)
        {
            return false;
        }

        return this->type == other->type;
    }

    virtual std::string to_string()
    {
        return "";
    }

    int type;

    enum Types
    {
        SUM = 0,
        SUBTRACT,
        DIVISION,
        MULTIPLICATION,
        ASSIGN,

        INTEGER,
        FLOAT,

        DECLVAR,
        VAR,

        PRINT,

        SEQUENCE,
        BRANCHING,
        FORCYCLE, WHILECYCLE,

        EQUALS,
        GREATER, GOQ,
        LESSER, LOQ,
        OR, AND
    };
};

class SequenceNode : public Node
{
public:
    SequenceNode(const std::vector<std::shared_ptr<Node>>& nodes = {}, const std::unordered_map<std::string, std::shared_ptr<Type>>& variables = std::unordered_map<std::string, std::shared_ptr<Type>>()) : Node(SEQUENCE), nodes(nodes), variables(variables) {};
    ~SequenceNode() { nodes.clear(); };

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    std::vector<std::shared_ptr<Node>> nodes;

    std::unordered_map<std::string, std::shared_ptr<Type>> variables;
};

class DeclareVariableNode : public Node
{
public:
    DeclareVariableNode(Type vartype, std::string name, std::shared_ptr<Node> expression = 0) : Node(DECLVAR), vartype(vartype), name(name), expression(expression) {}

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    Type vartype;
    std::string name;

    std::shared_ptr<Node> expression;
};

class VariableNode : public Node
{
public:
    VariableNode(std::string name, Type vartype) : Node(VAR), name(name), vartype(vartype) {}

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    std::string name;
    Type vartype;
};

class IntNumNode : public Node
{
public:
    IntNumNode(int val) : Node(INTEGER), val(val) {}

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    int val;
};

class FloatNumNode : public Node
{
public:
    FloatNumNode(float val) : Node(FLOAT), val(val) {}

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    float val;
};

class BinaryNode : public Node
{
public:
    BinaryNode(int type, std::shared_ptr<Node> left, std::shared_ptr<Node> right, std::string op) : Node(type), left(left), right(right), op(op) {}

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    std::string op;
};

class AssignNode : public BinaryNode
{
public:
    AssignNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(ASSIGN, left, right, "=") {}
};

class SumNode : public BinaryNode
{
public:
    SumNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(SUM, left, right, "+") {}

    virtual bool is_same(const Node* other);
};

class SubtractNode : public BinaryNode
{
public:
    SubtractNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(SUBTRACT, left, right, "-") {}
};

class MultiplicationNode : public BinaryNode
{
public:
    MultiplicationNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(MULTIPLICATION, left, right, "*") {}

    virtual bool is_same(const Node* other);
};

class DivisionNode : public BinaryNode
{
public:
    DivisionNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(DIVISION, left, right, "/") {}
};

class EqualsNode : public BinaryNode
{
public:
    EqualsNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(EQUALS, left, right, "=") {}
};

class GreaterNode : public BinaryNode
{
public:
    GreaterNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(GREATER, left, right, ">") {}
};

class LesserNode : public BinaryNode
{
public:
    LesserNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(LESSER, left, right, "<") {}
};

class GOQNode : public BinaryNode
{
public:
    GOQNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(GOQ, left, right, ">=") {}
};

class LOQNode : public BinaryNode
{
public:
    LOQNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(LOQ, left, right, "<=") {}
};

class ORNode : public BinaryNode
{
public:
    ORNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(OR, left, right, "||") {}
};

class ANDNode : public BinaryNode
{
public:
    ANDNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : BinaryNode(AND, left, right, "&&") {}
};

class PrintNode : public Node
{
public:
    PrintNode() : Node(PRINT) {}
    PrintNode(const std::vector<std::shared_ptr<Node>>& expressions) : Node(PRINT), expressions(expressions) {}

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    std::vector<std::shared_ptr<Node>> expressions;
};

class BranchingNode : public Node
{
public:
    BranchingNode(std::shared_ptr<Node> statement, std::shared_ptr<Node> if_body, std::shared_ptr<Node> else_body = 0) : Node(BRANCHING), statement(statement), if_body(if_body), else_body(else_body) {}

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    std::shared_ptr<Node> statement;
    std::shared_ptr<Node> if_body;
    std::shared_ptr<Node> else_body;
};

class ForCycleNode : public Node
{
public:
    ForCycleNode(std::shared_ptr<Node> init, std::shared_ptr<Node> condition, std::shared_ptr<Node> step, std::shared_ptr<Node> body) : Node(FORCYCLE),
    init(init), condition(condition), step(step), body(body) {}

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    std::shared_ptr<Node> init;
    std::shared_ptr<Node> condition;
    std::shared_ptr<Node> step;
    std::shared_ptr<Node> body;
};

class WhileCycleNode : public Node
{
public:
    WhileCycleNode(std::shared_ptr<Node> condition, std::shared_ptr<Node> body) : Node(WHILECYCLE), 
    condition(condition), body(body) {}

    virtual bool is_same(const Node* other);
    virtual std::string to_string();

    std::shared_ptr<Node> condition;
    std::shared_ptr<Node> body;
};

#endif /* NODES_H */