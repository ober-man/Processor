enum Flag
{
    ERR_FLAG = 0,
    CMD = 1,
    NUM = 2,
    REG = 3,
    NUL = 4,
    LABEL = 5,
    LABEL_ARG = 6,
    ADDRESS = 7,
};

enum Command
{
    ERR_CMD = 100,
    PUSH = 101,
    POP = 102,
    ADD = 103,
    SUB = 104, /// up - down
    MUL = 105,
    DIV = 106, /// up : down
    MOD = 107, /// up % down
    INPUT = 108,
    OUTPUT = 109,
    DUMP = 110,
    JMP = 111,
    BEGIN = 112,
    END = 113,
    SQRT = 114,
    TOP = 115,
    ABS = 116,
    CMP = 117,
    JE = 118,
    JNE = 119,
    JB = 120,
    JBE = 121,
    JA = 122,
    JAE = 123
};

enum Register
{
    ERR_REG = 200,
    AX = 201,
    BX = 202,
    CX = 203,
    DX = 204,
    SI = 205,
    DI = 206,
    BP = 207
};

enum Error
{
    UNKNOWN = 300,      /// if label is wrong
    WRONG_TOKEN = 301,  /// if was used unknown word
    NEED_ARG = 302,     /// if function have no argument, but it must have
    NO_ARG = 303,       /// if function have argument, but it mustn't have
    WRONG_END = 304,    /// if list of commands ends with wrong command
    NO_BEGIN = 305,     /// if list of commands doesn't contain BEGIN
    MANY_BEGIN = 306,   /// if there is more than 1 words BEGIN


};

/// Structure using in syntax analysis
/// contain one object (with flag and code)
struct Lexem
{
    Flag flag; //Flag
    double obj; //object_t
};

/// Structure using in semantic analysis
/// contain command (with flag and code)
/// and argument (with type and value)
struct Instruction
{
    int cmd_flag; //Flag
    int cmd_code; //Command
    int arg_flag; //Flag
    double value; //argument_t
};
