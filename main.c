
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA.h" //Vacio, los prototipos
#include "mascaras.h"
#include "Operaciones_Generales.h"
#include "Operaciones.h"
#include "Codigos_Registros.h"
#include "Dissasembler.h"

int verifica_cabecera(unsigned char cabecera[5]);
void inicializacion(char nombre_arch[],TipoMKV *MKV);
void ejecucion(TipoMKV *MKV);

int main(){
    printf("Comienzo de la ejecucion:) cuak \n");
    TipoMKV MKV;
    ejecucion(&MKV);
    return 1;
}



int verifica_cabecera(unsigned char cabecera[]){ //Verifica la version de la cabecera
    int i=0;
    unsigned char comp[7]={'V','M','X','2','5',0b00000001};
    while (i<6 && comp[i]==cabecera[i]){
        i++;
    }
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
    unsigned char cabecera[7];
    arch= fopen(nombre_arch, "rb");
        if (arch == NULL)
            printf("Error al abrir el archivo, verifique que el nombre ingresado sea el correcto e intente nuevamente. Si es un profesor y esto no funciona, tengan piedad con nuestras almas. cuack");
        else{
            for (int j=0;j<6;j++){
                fread(&cabecera[j], sizeof(unsigned char), 1, arch);
            }    
            if (verifica_cabecera(cabecera)){
                printf("Cabecera correcta \n");
                fread(&MKV->tabla_seg[0],sizeof(unsigned char),1,arch);         // base CS 
                fread(&MKV->tabla_seg[1], sizeof(unsigned char), 1, arch);        // tamano max CS
                MKV->tabla_seg[2]=MKV->tabla_seg[1]+1;                            //base DS
                MKV->tabla_seg[3]=16384-MKV->tabla_seg[2];                      //tamano max DS
                MKV->tabla_seg[0]=0;
                
                while (fread(&MKV->mem[MKV->tabla_seg[0]+desp], sizeof(unsigned char), 1, arch) == 1){  //Guarda las instrucciones en el code segment
                    desp++;                    
                }
                printf("Lectura de archivo realizada correctamente\n");
                fclose(arch);
            }
            else
                printf("Error al leer cabecera");
        }
}



void ejecucion(TipoMKV *MKV){

    char str[200];
    //printf("Ingrese el nombre del archivo que quiere ejecutar\n");
    //scanf("%s",str);
    strcpy(str,"ejerciciomv1.vmx");
    inicializacion(str,MKV);
    dissa(*MKV);
    MKV->flag=0;
    unsigned char instruccion;
    int TopA,TopB;
    int dirfis;
    int shift;
    void (*Fops2[16])(TipoMKV *, int, int, int, int )={MOV , ADD , SUB , MUL , DIV , CMP , SHL , SHR , SAR , AND , OR , XOR , SWAP , LDL , LDH , RND};
    void (*Fops1[9])(TipoMKV *, int, int )={SYS , JMP , JZ , JP , JN , JNZ , JNP , JNN , NOT};
    MKV->reg[CS] = MKV->tabla_seg[0] << 16;                          //inicializo el CS en los 8 MSB
    MKV->reg[DS] = 0x00010000;                    //Inicializo del DS una posicion mas del CS 
    MKV->reg[IP] = MKV->reg[CS];
    while ( MKV->reg[IP]!=-1 && !MKV->flag){   // mientas no exista un error o se termine la memoria                 MKV->codigo_error==0 &&
        dirfis=logifisi(*MKV ,MKV->reg[IP]);
        printf("direccion fisica actual de la instruccion: %d \n",dirfis);
        if (dirfis==-1)
            verificaerrores(MKV,3);   //error: fallo de segmento
        else{
            instruccion=MKV->mem[dirfis];   // guardo la instruccion de donde apunta IP
            printf("instruccion actual: %x \n",instruccion);
            if (!codinvalido(instruccion & MASC_CODOP))     // error: Codigo invalido
                verificaerrores(MKV,1);
            else{
                MKV->reg[OPC]= instruccion & MASC_CODOP;        // guardo el cod de operacion en OPC
                printf("Codigo Operacion Actual %x y es codigo: ",MKV->reg[OPC]);
                imprimeMnemonico(MKV->reg[OPC]);
                printf("\n");
                getOperandos(MKV,instruccion,dirfis);
                if (MKV->reg[OPC]==0x0F)                       // STOP
                    MKV->reg[IP]=-1;
                else{
                    if (MKV->reg[OPC]>=0x10 && MKV->reg[OPC]<=0x1F){ //dos operandos (testeado)
                        TopA=(instruccion & MASC_TOPA) >> 4;
                        TopB=(instruccion & MASC_TOPB) >> 6;
                        cambioip(MKV,TopA,TopB);
                        Fops2[instruccion & 0x0F](MKV,escopeta(MKV->reg[OPA]),TopA,escopeta(MKV->reg[OPB]),TopB);
                    }
                    else
                        if (MKV->reg[OPC]>=0x00 && MKV->reg[OPC]<=0x08){    //un operando
                            TopA=(instruccion & MASC_TOPB) >> 6;
                            //SWAP(MKV,escopeta(MKV->reg[OPA]),TopA,escopeta(MKV->reg[OPB]),TopB);
                            cambioip(MKV,TopA,0);
                            Fops1[instruccion & 0x0F](MKV,escopeta(MKV->reg[OPA]),TopA);
                            
                        }
                }
            }
        }
    }
    if (MKV->reg[IP]==-1)
        printf("TERMINO STOP");
}



