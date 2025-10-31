\\extra 1024
\\stack 1024

main: mov bp,SP
      mov eax,[bp]
      mov ecx,[bp+4]
      mov eex,[bp+8]
      
      
reinicio: mov edx,[eex]
          sys 4
      add edx,4
      sub ecx,1
      add eex,4
      cmp ecx,0
      jnz reinicio

      ret

