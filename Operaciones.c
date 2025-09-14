#include "TDA.h"
#include "mascaras.h"
#include "Operaciones_Generales.c"
void NZ_CC (int valor, TipoMKV *MKV){
    MKV->reg[CC]=0;
    if (valor==0)
        MKV->reg[CC] |= 0x40000000; // setea el bit Z 0100 0000 0000 0000 0000 0000 0000 0000
    else
        if (valor<0)
            MKV->reg[CC]|=0x80000000; // setea el bit N 1000 0000 0000 0000 0000 0000 0000 0000
}   //lo afectan ADD, SUB, MUL, DIV, CMP, AND, OR, XOR, SHL, SHR, SAR, NOT

int get_direccion_A (TipoMKV MKV,int opA,int TopA){
    int dirfis;
    //mov[reg + cte ],opb / mov [cte],opb / mov [reg],opb contemplar estos casos 
    int offset=0,cod; // para tipo memoria
    if (TopA==3){
        offset= opA & MASC_OFFMOV;  //devuelve el offset
        cod = (opA >> 16) & MASC_CODMOV; 
    }
    else   
        cod=opA; 
    if((dirfis=logifisi(MKV,MKV->reg[cod]))!=-1)
        return offset+dirfis;
    else{
        verificaerrores(3);
    }    

}

int get_Valor (TipoMKV *MKV,int op,int Top){
    if (Top==1)
        return MKV->reg[op];
    else
        if (Top==2)
            return op;
        else
            if (Top==3){
                int valor=0,aux;
                int dirlog=0x00010000 + MKV->reg[(op & 0x001F00) >> 16]+ op & MASC_OFFMOV;  //verificar
                int dirfis=logifisi(*MKV,dirlog);
                for (int i=0;i<4;i++){
                   aux=MKV->mem[dirfis+1+i];
                   valor+=aux<<(3-i)*8;
                }
                return valor;   
            }   
} 

void MOV (TipoMKV *MKV,int opA, int TopA, int opB, int TopB){ //testeado todo ok 
    int dirfis,i;
    int offset,cod; // para tipo memoria

    int valorB=getValor(MKV,opB,TopB);
        if (TopA==3){
            dirfis=get_direccion_A(*MKV,opA,TopA);
            if (dirfis+CANTCELDAS<=MEMORIA && dirfis>=MKV->tabla_seg[3])
                for (i=CANTCELDAS;i<0;i--){ 
                    MKV->mem[dirfis++]=(char)(valorB >> ((i-1)*8)) & 0x000000FF ; 
                }
        }
        else
            MKV->reg[opA]=valorB;
}

void ADD (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA+= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void SUB (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){ 
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA-= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void MUL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA*= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void DIV (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    if(valorB != 0){
        valorA = (int) valorA / valorB;
        MOV(MKV,opA,TopA,valorA,2);
        NZ_CC(valorA,MKV);
        MKV->reg[AC] = valorA % valorB; //LE PONGO EL RESTO AL AC, NO SE COMO LE PUSIMOS NOSOTROS
    }
    else{
        MKV->codigo_error = 2;
        verificaerrores(MKV->codigo_error);
    }    
}

void CMP (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){ 
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA-= valorB;
    NZ_CC(valorA,MKV);
}

void SAR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){ //SHR ECX, 4 -> SERIA COMO HACER MOV ECX, [ECX] >> 4
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA = valorA >> valorB;
    MOV(MKV,opA,TopA,valorA,2); //EL 2 ES PORQUE ES INMEDIATO, ME LO TIRÓ COPILOT 
    NZ_CC(valorA,MKV);
}

void SHR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){ //SHR ECX, 4 -> SERIA COMO HACER MOV ECX, [ECX] >> 4
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    for (int i=0;i<valorB;i++){
        valorA>>=1;
        valorA=valorA & 0b01111111111111111111111111111111;
    }
    MOV(MKV,opA,TopA,valorA,2); //EL 2 ES PORQUE ES INMEDIATO, ME LO TIRÓ COPILOT 
    NZ_CC(valorA,MKV);
}

void SHL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){ 
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA = valorA << valorB;
    MOV(MKV,opA,TopA,valorA,2); //EL 2 ES PORQUE ES INMEDIATO, ME LO TIRÓ COPILOT 
    NZ_CC(valorA,MKV);
}
 
void AND (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA &= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void OR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA |= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void XOR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA ^= valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void SWAP(TipoMKV *MKV,int MKV->reg[OPA],int TopA,int MKV->reg[OPB],int TopB){
}

void LDH (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB) & MASC_LDL ; // que pasa si no dan un inmediato??
    valorA = get_Valor(MKV,opA,TopA);
    valorA &=MASC_LDH; 
    valorA|=valorB<<16;
    MOV(MKV,opA,TopA,valorA,2);
}

void LDL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB) & MASC_LDL ; // que pasa si no dan un inmediato??
    valorA = get_Valor(MKV,opA,TopA); 
    valorA &=MASC_LDL; 
    valorA|=valorB;
    MOV(MKV,opA,TopA,valorA,2);
}

void RND (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    srand();
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = rand() % (valB + 1); //rand() genera un numero aleatorio y al hacerle % (valB + 1) se establece el rango entre 0 y valB.    
    MOV(MKV,opA,TopA,valorA,2);
}
    



