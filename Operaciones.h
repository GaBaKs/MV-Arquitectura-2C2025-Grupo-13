#include "TDA.h"

void NZ_CC (int valor, TipoMKV *MKV);

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

void SYS1 (TipoMKV *MKV,int cantDatos, int dirfis,int segmento,int i);

void SYS2 (TipoMKV *MKV,int cantDatos, int dirfis,int segmento, int i);

void SYS3 (TipoMKV *MKV,int cantDatos, int dirfis, int segmento);

void SYS4 (TipoMKV *MKV,int dirfis, int segmento);

void SYS7 ();

void SYSF (TipoMKV *MKV);

void JMP(TipoMKV *MKV, int opA, int TopA);

void JZ(TipoMKV *MKV, int opA, int TopA);

void JP(TipoMKV *MKV, int opA, int TopA);

void JN(TipoMKV *MKV, int opA, int TopA);

void JNZ(TipoMKV *MKV, int opA, int TopA);

void JNP(TipoMKV *MKV, int opA, int TopA);

void JNN(TipoMKV *MKV, int opA, int TopA);

void NOT(TipoMKV *MKV, int opA, int TopA);

void PUSH(TipoMKV *MKV, int opA, int TopA);

void POP(TipoMKV *MKV,int opA,int TopA);

void CALL(TipoMKV *MKV,int opA,int TopA);

void RET(TipoMKV *MKV);

