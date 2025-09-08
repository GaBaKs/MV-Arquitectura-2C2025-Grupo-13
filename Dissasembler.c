#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dissasembler.h" //Vacio, los prototipos


// hacer 3 typedef separados



void imprimeMnemonico(unsigned char cod){ 

    switch(orden){
        case 0x00: 
            printf("SYS ");
            break;
        case 0x01: 
            printf("JMP ");
            break;
        case 0x02: 
            printf("JZ ");
            break;
        case 0x03:
            printf("JP ");
            break;
        case 0x04: 
            printf("JN ");
            break;
        case 0x05: 
            printf("JNZ ");
            break;
        case 0x06: 
            printf("JNP ");
            break;
        case 0x07: 
            printf("JNN ");
            break;
        case 0x08:
            printf("NOT ");
            break;
        case 0x10: 
            printf("MOV ");
            break;
        case 0x11: 
            printf("ADD ");
            break;
        case 0x12: 
            printf("SUB ");
            break;
        case 0x13: 
            printf("SWAP ");
            break; 
        case 0x14: 
            printf("MUL ");
            break;
        case 0x15: 
            printf("DIV ");
            break;
        case 0x16: 
            printf("CMP ");
            break;
        case 0x17: 
            printf("SHL ");
            break;
        case 0x18: 
            printf("SHR ");
            break;
        case 0x19: 
            printf("AND ");
            break;
        case 0x1A: 
            printf("OR ");
            break;
        case 0x1B: 
            printf("XOR ");
            break;
        case 0x1C: 
            printf("SWAP ");
            break;    
        case 0x1D: 
            printf("LDL ");
            break;
        case 0x1E: 
            printf("LDH ");
            break;
        case 0x1F: 
            printf("RND ");
            break;
        case 0x0F:
            printf("STOP ");
            break;
    }
}

void imprimeTAB(){
    printf("\t \t | ");  
}


