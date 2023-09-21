#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <unordered_map>

#define DECL_TOKEN_TYPE(enum_val) const Token enum_val##_T(Token::enum_val)

class Token 
{
public:
    int type;
    int ival;
    float fval;
    std::string sval;

    int line;
    int chr;

    Token() : type(EMPTY) {}
    Token(int type, int line = 0, int chr = 0, int val = 0) : type(type), ival(val), line(line), chr(chr) {}
    Token(float val, int line = 0, int chr = 0) : type(FLOAT), fval(val), line(line), chr(chr) {}
    Token(const std::string& val, int line = 0, int chr = 0) : type(STRING), sval(val), line(line), chr(chr) {}

    Token(const Token& other);

    Token operator=(const Token& other);

    bool is_same(const Token& other) const;
    std::string to_string() const;

    enum Types
    {
        EMPTY = -1,
        INTEGER = 0,
        FLOAT,
        LBRACE,
        RBRACE,
        LSQUARE,
        RSQUARE,
        LPARENTHESIS,
        RPARENTHESIS,
        SEMICOLON,
        COMMA,
        PLUS,
        MINUS,
        EQUAL,
        EQEQ,
        ASTERISK,
        SLASH,

        INTTYPE,
        FLOATTYPE,
        
        STRING,

        PRINT,
        IF, ELSE,
        FOR, WHILE,
        LESSER, GREATER, 
        LOQ, // Lesser or equal
        GOQ, // Greater or equal
        OR, AND, OROR, ANDAND
    };
};

DECL_TOKEN_TYPE(COMMA);
DECL_TOKEN_TYPE(SEMICOLON);
DECL_TOKEN_TYPE(PLUS);
DECL_TOKEN_TYPE(MINUS);
DECL_TOKEN_TYPE(EQUAL);
DECL_TOKEN_TYPE(ASTERISK);
DECL_TOKEN_TYPE(SLASH);

DECL_TOKEN_TYPE(LBRACE);
DECL_TOKEN_TYPE(RBRACE);
DECL_TOKEN_TYPE(LSQUARE);
DECL_TOKEN_TYPE(RSQUARE);
DECL_TOKEN_TYPE(LPARENTHESIS);
DECL_TOKEN_TYPE(RPARENTHESIS);

DECL_TOKEN_TYPE(PRINT);

DECL_TOKEN_TYPE(IF);
DECL_TOKEN_TYPE(ELSE);
DECL_TOKEN_TYPE(FOR);
DECL_TOKEN_TYPE(WHILE);
DECL_TOKEN_TYPE(EQEQ);
DECL_TOKEN_TYPE(LESSER);
DECL_TOKEN_TYPE(GREATER);
DECL_TOKEN_TYPE(LOQ);
DECL_TOKEN_TYPE(GOQ);
DECL_TOKEN_TYPE(OR);
DECL_TOKEN_TYPE(AND);
DECL_TOKEN_TYPE(OROR);
DECL_TOKEN_TYPE(ANDAND);

class IntegerToken : public Token 
{
public:
    IntegerToken(int val, int line = 0, int chr = 0) : Token(Token::INTEGER, line, chr, val) {};
};

class Lexer
{
private:
    int position;

    int line;
    int chr;

    std::unordered_map<std::string, int> keywords;
public:
    Lexer();
    ~Lexer();

    std::vector<Token> make_tokens(const std::string& code);

    Token make_number(const std::string& code);

    void advance(const std::string& code);
    void reload();

    Token get_token(const std::string& code);
    std::string get_word(const std::string& code);

    bool is_empty(char c) const { return c == ' ' || c == '\n' || c == '\t'; }
    bool is_digit(char c) const { return c >= '0' && c <= '9'; }
    bool is_valid_for_naming(char c) const { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';}
    bool is_number(const std::string& s);
};


#endif /* LEXER_H */