#include <stdio.h>
#define MAX_MAP_NAME 12
#define MAX_MAPS 100
int main()
{
    char mapName[MAX_MAP_NAME];
    int mapNumber;

    for(mapNumber=0;mapNumber<MAX_MAPS;mapNumber++){
        sprintf(mapName,"mapa%d.txt",mapNumber);
        printf("Nome do arquivo do mapa:%s\n",mapName);
    }
    return 0;
}
