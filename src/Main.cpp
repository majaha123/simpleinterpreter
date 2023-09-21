#include <Interpreter.h>
#include <fstream>
#include <sstream>

int main(int argc, char** argv)
{
    Lexer l;

    while (1)
    {
        try
        {
            std::string input;

            std::fstream file("C:\\Users\\user123\\Desktop\\MS.txt");
            std::stringstream ss; 
            ss << file.rdbuf();

            input = ss.str();

            std::vector<Token> tokens = l.make_tokens(input);

            Parser p(tokens);

            std::shared_ptr<SequenceNode> sn = p.make_tree();

            Interpreter i;

            i.run(sn.get());

            break;
        }
        catch (const ParserException& e)
        {
            std::cout << "A Parser exception occured! - " << e.what() << std::endl;
        }
        catch (const InterpreterException& e)
        {
            std::cout << "A Interpreter exception occured! - " << e.what() << std::endl;
        }
    }

    return 0;
}