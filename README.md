# my-little-asm

`mla` is my custom Assembly-like language, implemented from
scratch in C++. It has an `assembler` and `simulator`.

![mla](http://alexdantas.net/projects/images/my-little-asm.png)

This is an example on how Assembly languages work - a set of
human-readable instructions that are translated into machine
code. Normally the language would be designed for some hardware
to execute, but in our case we simulate it step by step.

With our basic instruction set we can potentially write any
existing program out there - also called [Turing-completeness](http://en.wikipedia.org/wiki/Turing_completeness).

Long history short, we have a parser that reads each instruction
on the text file and converts them into integers - indexing
all 14 possible instructions.

The output file is then fed into the simulator,
that actually implements those instructions.

## Usage

First, make sure to compile everything by doing:

    make

Then, write any program according to the [language definition](#language-definition) below - saving with the `.s` extension. Then:

    $ ./assembler file.s file.o
    $ ./simulator file.o

See sample programs under the `examples` folder.

## Sample code

```asm
SECTION TEXT
LABEL:   INPUT  N1
         COPY   N1,    N2     ; comment
		 COPY   N2,    N3[0]  ; wow, array support!
		 COPY   N3[0], N3[1]
		 OUTPUT N3[1]
		 STOP

SECTION DATA
N1: SPACE
N3: SPACE  4        ; Array of size 4
N2: CONST  -0x10    ; Look ma, hex constant!
```

## Language definition

`mla` follows a MIPS Assembly-like syntax.

Highlights:

* 14 instructions.
* `;` starts comment until the end of line.
* Labels references memory addresses.
* Accumulator register (`ACC`) implied on arithmetic instructions.

Code is divided in two sections:

* Data section (for variables and constants)
* Text section (for actual code)

## Instructions

| Name                | Instruction  | Description                |
| :-----------------: | :----------: | :------------------------: |
| Add                 | `ADD`        | `ACC <- ACC + MEM[OP]`     |
| Subtract            | `SUB`        | `ACC <- ACC - MEM[OP]`     |
| Multiply            | `MULT`       | `ACC <- ACC * MEM[OP]` |
| Divide              | `DIV`        | `ACC <- ACC / MEM[OP]` |
| Jump Unconditional  | `JMP`        | `PC  <- OP` |
| Jump if Negative    | `JMPN`       | `If ACC < 0 then PC <- OP` |
| Jump if Positive    | `JMPP`       | `If ACC > 0 then PC <- OP` |
| Jump if Zero        | `JMPZ`       | `If ACC = 0 then PC <- OP` |
| Copy                | `COPY`       | `MEM[OP2] <- MEM[OP1]` |
| Load from Memory    | `LOAD`       | `ACC <- MEM[OP]` |
| Store into Memory   | `STORE`      | `MEM[OP] <- ACC` |
| Store Input         | `INPUT`      | `MEM[OP] <- STDIN` |
| Output Memory       | `OUTPUT`     | `STDOUT <- MEM[OP]` |
| Stop Execution      | `STOP`       | `stops program` |

Note:

* `MEM[OP]` means memory address specified by operand and `PC`
  is the program counter (current instruction address).

## Directives

| Name            | Effect  |
| :-------------: | :-----: |
| `SECTION TEXT`  | Signals begin of instructions. |
| `SECTION DATA`  | Start of data definitions. |
| `SPACE`         | Saves an empty space on memory for data storage.|
| `SPACE N`       | Saves an empty array in memory of size `N`. |
| `CONST X`       | Saves a constant of value `X` in memory. |

## Contact

Hello, there! I'm Alexandre Dantas and I programmed this lovely
thing over two weeks. Don't take this project too seriously,
`mla` was made as an assignment for a _System Software_ class
on my Computer Science .

* Website: http://alexdantas.net/
* Email: <eu@alexdantas.net>

