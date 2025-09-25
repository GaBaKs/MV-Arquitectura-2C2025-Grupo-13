#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA.h"
#include "mascaras.h"
#include "Operaciones_Generales.h"
#include "Codigos_Registros.h"
// hacer 3 typedef separados

void getOperandosDissa(TipoMKV MKV,char instruccion,int dirfis,int *opA,int *opB, int TopA, int TopB){              
        int LopA=0;
        int LopB=0;
        dirfis++;
        for (int i=0;i<TopB;i++){
            LopB= (LopB)<<8;
            LopB+=MKV.mem[dirfis++];   
        }
        if (TopB==2)
            LopB=escopeta2bytes(LopB);
        if (TopA==0)
            LopA=LopB;
        else
            for (int i=0;i<TopA;i++){ 
                LopA=(LopA)<<8;                   
                LopA+=MKV.mem[dirfis++]; 
            } 
       *opA = (int)LopA;
       *opB = (int)LopB;
}

void devuelveRegistro(unsigned char car){
    int car2=(int) car;
   switch(car2) {
    case 0x03:
        printf("IP");
        break;
    case 17:
        printf("CC");
        break;
    case 0x1A:
        printf("CS");
        break;
    case 0x1B:  
        printf("DS");
        break;
    case 0x10:
        printf("AC");
        break;
    case 0x0A:
        printf("EAX");
        break;
    case 0x0B:
        printf("EBX");
        break;
    case 0x0C:
        printf("ECX");
        break;
    case 0x0D:
        printf("EDX");
        break;
    case 0x0E:
        printf("EEX");
        break;
    case 0x0F:
        printf("EFX");
        break;
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
     switch (TopA+TopB+1){
            case 6:
                printf("");
            case 5:
                printf("           ");
                break;
            case 4:
                printf("               ");
                break;
            case 3:
                printf("                   ");
                break;
            case 2:
                printf("            ");
                break;
            case 1:
                printf("               ");
                break;
        }
        printf(" | ");

   
}

void imprimeOperandos(TipoMKV MKV,int dirfis, int TopA,int TopB ){

    int dirmax=dirfis+TopA+TopB+1;
    for (int i=dirfis;i<dirmax;i++){
        printf(" %02X ",MKV.mem[i]);
    }
}
void dissa(TipoMKV MKV){
    int dirfis,cod,opA,opB,TopA,TopB;
    unsigned char instruccion;
    dirfis=logifisi(MKV,MKV.reg[CS]);
    instruccion=MKV.mem[dirfis];
    while(dirfis<(MKV.tabla_seg[1]+MKV.tabla_seg[0])){
        cod=instruccion & MASC_CODOP;
        TopB=(instruccion & MASC_TOPB) >> 6; 
        TopA=(instruccion & MASC_TOPA) >> 4; 
        getOperandosDissa(MKV,instruccion,dirfis,&opA,&opB,TopA,TopB);
        printf("[%04X] ",dirfis);
        imprimeOperandos(MKV,dirfis,TopA,TopB);
        imprimeTAB(instruccion,TopA,TopB);
        imprimeMnemonico(cod);
        if (cod>=0x10 && cod<=0x1F){
            if (TopA==3){
                if ((opA&MASC_OFFSET )==0){
                    printf("\t[");
                    devuelveRegistro(((opA&MASC_CODMEM)>>16));
                    printf("],");
                }    
                else{
                    printf("\t[");
                    devuelveRegistro((opA&MASC_CODMEM)>>16);
                    printf("+%d],",opA&MASC_OFFSET);
                }            
            }
            else{
                if (TopA==1){
                    printf("\t");
                    devuelveRegistro(opA);
                    printf(",");
                }
            }        
                
            if (TopB==3){
                if ((opB&MASC_OFFSET)==0){
                    printf("[");
                    devuelveRegistro(((opB&MASC_CODMEM)>>16));
                    printf("]");
                }
                else{
                    printf("[");
                    devuelveRegistro((opB&MASC_CODMEM)>>16);
                    printf("+%d]",opB&MASC_OFFSET);
                }
            }
            else
                if (TopB==2){
                    printf(" %d",opB);
                }      
                else{
                    //printf("\t ");
                    devuelveRegistro(opB);    
                } 
        }
        else{//de un operando
            switch (TopB){
            case 3:
                if ((opA&MASC_OFFSET)==0){
                    printf("\t[");
                    devuelveRegistro((opA&MASC_CODMEM)>>16);
                    printf("]");
                    
                }     
                else{
                    printf("\t[");
                    devuelveRegistro((opA&MASC_CODMEM)>>16);
                    printf("+%d]",opA&MASC_OFFSET);
                }
                    
                break;
            case 2:
                    printf("\t%d",opA); 
                    //devuelveRegistro(((opA&MASC_CODMEM)>>16));    
                break;
            default:
                printf("\t%d",opA); 
                break;
            }
        }
        printf(" \n");
        //dirfis=nuevadirfis(dirfis,TopA,TopB);
        dirfis+=TopA+TopB+1;
        instruccion=MKV.mem[dirfis];
    }/*
    cod=instruccion & MASC_CODOP;
    printf("[%04X] ",dirfis);
    imprimeMnemonico(cod);//deberia de ser STOP
    */
}

