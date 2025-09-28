#include "TDA.h"

void verificaerrores(TipoMKV *MKV,int codigo_error);

int logifisi(TipoMKV MKV,int dirlog);

void larmar(TipoMKV *MKV,int op);

void getMemoria(TipoMKV *MKV);

void setMemoria(TipoMKV *MKV);

int getValor (TipoMKV *MKV,int op,int Top);

void setInmediato(TipoMKV *MKV,int opA,int TopA,int valorB);

void setRegistro(TipoMKV *MKV,int opA,int TopA,int valorB);

int getRegistro(TipoMKV MKV, int opA);

void setValor(TipoMKV *MKV,int op, int Top, int valor);

void cambioip(TipoMKV *MKV,int TopA,int TopB);

int escopeta(int corredera);

int escopeta2bytes(int corredera);

int codinvalido(char instruccion);

void getOperandos(TipoMKV *MKV,unsigned char instruccion,int dirfis);

int bintoint(char str[33]);

void print_bin(int n);
