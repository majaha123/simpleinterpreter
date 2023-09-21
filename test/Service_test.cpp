#include<Lexer.h>
#include<Parser.h>
#include<Interpreter.h>

#include<gtest/gtest.h>

std::string fibb_nums(int n)
{
    std::string out;

    int a = 0;
    int b = 1;
    int c;

    for (int i = 0; i < n; i = i + 1)
    {
        c = a + b;
        a = b;
        b = c;

        out += std::to_string(b) + ".000000 \n";
    }

    return out;
}

TEST(PRACTICAL_USE, FIBONACCI_NUMBERS)
{
    Lexer l;

    std::string input = "int a = 0;int b = 1;int c;int n = 20;for (int i = 0; i < n; i = i + 1){c = a + b;a = b;b = c;print(b);};";

    std::vector<Token> tokens = l.make_tokens(input);

    Parser p(tokens);

    std::shared_ptr<SequenceNode> sn = p.make_tree();

    Interpreter i;

    testing::internal::CaptureStdout();

    i.run(sn.get());

    ASSERT_EQ(testing::internal::GetCapturedStdout(), fibb_nums(20));
}