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

void devuelveRegistro(int registro){
   switch(registro){ 
    case 0x00:
        printf("LAR");
        break;
    case 0x01:
        printf("MAR");
        break;
    case 0x02:
        printf("MBR");
        break;
    case 0x03:
        printf("IP");
        break;
    case 0x04:
        printf("OPC");
    break;
    case 0x05:
        printf("OP1");
        break;
    case 0x06:
        printf("OP2");
        break;
    case 0x07:
        printf("SP");
        break;
    case 0x08:
        printf("BP");
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
    case 0x1C:
        printf("ES");
        break;
    case 0x1D:
        printf("SS");
        break;
    case 0x1E:
        printf("KS");
        break;
    case 0x1F:
        printf("PS");
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
    case 0x4A:// 01 0 01010
        printf("AL");
        break;
    case 0x4B:
        printf("BL");
        break;
    case 0x4C:
        printf("CL");
        break;
    case 0x4D:
        printf("DL");
        break;
    case 0x4E:
        printf("EL");
        break;
    case 0x4F:
        printf("FL");
        break;
    case 0x8A:
        printf("AH");
        break;
    case 0x8B:
        printf("BH");   
        break;
    case 0x8C:
        printf("CH");
        break;  
    case 0x8D:
        printf("DH");
        break;
    case 0x8E:
        printf("EH");
        break;
    case 0x8F:
        printf("FH");
        break;
    case 0xCA:
        printf("AX");  
        break;
    case 0xCB:
        printf("BX");
        break;
    case 0xCC:
        printf("CX");
        break;
    case 0xCD:
        printf("DX");
        break;
    case 0xCE:
        printf("EX");
        break;
    case 0xCF:
        printf("FX");
        break;
    default:
        printf("REGISTRO INVALIDO");
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
        case 0x0B:
            printf("PUSH ");
            break;
        case 0x0C:
            printf("POP ");
            break;
        case 0x0D:
            printf("CALL ");
            break;
        case 0x0E:
            printf("RET \n");
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
     int valormax=1+TopB+TopA;
        for (int i=valormax;i<=7;i++)
            printf("    ");
        printf(" | ");
}

void imprimeOperandos(TipoMKV MKV,int dirfis, int TopA,int TopB ){
    int dirmax=dirfis+TopA+TopB+1;
    for (int i=dirfis;i<dirmax;i++){
        printf(" %02X ",MKV.mem[i]);
    }
}

void imprimehexa_Const(TipoMKV MKV,int dirfis,int *cant){
    int dirmax=dirfis+6;
    *cant=1;
    while (MKV.mem[dirfis]!=0 && *cant<=6){
        printf(" %02X ",MKV.mem[dirfis]);
        dirfis++;
        (*cant)++;
    }
    //printf("el cant es %d\n",*cant);
    if (*cant==7){
         printf(" .. ");
    }
    if (*cant<6)
        printf(" %02X ",MKV.mem[dirfis]); //imprimo el 0 final
    (*cant)++;
}
void imprimeTAB_Const(int cant){
        for (int i=cant;i<=8;i++)
            printf("    ");
        printf(" | ");
}

void ImprimeKS(TipoMKV MKV){
    int cant=0;
    int dirfis;
        dirfis=logifisi(MKV,MKV.reg[KS]);  
        while ((dirfis<(MKV.tabla_seg[(MKV.reg[KS] & MASC_LDH) >>16][0]  +  MKV.tabla_seg[(MKV.reg[KS] & MASC_LDH) >>16][1]))){
            printf(" [%04X]",dirfis);
            imprimehexa_Const(MKV,dirfis,&cant);
            imprimeTAB_Const(cant);
            int i=0;
            char palabra[200];
            printf("\"");
            while(MKV.mem[dirfis+i]!=0 && dirfis+i<=MKV.tabla_seg[KS][0]+MKV.tabla_seg[KS][1]){ 
                if (MKV.mem[dirfis+i]>=32 && MKV.mem[dirfis+i]<=126){ //caracteres imprimibles
                    palabra[i]=MKV.mem[dirfis+i];
                }
                else{
                    palabra[i]='.';
                }
                i++;
            }
            palabra[i]='\0';
            printf("%s\"\n",palabra);
            dirfis+=i+1;
        } 
}
void dissa(TipoMKV MKV){
    int dirfis,cod,opA,opB,TopA,TopB;
    int dirfisEntrypoint;
    unsigned char instruccion;
        if (MKV.reg[KS]!=-1)
            ImprimeKS(MKV);
        dirfis=logifisi(MKV,MKV.reg[CS]);
        instruccion=MKV.mem[dirfis];
        dirfisEntrypoint=logifisi(MKV,MKV.reg[IP]);
        while(dirfis<(MKV.tabla_seg[(MKV.reg[CS] & MASC_LDH) >>16][0]  +  MKV.tabla_seg[(MKV.reg[CS] & MASC_LDH) >>16][1])){ 
            cod=instruccion & MASC_CODOP;
            TopB=(instruccion & MASC_TOPB) >> 6; 
            TopA=(instruccion & MASC_TOPA) >> 4; 
            getOperandosDissa(MKV,instruccion,dirfis,&opA,&opB,TopA,TopB);
            if (dirfis==dirfisEntrypoint){
                printf(">[%04X]",dirfis);  
            }
            else
                printf(" [%04X]",dirfis);
            
            imprimeOperandos(MKV,dirfis,TopA,TopB);
            imprimeTAB(instruccion,TopA,TopB);
            imprimeMnemonico(cod);
            if (cod>=0x10 && cod<=0x1F){//dos operandos
                if (TopA==3){
                    if ((opA&MASC_CODMEM)==0)
                        printf("\t");
                    else
                        if ((opA&MASC_CODMEM)==2)
                            printf("\tw");
                        else
                            if ((opA&MASC_CODMEM)==3)                        
                                printf("\tb");
                    if((opA&MASC_OFFSET)==0){
                        printf("[");
                        devuelveRegistro((opA&MASC_CODMEM)>>16);
                        printf("],");
                    }    
                    else{
                        printf("[");
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
                    if ((opB&MASC_CODMEM)==0)
                        printf("");
                    else
                        if ((opB&MASC_CODMEM)==2)
                            printf("w");
                        else
                            if ((opB&MASC_CODMEM)==3)
                                printf("b");
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
                    if ((opA&MASC_CODMEM)==0)
                        printf("\t");
                    else
                        if ((opA&MASC_CODMEM)==2)
                            printf("\tw");
                        else
                            if ((opA&MASC_CODMEM)==3)                        
                                printf("\tb");                
                    if ((opA&MASC_OFFSET)==0){
                        printf("[");
                        devuelveRegistro((opA&MASC_CODMEM)>>16);
                        printf("]");
                        
                    }     
                    else{
                        printf("[");
                        devuelveRegistro((opA&MASC_CODMEM)>>16);
                        printf("+%d]",opA&MASC_OFFSET);
                    }     
                    break;
                case 2:
                    printf("\t%d",opA);   
                    break;
                case 1:
                    printf("\t");
                    devuelveRegistro(opA);    
                    break;                
                }
        }
        printf(" \n");
        dirfis+=TopA+TopB+1;
        instruccion=MKV.mem[dirfis];
    }
}

