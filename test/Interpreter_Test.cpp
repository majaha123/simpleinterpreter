#include <gtest/gtest.h>
#include <Interpreter.h>

TEST(INTERPRETER_TEST, BASIC_TEST1)
{
    Interpreter i;

    SequenceNode sn;
    sn.nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(new SumNode(
        std::shared_ptr<Node>(new FloatNumNode(2.f)),
        std::shared_ptr<Node>(new FloatNumNode(2.f))
    )))) };

    i.run(&sn);

    ASSERT_FLOAT_EQ(i.get_var("a").fval, 4.f);
}

TEST(INTERPRETER_TEST, BASIC_TEST2)
{
    Interpreter i;

    SequenceNode sn;
    sn.nodes = { std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "a", std::shared_ptr<Node>(new SumNode(
        std::shared_ptr<Node>(new FloatNumNode(2.f)),
        std::shared_ptr<Node>(new FloatNumNode(2.f))
    )))),
    std::shared_ptr<Node>(new DeclareVariableNode(FLOAT_TYPE, "b", std::shared_ptr<Node>(new SumNode(
        std::shared_ptr<Node>(new FloatNumNode(2.f)),
        std::shared_ptr<Node>(new VariableNode("a", FLOAT_TYPE))
    )))) };

    i.run(&sn);

    ASSERT_FLOAT_EQ(i.get_var("a").fval, 4.f);
    ASSERT_FLOAT_EQ(i.get_var("b").fval, 6.f);
}

TEST(INTERPRETER_TEST, BASIC_PRINT)
{
    Interpreter i;

    SequenceNode sn;
    sn.nodes = { std::shared_ptr<Node>(new PrintNode({
        std::shared_ptr<Node>(new SumNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2))))
    })) };

    testing::internal::CaptureStdout();

    i.run(&sn);

    ASSERT_EQ(testing::internal::GetCapturedStdout(), std::string("4.000000 \n"));
}

TEST(INTERPRETER_ASSIGN, BASIC_ASSIGN)
{
    Interpreter i;

    SequenceNode sn;
    sn.nodes = { std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(
        new SumNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2)))
    ))),
    std::shared_ptr<Node>(new AssignNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new SumNode(
        std::shared_ptr<Node>(new MultiplicationNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)))),
        std::shared_ptr<Node>(new IntNumNode(2))
    )))) };

    i.run(&sn);

    ASSERT_EQ(i.get_var("a").ival, 10);
}

TEST(INTERPRETER_BRANCHING, BASIC_BRANCHING)
{
    Interpreter i;

    SequenceNode sn;
    sn.nodes = { std::shared_ptr<Node>(new BranchingNode(
        std::shared_ptr<Node>(new LesserNode(std::shared_ptr<Node>(new SumNode(std::shared_ptr<Node>(new IntNumNode(2)), std::shared_ptr<Node>(new IntNumNode(2)))),
        std::shared_ptr<Node>(new IntNumNode(3)))),
        std::shared_ptr<Node>(new PrintNode({ std::shared_ptr<Node>(new IntNumNode(1)) })),
        std::shared_ptr<Node>(new PrintNode({ std::shared_ptr<Node>(new IntNumNode(0)) })))) }; // if (2 + 2 < 3) { print(1); } else { print(0); } -> should print '0'

    testing::internal::CaptureStdout();

    i.run(&sn);

    ASSERT_EQ(testing::internal::GetCapturedStdout(), std::string("0.000000 \n"));
}

TEST(INTERPRETER_CYCLING, BASIC_FOR_CYCLING)
{
    Interpreter i;

    std::shared_ptr<Node> n = std::shared_ptr<ForCycleNode>(new ForCycleNode(std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(new IntNumNode(0)))),
    std::shared_ptr<Node>(new LesserNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new IntNumNode(5)))),
    std::shared_ptr<Node>(new AssignNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<SumNode>(
        new SumNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new IntNumNode(1)))
    ))),
    std::shared_ptr<Node>(new SequenceNode({ std::shared_ptr<Node>(new PrintNode({ std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)) })) }))));

    testing::internal::CaptureStdout();

    i.run(n.get());

    ASSERT_EQ(testing::internal::GetCapturedStdout(), std::string("0.000000 \n1.000000 \n2.000000 \n3.000000 \n4.000000 \n"));
}

TEST(INTERPRETER_CYCLING, BASIC_WHILE_CYCLING)
{
    Interpreter i;

    std::shared_ptr<Node> n = std::shared_ptr<SequenceNode>(new SequenceNode({
        std::shared_ptr<Node>(new DeclareVariableNode(INTEGER_TYPE, "a", std::shared_ptr<Node>(new IntNumNode(0)))),
        std::shared_ptr<WhileCycleNode>(new WhileCycleNode(
        std::shared_ptr<Node>(new LesserNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new IntNumNode(5)))),
        std::shared_ptr<Node>(new SequenceNode({ 
            std::shared_ptr<Node>(new AssignNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<SumNode>(
            new SumNode(std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)), std::shared_ptr<Node>(new IntNumNode(1)))
        ))),
        std::shared_ptr<Node>(new PrintNode({ std::shared_ptr<Node>(new VariableNode("a", INTEGER_TYPE)) })) 
        }))))
    })); // int a = 0; while (a < 5) { a = a + 1; print(a); };

    testing::internal::CaptureStdout();

    i.run(n.get());

    ASSERT_EQ(testing::internal::GetCapturedStdout(), std::string("1.000000 \n2.000000 \n3.000000 \n4.000000 \n5.000000 \n"));
}