#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#define MAX_COLUNAS 30
#define MAX_LINHAS 20
#define LADO 20


typedef struct POSICAO{
    int x, y;
}posicao;

int leMapa(char nomeArq[], char mapa[][MAX_COLUNAS], posicao *jogador){
    FILE *arq;
    int i, j;

    if (!(arq = fopen(nomeArq, "r"))){
        printf("Erro na abertura do mapa!");
    }else{
        while(!(feof(arq))){
            for (i = 0; i<MAX_LINHAS; i++){
                for(j = 0; j<MAX_COLUNAS; j++){
                    mapa[i][j] = getc(arq);
                    if (mapa[i][j] == 'J'){
                        jogador->x = j;
                        jogador->y = i;
                    }
                }
                getc(arq);
            }
        }
        return 1;
    }
    fclose(arq);
    return 0;
}

int escrevePosicao(char nomeArq[], posicao posicao){
    FILE *arq;
    int retorno = 0;

    if(!(arq = fopen(nomeArq, "w"))){
        printf("Erro na criacao de arquivo posicao!");
    }else{
        if(fprintf(arq, "%d,%d", posicao.x, posicao.y) < 0){
            printf("Erro na escrita!");
            retorno = 1;
        }
    }
    fclose(arq);
    return retorno;
}



int main(){
    posicao jogador;
    char mapa[MAX_LINHAS][MAX_COLUNAS];
    int i, j;

    if (leMapa("mapa1.txt", mapa, &jogador) == 1){
        InitWindow(600, 400, "B)");
        SetTargetFPS(60);
        while(!WindowShouldClose()){
            BeginDrawing();
            ClearBackground(RAYWHITE);
            for(i=0; i<MAX_LINHAS; i++){
                for(j=0; j<MAX_COLUNAS; j++){
                     if(mapa[i][j]=='#'){
                        DrawRectangle(j*20, i*20, LADO, LADO, BLUE);
                    }
                    if(mapa[i][j]=='S'){
                        DrawRectangle(j*20, i*20, LADO, LADO, GRAY);
                    }
                    if(mapa[i][j]=='O'){
                        DrawRectangle(j*20, i*20, LADO, LADO, YELLOW);
                    }
                    if(mapa[i][j]=='A'){
                        DrawRectangle(j*20, i*20, LADO, LADO, GREEN);
                    }
                    if(mapa[i][j]=='T'){
                        DrawRectangle(j*20, i*20, LADO, LADO, BROWN);
                    }
                }
            }
            DrawRectangle(jogador.x*20, jogador.y*20, LADO, LADO, RED);
            EndDrawing();
        }
        escrevePosicao("pos.txt", jogador);
    }
    return 0;
}
