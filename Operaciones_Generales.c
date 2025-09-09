#include <stdio.h>
#include <stdlib.h>
#include <string.h>

generavectornmemo(Tmnemo Vmnemo)
{
    Vmnemo[LAR].mnemonico="LAR"; //0
    Vmnemo[MAR].mnemonico="MAR"; //1
    Vmnemo[MBR].mnemonico="MBR"; //2
    Vmnemo[IP].mnemonico="IP";   //3
    Vmnemo[OPC].mnemonico="OPC"; //4
    Vmnemo[OP1].mnemonico="OP1"; //5
    Vmnemo[OP2].mnemonico="OP2"; //6
    Vmnemo[EAX].mnemonico="EAX"; //10
    Vmnemo[EBX].mnemonico="EBX"; //11
    Vmnemo[ECX].mnemonico="ECX"; //12
    Vmnemo[EDX].mnemonico="EDX"; //13
    Vmnemo[EEX].mnemonico="EEX"; //14
    Vmnemo[EFX].mnemonico="EFX"; //15
    Vmnemo[AC].mnemonico="AC";   //16
    Vmnemo[CC].mnemonico="CC";   //17
    Vmnemo[CS].mnemonico="CS";   //26
    Vmnemo[DS].mnemonico="DS";   //27
}