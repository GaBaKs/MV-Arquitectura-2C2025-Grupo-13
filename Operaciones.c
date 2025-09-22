#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "TDA.h"
#include "mascaras.h"
#include "Operaciones_Generales.h"
#include "Codigos_Registros.h"

void NZ_CC (int valor, TipoMKV *MKV){
    MKV->reg[CC]=0;
    if (valor==0)
        MKV->reg[CC] |= 0x40000000; // setea el bit Z 0100 0000 0000 0000 0000 0000 0000 0000
    else
        if (valor<0)
            MKV->reg[CC]|=0x80000000; // setea el bit N 1000 0000 0000 0000 0000 0000 0000 0000
}   //lo afectan ADD, SUB, MUL, DIV, CMP, AND, OR, XOR, SHL, SHR, SAR, NOT


int get_Valor (TipoMKV *MKV,int op,int Top){

    if (Top==1)
        return MKV->reg[op];
    else
        if (Top==2)
            return escopeta2bytes(op);
        else
            if (Top==3){
                larmar(MKV,op);
                getMemoria(MKV);
                return MKV->reg[MBR];
            }
}

void MOV (TipoMKV *MKV,int opA, int TopA, int opB, int TopB){
    int dirfis,i;int valorB;
    int offset,cod; // para tipo memoria
    if (TopB!=2)
     valorB=get_Valor(MKV,opB,TopB);
    else
        valorB=opB;
    printf("valorB: %x opB= %x \n",valorB,opB);
    if (TopA==3){
        larmar(MKV,opA);
        MKV->reg[MBR]=valorB;
        setMemoria(MKV);
    }
    else
        MKV->reg[opA]=valorB;
         
}

void ADD (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA+= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void SUB (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA-= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void MUL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA*= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void DIV (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    if(valorB != 0){
        valorA = (int) valorA / valorB;
        MOV(MKV,opA,TopA,valorA,2);
        NZ_CC(valorA,MKV);
        MKV->reg[AC] = valorA % valorB; //guardo el resto en AC
    }
    else
        verificaerrores(MKV,2);     //error: division por cero
}

void CMP (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA-= valorB;
    printf("el valor de A ene le CMP%d\n",valorA);
    NZ_CC(valorA,MKV);
}

void SHL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA = valorA << valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void SHR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA&=0x7FFFFFFF;
    valorA>>=valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void SAR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA = valorA >> valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void AND (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA &= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void OR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA |= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void XOR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA ^= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void SWAP(TipoMKV *MKV,int opA,int TopA,int opB,int TopB){
    int valorB, valorA;
    int aux;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    MOV(MKV,opA,TopB,valorB,2);
    MOV(MKV,opB,TopA,valorA,2);
}

void LDL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB) & MASC_LDL ; 
    valorA = get_Valor(MKV,opA,TopA);
    valorA|=valorB;
    MOV(MKV,opA,TopA,valorA,2);
}

void LDH (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB) << 16; 
    valorA = get_Valor(MKV,opA,TopA);
    valorA|=valorB;
    MOV(MKV,opA,TopA,valorA,2);
}


void RND (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    srand(time(NULL));
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = rand() % (valorB + 1); //rand() genera un numero aleatorio y al hacerle % (valB + 1) se establece el rango entre 0 y valB.
    MOV(MKV,opA,TopA,valorA,2);
}

void SYS(TipoMKV *MKV, int opA, int TopA){
    int i,k,dirfis,j,tam,dato,x,y;
    unsigned int cantDatos;
    char c;
    char bin[33];
    dirfis=logifisi(*MKV,MKV->reg[EDX]);
    printf("VALOR DE EDX %x",MKV->reg[EDX]);
    printf("VALOR DE DS EN EL SYS: %x %d",MKV->reg[DS],MKV->reg[DS]);
    i=(MKV->reg[ECX] & MASC_LDH) >> 16; //tamanio por celda
    cantDatos=MKV->reg[ECX] & MASC_LDL;  //cant celdas (cant datos)
    switch(opA){

     case 1:
        //verifico que formato tengo que leer
        for (j=0;j<cantDatos;j++){         //cantidad de numeros a leer
            printf("[%04X]: ",dirfis);
            switch(MKV->reg[EAX]){

            case 0x01:scanf("%d",&dato);
                  break;
            case 0x02: scanf(" %c",&c);
                       dato=(int)c;
                  break;
            case 0x04: scanf("%o",&dato);
                  break;
            case 0x08: scanf("%x",&dato);
                  break;
            case 0x10: scanf("%s",bin);
                       dato=bintoint(bin);
                   break;
            default:
                    verificaerrores(MKV,4); // error de formato
            }
           if (dirfis+i*cantDatos<=MEMORIA && dirfis>=MKV->tabla_seg[2])
             for (int k=i-1;k>=0;k--){      //tamanio de celda
                int aux=(dato & 0xFF000000) >> 24;
                MKV->mem[dirfis++]=aux ;
                dato<<=8;
             }       
            else{
                verificaerrores(MKV,3); //error de segmento
            }
        }
        break;

    case 2:
        if (dirfis+i<=MEMORIA && dirfis>=MKV->tabla_seg[2]){
            for(j=0;j<cantDatos;j++){ //CANTIDAD DE CELDAS QUE FORMAN CADA DATO
                x = 0;
                printf("[%04X]: ",dirfis);
                for(k=0;k<i;k++){ //DIMENSION
                    x = x << 8;
                    x =  x +(int) MKV->mem[dirfis] ;
                    if((MKV->reg[EAX] && 0x02) == 2){ //ES UN CHAR
                        y = MKV->mem[dirfis];
                        if(y > 31 || y < 127)
                            printf(" %c",y);
                        else
                            printf(". ");
                    }
                    dirfis++;
                }
                if((MKV->reg[EAX] & 0x01) == 1) //ES DECIMAL
                    printf("%d ",x);
                if((MKV->reg[EAX] & 0x04) == 4) //ES OCTAL
                    printf("%o ",x);
                if((MKV->reg[EAX] & 0x08) == 8) //ES HEXADECIMAL
                    printf("%x ",x);
                if((MKV->reg[EAX] & 0x10) == 10) //ES BINARIO
                    print_bin(x);
                printf("\n");
            }
        }
        else{
            verificaerrores(MKV,3); //error de segmento
        } 
        break;   
    } 
}

void JMP(TipoMKV *MKV, int opA, int TopA){
   int valor=get_Valor(MKV,opA,TopA);
   if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1])
        MKV->reg[IP]=valor;
    else
        verificaerrores(MKV,3); //fallo de segmento
}

void JZ(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_Z) == MASC_Z)
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JP(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == MASC_N && (MKV->reg[CC]&MASC_Z) == MASC_Z)//if(MKV->reg[CC[0]] == 0 && MKV->reg[CC[1]] == 0)
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JN(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == MASC_N && (MKV->reg[CC]&MASC_Z) == MASC_Z) // los cambie ya que son int y no vectores
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JNZ(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_Z) == MASC_Z)
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JNP(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == MASC_N || (MKV->reg[CC]&MASC_Z) == MASC_Z)
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JNN(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == MASC_N)
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void NOT(TipoMKV *MKV, int opA, int TopA){
    int valorA;
    valorA=get_Valor(MKV,opA,TopA);
    valorA=~valorA;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}








