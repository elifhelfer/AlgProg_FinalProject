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
#define LADOPEQUENO 10
#define O_MAX 600

//Comando pra eu poder compilar os códigos no linux, ignore: cc jogoPrototipo4.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
typedef struct posicao{
    //Struct que possui informações referentes a posição, deslocamento, etc
    int x, y, desY, desX, sentido, visivel;
}POSICAO;

typedef struct toupeira{
    //Informações sobre as toupeiras
    POSICAO posicao;
    int contPassos;
}TOUPEIRA;

typedef struct jogador{
    //Informações referentes ao jogador
    POSICAO posicao;
    int vidas;
    int powerup;
}JOGADOR;

void desenhaMapa(char mapa[MAX_LINHAS][MAX_COLUNAS]){
    //Função que lê a matriz 'mapa', que contem os caracteres que descrevem cada objeto
    int i, j;
        for(i=0; i<MAX_COLUNAS; i++){
                for(j=0; j<MAX_LINHAS; j++){
                    if(mapa[j][i]=='#'){
                        DrawRectangle(i*LADO, j*LADO, LADO, LADO, BLUE); //'LADO' é a aresta padrão dos quadrados
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
                    if(mapa[j][i]=='E'){
                        DrawRectangle((i*LADO)+10, (j*LADO)+10, LADOPEQUENO, LADOPEQUENO, PURPLE); //Posições x e y diferentes, para centralizar o quadrado menor
                    }
                }
            }
}

void toupeiraRand(POSICAO *pPosicao){
    //Função que escolhe um valor aleatorio entre -1, 0 e 1 para atribuir ao deslocamento de cada toupeira
    pPosicao->desX = GetRandomValue(-1,1);
    pPosicao->desY = GetRandomValue(-1,1);
}

int leMapa(char nomeArq[], char mapa[][MAX_COLUNAS], JOGADOR *pJogador, TOUPEIRA toupeiras[], int *toupeira_n){
    //Lê o mapa de um arquivo txt, preenche a matriz 'mapa' e retorna por referencia informações importantes acerca da posição de elementos do jogo no mapa, além de inicializar algumas variaveis.
    //Função de inicialização do jogo, basicamente.
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
                        pJogador->posicao.x = j*LADO;
                        pJogador->posicao.y = i*LADO;
                        pJogador->vidas = 3;
                        pJogador->powerup = 0;
                    }
                    else if (mapa[i][j] == 'T'){
                        toupeiras[*toupeira_n].posicao.x = j*LADO;
                        toupeiras[*toupeira_n].posicao.y = i*LADO;
                        toupeiras[*toupeira_n].contPassos = 0;
                        toupeiras[*toupeira_n].posicao.visivel = 1;
                        toupeiraRand(&toupeiras[*toupeira_n].posicao);
                        *toupeira_n += 1;
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

int podeMoverJ(JOGADOR *jogador, int largura, int altura, char mapa[MAX_LINHAS][MAX_COLUNAS]){
    int pode = 1;
    int posFimX = jogador->posicao.x/LADO + jogador->posicao.desX;
    int posFimY = jogador->posicao.y/LADO + jogador->posicao.desY;
    if ((mapa[posFimY][posFimX] == '#') || (mapa[posFimY][posFimX] == 'S')){
        pode = 0;
    }
    if (mapa[posFimY][posFimX] == 'E'){
        jogador->powerup = 1;
        printf("POWERUPINICIO\n");
        mapa[posFimY][posFimX] = ' ';
    }
    return pode;
}

int podeMoverT(POSICAO posicao, int largura, int altura, char mapa[MAX_LINHAS][MAX_COLUNAS]){
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
    pPosicao->x += pPosicao->desX*LADO;
    pPosicao->y += pPosicao->desY*LADO;
}

void sentidoTiro(POSICAO *tiro, POSICAO jogador){
    switch (jogador.sentido){
                case 1:
                    tiro->desX = 0;
                    tiro->desY = -1;
                    break;
                case 2:
                    tiro->desX = 1;
                    tiro->desY = 0;
                    break;
                case 3:
                    tiro->desX = 0;
                    tiro->desY = 1;
                    break;
                case 4:
                    tiro->desX = -1;
                    tiro->desY = 0;
                    break;
    }
}

int tiroColisao(POSICAO tiro, char mapa[MAX_LINHAS][MAX_COLUNAS], TOUPEIRA toupeiras[], int toupeira_n){
    int i;

    if (mapa[tiro.y/LADO][tiro.x/LADO] == '#'){
        return 1;
    }else if(mapa[tiro.y/LADO][tiro.x/LADO] == 'S'){
        mapa[tiro.y/LADO][tiro.x/LADO] = ' ';
        return 1;
    }else for (i=0; i<toupeira_n; i++){
        if(tiro.y == toupeiras[i].posicao.y && tiro.x == toupeiras[i].posicao.x){
            toupeiras[i].posicao.visivel = 0;
            return 1;
            }
    }return 0;
}

void pressionaTecla(JOGADOR *jogador, POSICAO *tiro){

    if (IsKeyPressed(KEY_UP)){
            jogador->posicao.desX = 0;
            jogador->posicao.desY = -1;
            jogador->posicao.sentido = 1;
        }
        if (IsKeyPressed(KEY_RIGHT)){
            jogador->posicao.desX = 1;
            jogador->posicao.desY = 0;
            jogador->posicao.sentido = 2;
        }
        if (IsKeyPressed(KEY_DOWN)){
            jogador->posicao.desX = 0;
            jogador->posicao.desY = 1;
            jogador->posicao.sentido = 3;
        }
        if (IsKeyPressed(KEY_LEFT)){
            jogador->posicao.desX = -1;
            jogador->posicao.desY = 0;
            jogador->posicao.sentido = 4;
        }
        if (IsKeyPressed(KEY_G)){
            if (tiro->visivel == 0){
                sentidoTiro(tiro, jogador->posicao);
                tiro->x = jogador->posicao.x;
                tiro->y = jogador->posicao.y;
                tiro->visivel = 1;
            }
        }
}

int main(){
    POSICAO tiro;
    JOGADOR jogador;
    TOUPEIRA toupeiras[M_MAX];
    int tempoToupeira = 0;
    int tempoPowerup = 0;
    int toupeira_n = 0;
    int i,j;
    char mapa[MAX_LINHAS][MAX_COLUNAS];

    tiro.visivel = 0;
    jogador.posicao.desX = 0;
    jogador.posicao.desY = 0;
    jogador.posicao.sentido = 2;

    leMapa("mapa1.txt", mapa, &jogador, toupeiras, &toupeira_n);

    InitWindow(LARGURA, ALTURA, ":)");
    SetTargetFPS(60);

    while (!WindowShouldClose()){

        pressionaTecla(&jogador, &tiro);

        //Mover:
        if(podeMoverJ(&jogador, LARGURA, ALTURA, mapa) == 1){
            move(&jogador.posicao);
        }

        jogador.posicao.desX = 0;
        jogador.posicao.desY = 0;

        for(i=0; i<toupeira_n; i++){
            if (tempoToupeira == VELOCIDADE){
                if (jogador.posicao.y == toupeiras[i].posicao.y && jogador.posicao.x == toupeiras[i].posicao.x){
                    jogador.vidas--;
                    printf("vidas: %d\n", jogador.vidas);
                }
                moveToupeira(&toupeiras[i], podeMoverT(toupeiras[i].posicao, LARGURA, ALTURA, mapa));
            }
        }

        if(tiro.visivel == 1){
            move(&tiro);
            if (tiroColisao(tiro, mapa, toupeiras, toupeira_n) == 1){
                tiro.visivel = 0;
            }
        }

        if (jogador.powerup == 1){
            tempoPowerup++;
        }

        if(tempoPowerup == 180){
            jogador.powerup = 0;
            tempoPowerup = 0;
            printf("FIMPOWERUP\n");
        }

        if (tempoToupeira == VELOCIDADE) tempoToupeira = 0;
        tempoToupeira++;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        desenhaMapa(mapa);
        DrawRectangle(jogador.posicao.x, jogador.posicao.y, LADO, LADO, RED);
        if (tiro.visivel == 1){
            DrawRectangle(tiro.x, tiro.y, LADOPEQUENO, LADOPEQUENO, SKYBLUE);
        }
        for (i=0; i<toupeira_n; i++){
            if (toupeiras[i].posicao.visivel == 1){
                DrawRectangle(toupeiras[i].posicao.x, toupeiras[i].posicao.y, LADO, LADO, BROWN);
            }
        }
        EndDrawing();

    }
    CloseWindow();

    return 0;
}
