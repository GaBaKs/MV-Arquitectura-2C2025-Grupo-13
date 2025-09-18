#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mascaras.h"
#include "TDA.h"
#include "Codigos_Registros.h"
/*generavectornmemo(Tmnemo Vmnemo)
{
    Vmnemo[LAR].mnemonico="LAR"; //0
    Vmnemo[MAR].mnemonico="MAR"; //1
    Vmnemo[MBR].mnemonico="MBR"; //2
    Vmnemo[IP].mnemonico="IP";   //3
    Vmnemo[OPC].mnemonico="OPC"; //4
    Vmnemo[OP1].mnemonico="OP1"; //5
    Vmnemo[OP2].mnemonico="OP2"; //6
    Vmnemo[EAX].mnemonico="EAX"; //10
    Vmnemo[EBX].mnemonico="EBX"; //11
    Vmnemo[ECX].mnemonico="ECX"; //12
    Vmnemo[EDX].mnemonico="EDX"; //13
    Vmnemo[EEX].mnemonico="EEX"; //14
    Vmnemo[EFX].mnemonico="EFX"; //15
    Vmnemo[AC].mnemonico="AC";   //16
    Vmnemo[CC].mnemonico="CC";   //17
    Vmnemo[CS].mnemonico="CS";   //26
    Vmnemo[DS].mnemonico="DS";   //27
}
*/
void verificaerrores(int codigo_error){
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
    }
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
   
    int dirlog=0x00010000 + MKV->reg[(op & 0x001F00) >> 16]+ op & MASC_OFFMOV;
    int auxL=logifisi(*MKV,dirlog);
    MKV->reg[LAR]=dirlog;
    if (auxL!=-1)
        MKV->reg[MAR]=0x00040000+auxL; //HARDCODEADO
    else
        verificaerrores(3);             // Error de segmento
   
}

void getMemoria(TipoMKV *MKV){      // guarda en MBR el dato de la direccion guardada en MAR
     int max,valor,aux;
     int dirfis=MKV->reg[MAR] & MASC_MARL;
     max=MKV->reg[MAR] & MASC_MARH;
     for (int i=0;i<max;i++){
                   aux=MKV->mem[dirfis+1+i];
                   valor+=aux<<(3-i)*8;
                }
    MKV->reg[MBR]=valor;
}

void setMemoria(TipoMKV *MKV){      // guarda el dato de MBR en memoria en la direc de MAR
    int dirfis,valor=MKV->reg[MBR];
    dirfis=MKV->reg[MAR] & MASC_MARL;
if (dirfis+CANTCELDAS<=MEMORIA && dirfis>=MKV->tabla_seg[3])
                for (int i=CANTCELDAS;i<0;i--){ 
                    MKV->mem[dirfis++]=(char)(valor >> ((i-1)*8)) & 0x000000FF ; 
                }
else
    verificaerrores(3); //fallo de segmento
}



void cambioip(TipoMKV *MKV,int TopA,int TopB){
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

int codinvalido(char instruccion){
    if (instruccion<0 || instruccion>0x1F)
        return 1;
    else
        return 0;
}

                   

void getOperandos(TipoMKV *MKV,char instruccion,int dirfis){              
    int TopA,TopB;
        TopA=instruccion & MASC_TOPA >> 4; 
        TopB=instruccion & MASC_TOPB >> 6; 
        if (dirfis+TopA+TopB+1<=MKV->tabla_seg[2]){
            MKV->reg[OPB]=TopB;
            if (TopB==1)
                MKV->reg[OPB]<<=16; // 00 00 00 01 ---> 00 01 00 00
            else
                if (TopB==2)
                    MKV->reg[OPB]<<=8; // 00 00 00 02 ---> 00 00 02 00  
            
            for (int i=0;i<TopB;i++){
                MKV->reg[OPB]=MKV->reg[OPB]<<8; 
                MKV->reg[OPB]+=MKV->mem[dirfis+1+i];    
            }
            MKV->reg[OPA]=TopA;
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
        }
        else
           verificaerrores(3);  
}        
       

  


