;; Fibonacci of a number read from standard input.

        SECTION DATA
ONE:    CONST   0x000000000000000001 ; lel
I:      SPACE
NUM:    SPACE
OLDNUM: SPACE

        SECTION TEXT
        INPUT   I
        COPY    UM,     NUM
        COPY    UM,     OLDNUM

LOOP:   LOAD    NUM
        ADD     OLDNUM

        COPY    NUM,     OLDNUM
        STORE   NUM

        LOAD    I
        SUB     UM              ; i--
        JMPZ    END             ; if (i == 0)
        JMP     LOOP            ;    goto END

END:    OUTPUT  NUM
        STOP

