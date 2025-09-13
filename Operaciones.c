#include "TDA.h"
#include "mascaras.h"
#include "Operaciones_Generales.c"
void NZ_CC(int valor, TipoMKV *MKV){
    MKV->reg[CC]=0;
    if (valor==0)
        MKV->reg[CC] |= 0x40000000; // setea el bit Z 0100 0000 0000 0000 0000 0000 0000 0000
    else
        if (valor<0)
            MKV->reg[CC]|=0x80000000; // setea el bit N 1000 0000 0000 0000 0000 0000 0000 0000
}   //lo afectan ADD, SUB, MUL, DIV, CMP, AND, OR, XOR, SHL, SHR, SAR, NOT



/*void MOV(TipoMKV *MKV,int opA, short TopA, int opB, short TopB){
    int valor,direccionF;
    //obtengo el valor de opB
   // direccionF=logifisi(MKV,dirlog) ;      // / 00 00 00 00 | valor leido
    /*switch (TopB){
        case 0x01: //registro
            valor=Consigue_Valor(MKV->reg[OPB];
            break;
        case 0x03:{ //direccion de memoria
            valor=Consigue_valor(MKV->mem[direccionF];
            }
            break;
        case 0x02: //valor inmediato
            valor=opB;
            break;
       
    }
    //valorB=Consigue_Valor(MKV->reg[OPB]; // int 
    
    //guardo el valor en opA
    switch (TopA){
        case 0x01: //registro
            MKV->reg[opA]=opB;
            break;
        case 0x03://direccion de memoria
            if ((direccionF=logifisi(MKV,0x00010000+opA))==-1){ 
                MKV->codigo_error=3; //Fallo de segmento
                return;
            }
            else
                MKV->mem[direccionF]=valor;  
            break;
        case 0x02: //valor inmediato
            MKV->codigo_error=1; //Error de opracion invalida
            break;
        case 0x00: //registro indirecto
            MKV->mem[logifisi(MKV,MKV->reg[DS]+MKV->reg[opA])]=valor;
            break;
    }
}  */

int get_direccion_A(TipoMKV MKV,int opA,int TopA){
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

int get_Valor(TipoMKV *MKV,int op,int Top){


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

void MOV(TipoMKV *MKV,int opA, int TopA, int opB, int TopB){ //hacer bien 
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
       
 

void SHR(TipoMKV MKV, int opA, int opB, short TopA, short TopB){ //chequear
    int valor, direccionF;
    switch(TopB){
        case 0x01: //registro
            valor = MKV->reg[opB];
            break;
        case 0x02: //inmediato
            valor = opB;
            break;
        case 0x03:{ //direccion de memoria
            direccionF = -1;
            logifisi(MKV,0x00010000+opB,&direccionF);
            if(direccionF != -1) //la direccion fisica del opB es valida
                valor = Consigue_valor(MKV->mem[direccionF]);
            else
                //NI IDEA
        } 
    }
        switch (TopA){
        case 0x01: //registro
            MKV->reg[opB] = MKV->[opB] >> valor;
            break;
        case 0x02: //inmediato
            opA = opA >> valor;
            break;
        case 0x03: { //direccion de memoria
            direccionF = -1;
            logifisi(MKV,0x00010000+opA,&direccionF);
            if(direccionF != -1) //la direccion fisica del opB es valida
                MKV->mem[direccionF] = MKV->mem[direccionF] >> valor; // REVISAR
            else
                //NI IDEA
            }
        }
}

void SHL(TipoMKV MKV, int opA, int opB, short TopA, short TopB){
    int valor, direccionF;
    switch(TopB){
        case 0x01: //registro
            valor = MKV->reg[opB];
            break;
        case 0x02: //inmediato
            valor = opB;
            break;
        case 0x03:{ //direccion de memoria
            direccionF = -1;
            logifisi(MKV,0x00010000+opB,&direccionF);
            if(direccionF != -1) //la direccion fisica del opB es valida
                valor = Consigue_valor(MKV->mem[direccionF]);
            else
                //NI IDEA
        } 
    }
        switch (TopA){
        case 0x01: //registro
            MKV->reg[opB] = MKV->[opB] << valor;
            break;
        case 0x02: //inmediato
            opA = opA << valor;
            break;
        case 0x03: { //direccion de memoria
            direccionF = -1;
            logifisi(MKV,0x00010000+opA,&direccionF);
            if(direccionF != -1) //la direccion fisica del opB es valida
                MKV->mem[direccionF] = MKV->mem[direccionF] << valor; // REVISAR
            else
                //NI IDEA
            }
        }//
}



void SWAP(TipoMKV *MKV,int MKV->reg[OPA],int TopA,int MKV->reg[OPB],int TopB){

    


}

void ADD(){

}
void MUL(){

}
void DIV(){

}