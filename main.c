#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA.h" //Vacio, los prototipos


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
    arch= fopen(nombre_arch, "rb");
        if (arch == NULL)
            printf("Error al abrir el archivo");
        else{
            for (int j=0;i<5;i++)
                fread(&cabecera[j], sizeof(unsigned char), 1, arch);
                fread(&version, sizeof(unsigned char), 1, arch);
            if (verifica_cabecera(cabecera)){ 
                MKV->tabla_seg[0]= 0;     // base CS
                fread(MKV.tabla_seg[1], sizeof(unsigned short), 1, arch);       // se lee el tamanio del codigo
                MKV->tabla_seg[2]=MKV->tabla_seg[1];                    
                MKV->tabla_seg[3]=16384-MKV->tabla_seg[2];
                *CS = MKV->tabla_seg[0];                                           //inicializo el CS 
                *DS = MKV->tabla_seg[1]+1;                                         //Inicializo del DS una posicion mas del CS



                 while (fread(MKV.mem[*CS], sizeof(unsigned char), 1, arch) == 1)  //Guarda las instrucciones en el code segment
                     (*CS)++;  
                
                 
                 fclose(arch);
            } 
            else
                printf("Error al leer cabecera");
        }
}
int Tamanio_Instruccion()


void ejecucion(TipoMKV *MKV){
   


}

