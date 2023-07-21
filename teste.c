#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define LARGURA 800
#define ALTURA 800
#define LADO 20
#define VALOR 2

int main(void){
    int x=((ALTURA/2)-LADO), y=((LARGURA/2)-LADO), i, j, obs;
    int tela[40][40];

    srand(time(NULL));
    for(j=0; j<40; j++){
        for(i=0; i<40; i++){
            obs = rand() % VALOR;
            tela[i][j] = obs;
        }
    }
    tela[x/20][y/20] = 0; //onde o bloco vermelho começa, nao tem obstaculo
    InitWindow(LARGURA, ALTURA, ":)");
    SetTargetFPS(60);

    while (!WindowShouldClose()&&!(((x>(LARGURA-LADO))||(x<0))||((y>(ALTURA-LADO))||(y<0)))){

        if (IsKeyPressed(KEY_RIGHT)){
            if((tela[(x/20)+1][y/20])==0)
                x = x+20;
        }
        if (IsKeyPressed(KEY_LEFT)){
            if((tela[(x/20)-1][y/20])==0)
                x = x-20;
        }
        if (IsKeyPressed(KEY_UP)){
            if((tela[(x/20)][(y/20)-1])==0)
                y = y-20;
        }
        if (IsKeyPressed(KEY_DOWN)){
            if((tela[(x/20)][(y/20)+1])==0)
                y = y+20;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for(j=0; j<40; j++){
            for(i=0; i<40; i++){
                if(tela[i][j]==1){
                    DrawRectangle(i*20, j*20, LADO, LADO, BLUE);
                }
            }
        }
        DrawRectangle(x, y, LADO, LADO, RED);
        EndDrawing();
    }
    CloseWindow();
}
