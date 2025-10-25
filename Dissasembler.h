#include "TDA.h"


void getOperandosDissa(TipoMKV MKV,char instruccion,int dirfis,int *opA,int *opB, int TopA, int TopB);

void devuelveRegistro(int registro);

void imprimeMnemonico(unsigned char cod);

int nuevadirfis(int dirfis,int TopA,int TopB);

void imprimeOperandos(TipoMKV MKV,int dirfis, int TopA,int TopB );

void imprimehexa_Const(TipoMKV MKV,int dirfis,int *cant);

void ImprimeKS(TipoMKV MKV);

void imprimeTAB(char instruccion,int TopA,int TopB);

void imprimeTAB_Const(int cant);

void dissa(TipoMKV MKV);
