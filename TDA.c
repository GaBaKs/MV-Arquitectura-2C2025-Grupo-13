#define MAX 10

typedef struct{
    unsigned int Reg[REGISTROS];           // Cantidad de registros
    unsigned char Mem[MEMORIA];            // Cantidad de memoria
    unsigned char codigo_error;            // Codigo de error
    unsigned short tabla_Seg[4];           
} TipoMKV;



typedef struct              //se usa para el disassembler
{
    char nmemonico[MAX];
    void (*ptr_2)(char op1,char op2);
    void (*ptr_1)(char op1);
    void (*ptr_0)();
}mnemo;



typedef mnemo[32] Tmnemo;

