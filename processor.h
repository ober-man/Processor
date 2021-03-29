#pragma once

#include"functions.h"
#include"declaration.h"
const size_t MAX_ELEMS = 100;

class Processor
{
    private:
        Stack<double> data_stack;
        double regs[7];          // AX, BX, CX, DX, SI, DI, BP
        size_t IP;               // Command counter, shows the next command number, starts from the 0!
        Instruction* instrs;     // Array with commands
        bool above_flag;         // (true) if command returns > 0
                                 //            (false) else
        bool ZF;                 // Zero Flag: (true) if command returns 0
                                 //            (false) else

        size_t ReadCommands(const char* in_file);
        void CommandPush(int arg_flag, double value);
        void CommandPop(int reg);
        void CommandTop(int reg);
        void CommandAdd();
        void CommandSub();
        void CommandMul();
        void CommandDiv();
        void CommandMod();
        void CommandInput(int reg);
        void CommandOutput(int reg);
        void CommandDump();
        void CommandAbs();
        void CommandCmp();
        void CommandJmp(size_t address, size_t limit);
        void CommandJe(size_t address, size_t limit);
        void CommandJne(size_t address, size_t limit);
        void CommandJb(size_t address, size_t limit);
        void CommandJbe(size_t address, size_t limit);
        void CommandJa(size_t address, size_t limit);
        void CommandJae(size_t address, size_t limit);
        void CommandSqrt();

    public:
        Processor()
        {
            data_stack.Create(MAX_ELEMS);
            for(int i = 0; i < 7; ++i)
                regs[i] = 0;
            IP = 0;
            instrs = NULL;
            above_flag = false;
            ZF = false;
        }
        void Run(const char* in_file, size_t number_of_blocks);
        ~Processor()
        {
            data_stack.Destroy();
            delete [] instrs;
        }
};

void Processor::Run(const char* in_file, size_t number_of_blocks)
{
    instrs = new Instruction[number_of_blocks];
    size_t number_of_commands = ReadCommands(in_file);
    if(number_of_blocks != number_of_commands)
    {
        printf("Error in reading\n");
        std::cout << number_of_commands;
        exit(1);
    }
    /// Starting from the word "begin"
    while(instrs[IP].cmd_code != BEGIN)
        ++IP;
    if(IP == number_of_commands)
        CompError(NO_BEGIN, 0);

    IP++; // next command after "BEGIN"
    while(IP < number_of_commands)
    {
        if(instrs[IP].cmd_flag != LABEL && instrs[IP].cmd_flag != CMD)
        {
            printf("Compilation error\n");
            exit(1);
        }

        if(instrs[IP].cmd_flag == LABEL)
        {
            ++IP;
            continue;
        }
        /// Command
        switch(instrs[IP].cmd_code)
        {
            case PUSH:
                CommandPush(instrs[IP].arg_flag, instrs[IP].value);
                break;
            case POP:
                CommandPop((int)instrs[IP].value);
                break;
            case TOP:
                CommandTop((int)instrs[IP].value);
                break;
            case ADD:
                CommandAdd();
                break;
            case SUB:
                CommandSub();
                break;
            case MUL:
                CommandMul();
                break;
            case DIV:
                CommandDiv();
                break;
            case MOD:
                CommandMod();
                break;
            case INPUT:
                CommandInput((int)instrs[IP].value);
                break;
            case OUTPUT:
                CommandOutput((int)instrs[IP].value);
                break;
            case DUMP:
                CommandDump();
                break;
            case JMP:
                CommandJmp((int)instrs[IP].value, number_of_commands);
                break;
            case JE:
                CommandJe((int)instrs[IP].value, number_of_commands);
                break;
            case JNE:
                CommandJne((int)instrs[IP].value, number_of_commands);
                break;
            case JB:
                CommandJb((int)instrs[IP].value, number_of_commands);
                break;
            case JBE:
                CommandJbe((int)instrs[IP].value, number_of_commands);
                break;
            case JA:
                CommandJa((int)instrs[IP].value, number_of_commands);
                break;
            case JAE:
                CommandJae((int)instrs[IP].value, number_of_commands);
                break;
            case CMP:
                CommandCmp();
                break;
            case BEGIN:
                CompError(MANY_BEGIN, IP);
                break;
            case END:
                printf("End of the program\n");
                break;
            case SQRT:
                CommandSqrt();
                break;
            case ABS:
                CommandAbs();
                break;
            default:
                printf("%d\n", instrs[IP].cmd_code);
                exit(1);
        }
        ++IP;
    }
}

size_t Processor::ReadCommands(const char* in_file)
{
    /// Checking correctness of entry
    assert(in_file != NULL);
    int num = 0;
    FILE* in = fopen(in_file, "r");
    while(fscanf(in, "%d %d %d %lg\n", &instrs[num].cmd_flag, &instrs[num].cmd_code, &instrs[num].arg_flag, &instrs[num].value) == 4)
        ++num;
    fclose(in);
    return num;
}

void Processor::CommandPush(int arg_flag, double value)
{
    bool check = 0;
    if(arg_flag == REG)
        switch((int)value)
        {
            case AX:
                check = data_stack.Push(regs[0]);
                break;
            case BX:
                check = data_stack.Push(regs[1]);
                break;
            case CX:
                check = data_stack.Push(regs[2]);
                break;
            case DX:
                check = data_stack.Push(regs[3]);
                break;
            case SI:
                check = data_stack.Push(regs[4]);
                break;
            case DI:
                check = data_stack.Push(regs[5]);
                break;
            case BP:
                check = data_stack.Push(regs[6]);
                break;
            default:
                printf("Push error 1\n");
                exit(1);
        }
    else
        check = data_stack.Push(value);
    if(!check)
    {
        printf("Push error 2\n");
        exit(1);
    }
}

void Processor::CommandPop(int reg)
{
    bool check = false;
    switch(reg)
    {
        case AX:
            check = data_stack.Pop(&regs[0]);
            break;
        case BX:
            check = data_stack.Pop(&regs[1]);
            break;
        case CX:
            check = data_stack.Pop(&regs[2]);
            break;
        case DX:
            check = data_stack.Pop(&regs[3]);
            break;
        case SI:
            check = data_stack.Pop(&regs[4]);
            break;
        case DI:
            check = data_stack.Pop(&regs[5]);
            break;
        case BP:
            check = data_stack.Pop(&regs[6]);
            break;
        default:
            printf("Pop error 1\n");
            exit(1);
    }
    if(!check)
    {
        printf("Pop error 2\n");
        exit(1);
    }
}

void Processor::CommandTop(int reg)
{
    bool check = false;
    switch(reg)
    {
        case AX:
            check = data_stack.Top(&regs[0]);
            break;
        case BX:
            check = data_stack.Top(&regs[1]);
            break;
        case CX:
            check = data_stack.Top(&regs[2]);
            break;
        case DX:
            check = data_stack.Top(&regs[3]);
            break;
        case SI:
            check = data_stack.Top(&regs[4]);
            break;
        case DI:
            check = data_stack.Top(&regs[5]);
            break;
        case BP:
            check = data_stack.Top(&regs[6]);
            break;
        default:
            printf("Top error 1\n");
            exit(1);
    }
    if(!check)
    {
        printf("Top error 2\n");
        exit(1);
    }
}

void Processor::CommandAdd()
{
    bool check = false;
    double up_arg = 0;
    double down_arg = 0;
    check = data_stack.Pop(&down_arg);
    if(!check)
    {
        printf("Add error 1\n");
        exit(1);
    }
    check = data_stack.Pop(&up_arg);
    if(!check)
    {
        printf("Add error 2\n");
        exit(1);
    }
    double res = up_arg + down_arg;
    check = data_stack.Push(res);
    if(!check)
    {
        printf("Add error 3\n");
        exit(1);
    }
    int ret = Compare(res, 0);
    if(ret == 0)
        ZF = true;
    else ZF = false;
    if(ret > 0)
        above_flag = true;
    else above_flag = false;
}

void Processor::CommandSub()
{
    bool check = false;
    double up_arg = 0;
    double down_arg = 0;
    check = data_stack.Pop(&down_arg);
    if(!check)
    {
        printf("Sub error 1\n");
        exit(1);
    }
    check = data_stack.Pop(&up_arg);
    if(!check)
    {
        printf("Sub error 2\n");
        exit(1);
    }
    double res = up_arg - down_arg;
    check = data_stack.Push(res);
    if(!check)
    {
        printf("Sub error 3\n");
        exit(1);
    }
    int ret = Compare(res, 0);
    if(ret == 0)
        ZF = true;
    else ZF = false;
    if(ret > 0)
        above_flag = true;
    else above_flag = false;
}

void Processor::CommandMul()
{
    bool check = false;
    double up_arg = 0;
    double down_arg = 0;
    check = data_stack.Pop(&down_arg);
    if(!check)
    {
        printf("Mul error 1\n");
        exit(1);
    }
    check = data_stack.Pop(&up_arg);
    if(!check)
    {
        printf("Mul error 2\n");
        exit(1);
    }
    double res = up_arg * down_arg;
    check = data_stack.Push(res);
    if(!check)
    {
        printf("Mul error 3\n");
        exit(1);
    }
    int ret = Compare(res, 0);
    if(ret == 0)
        ZF = true;
    else ZF = false;
    if(ret > 0)
        above_flag = true;
    else above_flag = false;
}

void Processor::CommandDiv()
{
    bool check = false;
    double up_arg = 0;
    double down_arg = 0;
    check = data_stack.Pop(&down_arg);
    if(!check)
    {
        printf("Div error 1\n");
        exit(1);
    }
    if(Compare(down_arg, 0) == 0)
    {
        printf("Can't divide by 0");
        exit(1);
    }
    check = data_stack.Pop(&up_arg);
    if(!check)
    {
        printf("Div error 2\n");
        exit(1);
    }
    double res = up_arg / down_arg;
    check = data_stack.Push(res);
    if(!check)
    {
        printf("Div error 3\n");
        exit(1);
    }
    int ret = Compare(res, 0);
    if(ret == 0)
        ZF = true;
    else ZF = false;
    if(ret > 0)
        above_flag = true;
    else above_flag = false;
}

void Processor::CommandMod()
{
    bool check = false;
    double up_arg = 0;
    double down_arg = 0;
    check = data_stack.Pop(&down_arg);
    if(!check)
    {
        printf("Mod error 1\n");
        exit(1);
    }
    if(Compare(down_arg, 0) == 0)
    {
        printf("Can't divide by 0");
        exit(1);
    }
    check = data_stack.Pop(&up_arg);
    if(!check)
    {
        printf("Mod error 2\n");
        exit(1);
    }
    double res = (int)up_arg % (int)down_arg;
    check = data_stack.Push(res);
    if(!check)
    {
        printf("Mod error 3\n");
        exit(1);
    }
    int ret = Compare(res, 0);
    if(ret == 0)
        ZF = true;
    else ZF = false;
    if(ret > 0)
        above_flag = true;
    else above_flag = false;
}

void Processor::CommandInput(int reg)
{
    printf("Enter a number\n");
    switch(reg)
    {
        case AX:
            std::cin >> regs[0];
            break;
        case BX:
            std::cin >> regs[1];
            break;
        case CX:
            std::cin >> regs[2];
            break;
        case DX:
            std::cin >> regs[3];
            break;
        case SI:
            std::cin >> regs[4];
            break;
        case DI:
            std::cin >> regs[5];
            break;
        case BP:
            std::cin >> regs[6];
            break;
        default:
            printf("Input error: register %d\n", reg);
            exit(1);
    }
}

void Processor::CommandOutput(int reg)
{
    switch(reg)
    {
        case AX:
            std::cout << "Register AX contains " << regs[0] << std::endl;
            break;
        case BX:
            std::cout << "Register BX contains " << regs[1] << std::endl;
            break;
        case CX:
            std::cout << "Register CX contains " << regs[2] << std::endl;
            break;
        case DX:
            std::cout << "Register DX contains " << regs[3] << std::endl;
            break;
        case SI:
            std::cout << "Register SI contains " << regs[4] << std::endl;
            break;
        case DI:
            std::cout << "Register DI contains " << regs[5] << std::endl;
            break;
        case BP:
            std::cout << "Register BP contains " << regs[6] << std::endl;
            break;
        default:
            printf("Output error\n");
            exit(1);
    }
}

void Processor::CommandDump()
{
    bool check = false;
    check = data_stack.Dump();
    if(!check)
    {
        printf("Dump error\n");
        exit(1);
    }
    std::cout << "Register AX contains " << regs[0] << std::endl;
    std::cout << "Register BX contains " << regs[1] << std::endl;
    std::cout << "Register CX contains " << regs[2] << std::endl;
    std::cout << "Register DX contains " << regs[3] << std::endl;
    std::cout << "Register SI contains " << regs[4] << std::endl;
    std::cout << "Register DI contains " << regs[5] << std::endl;
    std::cout << "Register BP contains " << regs[6] << std::endl;
    std::cout << "Register IP is on the " << IP << " command" << std::endl;
    std::cout << "Zero Flag is " << ZF << std::endl << std::endl;
    std::cout << "Above flag is " << above_flag << std::endl << std::endl;
}

void Processor::CommandJmp(size_t address, size_t limit)
{
    if(address < 0 || address > limit)
    {
        printf("Wrong label\n");
        exit(1);
    }
    IP = address;
}

void Processor::CommandJe(size_t address, size_t limit)
{
    if(address < 0 || address > limit)
    {
        printf("Wrong label\n");
        exit(1);
    }
    if(ZF == true)
        IP = address;
}

void Processor::CommandJne(size_t address, size_t limit)
{
    if(address < 0 || address > limit)
    {
        printf("Wrong label\n");
        exit(1);
    }
    if(ZF == false)
        IP = address;
}

void Processor::CommandJb(size_t address, size_t limit)
{
    if(address < 0 || address > limit)
    {
        printf("Wrong label\n");
        exit(1);
    }
    if(above_flag == false)
        IP = address;
}

void Processor::CommandJbe(size_t address, size_t limit)
{
    if(address < 0 || address > limit)
    {
        printf("Wrong label\n");
        exit(1);
    }
    if(above_flag == false || ZF == true)
        IP = address;
}

void Processor::CommandJa(size_t address, size_t limit)
{
    if(address < 0 || address > limit)
    {
        printf("Wrong label\n");
        exit(1);
    }
    if(above_flag == true)
        IP = address;
}

void Processor::CommandJae(size_t address, size_t limit)
{
    if(address < 0 || address > limit)
    {
        printf("Wrong label\n");
        exit(1);
    }
    if(above_flag == true || ZF == true)
        IP = address;
}

void Processor::CommandCmp()
{
    bool check = false;
    double up_arg = 0;
    double down_arg = 0;
    check = data_stack.Pop(&down_arg);
    if(!check)
    {
        printf("Cmp error 1\n");
        exit(1);
    }
    check = data_stack.Pop(&up_arg);
    if(!check)
    {
        printf("Cmp error 2\n");
        exit(1);
    }
    int res = up_arg - down_arg;
    int ret = Compare(res, 0);
    if(ret == 0)
        ZF = true;
    else ZF = false;
    if(ret > 0)
        above_flag = true;
    else above_flag = false;
}

void Processor::CommandAbs()
{
    bool check = false;
    double num = 0;
    check = data_stack.Pop(&num);
    if(!check)
    {
        printf("Abs error 1");
        exit(1);
    }
    double res = abs(num);
    check = data_stack.Push(res);
    if(!check)
    {
        printf("Sqrt error 2");
        exit(1);
    }
    int ret = Compare(res, 0);
    if(ret == 0)
        ZF = true;
    else ZF = false;
    if(ret > 0)
        above_flag = true;
    else above_flag = false;
}

void Processor::CommandSqrt()
{
    bool check = false;
    double num = 0;
    check = data_stack.Pop(&num);
    if(!check)
    {
        printf("Sqrt error 1");
        exit(1);
    }
    if(Compare(num, 0) == -1)
    {
        printf("Can't extract square root from negative number\n");
        exit(1);
    }
    double res = sqrt(num);
    check = data_stack.Push(res);
    if(!check)
    {
        printf("Sqrt error 2");
        exit(1);
    }
    int ret = Compare(res, 0);
    if(ret == 0)
        ZF = true;
    else ZF = false;
    if(ret > 0)
        above_flag = true;
    else above_flag = false;
}
