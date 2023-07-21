#include "raylib.h"
#include <string.h>
#include <time.h>
#define LARGURA 800
#define ALTURA 800
#define LADO 20

int podeMover(int x, int y, int desX, int desY, int largura, int altura){
    int pode = 1;
    int posFimX = x + desX*20;
    int posFimY = y + desY*20;
    if (posFimX < 0 || posFimY < 0 || posFimX > largura-20 || posFimY > altura-20)
        pode = 0;
return pode;
}

void move(int desX, int desY, int *x, int *y){
    if (podeMover(*x, *y, desX, desY, LARGURA, ALTURA)==1){
        *x += desX*LADO;
        *y += desY*LADO;
    }
}

int main(void){

    srand(time(NULL));

    int x = ((rand()%40)*LADO);
    int y = ((rand()%40)*LADO);
    int desX, desY;

    InitWindow(LARGURA, ALTURA, ":)");
    SetTargetFPS(20);

    while (!WindowShouldClose()){

        if (IsKeyPressed(KEY_RIGHT)){
            desX = 1;
            desY = 0;
        }
        if (IsKeyPressed(KEY_LEFT)){
            desX = -1;
            desY = 0;
        }
        if (IsKeyPressed(KEY_UP)){
            desX = 0;
            desY = -1;
        }
        if (IsKeyPressed(KEY_DOWN)){
            desX = 0;
            desY = 1;
        }
        move(desX, desY, &x, &y);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(x, y, LADO, LADO, RED);
        EndDrawing();
    }
    CloseWindow();
}
