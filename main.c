#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA.h" //Vacio, los prototipos
#include "mascaras.h"
#include "Operaciones_Generales.c" 
#include "Operaciones.c"
#include "Codigos_Registros.h"


int main(){

    return 0;
}



int verifica_cabecera(unsigned char cabecera[5]){ //Verifica la version de la cabecera
    int i=0;
    unsigned char comp[5]={'V','M','X','2','5','1'};
    while (i<6 && comp[i]==cabecera[i])
        i++;
    if (i==6)
        return 1; //Version correcta
    else
        return 0; //Version erronea 
}

void inicializacion(char nombre_arch[],TipoMKV *MKV){
    FILE *arch;
    int i;
    i = 0; 
    int desp=0;
    unsigned char cabecera[6];
    MKV->codigo_error=0;
    arch= fopen(nombre_arch, "rb");
        if (arch == NULL)
            printf("Error al abrir el archivo");
        else{
            for (int j=0;i<5;i++)
                fread(&cabecera[j], sizeof(unsigned char), 1, arch);
            fread(&cabecera[5], sizeof(unsigned char), 1, arch);
            if (verifica_cabecera(cabecera)){ 
                fread(&MKV->tabla_seg[0],sizeof(unsigned char),1,arch);         // base CS
                fread(MKV.tabla_seg[1], sizeof(unsigned char), 1, arch);        // tamano max CS
                MKV->tabla_seg[2]=MKV->tabla_seg[1];                            //base DS            
                MKV->tabla_seg[3]=16384-MKV->tabla_seg[2];                      //tamano max DS
                 while (fread(MKV.mem[MKV->reg[CS]+desp], sizeof(unsigned char), 1, arch) == 1)  //Guarda las instrucciones en el code segment
                     desp++;    
                 fclose(arch);
            } 
            else
                printf("Error al leer cabecera");
        }
}



void ejecucion(TipoMKV *MKV){
    inicializacion(nombre_arch,MKV);
    char instruccion;
    char TopA,TopB;
    int dirfisica;
    int shift;  
    void (*Fops2[16])(int, int, int, int )={MOV , ADD , SUB , MUL , DIV , CMP , SHL , SHR , SAR , AND , OR , XOR , SWAP , LDL , LDH , RND};
    void (*Fops1[9])(int,int )={SYS , JMP , JZ , JP , JN , JNZ , JNP , JNN , NOT};
    MKV->reg[CS] = MKV->tabla_seg[0] << 16;                          //inicializo el CS en los 8 MSB
    MKV->reg[DS] = MKV->tabla_seg[1]+0x00010000;                    //Inicializo del DS una posicion mas del CS
    MKV->reg[IP] = MKV->reg[CS];
    while (MKV->codigo_error==0 && MKV->reg[IP]!=-1 ){   // mientas no exista un error o se termine la memoria

        dirfis=logifisi(MKV,MKV->reg[IP]);
        if (dirfis==-1)
        MKV->codigo_error=3;            //error: fallo de segmento
        else{         
            instruccion=MKV->mem[dirfis];   // guardo la instruccion de donde apunta IP 
            if (codinvalido(instruccion))     // error: Codigo invalido
                MKV->codigo_error=1;
            else{     
                MKV->reg[OPC]= instruccion & MASC_CODOP;        // guardo el cod de operacion en OPC 
                if (MKV->reg[OPC]==0x0F)                       // STOP
                    MKV->reg[IP]=-1;    
                else{              
                    if (MKV->reg[OPC]>=0x10 && MKV->reg[OPC]<=0x1F){        //dos operandos (testeado)
                        getOperandos(MKV,instruccion,dirfis,'a');
                        //obs: hay que comprobar el cambio de dirfisica
                        getOperandos(MKV,instruccion,dirfis,'b');
                        cambioip(MKV,TopA,TopB);
                        Fops2[instruccion & 0x0F](escopeta(MKV->reg[OP1]),(instruccion & MASC_TOPA) >> 4,escopeta(MKV->reg[OP2]),(instruccion & MASC_TOPB) >> 6);   
                    }
                    else
                        if (MKV->reg[OPC]>=0x00 && MKV->reg[OPC]<=0x08){    //un operando
                            getOperandos(MKV,instruccion,dirfis,'b');
                            MKV->reg[OPA]=MKV->reg[OPB];    //por especificacion de la catedra
                            Fops1[instruccion](escopeta(MKV->reg[OPA]),instruccion & MASC_TOPB) >> 6;                        
                        }          
                }
            }
        }



    }
    verificaerrores(MKV->codigo_error);

}


}


