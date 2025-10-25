\\DATA 2000
\\STACK 2000
POT:	PUSH BP
        MOV BP,SP
        PUSH EDX
        PUSH ECX
        MOV ECX,[BP+8]	;asigno el segundo parametro 
        MOV EDX,[BP+12]	;asigno el primer parametro
        MOV EAX,EDX	;devuelvo en eax
        SUB ECX,1
        JZ FIN
OTRO:	MUL EAX,EDX
        SUB ECX,1
        JNP OTRO
FIN:	POP ECX
        POP EDX
        MOV SP,BP
        POP BP
        RET

MAIN:   PUSH BP
        MOV BP,SP
        MOV EDX,DS
        LDH ECX,4
        LDL ECX,2
        MOV EAX,1
        SYS 1
        PUSH [DS]
        PUSH [DS+4]
        CALL POT
        ADD SP,8
        MOV EDX,EAX
        MOV EAX,1
        LDH ECX,4
        LDL ECX,1
        SYS 2
        MOV SP,BP
        POP BP
        RET