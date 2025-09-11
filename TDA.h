#define MAX 10
#define REGISTROS 32
#define MEMORIA 16384
#define 
typedef struct{
    unsigned int reg[REGISTROS];           // Cantidad de registros
    unsigned char mem[MEMORIA];            // Cantidad de memoria
    unsigned char codigo_error;            // Codigo de error
    unsigned short tabla_seg[4];           
} TipoMKV;



/*
typedef struct              //se usa para el disassembler
{
    char nmemonico[MAX];
    void (*ptr_2)(char ,char );
    void (*ptr_1)(char );
    void (*ptr_0)();
}mnemo;



typedef mnemo Tmnemo[32];
posiblemente todo esto se borre, lo deje por las dudas por si lo que 
voy a hacer no funciona o no les gusta

*/