#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA.h" //Vacio, los prototipos
#include "mascaras.h"
#include "Operaciones_Generales.h"
#include "Operaciones.h"
#include "Codigos_Registros.h"
#include "Dissasembler.h"
#define nulo -1


void inicializacion(char nombre_arch[], TipoMKV *MKV,int argc,char *argv[],char *nombreVMI);
void creotablaseg(TipoMKV *MKV,short vectoraux[]);
void cargaPS(TipoMKV *MKV,int argc,char *argv[],int * tamPS,int *argc2);
void inicia_SS(TipoMKV *MKV, int argc2);
void inicia_Memoria(int argc, char *argv[], TipoMKV *MKV);
int detecta_Tipo_Arch(char *argv[]);
int verifica_cabecera(unsigned char cabecera[],int version);
void ejecucion(TipoMKV *MKV,char * nombreVMI);

int main(int argc, char *argv[]){
    printf("Bienvenido a la Maquina virtual del grupo 13! Autores: Mario Arriaga, Tomas Candotto y Gabriel Seneca :)\n");
    TipoMKV MKV;
    MKV.tamanoRAM=MEMORIA;
    char str[200];
    strcpy(str,argv[1]);
    printf("%s\n",str);
    int j=1;  
    MKV.flag=0;
    char nombreVMI[200];
    inicializacion(str,&MKV,argc,argv,nombreVMI);
    //compruebo dissa
    while (j<argc && (strcmp(argv[j],"-d")==0))
        j++;
    if (j<argc){
         dissa(MKV);
         printf("\n");
    }
    ejecucion(&MKV,nombreVMI);
    return 1;
}
void inicializa_tablaseg_en_cero(TipoMKV *MKV){
    for (int i=0;i<8;i++){
        MKV->tabla_seg[i][0]=0;
        MKV->tabla_seg[i][1]=0;
    }
}

void inicializacion(char nombre_arch[], TipoMKV *MKV,int argc,char *argv[],char *nombreVMI){ 
    FILE *arch;
    int i;
    i = 0;
    int desp=0;
    int tamPS;
    int argc2;
    int aux;
    int version=0;
    unsigned char cabecera[5];
    arch= fopen(nombre_arch, "rb");
        if (arch == NULL)
            printf("Error al abrir el archivo, verifique que el nombre ingresado sea el correcto e intente nuevamente. Si es un profesor y esto no funciona, tengan piedad con nuestras almas. cuack");
        else{  
            for (int j=0;j<5;j++){
                fread(&cabecera[j], sizeof(unsigned char), 1, arch);
            }    
            fread(&version, sizeof(unsigned char), 1, arch);
            int cabecera2=verifica_cabecera(cabecera,version);       //verifico la version
            inicializa_tablaseg_en_cero(MKV);
            MKV->mem = (unsigned char*)malloc((MKV->tamanoRAM) * sizeof(unsigned char));    
            if (cabecera2==1){           //VMX25 parte 1
                fread(&MKV->tabla_seg[0][0], sizeof(unsigned char), 1, arch); 
                fread(&MKV->tabla_seg[0][1], sizeof(unsigned char), 1, arch);        // tamano max CS
                MKV->tabla_seg[1][0]=MKV->tabla_seg[0][1];                            //base DS
                MKV->tabla_seg[1][1]=MKV->tamanoRAM-MKV->tabla_seg[1][0];                      //tamano max DS   
                while (fread(&MKV->mem[desp], sizeof(unsigned char), 1, arch) == 1){  //Guarda las instrucciones en el code segment
                    desp++;
                    printf("uwu");                    
                }
                
                printf("Lectura de archivo realizada correctamente para la version 1\n");
                fclose(arch);
            }
            else
                if (cabecera2==2){           //VMX25 parte 2
                    if (strstr(argv[1],".vmi")==NULL)
                        strcpy(nombreVMI,"?");
                    else
                        strcpy(nombreVMI,argv[1]);
                    MKV->reg[CS]=-1;
                    MKV->reg[DS]=-1;
                    MKV->reg[ES]=-1;
                    MKV->reg[SS]=-1;
                    MKV->reg[KS]=-1;
                    MKV->reg[PS]=-1;
                    unsigned short vectoraux[6];
                    inicia_Memoria(argc,argv,MKV);
                    cargaPS(MKV,argc,argv,&tamPS,&argc2);
                    vectoraux[5]=tamPS;
                    for(int j=0;j<5;j++){
                        fread(&vectoraux[j],sizeof(short),1,arch);
                    }
                    creotablaseg(MKV,vectoraux);
                    if (!MKV->flag){
                        MKV->reg[SP]=MKV->reg[SS]+MKV->tabla_seg[(MKV->reg[SS] & 0x000F0000) >> 16][1];    // inicio SP con el valor de SS y el tam de la pila
                        fread(&aux,sizeof(unsigned char),1,arch);
                        MKV->reg[IP]=MKV->reg[CS]+aux;
                        inicia_SS(MKV,argc2);
                        while ( desp != MKV->tabla_seg[(MKV->reg[CS]&MASC_LDH)>>16][1]){  //Guarda las instrucciones en el code segment
                            fread(&MKV->mem[MKV->tabla_seg[(MKV->reg[CS]&MASC_LDH)>>16][0]+desp], sizeof(char), 1, arch);
                            desp++;                    
                        }
                        desp=0;
                        while(desp != MKV->tabla_seg[(MKV->reg[KS]&MASC_LDH)>>16][1]){  //Guarda las instrucciones en el code segment
                            fread(&MKV->mem[MKV->tabla_seg[(MKV->reg[KS]&MASC_LDH)>>16][0]+desp], sizeof(char), 1, arch);  //Guarda las instrucciones en el code segment
                            desp++;                    
                        }
                    fclose(arch);
                    }
                }
                else
                    if (cabecera2==3){       //VMI25 1
                        fclose(arch);
                        cargaVMI(MKV,nombre_arch);
                    }
                    else
                        printf("Error al leer cabecera");
        }
           
}


void creotablaseg(TipoMKV *MKV,short vectoraux[]){
    int contseg=0;
    int base=0;
    int suma=0;
    int i=0;
    while (i<6 && suma<=MKV->tamanoRAM){
        suma+=vectoraux[i];
        i++;
    }
    if (suma>MKV->tamanoRAM){
        verificaerrores(MKV,7); //error de insuficiente memoria
        MKV->flag=1;
    }
    else{
        if (vectoraux[5]!=0){      // hay param segment
            MKV->tabla_seg[contseg][0]=base;
            MKV->tabla_seg[contseg][1]=vectoraux[5];
            MKV->reg[PS]=contseg<<16;
            base+=vectoraux[5];
            contseg++;
        }
        if (vectoraux[4]!=0){   // hay const segment
            MKV->tabla_seg[contseg][0]=base;
            MKV->tabla_seg[contseg][1]=vectoraux[4];
            MKV->reg[KS]=contseg<<16;
            base+=vectoraux[4];
            contseg++;
        }
        for (int i=0;i<4;i++){
            if (vectoraux[i]!=0){
                MKV->tabla_seg[contseg][0]=base; 
                MKV->tabla_seg[contseg][1]=vectoraux[i];
                MKV->reg[26+i]=contseg<<16;          //situa en orden CS, DS, ES y SS
                base+=vectoraux[i];
                contseg++;
            }
        }
    }
}

void cargaPS(TipoMKV *MKV,int argc,char *argv[],int * tamPS,int *argc2){
    int i=1;
   *argc2=0;
    int aux=0;
    int argv2[200];
    while (i<argc && strcmp(argv[i],"-p")!=0)
        i++;
    if (i<argc){        // hay -p
        i++;
        while (i<argc){
            int largo=strlen(argv[i]);
            argv2[*argc2]=aux;       //aux va a ir contando el inicio de cada palabra, osea que calcula el offset
            *argc2++;
            for (int k=0;k<=largo;k++){
                MKV->mem[aux]=argv[i][k]; // guardo char a char las palabras con el nulo
                aux++;
            }
            i++;
        }
        // guardo las direcciones despues de todos los datos
        for (i=0;i<*argc2;i++){
            for (int k=4;k>0;k--){ // hacer mascara
                MKV->mem[aux]=(char)(argv2[i] >> (((k-1)*8)) & 0x000000FF) ;  // guarda en memoria los punteros a cada palabra
                aux++;
            }
        } 
        (*tamPS) = aux;
    }
    else
        (*tamPS) = 0;
}

void inicia_SS(TipoMKV *MKV, int argc2){
    int dirfis;
    if (MKV->reg[SP]-12<MKV->reg[SS])
        verificaerrores(MKV,5);    // STACK OVERFLOW
    else{
        MKV->reg[SP]-= 4;
        dirfis=logifisi(*MKV,MKV->reg[SP]);
        if (argc2==0){
            for (int i=CANTCELDAS;i>0;i--) 
            MKV->mem[dirfis++]=0xFF; 
        }
        else
            for (int i=CANTCELDAS;i>0;i--) 
                MKV->mem[dirfis++]=0;       // carga puntero a 1er elemento de argv en PS
        MKV->reg[SP]-= 4;
        dirfis=logifisi(*MKV,MKV->reg[SP]);
        for (int i=CANTCELDAS;i>0;i--) 
            MKV->mem[dirfis++]=(char)(argc2 >> (((i-1)*8)) & 0x000000FF);            //carga argc2
        MKV->reg[SP]-= 4;
        dirfis=logifisi(*MKV,MKV->reg[SP]);
        for (int i=CANTCELDAS;i>0;i--) 
            MKV->mem[dirfis++]=0xFF;              //carga -1(para llamar con ret)
    }
}

void inicia_Memoria(int argc, char *argv[], TipoMKV *MKV){ //Inicia la memoria RAM
    int j=0;
    while (j<argc && (strstr(argv[j],"m=")==NULL))
        j++;
    if (j<argc){
         MKV->tamanoRAM=(unsigned int)atoi(argv[j] + 2);
    }

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

int verifica_cabecera(unsigned char cabecera[],int version){ //Verifica la version de la cabecera
   int i=0;
    int flag=0;
    unsigned char comp[6]={'V','M','I','2','5'};
    if (cabecera[2]=='X'){
        comp[2]='X';
    }
    while (i<5 && comp[i]==cabecera[i]){
        i++;
    }
    if (i==5){
        if (cabecera[2]=='X' && version==1)  
                return 1;       // VMX primera version
        
        else
            if (cabecera[2]=='X' && version==2)
                return 2;   //VMX segunda version
            else    
                if (cabecera[2]=='I' && version==1)        
                return 3;   // VMI25 
    }
    else                
        return 0;           // error
}

void ejecucion(TipoMKV *MKV,char * nombreVMI){
    unsigned char instruccion;
    int TopA,TopB;
    int dirfis;
    int shift;
    MKV->breakpoint=0;
    void (*Fops2[16])(TipoMKV *, int, int, int, int )={MOV , ADD , SUB , MUL , DIV , CMP , SHL , SHR , SAR , AND , OR , XOR , SWAP , LDL , LDH , RND};
    void (*Fops1[12])(TipoMKV *, int, int )={SYS , JMP , JZ , JP , JN , JNZ , JNP , JNN , NOT, PUSH, POP, CALL};
    while ( MKV->reg[IP]!=-1 && !MKV->flag && MKV->reg[IP]<MKV->tabla_seg[(MKV->reg[CS] & MASC_LDH) >>16][0]  +  MKV->tabla_seg[(MKV->reg[CS] & MASC_LDH) >>16][1]){   // mientas no exista un error o se termine la memoria
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
                else
                    if (MKV->reg[OPC]==0x0E)                    //RET
                        RET(MKV);
                    else{
                        if (MKV->reg[OPC]>=0x10 && MKV->reg[OPC]<=0x1F){ //dos operandos
                            TopA=(instruccion & MASC_TOPA) >> 4;
                            TopB=(instruccion & MASC_TOPB) >> 6;
                            cambioip(MKV,TopA,TopB);
                            Fops2[instruccion & 0x0F](MKV,escopeta(MKV->reg[OPA]),TopA,escopeta(MKV->reg[OPB]),TopB);
                        }
                        else
                            if (MKV->reg[OPC]==0x0A || MKV->reg[OPC]==0x09)
                                verificaerrores(MKV,1);
                            else
                                if (MKV->reg[OPC]>=0x00 && MKV->reg[OPC]<=0x0D){    //un operando
                                    TopA=(instruccion & MASC_TOPB) >> 6;
                                    cambioip(MKV,TopA,0);
                                    Fops1[instruccion & 0x0F](MKV,escopeta(MKV->reg[OPA]),TopA);  
                                } 
                    }
            }
        if (!MKV->flag && MKV->breakpoint){       // encontro un breakpoint
            char quehago;
            if(strcmp(nombreVMI,"?")!=0){
                generaVMI(*MKV,nombreVMI);
                scanf("%c",&quehago);
                switch(quehago){
                    case 'g': MKV->breakpoint=0;
                        break;
                    case 'q': MKV->reg[IP]=-1;
                        break;
                }
            }
            else
                MKV->breakpoint=0;
        }
    }     
}
