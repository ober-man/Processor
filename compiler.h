#pragma once
#include "functions.h"
#define NOT_FOUND -1
const int MAXLEN = 25;

class Compiler
{
    private:
        size_t number_of_labels;
        char** labels;
        size_t* addresses;

        size_t number_of_lexems;
        Lexem* syntax;

        size_t number_of_instructions;
        Instruction* semantic;

        Flag GetFlag(char data[]);
        double GetObject(char data[]);
        Command GetCommand(char data[]);
        Register GetRegister(char data[]);
        size_t CorrectLabel(char data[]);
        void LabelRegistrator(char** pointers);
        void SyntaxAnalysis(char** pointers);
        void SemanticAnalysis();
        int FlagCMD(size_t lexem_counter, size_t instr_counter);
        void FlagLABEL(size_t lexem_counter, size_t instr_counter);
        void CommandWithArgument(size_t lexem_counter, size_t instr_counter);
        void CommandNoArgument(size_t lexem_counter, size_t instr_counter);
        void CommandJump(size_t lexem_counter, size_t instr_counter);
        void SemanticToFile(const char* out_file);

    public:
        Compiler()
        {
            number_of_labels = 0;
            labels = NULL;
            addresses = NULL;
          
            number_of_lexems = 0;
            syntax = NULL;

            number_of_instructions = 0;
            semantic = NULL;
        }
        size_t Compile(const char* in_file, const char* out_file);
        ~Compiler()
        {
            for(size_t i = 0; i < number_of_labels; ++i)
                delete [] labels[i];
            delete [] labels;
            //delete [] functions;
            delete [] addresses;
            delete [] syntax;
            delete [] semantic;
        }
};

Flag Compiler::GetFlag(char data[])
{
    assert(data != NULL);

    int len = strlen(data);
    for(int i = 0; i < len; ++i)
        data[i] = toupper(data[i]);

    if(strcmp(data, "PUSH") == 0)
        return CMD;
    if(strcmp(data, "POP") == 0)
        return CMD;
    if(strcmp(data, "ADD") == 0)
        return CMD;
    if(strcmp(data, "SUB") == 0)
        return CMD;
    if(strcmp(data, "MUL") == 0)
        return CMD;
    if(strcmp(data, "DIV") == 0)
        return CMD;
    if(strcmp(data, "MOD") == 0)
        return CMD;
    if(strcmp(data, "INPUT") == 0)
        return CMD;
    if(strcmp(data, "OUTPUT") == 0)
        return CMD;
    if(strcmp(data, "DUMP") == 0)
        return CMD;
    if(strcmp(data, "JMP") == 0)
        return CMD;
    if(strcmp(data, "BEGIN") == 0)
        return CMD;
    if(strcmp(data, "END") == 0)
        return CMD;
    if(strcmp(data, "SQRT") == 0)
        return CMD;
    if(strcmp(data, "TOP") == 0)
        return CMD;
    if(strcmp(data, "ABS") == 0)
        return CMD;
    if(strcmp(data, "CMP") == 0)
        return CMD;
    if(strcmp(data, "JE") == 0)
        return CMD;
    if(strcmp(data, "JNE") == 0)
        return CMD;
    if(strcmp(data, "JB") == 0)
        return CMD;
    if(strcmp(data, "JBE") == 0)
        return CMD;
    if(strcmp(data, "JA") == 0)
        return CMD;
    if(strcmp(data, "JAE") == 0)
        return CMD;

    if(strcmp(data, "AX") == 0)
        return REG;
    if(strcmp(data, "BX") == 0)
        return REG;
    if(strcmp(data, "CX") == 0)
        return REG;
    if(strcmp(data, "DX") == 0)
        return REG;
    if(strcmp(data, "SI") == 0)
        return REG;
    if(strcmp(data, "DI") == 0)
        return REG;
    if(strcmp(data, "BP") == 0)
        return REG;

    if(IsLabel(data))
        return LABEL;

    if(IsLabelArg(data))
        return LABEL_ARG;

    if(IsNumeral(data))
        return NUM;
    else
        return ERR_FLAG;
}

double Compiler::GetObject(char data[])
{
    switch(GetFlag(data))
    {
        case CMD:
            return (double)GetCommand(data);
        case REG:
            return (double)GetRegister(data);
        case NUM:
            return atof(data);
        case LABEL:
        case LABEL_ARG:
            return (double)CorrectLabel(data);
        default:
            return 0;
    }
}

Command Compiler::GetCommand(char data[])
{
    assert(data != NULL);

    int len = strlen(data);
    for(int i = 0; i < len; ++i)
        data[i] = toupper(data[i]);

    if(strcmp(data, "PUSH") == 0)
        return PUSH;
    if(strcmp(data, "POP") == 0)
        return POP;
    if(strcmp(data, "ADD") == 0)
        return ADD;
    if(strcmp(data, "SUB") == 0)
        return SUB;
    if(strcmp(data, "MUL") == 0)
        return MUL;
    if(strcmp(data, "DIV") == 0)
        return DIV;
    if(strcmp(data, "MOD") == 0)
        return MOD;
    if(strcmp(data, "INPUT") == 0)
        return INPUT;
    if(strcmp(data, "OUTPUT") == 0)
        return OUTPUT;
    if(strcmp(data, "DUMP") == 0)
        return DUMP;
    if(strcmp(data, "JMP") == 0)
        return JMP;
    if(strcmp(data, "BEGIN") == 0)
        return BEGIN;
    if(strcmp(data, "END") == 0)
        return END;
    if(strcmp(data, "SQRT") == 0)
        return SQRT;
    if(strcmp(data, "TOP") == 0)
        return TOP;
    if(strcmp(data, "ABS") == 0)
        return ABS;
    if(strcmp(data, "CMP") == 0)
        return CMP;
    if(strcmp(data, "JE") == 0)
        return JE;
    if(strcmp(data, "JNE") == 0)
        return JNE;
    if(strcmp(data, "JB") == 0)
        return JB;
    if(strcmp(data, "JBE") == 0)
        return JBE;
    if(strcmp(data, "JA") == 0)
        return JA;
    if(strcmp(data, "JAE") == 0)
        return JAE;
    else
        return ERR_CMD;
}

Register Compiler::GetRegister(char data[])
{
    assert(data != NULL);

    int len = strlen(data);
    for(int i = 0; i < len; ++i)
        data[i] = toupper(data[i]);

    if(strcmp(data, "AX") == 0)
        return AX;
    if(strcmp(data, "BX") == 0)
        return BX;
    if(strcmp(data, "CX") == 0)
        return CX;
    if(strcmp(data, "DX") == 0)
        return DX;
    if(strcmp(data, "SI") == 0)
        return SI;
    if(strcmp(data, "DI") == 0)
        return DI;
    if(strcmp(data, "BP") == 0)
        return BP;
    else
        return ERR_REG;
}

size_t Compiler::CorrectLabel(char data[])
{
    assert(data != NULL);
    char* res = DeleteColumn(data);

    for(size_t i = 0; i < number_of_labels; ++i)
        if(strcmp(res, labels[i]) == 0)
            return i;
    return NOT_FOUND;
}

void Compiler::LabelRegistrator(char** pointers)
{
    assert(pointers != NULL);

    for(size_t i = 0; i < number_of_lexems; ++i)
        if(IsLabel(pointers[i]))
            ++number_of_labels;

    labels = new char*[number_of_labels];
    for(size_t i = 0; i < number_of_labels; ++i)
        labels[i] = new char[MAXLEN];

    size_t label_counter = 0;
    for(size_t i = 0; i < number_of_lexems; ++i)
        if(IsLabel(pointers[i]))
        {
            char* res = DeleteColumn(pointers[i]);
            int len = strlen(res);
            for(int i = 0; i < len; ++i)
                res[i] = toupper(res[i]);
            strcpy(labels[label_counter], res);
            ++label_counter;
        }
}

void Compiler::SyntaxAnalysis(char** pointers)
{
    assert(pointers != NULL);
    syntax = new Lexem[number_of_lexems];
    for(size_t lexem_counter = 0; lexem_counter < number_of_lexems; ++lexem_counter)
    {
        /// Get flag of the command
        Flag flag = GetFlag(pointers[lexem_counter]);
        if(flag == ERR_FLAG)
            CompError(UNKNOWN, lexem_counter + 1);
        syntax[lexem_counter].flag = flag;

        /// Get the description of the command
        syntax[lexem_counter].obj = GetObject(pointers[lexem_counter]);
        //if(flag == LABEL || flag == FUNCTION)
        if(flag == LABEL || flag == LABEL_ARG)
            if(syntax[lexem_counter].obj == NOT_FOUND)
                CompError(UNKNOWN, lexem_counter + 1);
    }
}

void Compiler::SemanticAnalysis()
{
    semantic = new Instruction[number_of_lexems];
    addresses = new size_t[number_of_labels];
    int instr_counter = 0;
    int number = 0;
    for(size_t lexem_counter = 0; lexem_counter < number_of_lexems; ++lexem_counter)
    {
        /// Get information about flag in element
        switch(syntax[lexem_counter].flag)
        {
            case CMD:
                number = FlagCMD(lexem_counter, instr_counter);
                lexem_counter += number;
                break;

            /// These variants must be handled at another command
            case REG:
            case NUM:
            case LABEL_ARG:
                CompError(WRONG_TOKEN, instr_counter + 1);

            case LABEL:
                FlagLABEL(lexem_counter, instr_counter);
                break;

            default:
                printf("%d\n", syntax[lexem_counter].flag);
                exit(1);
        }
        ++instr_counter;
    }
    number_of_instructions = instr_counter;

    /// Addresses of jumps
    for(size_t i = 0; i < number_of_lexems; ++i)
        if(semantic[i].arg_flag == LABEL_ARG)
        {
            semantic[i].arg_flag = ADDRESS;
            int lab_num = (int)semantic[i].value;
            semantic[i].value = addresses[lab_num];
        }
}

///@return number of extra lexems
int Compiler::FlagCMD(size_t lexem_counter, size_t instr_counter)
{
    /// Handling all syntax errors
    semantic[instr_counter].cmd_flag = CMD;
    switch((int)syntax[lexem_counter].obj)
    {
        case PUSH:
        case POP:
        case TOP:
        case INPUT:
        case OUTPUT:
            CommandWithArgument(lexem_counter, instr_counter);
            return 1;

        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOD:
        case SQRT:
        case BEGIN:
        case END:
        case DUMP:
        case ABS:
        case CMP:
            CommandNoArgument(lexem_counter, instr_counter);
            return 0;

        case JMP:
        case JE:
        case JNE:
        case JB:
        case JBE:
        case JA:
        case JAE:
            CommandJump(lexem_counter, instr_counter);
            return 1;

        default:
            printf("%lg\n", syntax[lexem_counter].obj);
            exit(1);
    }
}

void Compiler::CommandWithArgument(size_t lexem_counter, size_t instr_counter)
{
    /// List can't end with this word
    if(lexem_counter == number_of_lexems - 1)
        CompError(NEED_ARG, instr_counter + 1);

    /// List can't end with this command
    if(lexem_counter == number_of_lexems - 2)
        CompError(WRONG_END, instr_counter + 1);

    semantic[instr_counter].cmd_code = (Command)syntax[lexem_counter].obj;

    /// Next step is depend on type of argument
    switch(syntax[lexem_counter + 1].flag)
    {
    case REG:
        semantic[instr_counter].arg_flag = REG;
        semantic[instr_counter].value = syntax[lexem_counter + 1].obj;
        break;

    case NUM:
        if(syntax[lexem_counter].obj != PUSH)
            CompError(NEED_ARG, instr_counter + 1);
        semantic[instr_counter].arg_flag = NUM;
        semantic[instr_counter].value = syntax[lexem_counter + 1].obj;
        break;

    default:
        /// Next word must be an argument: numeral or register
        CompError(NEED_ARG, instr_counter + 1);
    }
}

void Compiler::CommandNoArgument(size_t lexem_counter, size_t instr_counter)
{
    /// The next word CAN'T be an argument
    /// if next word exist
    if(lexem_counter != number_of_lexems - 1
       && syntax[lexem_counter + 1].flag != CMD
       && syntax[lexem_counter + 1].flag != LABEL)
       CompError(NO_ARG, instr_counter + 1);

    /// List can't end with this command
    if(lexem_counter == number_of_lexems - 1
       && syntax[lexem_counter].obj != END)
        CompError(WRONG_END, instr_counter + 1);

    semantic[instr_counter].cmd_code = (Command)syntax[lexem_counter].obj;
    semantic[instr_counter].arg_flag = NUL;
    semantic[instr_counter].value = 0;
}

void Compiler::CommandJump(size_t lexem_counter, size_t instr_counter)
{
    /// The next word must be label argument
    if(lexem_counter == number_of_lexems - 1
       || syntax[lexem_counter + 1].flag != LABEL_ARG)
       CompError(NEED_ARG, instr_counter + 1);

    semantic[instr_counter].cmd_code = (Command)syntax[lexem_counter].obj;
    semantic[instr_counter].arg_flag = LABEL_ARG;
    semantic[instr_counter].value = syntax[lexem_counter + 1].obj;
}

void Compiler::FlagLABEL(size_t lexem_counter, size_t instr_counter)
{
    int index = (int)syntax[lexem_counter].obj;
    addresses[index] = instr_counter;

    semantic[instr_counter].cmd_flag = LABEL;
    semantic[instr_counter].cmd_code = ERR_CMD;
    semantic[instr_counter].arg_flag = NUL;
    semantic[instr_counter].value = syntax[lexem_counter].obj;
}

void TestSemantic(Instruction* semantic, size_t num)
{
    printf("\n");
    std::cout << "Semantic analysis" << std::endl << std::endl;
    for(size_t i = 0; i < num; ++i)
    {
        std::cout << i << " command" << std::endl;
        std::cout << semantic[i].cmd_flag << std::endl;
        std::cout << semantic[i].cmd_code << std::endl;
        std::cout << semantic[i].arg_flag << std::endl;
        std::cout << semantic[i].value << std::endl << std::endl;
    }
    return;
}

void Compiler::SemanticToFile(const char* out_file)
{
    assert(out_file != NULL);
    FILE * out = fopen(out_file, "w");
    for(size_t i = 0; i < number_of_instructions; ++i)
    {
        fprintf(out, "%d", semantic[i].cmd_flag);
        fputs(" ", out);
        fprintf(out, "%d", semantic[i].cmd_code);
        fputs(" ", out);
        fprintf(out, "%d", semantic[i].arg_flag);
        fputs(" ", out);
        fprintf(out, "%g", semantic[i].value);
        fputs("\n", out);
    }
    fclose(out);
}

size_t Compiler::Compile(const char* in_file, const char* out_file)
{
    /// Enter data from the file
    char* buffer = FileToArray(in_file);

    /// Splitting by the words
    char** pointers = MyStrtok(buffer, " \n\t", &number_of_lexems);
    if(pointers == NULL)
    {
        printf("Error: empty array\n");
        exit(1);
    }
  
    /// Registration of labels and functions
    LabelRegistrator(pointers);

    /// Syntax analysis
    SyntaxAnalysis(pointers);
    delete [] pointers;
    delete [] buffer;

    /// Semantic analysis
    SemanticAnalysis();

    /// Printing in the .o file
    SemanticToFile(out_file);

    return number_of_instructions;
}
