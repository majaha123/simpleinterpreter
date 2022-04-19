#include <Parser.h>

UnexpectedTokenError::UnexpectedTokenError(Token t, Token expected)
{
    err = "Unexpected token " + t.to_string() + " at " + std::to_string(t.line) + " line, " + std::to_string(t.chr) + " column";

    if (expected.type != Token::EMPTY)
    {
        err += ". Expected " + expected.to_string();
    }
}

Parser::Parser(std::vector<Token> tokens) : position(0), tokens(tokens)
{

}

Parser::~Parser()
{

}

std::shared_ptr<Node> Parser::make_node()
{
    switch(cur_token().type)
    {
    case Token::INTTYPE:
    case Token::FLOATTYPE:
        return make_variable();
        break;
    case Token::PRINT:
        return make_print();
        break;
    case Token::STRING:
        return make_assign();
        break;
    case Token::IF:
        return make_branching();
        break;
    case Token::FOR:
        return make_for_cycle();
        break;
    case Token::WHILE:
        return make_while_cycle();
        break;
    }

    return std::shared_ptr<Node>(0);
}

std::shared_ptr<SequenceNode> Parser::make_tree(const std::unordered_map<std::string, std::shared_ptr<Type>>& variables)
{
    std::shared_ptr<SequenceNode> s_node(new SequenceNode({}, variables));

    sequence_nodes.push_back(s_node);

    while(position < tokens.size() && cur_token().type != Token::RBRACE)
    {
        std::shared_ptr<Node> n = make_node();

        if (n)
        {
            s_node->nodes.push_back(n);
        }

        try
        {
            expect_token(Token::SEMICOLON);
        }
        catch (const EndOfFileError& e)
        {
            return s_node;
        }
    }

    return s_node;
}

std::shared_ptr<DeclareVariableNode> Parser::make_variable()
{
    Type vartype;
    std::string name = "";
    std::shared_ptr<Node> expression = 0;

    switch(cur_token().type)
    {
    case Token::INTTYPE:
        vartype.type = Type::INTEGER;
        break;
    case Token::FLOATTYPE:
        vartype.type = Type::FLOAT;
        break;
    default:
        throw UnexpectedTokenError(cur_token());
        break;
    }

    advance();

    name = expect_token(Token::STRING).sval;

    if (cur_token().type == Token::EQUAL)
    {
        advance();

        expression = make_expression(vartype);
    }

    std::shared_ptr<Type> t(new Type);

    memcpy(t.get(), &vartype, sizeof(Type));

    cur_sequence_node()->variables[name] = t;

    return std::shared_ptr<DeclareVariableNode>(new DeclareVariableNode(vartype, name, expression));
}

std::shared_ptr<PrintNode> Parser::make_print()
{
    std::shared_ptr<PrintNode> out(new PrintNode());

    expect_token(Token::PRINT);

    expect_token(Token::LPARENTHESIS);

    while(cur_token().type != Token::RPARENTHESIS)
    {
        switch (cur_token().type)
        {
        case Token::INTEGER:
        case Token::FLOAT:
        case Token::LPARENTHESIS:
            out->expressions.push_back(make_expression(Type(Type::FLOAT)));
            break;
        case Token::STRING:
        {
            Type vartype = get_var(cur_token().sval);

            out->expressions.push_back(make_expression(vartype));
        }
            break;
        default:
            throw UnexpectedTokenError(cur_token());
            break;
        }

        expect_tokens({ Token::COMMA, Token::RPARENTHESIS });

        if (cur_token().type == Token::COMMA)
        {
            advance();
        }
    }

    advance();

    if (out->expressions.empty()) // print()
    {
        throw ParserException("Expected expression inside \'print\' function");
    }

    return out;
}

std::shared_ptr<Node> Parser::make_expression(Type type)
{
    switch(type.type)
    {
    case Type::INTEGER:
    case Type::FLOAT:
        return expr();
        break;
    case Type::BOOL:
        return logical_expr();
        break;
    }
}

std::shared_ptr<AssignNode> Parser::make_assign()
{
    Token t = expect_token(Token::STRING);

    Type vartype;
    std::string name = t.sval;

    try
    {
        vartype = get_var(name);
    }
    catch(const std::exception& e)
    {
        throw UndefinedNameError(t);
    }

    expect_token(Token::EQUAL);

    return std::shared_ptr<AssignNode>(new AssignNode(std::shared_ptr<Node>(new VariableNode(name, vartype)), make_expression(vartype)));
}

std::shared_ptr<BranchingNode> Parser::make_branching()
{
    expect_token(Token::IF);

    expect_token(Token::LPARENTHESIS);

    std::shared_ptr<Node> statement = make_expression(BOOL_TYPE);

    expect_token(Token::RPARENTHESIS);

    expect_token(Token::LBRACE);

    std::shared_ptr<Node> if_body = make_tree();

    expect_token(Token::RBRACE);

    expect_token(Token::ELSE);

    expect_token(Token::LBRACE);

    std::shared_ptr<Node> else_body = make_tree();

    expect_token(Token::RBRACE);

    return std::shared_ptr<BranchingNode>(new BranchingNode(statement, if_body, else_body));
}

std::shared_ptr<ForCycleNode> Parser::make_for_cycle()
{
    expect_token(Token::FOR);

    expect_token(Token::LPARENTHESIS);

    std::shared_ptr<Node> init = make_node();

    expect_token(Token::SEMICOLON);

    std::shared_ptr<Node> condition = make_expression(BOOL_TYPE);

    expect_token(Token::SEMICOLON);

    std::shared_ptr<Node> step = make_node();

    expect_token(Token::RPARENTHESIS);

    expect_token(Token::LBRACE);

    std::shared_ptr<SequenceNode> body = make_tree();

    expect_token(Token::RBRACE);

    return std::shared_ptr<ForCycleNode>(new ForCycleNode(init, condition, step, body));
}

std::shared_ptr<WhileCycleNode> Parser::make_while_cycle()
{
    expect_token(Token::WHILE);

    expect_token(Token::LPARENTHESIS);

    std::shared_ptr<Node> condition = make_expression(BOOL_TYPE);
    
    expect_token(Token::RPARENTHESIS);

    expect_token(Token::LBRACE);

    std::shared_ptr<SequenceNode> body = make_tree();

    expect_token(Token::RBRACE);

    return std::shared_ptr<WhileCycleNode>(new WhileCycleNode(condition, body));
}

std::shared_ptr<Node> Parser::logical_expr()
{
    std::shared_ptr<Node> result = logical_term();

    while (cur_token().type == Token::GREATER || cur_token().type == Token::GOQ || 
    cur_token().type == Token::LESSER || cur_token().type == Token::LOQ || 
    cur_token().type == Token::EQEQ)
    {
        switch (cur_token().type)
        {
        case Token::EQEQ:
            advance();
            result = std::shared_ptr<Node>(new EqualsNode(result, logical_term()));
            break;
        case Token::GREATER:
            advance();
            result = std::shared_ptr<Node>(new GreaterNode(result, logical_term()));
            break;
        case Token::GOQ:
            advance();
            result =  std::shared_ptr<Node>(new GOQNode(result, logical_term()));
            break;
        case Token::LESSER:
            advance();
            result =  std::shared_ptr<Node>(new LesserNode(result, logical_term()));
            break;
        case Token::LOQ:
            advance();
            result =  std::shared_ptr<Node>(new LOQNode(result, logical_term()));
            break;
        default:
            break;
        }
    }

    return result;
}

std::shared_ptr<Node> Parser::logical_term()
{
    std::shared_ptr<Node> result = logical_fact();

    while (cur_token().type == Token::AND || cur_token().type == Token::OR || cur_token().type == Token::ANDAND || cur_token().type == Token::OROR)
    {
        switch (cur_token().type)
        {
        case Token::AND: case Token::ANDAND:
            advance();
            result = std::shared_ptr<Node>(new ANDNode(result, logical_fact()));
            break;
        case Token::OR: case Token::OROR:
            advance();
            result = std::shared_ptr<Node>(new ORNode(result, logical_fact()));
            break;
        default:
            break;
        }
    }

    return result;
}

std::shared_ptr<Node> Parser::logical_fact()
{
    Token t = cur_token();

    if (cur_token().type == Token::LPARENTHESIS)
    {
        advance();

        std::shared_ptr<Node> result = logical_expr();

        expect_token(Token::RPARENTHESIS);

        return result;
    }
    
    return expr();
}

std::shared_ptr<Node> Parser::expr()
{
    std::shared_ptr<Node> result = term();

    while (cur_token().type == Token::PLUS || cur_token().type == Token::MINUS)
    {
        switch (cur_token().type)
        {
        case Token::PLUS:
            advance();
            result = std::shared_ptr<Node>(new SumNode(result, term()));
            break;
        case Token::MINUS:
            advance();
            result = std::shared_ptr<Node>(new SubtractNode(result, term()));
            break;
        default:
            break;
        }
    }

    return result;
}

std::shared_ptr<Node> Parser::term()
{
    std::shared_ptr<Node> result = fact();

    while (cur_token().type == Token::ASTERISK || cur_token().type == Token::SLASH)
    {
        switch (cur_token().type)
        {
        case Token::ASTERISK:
            advance();
            result = std::shared_ptr<Node>(new MultiplicationNode(result, fact()));
            break;
        case Token::SLASH:
            advance();
            result = std::shared_ptr<Node>(new DivisionNode(result, fact()));
            break;
        default:
            break;
        }
    }

    return result;
}

std::shared_ptr<Node> Parser::fact()
{
    Token t = cur_token();

    if (t.type == Token::LPARENTHESIS)
    {
        advance();

        std::shared_ptr<Node> result = expr();

        expect_token(Token::RPARENTHESIS);

        return result;
    }
    else if (t.type == Token::INTEGER || t.type == Token::FLOAT)
    {
        advance();
        switch (t.type)
        {
        case Token::INTEGER:
            return std::shared_ptr<Node>(new IntNumNode(t.ival));
            break;
        case Token::FLOAT:
            return std::shared_ptr<Node>(new FloatNumNode(t.fval));
            break;
        default:
            break;
        }
    }
    else if (t.type == Token::STRING)
    {
        advance();

        Type vartype = get_var(t.sval);

        if (vartype.type == Type::INTEGER || vartype.type == Type::FLOAT)
        {
            return std::shared_ptr<Node>(new VariableNode(t.sval, vartype));
        }
        else
        {
            throw VariableTypeError(Type(Type::FLOAT), vartype, t);
        }
    }

    throw UnexpectedTokenError(t);
}

void Parser::advance()
{
    position++;

    if (position >= tokens.size())
    {
        throw EndOfFileError();
    }
}

Type Parser::get_var(const std::string& name) const
{
    std::shared_ptr<Type> vartype;

    try
    {
        vartype = cur_sequence_node()->variables.at(name);
    }
    catch(const std::exception& e)
    {
        throw UndefinedNameError(cur_token());
    }
    
    return *vartype.get();
}

void Parser::expect_tokens(const std::vector<int>& tokens_types)
{
    for (auto i : tokens_types)
    {
        if (cur_token().type == i)
        {
            return;
        }
    }

    throw UnexpectedTokenError(cur_token());
}

Token Parser::expect_token(int token_type)
{
    if (cur_token().type != token_type)
    {
        if (token_type == Token::STRING)
        {
            throw UnexpectedTokenError(cur_token());
        }

        throw UnexpectedTokenError(cur_token(), Token(token_type));
    }

    Token t = cur_token();

    advance();

    return t;
}