#ifndef PARSER_H
#define PARSER_H

#include <Lexer.h>
#include <Nodes.h>

#include <unordered_map>
#include <string> // for mem_copy()

class ParserException
{
public:
    ParserException(std::string err = "") : err(err) {}

    const std::string& what() const { return err; }
protected:
    std::string err;
};

class UnexpectedTokenError : public ParserException
{
public:
    UnexpectedTokenError(Token t, Token expected = Token());
};

class UndefinedNameError : public ParserException
{
public:
    UndefinedNameError(Token t) : ParserException("Undefined name \'" + t.sval + "\' at" + std::to_string(t.line) + " line, " + std::to_string(t.chr) + " column") {}
};

class VariableTypeError : public ParserException
{
public:
    VariableTypeError(Type type1, Type type2, Token t) : ParserException("No conversion from " + std::to_string(type1.type) + "type to " + std::to_string(type2.type) + " type at" + std::to_string(t.line) + " line, " + std::to_string(t.chr) + " column") {}
};

class EndOfFileError : public ParserException
{
public:
    EndOfFileError() : ParserException("End of file!") {}
};

class Parser
{
public:
    Parser(std::vector<Token> tokens);
    ~Parser();

    std::shared_ptr<Node> make_node();

    std::shared_ptr<SequenceNode> make_tree(const std::unordered_map<std::string, std::shared_ptr<Type>>& variables = std::unordered_map<std::string, std::shared_ptr<Type>>());
    std::shared_ptr<DeclareVariableNode> make_variable();
    std::shared_ptr<PrintNode> make_print();
    std::shared_ptr<Node> make_expression(Type type);
    std::shared_ptr<AssignNode> make_assign();
    std::shared_ptr<BranchingNode> make_branching();
    std::shared_ptr<ForCycleNode> make_for_cycle();
    std::shared_ptr<WhileCycleNode> make_while_cycle();

    std::shared_ptr<Node> logical_expr();
    std::shared_ptr<Node> logical_term();
    std::shared_ptr<Node> logical_fact();

    std::shared_ptr<Node> expr();
    std::shared_ptr<Node> term();
    std::shared_ptr<Node> fact();

    void advance();

    Type get_var(const std::string& name) const;
private:
    const Token& cur_token() const { return tokens[position]; }
    const std::shared_ptr<SequenceNode> cur_sequence_node() const { return sequence_nodes.front(); }

    void expect_tokens(const std::vector<int>& tokens_types);
    Token expect_token(int token_type);

    int position;
    std::vector<Token> tokens;
    std::vector<std::shared_ptr<SequenceNode>> sequence_nodes;

    enum VariableTypes
    {
        INTEGER = 0,
        FLOAT,

        TYPE_COUNT
    };
};

#endif /* PARSER_H */