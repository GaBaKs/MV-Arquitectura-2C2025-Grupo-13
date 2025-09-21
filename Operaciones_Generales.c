#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mascaras.h"
#include "TDA.h"
#include "Codigos_Registros.h"
#include "math.h"
#include "Operaciones_Generales.h"

void verificaerrores(TipoMKV *MKV, int codigo_error){
    switch (codigo_error){
        case 1:
            printf("ERROR: Instruccion invalida");
            break;
        case 2:
            printf("ERROR: Division por cero");
            break;
        case 3:
            printf("ERROR: Fallo de segmento");  
            break;  
        case 4:
            printf("ERROR: FORMATO NO ESPECIFICADO");
    }
    MKV->flag = 1;
}

int logifisi(TipoMKV MKV,int dirlog){ 
    int dirfis,segmento,offset;
    segmento=dirlog>>16; 
    offset=dirlog & 0x0000FFFF;
        if (segmento==0){ //Segmento de codigo 
            dirfis=MKV.tabla_seg[0]+offset;//Direccion fisica = base CS + direccion logica
            if (dirfis>=MKV.tabla_seg[0] && dirfis<=MKV.tabla_seg[2]) //Verifica que la direccion fisica este dentro del segmento de codigo    
                return dirfis;
            else
                return -1;
        }
        else{
            dirfis=MKV.tabla_seg[2]+offset; //Direccion fisica = base DS + direccion logica
            if (dirfis>=0 && dirfis<=MKV.tabla_seg[3]) //Verifica que la direccion logica este dentro del segmento de datos       
                return dirfis;
            else
                return -1;
        }
}

void larmar(TipoMKV *MKV,int op){        // cada vez q se accede a memoria
    int aux=0;
    int dirlog= (MKV->reg[(op & 0x001F0000) >> 16] )+( op & MASC_OFFSET);
    printf("la dirlog de larmar: %x \n",dirlog); //00010000
    int auxL=logifisi(*MKV,dirlog);
    printf("Direccion fisica de larmar: %d",auxL);
    if (auxL==-1){      
        verificaerrores(MKV,1);
    }
    MKV->reg[LAR]=dirlog;
    if (auxL!=-1)
        MKV->reg[MAR]= 0x00040000+auxL; //HARDCODEADO
    else
        verificaerrores(MKV,3);             // Error de segmento
   
}

void getMemoria(TipoMKV *MKV){      // guarda en MBR el dato de la direccion guardada en MAR
     int max,valor,aux;
     int dirfis=MKV->reg[MAR] & MASC_MARL;
     max=(MKV->reg[MAR] & MASC_MARH) >>16;
     for (int i=0;i<max;i++){
        valor<<=8;
        aux=MKV->mem[dirfis+i];
        valor+=aux;
    }
    printf("valor recontruido de get memoria: %x \n",valor);
    MKV->reg[MBR]=valor;
}

void setMemoria(TipoMKV *MKV){      // guarda el dato de MBR en memoria en la direc de MAR
    int dirfis,valor=MKV->reg[MBR];
    dirfis=MKV->reg[MAR] & MASC_MARL;
if (dirfis+CANTCELDAS<=MEMORIA && dirfis>=MKV->tabla_seg[3])
                for (int i=CANTCELDAS;i>0;i--){ 
                    MKV->mem[dirfis++]=(char)(valor >> ((i-1)*8)) & 0x000000FF ; 
                }
else
    verificaerrores(MKV,3); //fallo de segmento
}

void cambioip(TipoMKV *MKV,int TopA,int TopB){
    int suma=1;
        if (TopA==0b01)
            suma++;
        else
            if (TopA==0b10)
                suma+=2;
            else
                if (TopA==0b11)
                    suma+=3;     
        if (TopB==0b01)
            suma++;
        else
            if (TopB==0b10)
                suma+=2;
            else
                if (TopB==0b11)
                    suma+=3;
            
    MKV->reg[IP]+=suma;
}

int escopeta(int corredera){
    corredera<<=8;  //chk
    corredera>>=8;  //chk
    return corredera;
}

int escopeta2bytes(int corredera){
    corredera<<=16;  //chk
    corredera>>=16;  //chk
    return corredera;
}

int codinvalido(char cod){

    if (cod>=0 || cod<=0x1F)
        return 1;
    else
        return 0;
}
                   
void getOperandos(TipoMKV *MKV, unsigned char instruccion,int dirfis){              
    int TopA,TopB;
        printf("dentro de get operandos instruccion: %x \n",instruccion);
        TopA=(instruccion & MASC_TOPA) >> 4;
        TopB=(instruccion & MASC_TOPB) >> 6; 
        if (dirfis+TopA+TopB<=MKV->tabla_seg[2]+MKV->tabla_seg[0]){
            MKV->reg[OPB]=TopB;
            printf("Valor de OPB con solo ToB: %x \n",MKV->reg[OPB]);
            if (TopB==1)
                MKV->reg[OPB]<<=16; // 00 00 00 01 ---> 00 01 00 00
            else
                if (TopB==2)
                    MKV->reg[OPB]<<=8; // 00 00 00 02 ---> 00 00 02 00  
            
            for (int i=0;i<TopB;i++){
                MKV->reg[OPB]=MKV->reg[OPB]<<8; 
                MKV->reg[OPB]+=MKV->mem[dirfis+1+i];    
            }
            printf("Valor de OPB final: %x \n",MKV->reg[OPB]);
            MKV->reg[OPA]=TopA;
            printf("Valor de OPA con solo ToA: %x \n",MKV->reg[OPA]);
            switch (TopA){

                case 0: MKV->reg[OPA]=MKV->reg[OPB];
                        break;

                case 1:MKV->reg[OPA]<<=16;
                        break;

                case 2: MKV->reg[OPA]<<=8;
                        break;

                default: break;
            }
            for (int i=0;i<TopA;i++){
                        MKV->reg[OPA]=MKV->reg[OPA]<<8; 
                        MKV->reg[OPA]+=MKV->mem[TopB+dirfis+1+i];   
            } 
            printf("Valor de OPA final: %x \n",MKV->reg[OPA]);
        }
        else
           verificaerrores(MKV,3);  
}        
int bintoint(char str[33]){     
    int j=strlen(str);
    int acum=0;
    printf("el valor de j es %d\n",j);
if (str[0]=='1'){
    for (int i=1;i<j;i++){
        if (str[i]=='0'){
            acum+=pow(2,j-i-1);
        }
    }

        return (acum+1)*-1;
}
    else{
        for (int i=0;i<j;i++){
            if (str[i]=='1'){
                acum+=pow(2,j-i-1);
            }
        }
        return acum;
    }
}




void print_bin(int n){
    for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
        printf("%d ", (n >> i) & 1);
    }
}