;;; Receives an input and echoes it back.
;;  _      _     ___   _     _  _____ _____  _     ____  ___    __    __   _
;; | |\/| \ \_/ |___| | |   | |  | |   | |  | |   | |_  |___|  / /\  ( (` | |\/|
;; |_|  |  |_|        |_|__ |_|  |_|   |_|  |_|__ |_|__       /_/--\ _)_) |_|  |

        SECTION TEXT
ROT:    INPUT   N1[0]                 ; getc(N1)
        COPY    N1[0], N2             ; N2 = N1
        OUTPUT  N2                    ; printf(N2)
        STOP

        SECTION DATA                  ; where variables are stored
N1:     SPACE   10                    ; array[10]
N2:     SPACE
N3:     CONST   4
N4:     CONST   0b1001                ; Binary constants(!)
N5:     CONST   0X30                  ; Case-insensitive 0x1 0X1 0b1 0B1

