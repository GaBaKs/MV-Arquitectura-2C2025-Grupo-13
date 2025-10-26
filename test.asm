\\STACK 30
\\DATA 200
\\EXTRA 3
nombre equ "\n puto el que lee\n"
numero equ "55"
main:   PUSH bp
        MOV bp,sp
        MOV EEX,10
        PUSH EEX
        MOV EDX, DS
        ADD EDX, 123
        MOV ECX, 50
        SYS 0x03
        mov edx, DS
        add edx,123
        sys 4
        pop EEX
        mov sp,bp
        pop bp
        ret