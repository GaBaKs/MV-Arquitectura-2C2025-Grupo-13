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
    direccionF=logifisi         00 00 00 00 | valor leido
    switch (TopB){
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
    valorB=Consigue_Valor(MKV->reg[OPB]; // int 
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
                MKV->mem[direccionF]=valor;  
            break;
        default:
        case 0x02: //valor inmediato
            MKV->codigo_error=1; //Error de opracion invalida
            break;
        case 0x00: //registro indirecto
            MKV->mem[logifisi(MKV,MKV->reg[DS]+MKV->reg[opA])]=valor;
            break;
    }
}  

void MOV(TipoMKV *MKV,int opA, short TopA, int opB, short TopB){ //hacer bien 
   
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