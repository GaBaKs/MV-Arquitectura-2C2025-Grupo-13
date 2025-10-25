\\STACK 2000
\\DATA 1000
\\EXTRA 3000
nombre equ "\n puto el que lee\n"
numero equ "55"
main:   PUSH bp
        MOV bp,sp
        MOV EDX, DS
        ADD EDX, 123
        MOV ECX, 50
        SYS 0x03
        mov edx, DS
        add edx,123
        sys 4
        add SP,4
        mov sp,bp
        pop bp
        ret



