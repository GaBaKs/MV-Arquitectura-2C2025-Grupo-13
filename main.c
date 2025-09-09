#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA.h" //Vacio, los prototipos


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
                MKV->reg[CS] = MKV->tabla_seg[0] << 8;                          //inicializo el CS en los 8 MSB
                MKV->reg[DS] = MKV->tabla_seg[1]+0x00010000;                    //Inicializo del DS una posicion mas del CS
                MKV->reg[IP] = MKV->reg[CS];


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
    void (*op2[16])(int, int, int, int )={MOV , ADD , SUB , MUL , DIV , CMP , SHL , SHR , SAR , AND , OR , XOR , SWAP , LDL , LDH , RND};
    void (*op1[9])(int,int )={SYS , JMP , JZ , JP , JN , JNZ , JNP , JNN , NOT};
    while (MKV->codigo_error==0 && MKV->reg[IP]!=-1 ){   // mientas no exista un error o se lea un STOP o se termine la memoria
            instruccion=MKV->mem[logifisi(MKV->reg[IP])];   // guardo la instruccion de donde apunta IP 
          //if codinvalido
          //    MKV->codigo_error=1;
          //else

            MKV->reg[OPC]= instruccion & MASC_CODOP;        // guardo el cod de operacion en OPC
            if (MKV->reg[OPC]==0x0F){                       // STOP
                //termina el programa
            }
            else
                if (MKV->reg[OPC]>=0x10 && MKV->reg[OPC]<=0x1F){        //dos operandos
                    //saco opA y opB

                    op2[instruccion & 0x0F](opA,Topa,opB,Topb);


                }
                else
                    if (MKV->reg[OPC]>=0x00 && MKV->reg[OPC]<=0x08){    //un operando
                        
                    }
    }


}

int dirfisica()
{

}