        MOV edx,DS
        mov [edx+4],1
        mov ecx,3
mult:   mul [edx+4],ecx
        add ecx,-1
        JNZ mult
        STOP


