#include <stdio.h>
#include <stdlib.h>
#include <string.h>

generavectornmemo(Tmnemo Vmnemo)
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

int logifisi(tipoMKV MKV,int dirlog){
    int dirfis,segmento,dirmask;
    segmento=dirlog>>16;
    dirmask=dirlog & 0x0000FFFF;
        if (segmento==0){ //Segmento de codigo 
            if (dirlog>=0 && dirlog<MKV.tabla_seg[2]){ //Verifica que la direccion logica este dentro del segmento de codigo
                dirfis=MKV.tabla_seg[0]+dirmask; //Direccion fisica = base CS + direccion logica
                return dirfis;
            }
            else{
                MKV.codigo_error=3; //Fallo de segmento
                return -1;
            }
        }
        else{
            if (dirlog>=0 && dirlog<MKV.tabla_seg[4]){ //Verifica que la direccion logica este dentro del segmento de datos
                dirfis=MKV.tabla_seg[3]+dirmask; //Direccion fisica = base DS + direccion logica
                return dirfis;
            }
            else{
                MKV.codigo_error=3; //Fallo de segmento
                return -1;
            }
        }
}