#include <Lexer.h>
#include <gtest/gtest.h>

#define LEXER_TESTING_MACRO ASSERT_EQ(result.size(),expected.size());\
    for(int i = 0; i < expected.size(); i++)\
    {\
        ASSERT_TRUE(expected[i].is_same(result[i]));\
    }

TEST(LEXER_UTILITY, L_UTILITY)
{
    Lexer l;

    ASSERT_TRUE(l.is_empty('\n'));
    ASSERT_TRUE(l.is_empty(' '));
    ASSERT_TRUE(l.is_empty('\t'));

    ASSERT_FALSE(l.is_empty('\''));

    for(char c = '0'; c <= '9'; c++)
    {
        ASSERT_TRUE(l.is_digit(c));
    }
    ASSERT_FALSE(l.is_digit('y'));

    ASSERT_TRUE(l.is_number("123332"));
    ASSERT_FALSE(l.is_number("232s232"));

    ASSERT_FALSE(COMMA_T.is_same(SEMICOLON_T));
    ASSERT_TRUE(COMMA_T.is_same(COMMA_T));

    ASSERT_FALSE(IntegerToken(21).is_same(IntegerToken(22)));
    ASSERT_TRUE(IntegerToken(22).is_same(IntegerToken(22)));

    ASSERT_FALSE(Token(21.f).is_same(Token(22.f)));
    ASSERT_TRUE(Token(22.f).is_same(Token(22.f)));

    ASSERT_FALSE(Token("s").is_same(Token("a")));
    ASSERT_TRUE(Token("b").is_same(Token("b")));

    ASSERT_FALSE(l.is_valid_for_naming('+'));
    ASSERT_TRUE(l.is_valid_for_naming('U'));

    ASSERT_EQ(SEMICOLON_T.to_string(), std::string(";"));
    ASSERT_NE(Token("s").to_string(), Token("a").to_string());
}

TEST(EMPTY_TEST_CASES, EMPTY)
{
    Lexer l;
    ASSERT_TRUE(l.make_tokens("").empty());
    ASSERT_TRUE(l.make_tokens("  \t \n\n\t\t\n  \n").empty());
}

TEST(INTEGER_CASES, INTEGER)
{
    Lexer l;

    std::vector<Token> result = l.make_tokens("1234");
    std::vector<Token> expected;

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].type, Token::INTEGER);
    ASSERT_EQ(result[0].ival, 1234);

    result = l.make_tokens("11 12 23 32 0 0 1");
    expected = std::vector<Token>({ IntegerToken(11), IntegerToken(12), IntegerToken(23), IntegerToken(32), IntegerToken(0), IntegerToken(0), IntegerToken(1) });

    ASSERT_EQ(result.size(),expected.size());

    for(int i = 0; i < expected.size(); i++)
    {
        ASSERT_EQ(result[i].type, expected[i].type);
        ASSERT_EQ(result[i].ival, expected[i].ival);
    }

    result = l.make_tokens("001 010 23 022");
    expected = std::vector<Token>({ IntegerToken(1), IntegerToken(10), IntegerToken(23), IntegerToken(22) });

    ASSERT_EQ(result.size(), expected.size());

    for(int i = 0; i < expected.size(); i++)
    {
        ASSERT_EQ(result[i].type, expected[i].type);
        ASSERT_EQ(result[i].ival, expected[i].ival);
    }
}

TEST(FLOAT_CASES, FLOAT)
{
    Lexer l;

    std::vector<Token> expected;

    std::vector<Token> result = l.make_tokens("123.321 0.01 .11 112. 803.308");
    expected = { Token(123.321f), Token(0.01f), Token(0.11f), Token(112.f), Token(803.308f) };

    ASSERT_EQ(result.size(),expected.size());

    for(int i = 0; i < expected.size(); i++)
    {
        ASSERT_EQ(result[i].type, expected[i].type);
        ASSERT_FLOAT_EQ(result[i].fval, expected[i].fval);
    }

    result = l.make_tokens("00.00 223. 3232. .12442 33532.32345");
    expected = { Token(00.00f), Token(223.f), Token(3232.f), Token(.12442f), Token(33532.32345f) };

    ASSERT_EQ(result.size(),expected.size());

    for(int i = 0; i < expected.size(); i++)
    {
        ASSERT_EQ(result[i].type, expected[i].type);
        ASSERT_FLOAT_EQ(result[i].fval, expected[i].fval);
    }
}

TEST(BASIC_CASES, BASIC)
{
    Lexer l;

    std::vector<Token> expected;

    std::vector<Token> result = l.make_tokens("123, 321, 456, 654");
    expected = { IntegerToken(123), Token(Token::COMMA), IntegerToken(321), Token(Token::COMMA), IntegerToken(456), Token(Token::COMMA), IntegerToken(654) };

    LEXER_TESTING_MACRO

    result = l.make_tokens("808;\n33, 42\n44;");
    expected = { IntegerToken(808), Token(Token::SEMICOLON), IntegerToken(33), Token(Token::COMMA), IntegerToken(42), IntegerToken(44), Token(Token::SEMICOLON) };

    LEXER_TESTING_MACRO

    result = l.make_tokens("()[]([()])([)](({()})({}))");
    expected = { LPARENTHESIS_T, RPARENTHESIS_T, LSQUARE_T, RSQUARE_T, LPARENTHESIS_T, LSQUARE_T, LPARENTHESIS_T, RPARENTHESIS_T, RSQUARE_T, 
        RPARENTHESIS_T, LPARENTHESIS_T, LSQUARE_T, RPARENTHESIS_T, RSQUARE_T, LPARENTHESIS_T, LPARENTHESIS_T, LBRACE_T, LPARENTHESIS_T, 
        RPARENTHESIS_T, RBRACE_T, RPARENTHESIS_T, LPARENTHESIS_T, LBRACE_T, RBRACE_T, RPARENTHESIS_T, RPARENTHESIS_T
        };
    
    LEXER_TESTING_MACRO

    result = l.make_tokens("2 + 2 = 4");
    expected = { IntegerToken(2), PLUS_T, IntegerToken(2), EQUAL_T, IntegerToken(4) };

    LEXER_TESTING_MACRO

    result = l.make_tokens("int a = 5 + 7, b = 3 - 2;\n float s = 24. - .32");
    expected = { Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(5), PLUS_T, IntegerToken(7), COMMA_T,
    Token("b"), EQUAL_T, IntegerToken(3), MINUS_T, IntegerToken(2), SEMICOLON_T, Token(Token::FLOATTYPE), Token("s"), EQUAL_T,
    Token(24.f), MINUS_T, Token(.32f) };

    LEXER_TESTING_MACRO
}

TEST(LEXER_IF_ELSE, IF_ELSE1)
{
    Lexer l;

    std::vector<Token> result = l.make_tokens("if (AdelLoh) { print(2) } else { print(3) }");
    std::vector<Token> expected = { IF_T, LPARENTHESIS_T, Token("AdelLoh"), RPARENTHESIS_T, LBRACE_T, PRINT_T, LPARENTHESIS_T, IntegerToken(2), RPARENTHESIS_T, RBRACE_T,
    ELSE_T, LBRACE_T, PRINT_T, LPARENTHESIS_T, IntegerToken(3), RPARENTHESIS_T, RBRACE_T };

    LEXER_TESTING_MACRO
}

TEST(LEXER_IF_ELSE, IF_ELSE2)
{
    Lexer l;

    std::vector<Token> result = l.make_tokens("if (2 + 2 == 3) { print(TRUE) } else { print(FALSE) }");
    std::vector<Token> expected = { IF_T, LPARENTHESIS_T, IntegerToken(2), PLUS_T, IntegerToken(2), EQEQ_T, IntegerToken(3), RPARENTHESIS_T, 
    LBRACE_T, PRINT_T, LPARENTHESIS_T, Token("TRUE"), RPARENTHESIS_T, RBRACE_T,
    ELSE_T, LBRACE_T, PRINT_T, LPARENTHESIS_T, Token("FALSE"), RPARENTHESIS_T, RBRACE_T };

    LEXER_TESTING_MACRO
}

TEST(LEXER_LOGICAL_EXPR, L_EXPR)
{
    Lexer l;

    std::vector<Token> result = l.make_tokens("== >= <= > <");
    std::vector<Token> expected = { EQEQ_T, GOQ_T, LOQ_T, GREATER_T, LESSER_T };

    LEXER_TESTING_MACRO
}

TEST(LEXER_PRINT, PRINT)
{
    Lexer l;

    std::vector<Token> result = l.make_tokens("print(2 + 2);");
    std::vector<Token> expected = { PRINT_T, LPARENTHESIS_T, IntegerToken(2), PLUS_T, IntegerToken(2), RPARENTHESIS_T, SEMICOLON_T };

    LEXER_TESTING_MACRO
}