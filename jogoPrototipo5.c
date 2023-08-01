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
    char tipo; //T se for uma toupeira, J se for o jogador
}POSICAO;

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

int podeMover(POSICAO entidade, int largura, int altura, char mapa[][MAX_COLUNAS]){
    int pode = 0;
    int posFimX = entidade.x/LADO + entidade.desX;
    int posFimY = entidade.y/LADO + entidade.desY;
    if ((mapa[posFimY][posFimX] == ' ') || (mapa[posFimY][posFimX] == 'J') || (mapa[posFimY][posFimX] == 'T')){
        pode = 1;
    }
    if (entidade.tipo == 'T' && mapa[posFimY][posFimX] == 'S'){
    pode = 1;
    }
    return pode;
}

void move(POSICAO entidade, POSICAO *pEntidade){
    pEntidade->x += entidade.desX*LADO;
    pEntidade->y += entidade.desY*LADO;
}

int main(){
    POSICAO jogador;
    POSICAO toupeiras[M_MAX];
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
                        jogador.x = j*20;
                        jogador.y = i*20;
                        jogador.tipo = 'J';
                    }
                    if(mapa[i][j]=='T'){
                        toupeiras[toupeira_n].x = j*20;
                        toupeiras[toupeira_n].y = i*20;
                        toupeiras[toupeira_n].tipo = 'T';
                        toupeira_n += 1;
                    }
                }
            }
    InitWindow(LARGURA, ALTURA, ":)");
    SetTargetFPS(60);

    while (!WindowShouldClose()){

        if (IsKeyPressed(KEY_RIGHT)){
            jogador.desX = 1;
            jogador.desY = 0;
        }
        if (IsKeyPressed(KEY_LEFT)){
            jogador.desX = -1;
            jogador.desY = 0;
        }
        if (IsKeyPressed(KEY_UP)){
            jogador.desX = 0;
            jogador.desY = -1;
        }
        if (IsKeyPressed(KEY_DOWN)){
            jogador.desX = 0;
            jogador.desY = 1;
        }
        if(podeMover(jogador, 600, 400, mapa) == 1){
            move(jogador, &jogador);
        }
        
        for(i=0; i<M_MAX; i++){
        	//if (podeMover(toupeiras[i], 600, 400, mapa) == 1){
			toupeiras[i].desX = GetRandomValue(-1,1)*LADO;
	       		toupeiras[i].desY = GetRandomValue(-1,1)*LADO;
			move(toupeiras[i], &toupeiras[i]);
			toupeiras[i].desX = 0;
			toupeiras[i].desY = 0;
        	//}
        }
        printf("\n*********************************TOUPEIRAS DX = %d TOUPEIRAS DESY = %d *************************************\n", toupeiras[1].desX, toupeiras[1].desY);
        jogador.desX = 0;
        jogador.desY = 0;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        desenhaMapa(mapa);
        DrawRectangle(jogador.x, jogador.y, LADO, LADO, RED);
        for (i=0; i<toupeira_n; i++){
        	DrawRectangle(toupeiras[i].x, toupeiras[i].y, LADO, LADO, BROWN);
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
