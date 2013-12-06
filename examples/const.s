        ;; Sample program in README.
		;; Gets an input and, regardless of it, shows
		;; -16 on the screen (-0x10).
SECTION TEXT
ROT: INPUT N1
     COPY N2, N1
     COPY N2, N3[0]
     COPY N3[0], N3[1]
     OUTPUT N3[1]
     STOP

SECTION DATA
N1:  SPACE
N2:  CONST -0x10
N3:  SPACE 2
