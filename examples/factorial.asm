;;; Simple factorial implementation.
;;; Receives number from standard input.

        SECTION DATA
ENTRY:  SPACE
RESULT: SPACE
ONE:    CONST   1

        SECTION TEXT
        COPY    ONE,    RESULT  ; fat = 1
        INPUT   ENTRY           ; a = scanf(int)

LOOP:   LOAD    ENTRY
        JMPZ    END
        JMPN    END             ; if (a <= 0) goto end

        LOAD    RESULT
        MULT    ENTRY
        STORE   RESULT          ; fat *= a

        LOAD    ENTRY
        SUB     ONE
        STORE   ENTRY           ; a--

        JMP     LOOP

END:    OUTPUT  RESULT          ; printf(fat)
        STOP

