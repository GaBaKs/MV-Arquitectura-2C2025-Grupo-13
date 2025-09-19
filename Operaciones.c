#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "TDA.h"
#include "mascaras.h"
#include "Operaciones_Generales.c"
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
            return op;
        else
            if (Top==3){
                larmar(MKV,op);
                getMemoria(MKV);
                return MKV->reg[MBR];   
            }   
} 

void MOV (TipoMKV *MKV,int opA, int TopA, int opB, int TopB){
    int dirfis,i;
    int offset,cod; // para tipo memoria

    int valorB=get_Valor(MKV,opB,TopB);
        if (TopA==3){
            larmar(MKV,opA);
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
        verificaerrores(2);     //error: division por cero 
}

void CMP (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){ 
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA-= valorB;
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
    valorA=0x7FFFFFFF;
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
    aux=valorA;
    valorA=valorB;
    valorB=aux;
    MOV(MKV,opA,TopA,valorA,2);
    MOV(MKV,opB,TopB,valorB,2);
}

void LDL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB) & MASC_LDL ;
    valorA = get_Valor(MKV,opA,TopA); 
    valorA &=MASC_LDH; 
    valorA|=valorB;
    MOV(MKV,opA,TopA,valorA,2);
}

void LDH (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB) & MASC_LDH ;
    valorA = get_Valor(MKV,opA,TopA);
    valorA &=MASC_LDL; 
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
    int cantCeldas,i,k,dirfis,j=0,tam,dato,x,y;
    char c;
    char bin[33];
    dirfis=logifisi(*MKV,MKV->reg[EDX]);
    cantCeldas=MKV->reg[ECX] & MASC_LDH; //cant celdas (cant datos)
    i=MKV->reg[ECX] & MASC_LDL;  //tamanio por celda
    switch(opA){

     case 1:
        //verifico que formato tengo que leer
        
        
        
        for (j=0;j<cantCeldas;j++){         //cantidad de numeros a leer
            printf("[%x]: ",dirfis);
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
                    verificaerrores(4); // error de formato
            }
           if (dirfis+i<=MEMORIA && dirfis>=MKV->tabla_seg[3])
             for (int k=i;k>0;k--){      //tamanio de celda
                    MKV->mem[dirfis++]=(char)(dato >> ((k-1)*8)) & 0x000000FF ; 
            else
                verificaerrores(3); //error de segmento      
            }

        }
    

    case 2:
        if (dirfis+i<=MEMORIA && dirfis>=MKV->tabla_seg[3]){
            for(j;j<cantCeldas;j++){ //DIMENSION
                x = 0;
                for(k=0;k<i;k++){ //CANTIDAD DE CELDAS QUE LEE EN MEMORIA
                    x = MKV->mem[dirfis] | x;
                    x = x << 8;
                    if((MKV->reg[EAX] && 0x02) == 2){ //ES UN CHAR
                        y = MKV->mem[dirfis];
                        if(y > 31 || y < 127)
                            printf("[%x] %c ",dirfis,y);
                        else
                            printf("[%x] . ",dirfis);
                    }
                    
                    if((MKV->reg[EAX] && 0x01) == 1) //ES DECIMAL
                        printf("[%x] %d ",dirfis,x);
                    if((MKV->reg[EAX] && 0x04) == 4) //ES OCTAL
                        printf("[%x] %o ",dirfis,x);
                    if((MKV->reg[EAX] && 0x08) == 8) //ES HEXADECIMAL
                        printf("[%x] %x ",dirfis,x);
                    if((MKV->reg[EAX] && 0x10) == 10){ //ES BINARIO
                        printf("[%x] ",dirfis);
                        print_bin(x);
                        dirfis++;   
                    }
                }
                printf("\n");
            }
        }
        else
            verificaerrores(3); //error de segmento  
        break;
    }
}

void JMP(TipoMKV *MKV, int opA, int TopA){
   int valor=get_Valor(MKV,opA,TopA);
   if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1])
        MKV->reg[IP]=valor;
    else
        verificaerrores(3); //fallo de segmento
}

void JZ(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_Z) == 1)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void JP(TipoMKV *MKV, int opA, int TopA){ 
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == 0 && (MKV->reg[CC]&MASC_Z) == 0)//if(MKV->reg[CC[0]] == 0 && MKV->reg[CC[1]] == 0)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento 
}

void JN(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == 1 && (MKV->reg[CC]&MASC_Z) == 0) // los cambie ya que son int y no vectores
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void JNZ(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_Z) == 0)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void JNP(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == 1 || (MKV->reg[CC]&MASC_Z) == 1)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void JNN(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == 0)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void NOT(TipoMKV *MKV, int opA, int TopA){
    int valorA;
    valorA=get_Valor(MKV,opA,TopA);
    valorA=~valorA;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}








