#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define LARGURA 600
#define ALTURA 400
#define LADO 20
#define MAX_COLUNAS 30
#define MAX_LINHAS 20
#define M_MAX 8

//Comando pra eu poder compilar os códigos no linux, ignore: cc jogoPrototipo4.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

typedef struct posicao{
    int x, y, desY, desX;
}POSICAO;

typedef struct toupeira{
    POSICAO posicao;
    int contPassos;
}TOUPEIRA;

typedef struct jogador{
    POSICAO posicao;
}JOGADOR;

void desenhaMapa(char mapa[][MAX_COLUNAS]){
    int i, j;
        for(i=0; i<MAX_COLUNAS; i++){
                for(j=0; j<MAX_LINHAS; j++){
                    if(mapa[j][i]=='#'){
                        DrawRectangle(i*LADO, j*LADO, LADO, LADO, BLUE);
                    }
                    if(mapa[j][i]=='S'){
                        DrawRectangle(i*LADO, j*LADO, LADO, LADO, GRAY);
                    }
                    if(mapa[j][i]=='O'){
                        DrawRectangle(i*LADO, j*LADO, LADO, LADO, YELLOW);
                    }
                    if(mapa[j][i]=='A'){
                        DrawRectangle(i*LADO, j*LADO, LADO, LADO, GREEN);
                    }
                }
            }
}

int podeMoverJ(POSICAO posicao, int largura, int altura, char mapa[][MAX_COLUNAS]){
    int pode = 1;
    int posFimX = posicao.x/LADO + posicao.desX;
    int posFimY = posicao.y/LADO + posicao.desY;
    if ((mapa[posFimY][posFimX] == '#') || (mapa[posFimY][posFimX] == 'S')){
        pode = 0;
    }
    return pode;
}

int podeMoverT(POSICAO posicao, int largura, int altura, char mapa[][MAX_COLUNAS]){
    int pode = 1;
    int posFimX = posicao.x/LADO + posicao.desX;
    int posFimY = posicao.y/LADO + posicao.desY;
    if (mapa[posFimY][posFimX] == '#'){
        pode = 0;
    }
    return pode;
}

void move(POSICAO *pPosicao){
    pPosicao->x += pPosicao->desX*LADO;
    pPosicao->y += pPosicao->desY*LADO;
}

void toupeiraRand(POSICAO *pPosicao){
    pPosicao->desX = GetRandomValue(-1,1);
    pPosicao->desY = GetRandomValue(-1,1);
}

void moveToupeira(TOUPEIRA *pToupeira, int pode){
    if(pToupeira->contPassos < 5 && pode){
        move(&pToupeira->posicao);
        pToupeira->contPassos++;
    }else{
        pToupeira->contPassos = 0;
        do{
            toupeiraRand(&pToupeira->posicao);
        }while (pToupeira->posicao.desX == 0 && pToupeira->posicao.desY == 0);
    }
}

int main(){
    JOGADOR jogador;
    TOUPEIRA toupeiras[M_MAX];
    jogador.posicao.desX = 0;
    jogador.posicao.desY = 0;
    int toupeira_n = 0;
    int i,j;
    char mapa[MAX_LINHAS][MAX_COLUNAS] = {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
                                          '#','J',' ',' ',' ',' ',' ','S','#',' ','T',' ',' ',' ','S','S','S','S','O','O','S','S',' ','#',' ','S','S','S','S','#',
                                          '#',' ',' ',' ','A',' ',' ',' ','#',' ',' ',' ',' ',' ','S','S','S','O','O','S','S','S',' ','#',' ','S','S','S','S','#',
                                          '#',' ','O',' ',' ',' ',' ',' ','#','#','#','#','#',' ',' ',' ','S','S','O','O','S','S',' ',' ',' ','S','E','O','O','#',
                                          '#','#','#','#','#','#','#',' ',' ',' ',' ','S','S',' ',' ',' ','S','S','S','S','S','S',' ',' ',' ','S','S','S','S','#',
                                          '#','E','O','O','S','S','#','#','#','#','#','#','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ','S','S',' ',' ','#',
                                          '#','S','S','S','S','S','S','S',' ',' ',' ',' ',' ',' ',' ',' ','S','S','S','S','S','S',' ','#',' ',' ','A',' ',' ','#',
                                          '#',' ',' ','T',' ',' ',' ',' ',' ',' ',' ','A',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ','T','S','S',' ','#',
                                          '#','S','O','S','S','S','O','S',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ','S','S','S',' ','#',
                                          '#','S','S','S','O','O','O','S','S','S',' ',' ',' ',' ','#','#','#','#','#','#','#',' ',' ',' ',' ','S','O','O',' ','#',
                                          '#','O','S','S','S','S','S','S','S','S','S',' ',' ',' ',' ',' ',' ',' ',' ',' ','T',' ',' ',' ',' ',' ',' ',' ',' ','#',
                                          '#','#','#','#','#','#','#','#','#','#','#','#','#','A',' ','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
                                          '#','T','S','S','S','S','S','S','S',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','S','S','S','E','#',
                                          '#','O','S','S','S','S','S','S',' ',' ',' ','#','S','S','S','S','S','#',' ',' ',' ','T',' ',' ',' ','S','O','O','S','#',
                                          '#','S','S','O','O','O','S','S',' ',' ',' ','#','S','S','O','O','S','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','S','S','#',
                                          '#',' ',' ','#','#','#','#','#','#','#','#','#','#','O','O','#','#','#','#','#','#','#','#','#','#','#','#','S','S','#',
                                          '#',' ',' ',' ',' ',' ','T',' ',' ','A',' ','S','S','S','S','S','S','S',' ',' ','T',' ',' ',' ','#',' ',' ',' ',' ','#',
                                          '#',' ','S','S','S','S','S','S','S',' ',' ',' ','S','S','S','S','S',' ',' ',' ',' ','#',' ',' ','#',' ','#',' ',' ','#',
                                          '#',' ','S','O','O','O','S','S','S',' ',' ',' ','S','S','E','S','S',' ',' ',' ',' ','#',' ',' ','A',' ','#',' ',' ','#',
                                          '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'};

    for(i=0; i<MAX_LINHAS; i++){
                for(j=0; j<MAX_COLUNAS; j++){
                    if(mapa[i][j]=='J'){
                        jogador.posicao.x = j*20;
                        jogador.posicao.y = i*20;
                    }
                    if(mapa[i][j]=='T'){
                        toupeiras[toupeira_n].posicao.x = j*20;
                        toupeiras[toupeira_n].posicao.y = i*20;
                        toupeiras[toupeira_n].contPassos = 0;
                        toupeiraRand(&toupeiras[toupeira_n].posicao);
                        toupeira_n++;
                    }
                }
            }
    InitWindow(LARGURA, ALTURA, ":)");
    SetTargetFPS(60);

    while (!WindowShouldClose()){

        if (IsKeyPressed(KEY_RIGHT)){
            jogador.posicao.desX = 1;
            jogador.posicao.desY = 0;
        }
        if (IsKeyPressed(KEY_LEFT)){
            jogador.posicao.desX = -1;
            jogador.posicao.desY = 0;
        }
        if (IsKeyPressed(KEY_UP)){
            jogador.posicao.desX = 0;
            jogador.posicao.desY = -1;
        }
        if (IsKeyPressed(KEY_DOWN)){
            jogador.posicao.desX = 0;
            jogador.posicao.desY = 1;
        }
        if(podeMoverJ(jogador.posicao, 600, 400, mapa) == 1){
            move(&jogador.posicao);
            jogador.posicao.desX = 0;
            jogador.posicao.desY = 0;
        }
        for(i=0; i<toupeira_n; i++){
            moveToupeira(&toupeiras[i], podeMoverT(toupeiras[i].posicao, 600, 400, mapa));
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        desenhaMapa(mapa);
        DrawRectangle(jogador.posicao.x, jogador.posicao.y, LADO, LADO, RED);
        for (i=0; i<toupeira_n; i++){
        	DrawRectangle(toupeiras[i].posicao.x, toupeiras[i].posicao.y, LADO, LADO, BROWN);
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
