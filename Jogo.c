#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define LARGURA 900
#define ALTURA 600
#define LADO 30
#define MAX_COLUNAS 30
#define MAX_LINHAS 20
#define M_MAX 200
#define VELOCIDADE 10
#define LADOTIRO 10

//Comando pra eu poder compilar os códigos no linux, ignore: cc jogoPrototipo4.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
typedef struct posicao{
    int x, y, desY, desX, sentido;
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

void moveTiro(POSICAO *pPosicao){
    pPosicao->x += pPosicao->desX*LADO/2;
    pPosicao->y += pPosicao->desY*LADO/2;
}

int main(){
    POSICAO tiro;
    JOGADOR jogador;
    TOUPEIRA toupeiras[M_MAX];
    jogador.posicao.desX = 0;
    jogador.posicao.desY = 0;
    int tempo = 0;
    int toupeira_n = 0;
    int i,j;
    char mapa[MAX_LINHAS][MAX_COLUNAS] = {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
                                          '#','J',' ',' ',' ',' ',' ','S','#',' ','T',' ',' ',' ','S','S','S','S','O','O','S','S',' ','#',' ','S','S','S','S','#',
                                          '#',' ',' ',' ','A',' ',' ',' ','#',' ',' ',' ',' ',' ','S','S','S','O','O','S','S','S',' ','#',' ','S','S','S','S','#',
                                          '#',' ','O',' ',' ',' ',' ',' ','#','#','#','#','#',' ',' ',' ','S','S','O','O','S','S',' ',' ',' ','S','E','O','O','#',
                                          '#','#','#','#','#','#','#',' ',' ',' ',' ','S','S',' ',' ',' ','S','S','S','S','S','S',' ',' ',' ','S','S','S','S','#',
                                          '#','E','O','O','S','S','#','#','#','#','#','#','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ','S','S',' ',' ','#',
                                          '#','S','S','S','S','S','S','S',' ',' ',' ',' ',' ',' ',' ',' ','S','S','S','S','S','S',' ','#',' ',' ','A',' ',' ','#',
                                          '#',' ',' ','T',' ',' ',' ',' ',' ',' ',' ','A',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ','T','S','S',' ','#',
                                          '#','S','O','S','S','S','O','S',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ',' ','S','S','S',' ','#',
                                          '#','S','S','S','O','O','O','S','S','S',' ',' ',' ',' ','#','#','#','#','#','#','#','#',' ',' ',' ','S','O','O',' ','#',
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
                        jogador.posicao.x = j*LADO;
                        jogador.posicao.y = i*LADO;
                    }
                    if(mapa[i][j]=='T'){
                        toupeiras[toupeira_n].posicao.x = j*LADO;
                        toupeiras[toupeira_n].posicao.y = i*LADO;
                        toupeiras[toupeira_n].contPassos = 0;
                        toupeiraRand(&toupeiras[toupeira_n].posicao);
                        toupeira_n++;
                    }
                }
            }
    InitWindow(LARGURA, ALTURA, ":)");
    SetTargetFPS(60);

    while (!WindowShouldClose()){

        if (IsKeyPressed(KEY_UP)){
            jogador.posicao.desX = 0;
            jogador.posicao.desY = -1;
            jogador.posicao.sentido = 1;
        }
        if (IsKeyPressed(KEY_RIGHT)){
            jogador.posicao.desX = 1;
            jogador.posicao.desY = 0;
            jogador.posicao.sentido = 2;
        }
        if (IsKeyPressed(KEY_DOWN)){
            jogador.posicao.desX = 0;
            jogador.posicao.desY = 1;
            jogador.posicao.sentido = 3;
        }
        if (IsKeyPressed(KEY_LEFT)){
            jogador.posicao.desX = -1;
            jogador.posicao.desY = 0;
            jogador.posicao.sentido = 4;
        }

        if (IsKeyPressed(KEY_G)){
            switch (jogador.posicao.sentido){
                case 1:
                    tiro.x = jogador.posicao.x+LADO/2-LADOTIRO/2;
                    tiro.y = jogador.posicao.y;
                    tiro.desX = 0;
                    tiro.desY = -1;
                    break;
                case 2:
                    tiro.x = jogador.posicao.x+LADO;
                    tiro.y = jogador.posicao.y+(LADO/2)-(LADOTIRO/2);
                    tiro.desX = 1;
                    tiro.desY = 0;
                    break;
                case 3:
                    tiro.x = jogador.posicao.x+(LADO/2)-(LADOTIRO/2);
                    tiro.y = jogador.posicao.y+LADO;
                    tiro.desX = 0;
                    tiro.desY = 1;
                    break;
                case 4:
                    tiro.x = jogador.posicao.x-LADOTIRO;
                    tiro.y = jogador.posicao.y+(LADO/2)-(LADOTIRO/2);
                    tiro.desX = -1;
                    tiro.desY = 0;
                    break;
            }
        }
        moveTiro(&tiro);
        if(podeMoverJ(jogador.posicao, LARGURA, ALTURA, mapa) == 1){
            move(&jogador.posicao);
            jogador.posicao.desX = 0;
            jogador.posicao.desY = 0;
        }

        tempo++;
        for(i=0; i<toupeira_n; i++){
            if (tempo == VELOCIDADE){
            moveToupeira(&toupeiras[i], podeMoverT(toupeiras[i].posicao, LARGURA, ALTURA, mapa));
            }
        }
        if (tempo == VELOCIDADE) tempo = 0;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        desenhaMapa(mapa);
        DrawRectangle(jogador.posicao.x, jogador.posicao.y, LADO, LADO, RED);
        DrawRectangle(tiro.x, tiro.y, LADOTIRO, LADOTIRO, SKYBLUE);
        for (i=0; i<toupeira_n; i++){
        	DrawRectangle(toupeiras[i].posicao.x, toupeiras[i].posicao.y, LADO, LADO, BROWN);
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
