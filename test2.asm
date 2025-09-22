	    MOV [4],0
	    LDH ECX,4
	    LDL ECX,1
	    MOV EDX,DS
	    MOV EAX,1
	    SYS 1
Inicio:	CMP [0],0
	    JZ Fin
	    OR [4],[0]
	    DIV [4],2
	    SHR [0],1
	    CMP AC,0
 	    JNZ IMP
	    MOV [4],0
	    JMP WRT
Imp:	MOV [4],1
Wrt:	LDH ECX,4  	;PREPARO TODO PARA PRINTEAR
	    LDL ECX, 1
	    MOV [EDX],[4]
	    MOV EAX,1 
	    SYS 2
Fin:	STOP
	