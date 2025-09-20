#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA.h"
#include "mascaras.h"
#include "Operaciones_Generales.c"

// hacer 3 typedef separados

void getOperandosDissa(TipoMKV MKV,char instruccion,int dirfis,int *opA,int *opB, int TopA, int TopB){              
        int LopA=0;
        int LopB=0;
        for (int i=0;i<TopB;i++){ 
            LopB+=MKV.mem[dirfis+1+i];
            LopB= (LopB)<<8;  
            printf("%X ",MKV.mem[dirfis+1+i]);
        }
        if (TopA==0)
            LopA=LopB;
        else
            for (int i=0;i<TopA;i++){                        
                        LopA+=MKV.mem[TopB+dirfis+1+i]; 
                        LopA=(LopA)<<8; 
                        printf("%X ",MKV.mem[TopB+dirfis+1+i]);  
            } 
       *opA = (int)LopA;
       *opB = (int)LopB;
}

char* devuelveRegistro(unsigned char car){
    switch(car){
        case 0x1A:
            return "CS";
            break;
        case 0x1B:  
            return "DS";
            break;
        case 0x10:
            return "AC";
            break;
        case 0x0A:
            return "EAX";
            break;
        case 0x0B:
            return "EBX";
            break;
        case 0x0C:
            return "ECX";
            break;
        case 0x0D:
            return "EDX";
            break;
        case 0x0E:
            return "EEX";
            break;
        case 0x0F:
            return "EFX";
    }
}

void imprimeMnemonico(unsigned char cod){ 

    switch(cod){
        case 0x00: 
            printf("SYS ");
            break;
        case 0x01: 
            printf("JMP ");
            break;
        case 0x02: 
            printf("JZ ");
            break;
        case 0x03:
            printf("JP ");
            break;
        case 0x04: 
            printf("JN ");
            break;
        case 0x05: 
            printf("JNZ ");
            break;
        case 0x06: 
            printf("JNP ");
            break;
        case 0x07: 
            printf("JNN ");
            break;
        case 0x08:
            printf("NOT ");
            break;
        case 0x10: 
            printf("MOV ");
            break;
        case 0x11: 
            printf("ADD ");
            break;
        case 0x12: 
            printf("SUB ");
            break;
        case 0x13: 
            printf("MUL ");
            break; 
        case 0x14: 
            printf("DIV ");
            break;
        case 0x15: 
            printf("CMP ");
            break;
        case 0x16: 
            printf("SHL ");
            break;
        case 0x17: 
            printf("SHR ");
            break;
        case 0x18: 
            printf("SAR ");
            break;
        case 0x19: 
            printf("AND ");
            break;
        case 0x1A: 
            printf("OR ");
            break;
        case 0x1B: 
            printf("XOR ");
            break;
        case 0x1C: 
            printf("SWAP ");
            break;    
        case 0x1D: 
            printf("LDL ");
            break;
        case 0x1E: 
            printf("LDH ");
            break;
        case 0x1F: 
            printf("RND ");
            break;
        case 0x0F:
            printf("STOP ");
            break;
    }
}

int nuevadirfis(int dirfis,int TopA,int TopB){
    int suma=1;
        if (TopA==0x01)
            suma++;
        else
            if (TopA==0x10)
                suma+=2;
            else
                if (TopA==0x11)
                    suma+=3;     
        if (TopB==0x01)
            suma++;
        else
            if (TopB==0x10)
                suma+=2;
            else
                if (TopB==0x11)
                    suma+=3;
        return dirfis+suma;
}

void imprimeTAB(char instruccion,int TopA,int TopB){

     switch (TopA+TopB){
            case 5:
                printf("   ");
                break;
            case 4:
                printf("      ");
                break;
            case 3:
                printf("         ");
                break;
            case 2:
                printf("            ");
                break;
            case 1:
                printf("                  ");
                break;
        }
        printf("   |  ");
}

void dissa(TipoMKV MKV){
    int dirfis,cod,opA,opB,TopA,TopB;
    char instruccion;
    dirfis=logifisi(MKV,MKV.reg[CS]);
    instruccion=MKV.mem[dirfis];
    while(instruccion!=0x0F){
        cod=instruccion & MASC_CODOP;
        TopB=instruccion & MASC_TOPB >> 6; 
        TopA=instruccion & MASC_TOPA >> 4; 
        printf("[%X] %X ",dirfis,cod);
        getOperandosDissa(MKV,instruccion,dirfis,&opA,&opB,TopA,TopB);
        imprimeTAB(instruccion,TopA,TopB);
        imprimeMnemonico(cod);
        if (cod>=0x10 && cod<=0x1F){
            if (TopA==3){
                if (opA&MASC_OFFSET!=0)
                    printf("\t[%s],",devuelveRegistro(opA&MASC_CODMEM>>16));
                else
                printf("\t[%s+%d],",devuelveRegistro(opA&MASC_CODMEM>>16),opA&MASC_OFFSET);
            }
            else
                if (TopA==2)
                    printf("\t \t%s,",devuelveRegistro(opA&MASC_CODMEM>>16));
            if (TopB==3){
                if (opB&MASC_OFFSET!=0)
                    printf("\t[%s]",devuelveRegistro(opB&MASC_CODMEM>>16));
                else
                printf("\t[%s+%d]",devuelveRegistro(opB&MASC_CODMEM>>16),opB&MASC_OFFSET);
            }
            else
                if (TopB==2)
                    printf("\t \t%s",devuelveRegistro(opB&MASC_CODMEM>>16));
                else
                    printf("\t \t%d",opB);       
        }
        else{//de un operando
            switch (TopA){
            case 3:
                if (opA&MASC_OFFSET!=0)
                    printf("\t[%s]",devuelveRegistro(opA&MASC_CODMEM>>16));
                else
                printf("\t[%s+%d]",devuelveRegistro(opA&MASC_CODMEM>>16),opA&MASC_OFFSET);
                break;
            case 2:
                printf("\t \t%s",devuelveRegistro(opA&MASC_CODMEM>>16));
                break;
            default:
                printf("\t \t%d",opA); 
                break;
            }
        }
        printf("\n");
        dirfis=nuevadirfis(dirfis,TopA,TopB);
        instruccion=MKV.mem[dirfis];
    }
    cod=instruccion & MASC_CODOP;
    printf("[%X] ",dirfis);
    imprimeMnemonico(cod);//deberia de ser STOP
}

