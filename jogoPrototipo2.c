#include "raylib.h"
#include <string.h>
#include <time.h>
#define LARGURA 600
#define ALTURA 400
#define LADO 20
#define MAX_COLUNAS 30
#define MAX_LINHAS 20

typedef struct posicao{
    int x, y, desY, desX;
}POSICAO;

/*void desenhaMapa(char **mapa){
    int i, j;
        for(i=0; i<MAX_COLUNAS; i++){
                for(j=0; j<MAX_LINHAS; j++){
                    if(mapa[j][i]=='#'){
                        DrawRectangle(i*20, j*20, LADO, LADO, BLUE);
                    }
                    if(mapa[j][i]=='S'){
                        DrawRectangle(i*20, j*20, LADO, LADO, GRAY);
                    }
                    if(mapa[j][i]=='O'){
                        DrawRectangle(i*20, j*20, LADO, LADO, YELLOW);
                    }
                    if(mapa[j][i]=='A'){
                        DrawRectangle(i*20, j*20, LADO, LADO, GREEN);
                    }
                    if(mapa[j][i]=='T'){
                        DrawRectangle(i*20, j*20, LADO, LADO, BROWN);
                    }

                }
            }
}
*/

int podeMover(POSICAO entidade, int largura, int altura, char **mapa){
    int pode = 1;
    int posFimX = entidade.x + entidade.desX*20;
    int posFimY = entidade.y + entidade.desY*20;
    if (mapa[posFimX/20][posFimY/20] != ' ')
        pode = 0;
    return pode;
}

void move(POSICAO entidade, POSICAO *pEntidade){
    pEntidade->x += entidade.desX*LADO;
    pEntidade->y += entidade.desY*LADO;
}

int main(){

    POSICAO jogador;

    int i,j;
    char mapa[MAX_LINHAS][MAX_COLUNAS] = {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
                                          '#','J',' ',' ',' ',' ',' ','S','#',' ','T',' ',' ',' ','S','S','S','S','O','O','S','S',' ','#',' ','S','S','S','S','#',
                                          '#',' ',' ',' ','A',' ',' ',' ','#',' ',' ',' ',' ',' ','S','S','S','O','O','S','S','S',' ','#',' ','S','S','S','S','#',
                                          '#',' ','O',' ',' ',' ',' ',' ','#','#','#','#','#',' ',' ',' ','S','S','O','O','S','S',' ',' ',' ','S','E','O','O','#',
                                          '#','#','#','#','#','#','#',' ','S','E','S','S','S',' ',' ',' ','S','S','S','S','S','S',' ',' ',' ','S','S','S','S','#',
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
        jogador.desX = 0;
        jogador.desY = 0;
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
        DrawRectangle(jogador.x, jogador.y, LADO, LADO, RED);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
