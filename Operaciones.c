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


int get_Valor (TipoMKV *MKV,int op,int Top){
    if (Top==1)
        return MKV->reg[op];
    else
        if (Top==2)
            return op;
        else
            if (Top==3){
                larmar(MKV,op);
                getMemoria(MKV);
                return MKV->reg[MBR];   
            }   
} 

void MOV (TipoMKV *MKV,int opA, int TopA, int opB, int TopB){
    int dirfis,i;
    int offset,cod; // para tipo memoria

    int valorB=get_Valor(MKV,opB,TopB);
        if (TopA==3){
            larmar(MKV,opA);
            setMemoria(MKV);
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
        MKV->reg[AC] = valorA % valorB; //guardo el resto en AC
    }
    else  
        verificaerrores(2);     //error: division por cero 
}

void CMP (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){ 
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA-= valorB;
    NZ_CC(valorA,MKV);
}

void SHL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){ 
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA = valorA << valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void SHR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA[0]=0;
    valorA>>=valorB;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}

void SAR (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    valorA = valorA >> valorB;
    MOV(MKV,opA,TopA,valorA,2);
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

void SWAP(TipoMKV *MKV,int opA,int TopA,int opB,int TopB){
    int valorB, valorA;
    int aux;
    valorB = get_Valor(MKV,opB,TopB);
    valorA = get_Valor(MKV,opA,TopA);
    aux=valorA;
    valorA=valorB;
    valorB=aux;
    MOV(MKV,opA,TopA,valorA,2);
    MOV(MKV,opB,TopB,valorB,2);
}

void LDL (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB) & MASC_LDL ;
    valorA = get_Valor(MKV,opA,TopA); 
    valorA &=MASC_LDH; 
    valorA|=valorB;
    MOV(MKV,opA,TopA,valorA,2);
}

void LDH (TipoMKV *MKV, int opA, int TopA, int opB, int TopB){
    int valorB, valorA;
    valorB = get_Valor(MKV,opB,TopB) & MASC_LDH ;
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
    
void SYS(TipoMKV *MKV, int opA, int TopA){

}

void JMP(TipoMKV *MKV, int opA, int TopA){
   int valor=get_Valor(MKV,opA,TopA);
   if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1])
        MKV->reg[IP]=valor;
    else
        verificaerrores(3); //fallo de segmento
}

void JZ(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if(MKV->reg[CC[1]] == 1)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void JP(TipoMKV *MKV, int opA, int TopA){ 
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if(MKV->reg[CC[0]] == 0 && MKV->reg[CC[1]] == 0)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento 
}

void JN(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if(MKV->reg[CC[0]] == 1 && MKV->reg[CC[1]] == 0)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void JNZ(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if(MKV->reg[CC[1]] == 0)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void JNP(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if(MKV->reg[CC[0]] == 1 || MKV->reg[CC[1]] == 1)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void JNN(TipoMKV *MKV, int opA, int TopA){
    int valor=get_Valor(MKV,opA,TopA);
    if (valor>=MKV->tabla_seg[0] && valor<=MKV->tabla_seg[0]+MKV->tabla_seg[1]){
        if(MKV->reg[CC[0]] == 0)
            MKV->reg[IP]=valor;
    }        
    else
         verificaerrores(3); //fallo de segmento
}

void NOT(TipoMKV *MKV, int opA, int TopA){
    int valorA;
    valorA=get_Valor(MKV,opA,TopA);
    valorA=~valorA;
    MOV(MKV,opA,TopA,valorA,2);
    NZ_CC(valorA,MKV);
}









