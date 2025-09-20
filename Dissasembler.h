#include "TDA.h"


void getOperandosDissa(TipoMKV MKV,char instruccion,int dirfis,int *opA,int *opB, int TopA, int TopB);

char* devuelveRegistro(unsigned char car);

void imprimeMnemonico(unsigned char cod);

int nuevadirfis(int dirfis,int TopA,int TopB);

void imprimeTAB(char instruccion,int TopA,int TopB);

void dissa(TipoMKV MKV);