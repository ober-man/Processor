#include"compiler.h"
#include"processor.h"

int main()
{
    Compiler comp;
    size_t number_of_blocks = comp.Compile("factorial.txt", "output.o");

    Processor proc;
    proc.Run("output.o", number_of_blocks);
    return 0;
}
