#include "TDA.h"

void NZ_CC (int valor, TipoMKV *MKV);

int get_Valor (TipoMKV *MKV,int op,int Top);

void MOV (TipoMKV *MKV,int opA, int TopA, int opB, int TopB);

void ADD (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void SUB (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void MUL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void DIV (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void CMP (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void SHL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void SHR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void SAR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void AND (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void OR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void XOR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void SWAP(TipoMKV *MKV,int opA,int TopA,int opB,int TopB);

void LDL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void LDH (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void RND (TipoMKV *MKV, int opA, int TopA, int opB, int TopB);

void SYS(TipoMKV *MKV, int opA, int TopA);

void JMP(TipoMKV *MKV, int opA, int TopA);

void JZ(TipoMKV *MKV, int opA, int TopA);

void JP(TipoMKV *MKV, int opA, int TopA);

void JN(TipoMKV *MKV, int opA, int TopA);

void JNZ(TipoMKV *MKV, int opA, int TopA);

void JNP(TipoMKV *MKV, int opA, int TopA);

void JNN(TipoMKV *MKV, int opA, int TopA);

void NOT(TipoMKV *MKV, int opA, int TopA);


