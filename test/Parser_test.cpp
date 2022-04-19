#include <Parser.h>
#include <gtest/gtest.h>

TEST(PARSER_NODES_IS_SAME_TEST, IS_SAME_TEST)
{
    IntNumNode i1(12), i2(13);
    FloatNumNode f1(15.5f), f2(15.f);

    SumNode sum1(std::shared_ptr<Node>(new IntNumNode(1)), std::shared_ptr<Node>(new IntNumNode(2))),
    sum2(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(1))),
    sum3(std::shared_ptr<Node>(new MultiplicationNode(
        std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2))
    )), std::shared_ptr<Node>(new IntNumNode(2)));

    MultiplicationNode mul1(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(3))),
    mul2(std::shared_ptr<Node>(new IntNumNode(3)), std::shared_ptr<Node>(new IntNumNode(2))),
    mul3(std::shared_ptr<Node>(new SumNode(
        std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2))
    )), std::shared_ptr<Node>(new IntNumNode(2)));

    DeclareVariableNode v1(INTEGER_TYPE, "a"), 
    v2(INTEGER_TYPE, "b"), 
    v3(FLOAT_TYPE, "c"), 
    v4(DeclareVariableNode(INTEGER_TYPE, "d", std::shared_ptr<Node>(&i1)));

    VariableNode var1("e", Type(INTEGER_TYPE)), var2("f", Type(FLOAT_TYPE)), var3("g", Type(FLOAT_TYPE));

    SequenceNode s1, s2;

    s1.nodes = { std::shared_ptr<Node>(new DeclareVariableNode(v1)), std::shared_ptr<Node>(new DeclareVariableNode(v2)) };
    s2.nodes = { std::shared_ptr<Node>(new DeclareVariableNode(v3)), std::shared_ptr<Node>(new DeclareVariableNode(v4)) };

    std::shared_ptr<Node> statement1(new LesserNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(3)))),
    statement2(new GreaterNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(3))));

    std::shared_ptr<Node> if_body1(new IntNumNode(2)), if_body2(new FloatNumNode(2.f)), else_body1(new IntNumNode(3)), else_body2(new FloatNumNode(3.f));

    std::vector<std::shared_ptr<Node>> statements = { statement1, statement2 };
    std::vector<std::shared_ptr<Node>> if_bodies = { if_body1, if_body2 };
    std::vector<std::shared_ptr<Node>> else_bodies = { else_body1, else_body2 };

    std::vector<std::shared_ptr<Node>> branching_nodes;

    for (auto i : statements)
    {
        for (auto j : if_bodies)
        {
            branching_nodes.push_back(std::shared_ptr<Node>(new BranchingNode(i, j)));

            for (auto k : else_bodies)
            {
                branching_nodes.push_back(std::shared_ptr<Node>(new BranchingNode(i, j, k)));
            }
        }
    }

    for (int i = 0; i < branching_nodes.size(); i++)
    {
        ASSERT_TRUE(branching_nodes[i]->is_same(branching_nodes[i].get()));
        for (int j = i + 1; j < branching_nodes.size(); j++)
        {
            ASSERT_FALSE(branching_nodes[i]->is_same(branching_nodes[j].get()));
        }
    }

    ASSERT_TRUE(i1.is_same(&i1));
    ASSERT_FALSE(i1.is_same(&i2));
    ASSERT_FALSE(i1.is_same(&f1));

    ASSERT_TRUE(f1.is_same(&f1));
    ASSERT_FALSE(f1.is_same(&i1));
    ASSERT_FALSE(f1.is_same(&f2));

    std::vector<Node*> vec = { &v1, &v2, &v3, &v4 };

    for (int i = 0; i < vec.size(); i++)
    {
        ASSERT_TRUE(vec[i]->is_same(vec[i]));
        for (int j = i + 1; j < vec.size(); j++)
        {
            ASSERT_FALSE(vec[i]->is_same(vec[j]));
        }
    }

    ASSERT_TRUE(var1.is_same(&var1));
    ASSERT_FALSE(var1.is_same(&var2));
    ASSERT_TRUE(var3.is_same(&var3));

    ASSERT_TRUE(s1.is_same(&s1));
    ASSERT_TRUE(s2.is_same(&s2));
    ASSERT_FALSE(s1.is_same(&s2));

    ASSERT_TRUE(sum1.is_same(&sum2));
    ASSERT_TRUE(mul1.is_same(&mul2));
    ASSERT_FALSE(sum1.is_same(&sum3));
    ASSERT_FALSE(sum3.is_same(&mul3));
}

TEST(PARSER_VARIABLE_TEST, BASIC_VARIABLE)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes.push_back(
        std::shared_ptr<Node>(
            new DeclareVariableNode(INTEGER_TYPE, "a")));

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, INT_VARIABLE_ASSIGN)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(2), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(new IntNumNode(2)))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, FLOAT_VARIABLE_ASSIGN)
{
    Parser p({ Token(Token::FLOATTYPE), Token("a"), EQUAL_T, Token(2.f), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(new FloatNumNode(2.f)))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, INT_VARIABLE_ASSIGN_SUM)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(2), PLUS_T, IntegerToken(2), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(
        new SumNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2)))))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, FLOAT_VARIABLE_ASSIGN_SUM)
{
    Parser p({ Token(Token::FLOATTYPE), Token("a"), EQUAL_T, Token(2.f), PLUS_T, Token(3.f), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(
        new SumNode(std::shared_ptr<Node>(new FloatNumNode(2.f)), std::shared_ptr<Node>(new FloatNumNode(3.f)))
    ))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, INT_VARIABLE_ASSIGN_DIFF)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(2), MINUS_T, IntegerToken(2), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(
        new SubtractNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2)))))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, FLOAT_VARIABLE_ASSIGN_DIFF)
{
    Parser p({ Token(Token::FLOATTYPE), Token("a"), EQUAL_T, Token(2.f), MINUS_T, Token(3.f), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(
        new SubtractNode(std::shared_ptr<Node>(new FloatNumNode(2.f)), std::shared_ptr<Node>(new FloatNumNode(3.f)))
    ))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, INT_VARIABLE_ASSIGN_MUL)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(2), ASTERISK_T, IntegerToken(2), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(
        new MultiplicationNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2)))))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, FLOAT_VARIABLE_ASSIGN_MUL)
{
    Parser p({ Token(Token::FLOATTYPE), Token("a"), EQUAL_T, Token(2.f), ASTERISK_T, Token(3.f), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(
        new MultiplicationNode(std::shared_ptr<Node>(new FloatNumNode(2.f)), std::shared_ptr<Node>(new FloatNumNode(3.f)))
    ))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, INT_VARIABLE_ASSIGN_DIV)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(2), SLASH_T, IntegerToken(2), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(
        new DivisionNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2)))))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, FLOAT_VARIABLE_ASSIGN_DIV)
{
    Parser p({ Token(Token::FLOATTYPE), Token("a"), EQUAL_T, Token(2.f), SLASH_T, Token(3.f), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(
        new DivisionNode(std::shared_ptr<Node>(new FloatNumNode(2.f)), std::shared_ptr<Node>(new FloatNumNode(3.f)))
    ))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, INT_VARIABLE_EXPRESSION_ORDER_ASSIGN1)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(2), PLUS_T, IntegerToken(2), ASTERISK_T, IntegerToken(2), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(
        new SumNode(
            std::shared_ptr<Node>(new MultiplicationNode(std::shared_ptr<Node>(new IntNumNode(2)), 
            std::shared_ptr<Node>(new IntNumNode(2)))), std::shared_ptr<Node>(new IntNumNode(2)))))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, INT_VARIABLE_EXPRESSION_ORDER_ASSIGN2)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, LPARENTHESIS_T, IntegerToken(2), PLUS_T, IntegerToken(2), RPARENTHESIS_T, ASTERISK_T, IntegerToken(2), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(
        new MultiplicationNode(
            std::shared_ptr<Node>(new SumNode(std::shared_ptr<Node>(new IntNumNode(2)), 
            std::shared_ptr<Node>(new IntNumNode(2)))), std::shared_ptr<Node>(new IntNumNode(2)))))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, INT_VARIABLE_EXPRESSION_ORDER_ASSIGN3)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(2), ASTERISK_T, IntegerToken(2), PLUS_T, IntegerToken(2), SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(
        new SumNode(
            std::shared_ptr<Node>(new MultiplicationNode(std::shared_ptr<Node>(new IntNumNode(2)), 
            std::shared_ptr<Node>(new IntNumNode(2)))), std::shared_ptr<Node>(new IntNumNode(2)))))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, FLOAT_VARIABLE_EXPRESSION_ORDER_ASSIGN1)
{
    Parser p({ Token(Token::FLOATTYPE), Token("a"), EQUAL_T, Token(2.f), SLASH_T, LPARENTHESIS_T, Token(3.f), PLUS_T, Token(5.f), RPARENTHESIS_T, SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(
        new DivisionNode(std::shared_ptr<Node>(new FloatNumNode(2.f)), 
        std::shared_ptr<Node>(new SumNode(std::shared_ptr<Node>(new FloatNumNode(3.f)), std::shared_ptr<Node>(new FloatNumNode(5.f)))))
    ))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, FLOAT_EXPRESSION_ASSIGN_COMPLEX_EXPRESSION)
{
    // float a = (3.f * 7.f + 2.f) / (15.f - 1.f); -> Div(Sum(Mul(3, 7), 2), Diff(15, 1))
    Parser p({ Token(Token::FLOATTYPE), Token("a"), EQUAL_T, LPARENTHESIS_T, Token(3.f), ASTERISK_T, Token(7.f), PLUS_T, Token(2.f), RPARENTHESIS_T, 
    SLASH_T, LPARENTHESIS_T, Token(15.f), MINUS_T, Token(1.f), RPARENTHESIS_T, SEMICOLON_T});

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(
        std::shared_ptr<Node>(new DivisionNode(std::shared_ptr<Node>(new SumNode(std::shared_ptr<Node>(new MultiplicationNode(
            std::shared_ptr<Node>(new FloatNumNode(3.f)), std::shared_ptr<Node>(new FloatNumNode(7.f))
        )),
        std::shared_ptr<Node>(new FloatNumNode(2.f)))),
        std::shared_ptr<Node>(new SubtractNode(
            std::shared_ptr<Node>(new FloatNumNode(15.f)),
            std::shared_ptr<Node>(new FloatNumNode(1.f))
        ))
    ))))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_VARIABLE_TEST, REUSE_VARIABLE)
{
    // float a = (3.f * 7.f + 2.f) / (15.f - 1.f); -> Div(Sum(Mul(3, 7), 2), Diff(15, 1))
    Parser p({ Token(Token::FLOATTYPE), Token("a"), EQUAL_T, Token(2.f), PLUS_T, Token(2.f), SEMICOLON_T, 
    Token(Token::FLOATTYPE), Token("b"), EQUAL_T, Token("a"), PLUS_T, Token(2.f), SEMICOLON_T});

    std::shared_ptr<SequenceNode> result = p.make_tree();
    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(new SumNode(
        std::shared_ptr<Node>(new FloatNumNode(2.f)),
        std::shared_ptr<Node>(new FloatNumNode(2.f))
    )))),
    std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "b", std::shared_ptr<Node>(std::shared_ptr<Node>(new SumNode(
        std::shared_ptr<Node>(new VariableNode("a", Type(FLOAT_TYPE))),
        std::shared_ptr<Node>(new FloatNumNode(2.f))
    ))))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_PRINT_TEST, BASIC_PRINT)
{
    Parser p({ PRINT_T, LPARENTHESIS_T, IntegerToken(2), PLUS_T, IntegerToken(2), RPARENTHESIS_T, SEMICOLON_T });

    std::shared_ptr<SequenceNode> result = p.make_tree();

    std::shared_ptr<PrintNode> print(new PrintNode());
    print->expressions.push_back(std::shared_ptr<Node>(new SumNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2)))));

    std::shared_ptr<SequenceNode> expected(new SequenceNode()); 
    expected->nodes = { print };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_ASSIGN_TEST, ASSIGN)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(2), PLUS_T, IntegerToken(2), SEMICOLON_T,
    Token("a"), EQUAL_T, IntegerToken(2), ASTERISK_T, Token("a"), PLUS_T, IntegerToken(2), SEMICOLON_T }); // int a = 2 + 2; a = 2 * a + 2;

    std::shared_ptr<SequenceNode> result = p.make_tree();

    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(
        new SumNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2)))
    ))),
    std::shared_ptr<Node>(new AssignNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new SumNode(
        std::shared_ptr<Node>(new MultiplicationNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)))),
        std::shared_ptr<Node>(new IntNumNode(2))
    )))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_BRANCHING, BASIC_BRANCHING)
{
    Parser p({ IF_T, LPARENTHESIS_T, IntegerToken(2), PLUS_T, IntegerToken(2), LESSER_T, IntegerToken(3), RPARENTHESIS_T,
    LBRACE_T, PRINT_T, LPARENTHESIS_T, IntegerToken(1), RPARENTHESIS_T, SEMICOLON_T, RBRACE_T, ELSE_T,
    LBRACE_T, PRINT_T, LPARENTHESIS_T, IntegerToken(0), RPARENTHESIS_T, SEMICOLON_T, RBRACE_T, SEMICOLON_T }); // if (2 + 2 < 3) { print(1); } else { print(0); };

    std::shared_ptr<SequenceNode> result = p.make_tree();

    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<BranchingNode>(new BranchingNode(std::shared_ptr<Node>(new LesserNode(
        std::shared_ptr<Node>(new SumNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2)))),
        std::shared_ptr<Node>(new IntNumNode(3))
    )),
    std::shared_ptr<Node>(new SequenceNode({ std::shared_ptr<Node>(new PrintNode({ std::shared_ptr<Node>(new IntNumNode(1)) })) })),
    std::shared_ptr<Node>(new SequenceNode({ std::shared_ptr<Node>(new PrintNode({ std::shared_ptr<Node>(new IntNumNode(0)) })) }))
    )) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_CYCLING, BASIC_FOR_CYCLING)
{
    Parser p({ FOR_T, LPARENTHESIS_T, Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(0), SEMICOLON_T, 
    Token("a"), LESSER_T, IntegerToken(5), SEMICOLON_T, Token("a"), EQUAL_T, Token("a"), PLUS_T, IntegerToken(1), RPARENTHESIS_T,
    LBRACE_T, PRINT_T, LPARENTHESIS_T, Token("a"), RPARENTHESIS_T, SEMICOLON_T, RBRACE_T, SEMICOLON_T }); // for (int a = 0; a < 5; a = a + 1) { print(a); };

    std::shared_ptr<SequenceNode> result = p.make_tree();

    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<ForCycleNode>(new ForCycleNode(std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(new IntNumNode(0)))),
    std::shared_ptr<Node>(new LesserNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new IntNumNode(5)))),
    std::shared_ptr<Node>(new AssignNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<SumNode>(
        new SumNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new IntNumNode(1)))
    ))),
    std::shared_ptr<Node>(new SequenceNode({ std::shared_ptr<Node>(new PrintNode({ std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)) })) })))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}

TEST(PARSER_CYCLING, BASIC_WHILE_CYCLING)
{
    Parser p({ Token(Token::INTTYPE), Token("a"), EQUAL_T, IntegerToken(0), SEMICOLON_T, 
    WHILE_T, LPARENTHESIS_T, Token("a"), LESSER_T, IntegerToken(5), RPARENTHESIS_T,
    LBRACE_T, Token("a"), EQUAL_T, Token("a"), PLUS_T, IntegerToken(1), SEMICOLON_T, 
    PRINT_T, LPARENTHESIS_T, Token("a"), RPARENTHESIS_T, SEMICOLON_T, RBRACE_T, SEMICOLON_T }); // int a = 0; while (a < 5) { a = a + 1; print(a); };

    std::shared_ptr<SequenceNode> result = p.make_tree();

    std::shared_ptr<SequenceNode> expected(new SequenceNode());
    expected->nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(new IntNumNode(0)))),
    std::shared_ptr<WhileCycleNode>(new WhileCycleNode(
    std::shared_ptr<Node>(new LesserNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new IntNumNode(5)))),
    std::shared_ptr<Node>(new SequenceNode({ 
        std::shared_ptr<Node>(new AssignNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<SumNode>(
        new SumNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new IntNumNode(1)))
    ))),
    std::shared_ptr<Node>(new PrintNode({ std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)) })) 
    })))) };

    ASSERT_TRUE(expected->is_same(result.get()));
}