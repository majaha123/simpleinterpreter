#include <Lexer.h>

#include <iostream>
#include <cmath>
#include <sstream>

Token::Token(const Token& other)
{
    this->type = other.type;

    this->ival = other.ival;
    this->fval = other.fval;
    this->sval = other.sval;

    this->line = other.line;
    this->chr = other.chr;
}

Token Token::operator=(const Token& other)
{
    this->type = other.type;

    this->ival = other.ival;
    this->fval = other.fval;
    this->sval = other.sval;

    this->line = other.line;
    this->chr = other.chr;

    return other;
}

bool Token::is_same(const Token& other) const
{
    if(this->type != other.type)
    {
        return false;
    }

    switch(this->type)
    {
    case Token::INTEGER:
        return this->ival == other.ival;
        break;
    case Token::FLOAT:
        return fabs(this->fval - other.fval) < 0.001f;
        break;
    case Token::STRING:
        return this->sval == other.sval;
        break;
    default:
        return true;
        break;
    }
}

std::string Token::to_string() const
{
    switch (type)
    {
    case INTEGER: return std::to_string(ival);
    case FLOAT: return std::to_string(fval);
    case LBRACE: return "{";
    case RBRACE: return "}";
    case LSQUARE: return "[";
    case RSQUARE: return "]";
    case LPARENTHESIS: return "(";
    case RPARENTHESIS: return ")";
    case SEMICOLON: return ";";
    case COMMA: return ",";
    case PLUS: return "+";
    case MINUS: return "-";
    case EQUAL: return "=";
    case EQEQ: return "==";
    case GREATER: return ">";
    case LESSER: return "<";
    case GOQ: return ">=";
    case LOQ: return "<=";
    case OR: return "|";
    case AND: return "&";
    case OROR: return "||";
    case ANDAND: return "&&";
    case ASTERISK: return "*";
    case SLASH: return "/";
    case INTTYPE: return "int";
    case FLOATTYPE: return "float";
    case STRING: return sval;
    case PRINT: return "print";
    case IF: return "if";
    case ELSE: return "else";
    case FOR: return "for";
    case WHILE: return "while";
    default:
        break;
    }
}

Lexer::Lexer() : position(0), line(0), chr(0)
{
    keywords["int"] = Token::INTTYPE;
    keywords["float"] = Token::FLOATTYPE;
    keywords["print"] = Token::PRINT;
    keywords["if"] = Token::IF;
    keywords["else"] = Token::ELSE;
    keywords["for"] = Token::FOR;
    keywords["while"] = Token::WHILE;

    reload();
}

Lexer::~Lexer()
{
    
}

std::vector<Token> Lexer::make_tokens(const std::string& code)
{
    std::vector<Token> out = {};

    while(position < code.length())
    {

        while(is_empty(code[position]) && position < code.length())
        {
            advance(code);
        }

        if(position >= code.length())
        {
            return out;
        }

        out.push_back(get_token(code));
    }

    reload();

    return out;
}

Token Lexer::make_number(const std::string& code)
{
    std::string s;

    int point_count = 0;

    while(position < code.length() && (is_digit(code[position]) || code[position] == '.'))
    {
        if(code[position] == '.')
        {
            point_count++;
            if(point_count > 1)
            {
                break;
            }
        }

        s.push_back(code[position]);
        advance(code);
    }

    if(point_count >= 1)
    {
        return Token(std::stof(s), line, chr);
    }
    else
    {
        return IntegerToken(std::stoi(s), line, chr);
    }
}

void Lexer::advance(const std::string& code)
{
    position++;

    if(code[position] == '\n')
    {
        line++;
        chr = 0;
    }
    else
    {
        chr++;
    }
}

void Lexer::reload()
{
    position = 0;
    line = 0;
    chr = 0;
}

Token Lexer::get_token(const std::string& code)
{
    int type = -1;
    switch(code[position])
    {
    case '{':
        type = Token::LBRACE;
        break;
    case '}':
        type = Token::RBRACE;
        break;
    case '(':
        type = Token::LPARENTHESIS;
        break;
    case ')':
        type = Token::RPARENTHESIS;
        break;
    case '[':
        type = Token::LSQUARE;
        break;
    case ']':
        type = Token::RSQUARE;
        break;
    case ';':
        type = Token::SEMICOLON;
        break;
    case ',':
        type = Token::COMMA;
        break;
    case '+':
        type = Token::PLUS;
        break;
    case '-':
        type = Token::MINUS;
        break;
    case '=':
        advance(code);

        if(code[position] == '=')
        {
            advance(code);

            return Token(Token::EQEQ, line, chr);
        }

        return Token(Token::EQUAL, line, chr);
        break;
    case '>':
        advance(code);

        if(code[position] == '=')
        {
            advance(code);

            return Token(Token::GOQ, line, chr);
        }

        return Token(Token::GREATER, line, chr);
        break;
    case '<':
        advance(code);

        if(code[position] == '=')
        {
            advance(code);

            return Token(Token::LOQ, line, chr);
        }

        return Token(Token::LESSER, line, chr);
        break;
    case '&':
        advance(code);

        if(code[position] == '&')
        {
            advance(code);

            return Token(Token::ANDAND, line, chr);
        }

        return Token(Token::AND, line, chr);
        break;
    case '|':
        advance(code);

        if(code[position] == '|')
        {
            advance(code);

            return Token(Token::OROR, line, chr);
        }

        return Token(Token::OR, line, chr);
        break;
    case '*':
        type = Token::ASTERISK;
        break;
    case '/':
        type = Token::SLASH;
        break;
    }

    if(type != -1)
    {
        advance(code);
        return Token(type, line, chr);
    }

    if(is_digit(code[position]) || code[position] == '.')
    {
        return make_number(code);
    }

    std::string word = get_word(code);
    
    try
    {
        int code = keywords.at(word);

        return Token(code);
    }
    catch(const std::exception& e)
    {
        return Token(word);
    }
}

std::string Lexer::get_word(const std::string& code)
{
    std::string out;

    while(!is_empty(code[position]) && is_valid_for_naming(code[position]) && position < code.length())
    {
        out.push_back(code[position]);
        advance(code);
    }

    return out;
}

bool Lexer::is_number(const std::string& s)
{
    for(int i = 0; i < s.length(); i++)
    {
        if(!is_digit(s[i]))
            return false;
    }

    return true;
}