#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define REGISTROS 32
#define MEMORIA 16384

typedef struct{
    unsigned int Reg[REGISTROS];           // Cantidad de registros
    unsigned char Mem[MEMORIA];            // Cantidad de memoria
    unsigned char codigo_error;            // Codigo de error
    unsigned short tabla_Seg[4];           
} TipoMKV;


int main(){

    return 0;
}



int verifica_cabecera(unsigned char cabecera[5]){ //Verifica la version de la cabecera
    int i=0;
    unsigned char comp[5]={'V','M','X','2','5'};
    while (i<5 && comp[i]==cabecera[i])
        i++;
    if (i==5)
        return 1; //Version correcta
    else
        return 0; //Version erronea 
}

void Lectura(char nombre_arch[],TipoMKV *MKV, int *DS,int *CS){
    FILE *arch;
    int i;
    i = 0; 
    char version;
    unsigned char cabecera[5];
    archivo = fopen(nombre_arch, "rb");
        if (arch == NULL)
            printf("Error al abrir el archivo");
        else{
            for (int j=0;i<5;i++)
                fread(&cabecera[j], sizeof(unsigned char), 1, arch);
                fread(&version, sizeof(unsigned char), 1, arch);
            if (verifica_cabecera(cabecera)){ 
                MKV->tablaseg[0]= 0;     // base CS
                fread(MKV.tablaseg[1], sizeof(unsigned short), 1, arch);       // se lee el tamanio del codigo
                MKV->tablaseg[2]=MKV->tablaseg[1];                    
                MKV->tablaseg[3]=16384-MKV->tablaseg[2];
                *CS = MKV->tablaseg[0];                                           //inicializo el CS 
                *DS = MKV->tablaseg[1]+1;                                         //Inicializo del DS una posicion mas del CS



                 while (fread(MKV.mem[*CS], sizeof(unsigned char), 1, arch) == 1)  //Guarda las instrucciones en el code segment
                     (*CS)++;  
                
                 
                 fclose(arch);
            } 
            else
                printf("Error al leer cabecera");
        }
}



void ejecucion(TipoMKV *MKV){
   


}

