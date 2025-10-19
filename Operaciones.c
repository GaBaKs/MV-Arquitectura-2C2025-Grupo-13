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




void MOV (TipoMKV *MKV,int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    setValor(MKV,opA,TopA,valorB);

}

void ADD (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    valorA+= valorB;
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}


void SUB (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    valorA-= valorB;
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}

void MUL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}

void DIV (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    if(valorB != 0){
        MKV->reg[AC] = valorA % valorB; //guardo el resto en AC
        valorA = (int) valorA / valorB;
        setValor(MKV,opA,TopA,valorA);
        NZ_CC(valorA,MKV);    
    }
    else
        verificaerrores(MKV,2);     //error: division por cero
}

void CMP (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    valorA-= valorB;
    NZ_CC(valorA,MKV);
}

void SHL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    valorA = valorA << valorB;
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}

void SHR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    for (int i=0;i<valorB;i++){
        valorA>>=1;
        valorA&=0x7FFFFFFF; //pone en 0 el bit mas significativo
    }
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}

void SAR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    valorA>>= valorB;
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}

void AND (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    valorA &= valorB;
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}

void OR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    valorA |= valorB;
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}

void XOR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    valorA ^= valorB;
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}

void SWAP(TipoMKV *MKV,int opA,int TopA,int opB,int TopB){
    int valorB, valorA;
    int aux;
    valorB = getValor(MKV,opB,TopB);
    valorA = getValor(MKV,opA,TopA);
    setValor(MKV,opA,TopB,valorB); //los TopA y TopB deberian ser iguales
    setValor(MKV,opB,TopA,valorA);
}

void LDL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB); 
    valorA = getValor(MKV,opA,TopA);
    valorA&=0xFFFF0000;
    valorA|=valorB&0x0000FFFF;
    setValor(MKV,opA,TopA,valorA);
}

void LDH (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB); 
    valorA = getValor(MKV,opA,TopA);
    valorA&=0x0000FFFF;
    valorA|=(valorB&0x0000FFFF)<<16;
    setValor(MKV,opA,TopA,valorA);
}

void RND (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    srand(time(NULL));
    int valorB, valorA;
    valorB = getValor(MKV,opB,TopB);
    valorA = rand() % (valorB + 1); //rand() genera un numero aleatorio y al hacerle % (valB + 1) se establece el rango entre 0 y valB.
    setValor(MKV,opA,TopA,valorA);
}

void SYS(TipoMKV *MKV, int opA, int TopA){
    int i,k,dirfis,j,tam,dato,x;
    char y;
    int flag=0;
    unsigned int cantDatos;
    char c[100];
    char bin[33];
    dirfis=logifisi(*MKV,MKV->reg[EDX]);
    i=(MKV->reg[ECX] & MASC_LDH) >> 16; //tamanio por celda
    cantDatos=MKV->reg[ECX] & MASC_LDL;  //cant celdas (cant datos)
    switch(opA){
    
     case 1:
        SYS1(MKV,cantDatos,dirfis);
        break;

    case 2:
        SYS2(MKV,cantDatos,dirfis);
        break;
    case 3: //STRING READ
        SYS3(MKV,cantDatos,dirfis):
        break;
    case 4: //STRING WRITE
        SYS4(MKV,dirfis):
        break;
    case 7: //CLEAR SCREEN
        SYS7():
        break;
    case 0xF; //BREAKPOINT
        SYSF():
        break;

    } 
}
void SYS1 (TipoMKV *MKV,int cantDatos, int dirfis){
    int flag=0,j,dato;
    char c[100];
    char bin[33];
    //verifico que formato tengo que leer
    for (j=0;j<cantDatos;j++){         //cantidad de numeros a leer
        printf("[%04X]: ",dirfis);
        switch(MKV->reg[EAX]){

        case 0x01:scanf("%d",&dato);
                break;
        case 0x02: scanf("%s",c);
                    flag=1;
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
        if (dirfis+i*cantDatos<=MEMORIA && dirfis>=MKV->tabla_seg[2]){ //cambiar
            if(!flag)
            for (int k=i-1;k>=0;k--){      //tamanio de celda
                int aux=(dato & 0xFF000000) >> 24;
                MKV->mem[dirfis++]=aux ;
                dato<<=8;
            }
            else{
                for (int k=i-1;k>=0;k--){     //tamanio de celda
                MKV->mem[dirfis++]=c[i-k-1];
                printf("%c", MKV->mem[dirfis-1]);
            } 
            }
                
        }        
        else{
            verificaerrores(MKV,3); //error de segmento
        }
    }
}

void SYS2 (TipoMKV *MKV,int cantDatos, int dirfis){
    int i,j,x,y;
    if (dirfis+i<=MEMORIA && dirfis>=MKV->tabla_seg[2]){ //cambiar
        for(j=0;j<cantDatos;j++){ //CANTIDAD DATOS A LEER
            x = 0;
            printf("[%04X]: ",dirfis);
            for(k=0;k<i;k++){ //DIMENSION, cantidad de celdas a leer para cada dato
                x = x << 8;
                x =  x +(int) MKV->mem[dirfis] ;
                if((MKV->reg[EAX] & 0x02) == 0X02){ //ES UN CHAR
                    y = MKV->mem[dirfis];
                    if(y > 31 && y < 127)
                        printf("%c",y);
                    else
                        printf(". ");
                }
                dirfis++;
            }
            printf(" ");
            if((MKV->reg[EAX] & 0x08) == 0x08) //ES HEXADECIMAL
                printf("0x%X ",x);
            if((MKV->reg[EAX] & 0x04) == 0x04) //ES OCTAL
                printf("0o%o ",x);
            if((MKV->reg[EAX] & 0x01) == 0X01) //ES DECIMAL
                printf("%d ",x);
            if((MKV->reg[EAX] & 0x10) == 0x10) //ES BINARIO
                print_bin(x);
            printf("\n");
        }
    }
    else{
        verificaerrores(MKV,3); //error de segmento
    }
}

void SYS3 (TipoMKV *MKV,int cantDatos, int dirfis){
    char cadena[200] = {0};
    int len;
    fgets(cadena, sizeof(cadena), stdin); // va guardando en la variable cadena hasta leer \n, pero lo guarda 
    int len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n') { 
        cadena[len - 1] = '\0';
        len--;
    }
    
}

void SYS4 (TipoMKV *MKV,int dirfis){
    int i=0;
    while(MKV->mem[dirfis+i]!=0){ // el programador deber ser prolijo, si no printea toda la memoria
        printf("%c",MKV->mem[dirfis+i]);
        i++;
    }
}

void SYS7 (){
    system("cls");
}

void SYSF (){

}

void JMP(TipoMKV *MKV, int opA, int TopA){
   int valor=getValor(MKV,opA,TopA);
   if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        MKV->reg[IP]=valor;
    }
    else
        verificaerrores(MKV,3); //fallo de segmento
}

void JZ(TipoMKV *MKV, int opA, int TopA){
    int valor=getValor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_Z) == MASC_Z){
            MKV->reg[IP]=valor;
        }
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JP(TipoMKV *MKV, int opA, int TopA){
    int valor=getValor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == 0 && (MKV->reg[CC]&MASC_Z) == 0){
            MKV->reg[IP]=valor;
        }
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JN(TipoMKV *MKV, int opA, int TopA){
    int valor=getValor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == MASC_N && (MKV->reg[CC]&MASC_Z) == 0)
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JNZ(TipoMKV *MKV, int opA, int TopA){
    int valor=getValor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_Z) == 0)
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JNP(TipoMKV *MKV, int opA, int TopA){
    int valor=getValor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == MASC_N || (MKV->reg[CC]&MASC_Z) == MASC_Z)
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void JNN(TipoMKV *MKV, int opA, int TopA){
    int valor=getValor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if((MKV->reg[CC]&MASC_N) == 0)
            MKV->reg[IP]=valor;
    }
    else
         verificaerrores(MKV,3); //fallo de segmento
}

void NOT(TipoMKV *MKV, int opA, int TopA){
    int valorA;
    valorA=getValor(MKV,opA,TopA);
    valorA=~valorA;
    setValor(MKV,opA,TopA,valorA);
    NZ_CC(valorA,MKV);
}

void PUSH(TipoMKV *MKV, int opA, int TopA){ //controlar el STACK OVERFLOW
    int valorA;
    MKV->reg[SP]-=4; //Muevo SP para ingresar el nuevo valor en el stack
    if(MKV->reg[SP]>/*LO Q HAYA EN LA TABLA*/){
        valorA=getValor(MKV,opA,TopA);
        MKV->reg[SP] = valorA;
    }
    else
        verificaerrores(MKV,5) //STACK OVERFLOW
}

void POP(TipoMKV *MKV){ // solo modifica el registro SP, controlar el STACK UNDERFLOWint valorA;
    MKV->reg[SP]+=4; //Muevo SP 4 posiciones(1 celda)para ingresar el nuevo valor en el stack
    if(MKV->reg[SP]>/*LO Q HAYA EN LA TABLA*/)
        verificaerrores(MKV,6) //STACK UNDERFLOW
}

void CALL(){ //almacenará en la pila los 4 bytes del valor del IP

}

void RET(){ //modificará el IP obteniendolo del tope de la pila, equivale a POP IP.

}





