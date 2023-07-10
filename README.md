# Processor
Small processor work emulator with no architectural optimizations.

### How it works
* compiler parses input file with code in my own assembler language to a sequence of commands
* processor executes commands

Processor contains 7 user registers (AX, BX, CX, DX, SI, DI, BP), Insruction Pointer (IP) register, data stack and 2 flags (Zero Flag and Above Flag). 
It takes command stream and executes them one by one. Supporting commands: input/ouput, stack commands push/pop/top/dump, arithmetic add/sub/mul/div/mod, compare cmp, control transfer jmp/je/jne.

To see examples, open file **linear.txt** (solve linear equation $$ ax + b = 0 $$) or **factorial.txt** (count factorial of the input number).
