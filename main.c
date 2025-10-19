
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
void inicializacion(char nombre_arch[], TipoMKV *MKV,int argc,char *argv[])
void ejecucion(TipoMKV *MKV);

int main(int argc, char *argv[]){
    printf("Bienvenido a la Maquina virtual del grupo 13! Autores: Mario Arriaga, Tomas Candotto y Gabriel Seneca :)\n");
    TipoMKV MKV;
    MKV.tamanoRAM=MEMORIA;
    char str[200]=argv[0];
    int j=1;  

    inicializacion(str,&MKV,argc,argv);
    
    
    //compruebo dissa
    while (j<argc && (strcmp(argv[j],"-d")==0))
        j++;
    if (j<argc){
         dissa(MKV);
         printf("\n");
    }
    ejecucion(&MKV);
    return 1;
}

void inicializacion(char nombre_arch[], TipoMKV *MKV,int argc,char *argv[]){ //DIOS
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
            int cabecera=verifica_cabecera(cabecera);       //verifico la version
            if (cabecera==1){           //VMX25 parte 1
                fread(&MKV->tabla_seg[0],sizeof(unsigned char),1,arch);         // base CS 
                fread(&MKV->tabla_seg[1], sizeof(unsigned char), 1, arch);        // tamano max CS
                MKV->tabla_seg[2]=MKV->tabla_seg[1];                            //base DS
                MKV->tabla_seg[3]=16384-MKV->tabla_seg[2];                      //tamano max DS
                MKV->tabla_seg[0]=0;
                
                while (fread(&MKV->mem[MKV->tabla_seg[0]+desp], sizeof(char), 1, arch) == 1){  //Guarda las instrucciones en el code segment
                    desp++;                    
                }
                printf("Lectura de archivo realizada correctamente para la version 1\n");
                fclose(arch);
            }
            else
                if (cabecera==2){           //VMX25 parte 2
                    inicia_Memoria(argc,argv,MKV);
                    cargaPS(MKV,argc,argv);
                    //continuolectura
                }
                else
                    if (cabecera==3){       //VMI25 1

                    }
                    else
                        printf("Error al leer cabecera");
        }
           
}

void cargaPS(TipoMKV *MKV,int argc,char *argv[],int * flag,int argc2,int argv2[200]){
    int i=1;
    argc2=-1;
    while (i<argc && strstr(argv[i],"-p")!=NULL)
        i++;
    if (i<argc){
        *flag=1;
        int aux=0;
        i++;
        while (i<argc){
            strcpy(MKV->mem[aux],argv[i]);
            argc2++;
            argv2[argc2]=aux;       //aux va a ir contando el inicio de cada palabra, osea que calcula el offset
            aux+=strlen(argv[i])+1;
            i++;
        }

    }
}

void inicia_Memoria(int argc, char *argv[], TipoMKV *MKV){ //Inicia la memoria RAM
    int j=0;
    while (j<argc && (strstr(argv[j],"m=")==NULL))
        j++;
    if (j<argc){
         MKV->tamanoRAM=(unsigned int)atoi(argv[j] + 2);
    }*

    MKV->mem = (unsigned char*)malloc((MKV->tamanoRAM) * sizeof(unsigned char));    
}


int detecta_Tipo_Arch(char *argv[]){ //Funcion que detecta si el archivo es un .vmx, un .vmi o ambos
    int i;
    if(strstr(argv[1],".vmx")!=NULL){
        if(strstr(argv[2],".vmi")==NULL)
            i = 1; //Solo .vmx
            
        else
            i = 2; //Ambos
    }
    else{
        if(strstr(argv[1],".vmi")!=NULL)
            i = 3; //Solo .vmi
        else
            i = 0; //Ninguno
    }
    return i;
}

int verifica_cabecera(unsigned char cabecera[]){ //Verifica la version de la cabecera
   int i=0;
    int flag=0;
    unsigned char comp[6]={'V','M','I','2','5'};

    if (cabecera[2]=='X'){
        comp[2]=='X';
    }
    while (i<5 && comp[i]==cabecera[i]){
        i++;
    }
    if (i==5){
        if (cabecera[2]=='X' && cabecera[6]==0b00000001)  
                return 1;       // VMX primera version
        
        else
            if (cabecera[2]=='X' && cabecera[6]==0b00000010)
                return 2;   //VMX segunda version
            else    
                if (cabecera[2]=='I' && cabecera[6]==0b00000001)        
                return 3;   // VMI25
    }
    else                
        return 0;           // error
}

void ejecucion(TipoMKV *MKV){
    MKV->flag=0;
    unsigned char instruccion;
    int TopA,TopB;
    int dirfis;
    int shift;
    void (*Fops2[16])(TipoMKV *, int, int, int, int )={MOV , ADD , SUB , MUL , DIV , CMP , SHL , SHR , SAR , AND , OR , XOR , SWAP , LDL , LDH , RND};
    void (*Fops1[9])(TipoMKV *, int, int )={SYS , JMP , JZ , JP , JN , JNZ , JNP , JNN , NOT, PUSH, POP, CALL};
    MKV->reg[CS] = MKV->tabla_seg[0] << 16;                          //inicializo el CS en los 8 MSB
    MKV->reg[DS] = 0x00010000;                    //Inicializo del DS una posicion mas del CS 
    MKV->reg[IP] = MKV->reg[CS];
    while ( MKV->reg[IP]!=-1 && !MKV->flag && MKV->reg[IP]<MKV->tabla_seg[1]+MKV->tabla_seg[0]){   // mientas no exista un error o se termine la memoria
        dirfis=logifisi(*MKV ,MKV->reg[IP]);
            instruccion=MKV->mem[dirfis];   // guardo la instruccion de donde apunta IP
            if (codinvalido(instruccion & MASC_CODOP)){     // error: Codigo invalido
                verificaerrores(MKV,1);
            }
            else{
                MKV->reg[OPC]= instruccion & MASC_CODOP;        // guardo el cod de operacion en OPC
                getOperandos(MKV,instruccion,dirfis);
                if (MKV->reg[OPC]==0x0F)                       // STOP
                    MKV->reg[IP]=-1;
                else{
                    if (MKV->reg[OPC]>=0x10 && MKV->reg[OPC]<=0x1F){ //dos operandos
                        TopA=(instruccion & MASC_TOPA) >> 4;
                        TopB=(instruccion & MASC_TOPB) >> 6;
                        cambioip(MKV,TopA,TopB);
                        Fops2[instruccion & 0x0F](MKV,escopeta(MKV->reg[OPA]),TopA,escopeta(MKV->reg[OPB]),TopB);
                    }
                    else
                        if (MKV->reg[OPC]>=0x00 && MKV->reg[OPC]<=0x0D){    //un operando
                            TopA=(instruccion & MASC_TOPB) >> 6;
                            cambioip(MKV,TopA,0);
                            Fops1[instruccion & 0x0F](MKV,escopeta(MKV->reg[OPA]),TopA);  
                        }
                        else
                            if (MKV->reg[OPC]==0x0E){ //RET no posee operandos
                                
                            }
                }
            }
        
    }

}



