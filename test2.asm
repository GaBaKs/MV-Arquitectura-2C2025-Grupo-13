\\DATA 1024
\\STACK 2000

SLEN:	PUSH BP
        MOV BP,SP
        PUSH EDX	;guardo el puntero
        MOV EDX,[BP+8]
        MOV EAX,0	;inicializo el contador
OTRO:	CMP [EDX],0	;chequeo si es\0 tabla ASCII es el 0 
        JZ FIN
        ADD EAX,1
        ADD EDX,1	;avanzo en la memoria un byte desde el puntero
        JMP OTRO
FIN:	POP EDX
        MOV SP,BP
        POP BP
        RET

MAIN:   PUSH BP
        MOV BP,SP
        MOV EDX,DS
        MOV ECX,50
        SYS 3
        MOV EDX,DS
        PUSH EDX
        CALL SLEN   ;devuelvo en eax
        ADD SP,4
        MOV [DS+100],EAX
        MOV EDX,DS
        ADD EDX,100
        LDH ECX,4
        LDL ECX,1
        MOV EAX,1
        SYS 2
        MOV SP,BP
        POP BP
        RET