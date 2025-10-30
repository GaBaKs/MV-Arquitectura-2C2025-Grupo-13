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
            break;
        case 5:
            printf("ERROR: Stack overflow");  
            break;
        case 6:
            printf("ERROR: Stack underflow");  
            break;
        case 7:
            printf("ERROR: Memoria insuficiente");  
            break;
    }
    MKV->flag = 1;
}

void error(TipoMKV *MKV,int a,int b){
    verificaerrores(MKV,1);
}

int logifisi(TipoMKV MKV,int dirlog){
    int dirfis,segmento,base,offset;
    segmento=(dirlog & 0x000F0000)>>16; 
    int aux;
    offset=dirlog & 0x0000FFFF;
    dirfis=0;
    aux=(int)MKV.tabla_seg[segmento][0];
    dirfis = aux+offset; 
    if(dirfis<=MKV.tabla_seg[segmento][1]+MKV.tabla_seg[segmento][0]){
        return dirfis;
    }
    else{
        return -1;
    }
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
    if ((cod<0x0B && cod>=0x09) || cod>0x1F || cod<0x0)
        return 1;
    else
        return 0;
}
                   
int bintoint(char str[33]){     
    int j=strlen(str);
    int acum=0;
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
    printf("0b");
    for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }

}

void cargaVMI(TipoMKV *MKV,char *nombre_arch){
    FILE *arch;
    int i,aux,alto,bajo;
    int tamsegmentos=0;
    char cabecera[6];
    arch=fopen(nombre_arch,"rb");
    if (arch!=NULL){
        for (int j=0;j<6;j++){
            fread(&cabecera[j], sizeof(unsigned char), 1, arch);
        }
         //VMI25 1
         alto=0;
         bajo=0;
         aux=0;
        fread(&alto,sizeof(unsigned char),1,arch);
        fread(&bajo,sizeof(unsigned char),1,arch);
        alto=alto<<8;
        MKV->tamanoRAM=(alto+bajo)*1024; // paso a bytes
        
        MKV->mem = (unsigned char*)malloc((MKV->tamanoRAM) * sizeof(unsigned char));    //inicializacion de la memoria RAM

        for(int j=0;j<32;j++){ //Este ciclo arma los registros leyendo byte a byte los 4 bytes de cada registro
            fread(&aux,sizeof(unsigned char),1,arch);
            MKV->reg[j]=(aux<<24);
            fread(&aux,sizeof(unsigned char),1,arch);
            MKV->reg[j]+=(aux<<16);
            fread(&aux,sizeof(unsigned char),1,arch);
            MKV->reg[j]+=(aux<<8);
            fread(&aux,sizeof(unsigned char),1,arch);
            MKV->reg[j]+=aux;
        }
        for(int k=0;k<8;k++){ //Este ciclo arma la tabla de segmentos 
            fread(&alto,sizeof(unsigned char),1,arch);
            fread(&bajo,sizeof(unsigned char),1,arch);
            MKV->tabla_seg[k][0]=(short)(alto<<8)+bajo;
            fread(&alto,sizeof(unsigned char),1,arch);
            fread(&bajo,sizeof(unsigned char),1,arch);
            MKV->tabla_seg[k][1]=(short)(alto<<8)+bajo;
            tamsegmentos+=MKV->tabla_seg[k][1];
        }
        if (tamsegmentos > MKV->tamanoRAM){
            printf("la tamsegmentos: %d",tamsegmentos);
            verificaerrores(MKV,7); //error de insuficiente memoria
        }
        else{
            for(i=0;i<MKV->tamanoRAM;i++){
                fread(&aux,sizeof(unsigned char),1,arch);
                MKV->mem[i]=(unsigned char)aux;
            }
        }
    fclose(arch);
    }
    else
        printf("Error al abrir el archivo .vmi");
}

void generaVMI(TipoMKV MKV,char *nombre_arch){ //agregar al .h
    FILE *arch;
    int i,aux;
    int alto,bajo;
    char nuevoarch[256],dato;
    strcpy(nuevoarch,nombre_arch);
    char *punto = strrchr(nuevoarch, '.'); // Busca la última aparición de '.'
    if (punto != NULL) {
        strcpy(punto, ".vmi"); // Reemplaza la extensión
    } else {
        strcat(nuevoarch, ".vmi"); // Si no hay extensión, la agrega. En caso de que el nombre sea muy largo, puede causar overflow
                                 // queda a criterio del usuario evitar eso, que explote la MKV 
    } 
    arch=fopen(nuevoarch,"wb");
    if (arch!=NULL){
        dato='V';
        fwrite(&dato,sizeof(char),1,arch);
        dato='M';
        fwrite(&dato,sizeof(char),1,arch);
        dato='I';
        fwrite(&dato,sizeof(char),1,arch);
        dato='2';
        fwrite(&dato,sizeof(char),1,arch);
        dato='5';
        fwrite(&dato,sizeof(char),1,arch);
        dato=1;//version
        fwrite(&dato,sizeof(char),1,arch);

        aux=MKV.tamanoRAM/1024; // lo paso a KiB
        alto=(aux&0xFF00)>>8;
        bajo=aux&0x00FF;
        dato=alto; 
        fwrite(&dato,sizeof(unsigned char),1,arch);
        dato=bajo;
        fwrite(&dato,sizeof(unsigned char),1,arch);
        //Fin de la carga del header
        for(int j=0;j<32;j++){ //Este ciclo arma los registros escribo byte a byte los 4 bytes de cada registro
            dato=((MKV).reg[j]&0xFF000000)>>24;//dato es char entonces no me preocupo por los bits de mas
            fwrite(&dato,sizeof(char),1,arch);
            dato=((MKV).reg[j]&0x00FF0000)>>16;
            fwrite(&dato,sizeof(char),1,arch);
            dato=((MKV).reg[j]&0x0000FF00)>>8;
            fwrite(&dato,sizeof(char),1,arch);
            dato=((MKV).reg[j]&0x000000FF);
            fwrite(&dato,sizeof(char),1,arch);
        }
        for(int k=0;k<8;k++){ //Este ciclo arma la tabla de segmentos 
            dato=((MKV).tabla_seg[k][0]&0xFF00)>>8;
            fwrite(&dato,sizeof(unsigned char),1,arch);
            dato=((MKV).tabla_seg[k][0]&0x00FF);
            fwrite(&dato,sizeof(unsigned char),1,arch);
            dato=((MKV).tabla_seg[k][1]&0xFF00)>>8;
            fwrite(&dato,sizeof(unsigned char),1,arch);
            dato=((MKV).tabla_seg[k][1]&0x00FF);
            fwrite(&dato,sizeof(unsigned char),1,arch);
        }
        for(i=0;i<MKV.tamanoRAM;i++){
            dato=(unsigned char)(MKV).mem[i];
            fwrite(&dato,sizeof(unsigned char),1,arch);
        }
    }
    fclose(arch);
}

void getOperandos(TipoMKV *MKV, unsigned char instruccion,int dirfis){              
    int TopA,TopB;
        TopA=(instruccion & MASC_TOPA) >> 4;
        TopB=(instruccion & MASC_TOPB) >> 6; 
        if (dirfis+TopA+TopB<=(MKV->tabla_seg[(MKV->reg[CS] & MASC_LDH)>>16][0]  +  MKV->tabla_seg[(MKV->reg[CS] & MASC_LDH) >>16][1])){
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
        else{
           verificaerrores(MKV,3);  
        }
}        

int getValor (TipoMKV *MKV,int op,int Top){
    if (Top==1)
        return getRegistro(*MKV,op);
    else
        if (Top==2)
            return escopeta2bytes(op); //la escopeta funciona como un "getInmediato"
        else
            if (Top==3){
                larmar(MKV,op);
                getMemoria(MKV);  
                return MKV->reg[MBR];
            }
}

void setValor(TipoMKV *MKV,int op, int Top, int valor){
    if (Top==1){
        setRegistro(MKV,op,valor);
    }
    else
        if (Top==3){
            larmar(MKV,op);
            MKV->reg[MBR]=valor;
            setMemoria(MKV);  
        }
}

int getRegistro(TipoMKV MKV, int opA){
    int reg=opA & 0x0000001F;
    int tipo= (opA & MASC_SECREG) >> 6;
    switch(tipo){
        case 0: return MKV.reg[reg];
            break;
        case 1: return ((MKV.reg[reg] & MASC_AL)<<24) >>24;
            break;
        case 2: return ((MKV.reg[reg] & MASC_AH)<< 16) >>24;
            break;
        case 3: return ((MKV.reg[reg] & MASC_AX)<<16) >>16;
            break;
        default: return MKV.reg[reg];
    }
    
}

void setRegistro(TipoMKV *MKV,int opA,int valorB){      //guarda un valorB en un registro
    int reg=opA & 0x0000001F;
    int tipo= (opA & MASC_SECREG) >> 6;
    switch(tipo){
        case 0: MKV->reg[reg]=valorB;
            break;
        case 1: MKV->reg[reg]=MKV->reg[reg] & 0xFFFFFF00 + valorB & MASC_AL;
            break;
        case 2: MKV->reg[reg]=(MKV->reg[reg] & 0xFFFF00FF) + ((valorB & MASC_AL)<<8);
            break;
        case 3:  MKV->reg[reg]=(MKV->reg[reg] & MASC_LDH) + (valorB & MASC_AX);
            break;
    }
}

void setInmediato(TipoMKV *MKV,int opA,int TopA,int valorB){ //valorB llega siendo un inmediato

    valorB=(escopeta2bytes(valorB));

    if (TopA==1){        //meto inmediato a registro
        MKV->reg[opA]=valorB;     
    }
    else
        if(TopA==3){    //meto inmediato a memoria
            larmar(MKV,opA);
            MKV->reg[MBR]=valorB;
            setMemoria(MKV);    
        }  
}

void larmar(TipoMKV *MKV,int op){        // cada vez q se accede a memoria
    int aux=0;
    short offset=(op & MASC_OFFSET);
    int dirlog= (MKV->reg[(op & 0x001F0000) >> 16]);
    printf("\n OFFSET: %x dirlog: %x",offset,dirlog);
    dirlog= dirlog+offset;
    printf("\n\n\nDIRLOG LARMAR: %x\n\n\n",dirlog);
    int auxL=logifisi(*MKV,dirlog);
    int opmem;
    MKV->reg[LAR]=dirlog;
    if (auxL!=-1){
        opmem=op & MASC_OPMEM;
        if (opmem==0)
             MKV->reg[MAR]= 0x00040000+auxL;
        else
            if (opmem==2)
                MKV->reg[MAR]= 0x00020000+auxL;
            else
                if (opmem==3){
                    MKV->reg[MAR]= 0x00010000+auxL;  
                    printf("LEE 1 DATO DE MEMORIA \n");
                }      
    }
    else{
        printf("error de segmento LARMAR");
        verificaerrores(MKV,3); 
                     // Error de segmento
    }
}

void getMemoria(TipoMKV *MKV){      // guarda en MBR el dato de la direccion guardada en MAR
     int max,valor,aux;
     valor=0;
     int dirfis=MKV->reg[MAR] & MASC_MARL;
     max=(MKV->reg[MAR] & MASC_MARH) >>16;
     printf("\n\n\n\nSNADHABSDH\n\n\n\n");
     for (int i=0;i<max;i++){
        valor<<=8;
        aux=MKV->mem[dirfis++];
        valor+=aux;
     }
    if (max==1){
        valor= (valor <<24);
        valor= valor >>24;
    }
    else
        if (max==2){
            valor= valor <<16;
            valor= valor >>16;
        }
    MKV->reg[MBR]=valor;
}

void setMemoria(TipoMKV *MKV){      // guarda el dato de MBR en memoria en la direc de MAR
    int segmento;
    int dirfis,valor=MKV->reg[MBR];
    dirfis=MKV->reg[MAR] & MASC_MARL;
    int cantceldas=(MKV->reg[MAR] & MASC_LDH) >> 16;
    segmento=(MKV->reg[LAR] & 0x000F0000)>>16;
    if (dirfis+cantceldas<=MKV->tabla_seg[segmento][0]+MKV->tabla_seg[segmento][1])
        for (int i=cantceldas;i>0;i--){ 
            MKV->mem[dirfis++]=(char)(valor >> (((i-1)*8)) & 0x000000FF) ; 
        }
    else 
        verificaerrores(MKV,3); //fallo de segmento
     
}

