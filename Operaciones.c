void NZ_CC(int valor, TipoMKV *MKV){
    MKV->reg[CC]=0;
    if (valor==0)
        MKV->reg[CC] |= 0x40000000; // setea el bit Z 0100 0000 0000 0000 0000 0000 0000 0000
    else
        if (valor<0)
            MKV->reg[CC]|=0x80000000; // setea el bit N 1000 0000 0000 0000 0000 0000 0000 0000
}   //lo afectan ADD, SUB, MUL, DIV, CMP, AND, OR, XOR, SHL, SHR, SAR, NOT

void MOV(TipoMKV *MKV,int opA, short TopA, int opB, short TopB){
    int valor,direccionF;
    //obtengo el valor de opB
    switch (TopB){
        case 0x01: //registro
            valor=MKV->reg[opB];
            break;
        case 0x03:{ //direccion de memoria
            if ((direccionF=logifisi(MKV,0x00010000+opB))==-1){ 
                MKV->codigo_error=3; //Fallo de segmento
                return;
            }
            else
            valor=MKV->mem[direccionF];
            }
            break;
        case 0x02: //valor inmediato
            valor=opB;
            break;
        case 0x00: //
            valor=0;
            break;
    }
    //guardo el valor en opA
    switch (TopA){
        case 0x01: //registro
            MKV->reg[opA]=valor;
            break;
        case 0x03:{//direccion de memoria
            if ((direccionF=logifisi(MKV,0x00010000+opA))==-1){ 
                MKV->codigo_error=3; //Fallo de segmento
                return;
            }
            else
                MKV->mem[direccionF]=valor;// 00 00 00 34 + 00 00 00 02 = 01 00 00 36  
            }
            break;
        default
        case 0x02: //valor inmediato
            MKV->codigo_error=1; //Error de opracion invalida
            break;
        case 0x00: //registro indirecto
            MKV->mem[logifisi(MKV,MKV->reg[DS]+MKV->reg[opA])]=valor;
            break;
    }
}  
