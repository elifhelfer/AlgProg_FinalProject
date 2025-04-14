#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#define LARGURA 900
#define ALTURA 700
#define LADO 30
#define MAX_COLUNAS 30
#define MAX_LINHAS 20
#define M_MAX 200
#define LADOPEQUENO 10
#define O_MAX 600
#define MAXNOMEMAPA 20
#define MAXMAPAS 100

//Comando pra eu poder compilar os códigos no linux, ignore: cc jogoPrototipo4.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
typedef struct posicao{
    //Struct que possui informações referentes a posição, deslocamento, etc
    int x, y, desY, desX, sentido, visivel;
}POSICAO;

typedef struct toupeira{
    //Informações sobre as toupeiras
    POSICAO posicao;
    int contPassos;
    POSICAO posicaoInicial;
}TOUPEIRA;

typedef struct jogador{
    //Informações referentes ao jogador
    POSICAO posicao;
    int vidas;
    int powerup;
    int esmeraldas;
    int pontos;
    int gameOver;
    int ganhou;
    POSICAO posicaoInicial;
}JOGADOR;

void desenhaMapa(char mapa[MAX_LINHAS][MAX_COLUNAS], Texture2D toupeiraText, Texture2D pedraText, Texture2D terraText, Texture2D fundoText, Texture2D powerupText, Texture2D esmeraldaText, Texture2D ouroText){
    //Função que lê a matriz 'mapa', que contem os caracteres que descrevem cada objeto
    int i, j;
        for(i=0; i<MAX_COLUNAS; i++){
                for(j=0; j<MAX_LINHAS; j++){
                    if(mapa[j][i]=='#'){
                        DrawTexture(pedraText, i*LADO, j*LADO, WHITE);
                    }
                    if(mapa[j][i]=='S'){
                        DrawTexture(terraText, i*LADO, j*LADO, WHITE);
                    }
                    if(mapa[j][i]=='O'){
                        DrawTexture(ouroText, i*LADO, j*LADO, WHITE);
                    }
                    if(mapa[j][i]=='A'){
                        DrawTexture(esmeraldaText, i*LADO, j*LADO, WHITE);
                    }
                    if(mapa[j][i]=='E'){
                        DrawTexture(powerupText, i*LADO, j*LADO, WHITE);
                    }
                    if(mapa[j][i]==' ' || mapa[j][i]== 'T' || mapa[j][i]== 'J'){
                        DrawTexture(fundoText, i*LADO, j*LADO, WHITE);
                    }
                }
            }
}

void toupeiraRand(POSICAO *pPosicao){
    //Função que escolhe um valor aleatorio entre -1, 0 e 1 para atribuir ao deslocamento de cada toupeira
    pPosicao->desX = GetRandomValue(-1,1);
    pPosicao->desY = GetRandomValue(-1,1);
}

int leMapa(char nomeArq[], char mapa[][MAX_COLUNAS], JOGADOR *pJogador, TOUPEIRA toupeiras[], int *toupeira_n, int *esmeralda_n){
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
                    mapa[i][j] = getc(arq); //Adiciona o caractere lido à matriz
                    if (mapa[i][j] == 'J'){
                        pJogador->posicao.x = j*LADO; //Retorna a posição inicial do jogador por referencia
                        pJogador->posicao.y = i*LADO;
                        pJogador->posicaoInicial.x = j*LADO;
                        pJogador->posicaoInicial.y = i*LADO;
                        //inicializa itens, vida, pontuação, etc
                        pJogador->powerup = 0;
                        pJogador->esmeraldas = 0;
                        pJogador->posicao.desX = 0;
                        pJogador->posicao.desY = 0;
                        pJogador->posicao.sentido = 2;

                    }
                    else if (mapa[i][j] == 'T'){
                        toupeiras[*toupeira_n].posicao.x = j*LADO; //Passa posição e outras informações referentes a cada toupeira por referencia;
                        toupeiras[*toupeira_n].posicao.y = i*LADO;
                        toupeiras[*toupeira_n].posicaoInicial.x = j*LADO;
                        toupeiras[*toupeira_n].posicaoInicial.y = i*LADO;
                        toupeiras[*toupeira_n].contPassos = 0;
                        toupeiras[*toupeira_n].posicao.visivel = 1;
                        toupeiraRand(&toupeiras[*toupeira_n].posicao);
                        *toupeira_n += 1; //A cada toupeira nova, a variavel "toupeira_n", que armazena o numero total de toupeiras no mapa, aumenta
                    }
                    else if (mapa[i][j] == 'A'){
                        *esmeralda_n += 1;
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

int podeMoverJ(JOGADOR *jogador, int largura, int altura, char mapa[MAX_LINHAS][MAX_COLUNAS], int toupeiras_n, TOUPEIRA toupeiras[]){
    //As variaveis posFimX e posFimY guardam qual será a posição na matriz que o jogador ocupará, se ir para a direção que o jogador indicou
    //Com isso, ele testa se essa posição está ocupada por uma parede, ou espaço soterrado. Nesses casos, retorna 0
    //Se a posição for um powerup, ele atribui por referencia o valor "1" ao campo powerup do jogador
    //Se o jogador puder se mover para a posiçãodesejada, retorna 1
    int i;
    int pode = 1;
    int posFimX = jogador->posicao.x/LADO + jogador->posicao.desX;
    int posFimY = jogador->posicao.y/LADO + jogador->posicao.desY;
    if ((mapa[posFimY][posFimX] == '#') || (mapa[posFimY][posFimX] == 'S')){
        pode = 0;
    }
    if (mapa[posFimY][posFimX] == 'E'){
        jogador->powerup = 1;
        mapa[posFimY][posFimX] = ' '; //Faz com que o powerup suma do mapa ao ser coletado
    }
    if (mapa[posFimY][posFimX] == 'A'){
        jogador->esmeraldas += 1;
        jogador->pontos += 100;
        mapa[posFimY][posFimX] = ' ';
    }
    if (mapa[posFimY][posFimX] == 'O'){
        jogador->pontos +=50;
        mapa[posFimY][posFimX] = ' ';
    }
    return pode;
}

int podeMoverT(POSICAO posicao, int largura, int altura, char mapa[MAX_LINHAS][MAX_COLUNAS]){
    //Função similar à "podeMoverJ", porém adaptado para as toupeiras, que podem se mover nos blocos soterrados.
    int pode = 1;
    int posFimX = posicao.x/LADO + posicao.desX;
    int posFimY = posicao.y/LADO + posicao.desY;
    if (mapa[posFimY][posFimX] == '#'){
        pode = 0;
    }
    return pode;
}

void move(POSICAO *pPosicao){
    //Muda a posição 'x' atual do jogador ou toupeira (medida em pixels), com base no deslocamento recebido.
    //O deslocamento em X e em Y pode ser um numero de -1 à 1, sendo:
    //-1: deslocamento negativo, para trás
    //0: sem deslocamento
    //1: deslocamento positivo, para frente
    //Esse valor é recebido e multiplicado pelo tamanho do lado do bloco, a fim de o transformar em um número de pixels, que então pode ser adicionado às coordenadas atuais do jogador/toupeira
    pPosicao->x += pPosicao->desX*LADO;
    pPosicao->y += pPosicao->desY*LADO;
}

void moveToupeira(TOUPEIRA *pToupeira, int pode){
    //Essa função realiza o movimento aleatorio das toupeiras. O contador de passos de cada toupeira começa em 0, e se ele for menor que 5,
    //e a toupeira puder se mover (parametro recebido pela função 'podeMoverT', ela se move e aumenta seu contador de passos.
    //caso essa condição seja falsa, o contador reinicia e a função "toupeiraRand" atribui valores de deslocamentos aleatórios à toupeira, e com um teste de condição para evitar que esse valor seja 0 em y e x, evitando que ela permaneça parada.
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

void sentidoTiro(POSICAO *tiro, POSICAO jogador){
    //Essa função atribui a direção em que o tiro deverá se movimentar com base na ultima direção em que o jogador andou.
    //toda vez que o jogador se movimenta, o campo "sentido" atualiza, seguindo o seguinte padrão:
    //1: Cima
    //2: Direita
    //3: Baixo
    //4: Esquerda
    //Com base nesse valor, o deslocamento do tiro é atualizado.
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

int tiroColisao(POSICAO tiro, char mapa[MAX_LINHAS][MAX_COLUNAS], TOUPEIRA toupeiras[], int toupeira_n, int *pontos){
    //A função verifica a posição da matriz ocupada pelas coordenadas x e y do tiro. Como essas coordenadas estão em pixels, é necessário as dividir
    //pelo lado dos blocos para obtermos um valor condizente com a matriz.
    //A função retorna 1 se o tiro atravessou algo, e 0 se não atravessou nada.
    //Se ele acertou uma área soterrada, ela some
    //A função tambem testa a posicao de cada toupeira para determinar se o tiro acertou alguma. Se sim, ela desaparece e não é mais desenhada.
    int i;

    if (mapa[tiro.y/LADO][tiro.x/LADO] == '#'){
        return 1;
    }else if(mapa[tiro.y/LADO][tiro.x/LADO] == 'S'){
        mapa[tiro.y/LADO][tiro.x/LADO] = ' ';
        return 1;
    }else for (i=0; i<toupeira_n; i++){
        if(tiro.y == toupeiras[i].posicao.y && tiro.x == toupeiras[i].posicao.x && toupeiras[i].posicao.visivel == 1){
            toupeiras[i].posicao.visivel = 0;
            *pontos+=200;
            return 1;
            }
    }return 0;
}

char funcionaMenu(int *menu){

    if (IsKeyPressed(KEY_N)){ //novo jogo
        return 'N';
    }
    if (IsKeyPressed(KEY_C)){ //carregar jogo
        return 'C';
    }
    if (IsKeyPressed(KEY_S)){ //salvar jogo
        return 'S';
    }
    if (IsKeyPressed(KEY_Q)){ //sair
        return 'Q';
    }
    if (IsKeyPressed(KEY_V)) *menu = 0;
}

void pressionaTecla(JOGADOR *jogador, POSICAO *tiro, int *menu ){

    //Essa função atualiza as informações de deslocamento a partir de qual tecla foi clicada pelo jogador.

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
                sentidoTiro(tiro, jogador->posicao); //Determina em que sentido o tiro devera se mover, baseado na ultima direção em que o jogador se deslocou
                tiro->x = jogador->posicao.x; //A posição inicial do tiro é a mesma que a do jogador
                tiro->y = jogador->posicao.y;
                tiro->visivel = 1; //Variavel que faz com que o tiro seja desenhado e se mova
            }
        }
        if (IsKeyPressed(KEY_TAB)) *menu=1;

        }

int salvaJogo(JOGADOR jogador, int n_toupeiras, TOUPEIRA toupeira[], char mapa[MAX_LINHAS][MAX_COLUNAS], int numeroMapa, int n_esmeraldas, int velocidade){
    //Escreve todas as informações importantes para um save em um arquivo de save. Informações da struct jogador, de cada uma das toupeiras, o numero total de toupeiras no mapa,
    //o numero do mapa/fase, o numero de esmeraldas restantes no mapa.
    FILE *arq;
    int i;

    if ((arq = fopen("save.txt", "wb")) == NULL){
        printf("\nErro ao salvar!\n");
        return 0;
    }else{
        fwrite(&jogador, sizeof(JOGADOR), 1, arq);
        fwrite(mapa, sizeof(char), MAX_COLUNAS*MAX_LINHAS, arq);
        fwrite(&n_toupeiras, sizeof(int), 1, arq);
        for (i=0; i>n_toupeiras; i++){
            fwrite(&toupeira[i], sizeof(TOUPEIRA), 1, arq);
        }
        fwrite(&numeroMapa, sizeof(int), 1, arq);
        fwrite(&n_esmeraldas, sizeof(int), 1, arq);
        fwrite(&velocidade, sizeof(int), 1, arq);
        fclose(arq);
        return 1;
    }
}

int carregaJogo(JOGADOR *jogador, int *n_toupeiras, TOUPEIRA toupeira[], char mapa[MAX_LINHAS][MAX_COLUNAS], int *numeroMapa, int *n_esmeraldas, int *velocidade){
    int i, j;
    FILE *arq;
    //Le todas as informações contidas no save e atualiza elas por ponteiros. Assim que o save é lido, o jogo volta pro seu loop principal e não precisa ler mais nenhuma informação
    //para funcionar exatamente como no momento em que foi salvo.

    if ((arq = fopen("save.txt", "rb")) == NULL){
        printf("\nErro ao carregar!\n");
        fclose(arq);
        return 0;
    }else{
        fread(jogador, sizeof(JOGADOR), 1 ,arq);
        fread(mapa, sizeof(char), MAX_COLUNAS*MAX_LINHAS, arq);
        fread(n_toupeiras, sizeof(int), 1, arq);
        for(i = 0; i>n_toupeiras; i++){
            fread(&toupeira[i], sizeof(TOUPEIRA), 1, arq);
        }
        fread(numeroMapa, sizeof(int), 1, arq);
        fread(n_esmeraldas, sizeof(int), 1, arq);
        fread(velocidade, sizeof(int), 1, arq);
    }
    fclose(arq);
    return 1;
}

int main(){
    POSICAO tiro;
    JOGADOR jogador;
    TOUPEIRA toupeiras[M_MAX];
    int tempoToupeira = 0; //Usado para fazer com que as toupeiras se movam mais devagar
    int tempoPowerup = 0; //Usado para o powerup ficar ativo por apenas 3 segundos
    int toupeira_n = 0; //Numero total de toupeiras no mapa
    int esmeralda_n = 0; //Numero total de esmeraldas no mapa
    int i,j;
    char mapa[MAX_LINHAS][MAX_COLUNAS];
    char nomeMapa[MAXNOMEMAPA];
    Texture2D jogadorText, toupeiraText, pedraText, terraText, fundoText, powerupText, esmeraldaText, ouroText, menuText, baixoText, tiroText, gameOverText, splashScreen; //texturas
    int numeroMapa = 1; //Ao abrir o jogo e não carregar um save, o numero do mapa inicia em 1
    int menu = 0;
    int mensagem; //Mensagem de morte, aleatoria.
    int velocidade = 10;
    int inicio = 0; //Se 0, mostra a tela inicial do jogo, com as opções de iniciar novo jogo ou carregar save.
    Image icone; //icone que aparece no topo da janela
    Vector2 posicaoVisao; //Struct vector2 para a área de visao do jogador.
    jogador.gameOver = 0;
    jogador.ganhou = 0;
    jogador.vidas = 3;
    tiro.visivel = 0;
    jogador.pontos = 0;
    srand(time(NULL));

    leMapa("mapa1.txt", mapa, &jogador, toupeiras, &toupeira_n, &esmeralda_n); //Função de inicialização

    InitWindow(LARGURA, ALTURA, "SpaceSpelunker");
    SetTargetFPS(60);
    jogadorText = LoadTexture("sprites/andando.png");
    toupeiraText = LoadTexture("sprites/toupeira.png");
    pedraText = LoadTexture("sprites/pedra.png");
    terraText = LoadTexture("sprites/terra.png");
    fundoText = LoadTexture("sprites/fundo.png");
    powerupText = LoadTexture("sprites/powerup.png");
    esmeraldaText = LoadTexture("sprites/esmeralda.png");
    ouroText = LoadTexture("sprites/ouro.png");
    menuText = LoadTexture("sprites/menu.png");
    baixoText = LoadTexture("sprites/baixo.png");
    tiroText = LoadTexture("sprites/projetil.png");
    gameOverText = LoadTexture("sprites/gameOver.png");
    splashScreen = LoadTexture("sprites/splashScreen.png");
    icone = LoadImage("sprites/toupeira.png");
    SetWindowIcon(icone);


    while (!WindowShouldClose()){ //Loop principal
        posicaoVisao.x = jogador.posicao.x+(LADO/2); //centro do jogador
        posicaoVisao.y = jogador.posicao.y+(LADO/2);
        if (inicio == 0){ //Tela inicial. Mostra uma arte com o titulo do jogo (Créditos: Álvaro Guglielmin Becker, muito obrigado! <3 E permite que o jogador inicie um novo jogo, ou carregue um save.
            if (IsKeyPressed(KEY_N)){
                inicio = 1;
            }
            if (IsKeyPressed(KEY_C) && carregaJogo(&jogador, &toupeira_n, toupeiras, mapa, &numeroMapa, &esmeralda_n, &velocidade) == 1){
                inicio = 1;
            }
            BeginDrawing();
            DrawTexture(splashScreen, 0, 0, WHITE);
            EndDrawing();
        }else{
            if (jogador.gameOver == 0){ //Repete o loop principal do jogo, até o jogador ganhar ou morrer
                if(menu == 0){ //Loop principal do jogo dentro de um if em função do menu, para que quando o menu seja aberto, o jogo pause
                    //Ir pra proxima fase
                    if (jogador.esmeraldas == esmeralda_n){ //Se o jogador coletar todas as esmeraldas, passa pra proxima fase.
                        esmeralda_n = 0; //variaveis que precisam ser resetadas são zeradas
                        tempoToupeira = 0;
                        tempoPowerup = 0;
                        toupeira_n = 0;
                        tiro.visivel = 0;
                        numeroMapa++; //numero do mapa/fase aumenta
                        velocidade -= 2; //Velocidade das toupeiras aumentam a cada fase
                        sprintf(nomeMapa,"mapa%d.txt",numeroMapa); //nome do proximo mapa é gerado
                        if (!(leMapa(nomeMapa, mapa, &jogador, toupeiras, &toupeira_n, &esmeralda_n))){ //le o mapa, atualiza as informações contidas nele, e volta pro loop principal do jogo
                            jogador.ganhou = 1;
                            jogador.gameOver = 1;                                                       //as vidas e pontuação do jogador são preservados de mapa para mapa
                        }
                    }
                    if(jogador.vidas == 0){
                        mensagem = GetRandomValue(0,11); //Pega um numero aleatorio para mostrar uma mensagem diferente de morte a cada vez que o jogador morre.
                        jogador.powerup = 0;
                        jogador.gameOver = 1;
                    }
                    pressionaTecla(&jogador, &tiro,&menu); //Atualiza informações de acordo com a tecla pressionada pelo usuario

                    //Mover:
                    if(podeMoverJ(&jogador, LARGURA, ALTURA, mapa, toupeira_n, toupeiras) == 1){ //Se a posição na qual o jogador quer se deslocar estiver livre, ele se move
                        move(&jogador.posicao);
                    }
                    jogador.posicao.desX = 0; //Reseta o deslocamento do jogador, até que ele pressione outra tecla
                    jogador.posicao.desY = 0;

                    for(i=0; i<toupeira_n; i++){ //O laço atualiza as informações de cada toupeira na tela
                        if(toupeiras[i].posicao.visivel == 1){ //As atualizações só ocorrem se a toupeira estiver visivel, ou seja, 'viva'
                            if (tempoToupeira == velocidade){ //A cada frame, o tempoToupeira aumenta em 1. As toupeiras só se movem quando esse timer fica igual à VELOCIDADE. Isso faz com que elas não se movam tão rápido, efetivamente dividindo o framerate pela velocidade. Assim, as toupeiras se movem a 6FPS.
                                moveToupeira(&toupeiras[i], podeMoverT(toupeiras[i].posicao, LARGURA, ALTURA, mapa)); //Se todas as condições forem atendidas, a toupeira se move.
                            }
                            if (jogador.posicao.y == toupeiras[i].posicao.y && jogador.posicao.x == toupeiras[i].posicao.x){
                                    jogador.vidas--; //Se alguma toupeira ocupar a mesma posição do jogador, as vidas dele diminuem e o jogador e as toupeiras retornam a suas posiçoes iniciais.
                                    jogador.posicao.x = jogador.posicaoInicial.x;
                                    jogador.posicao.y = jogador.posicaoInicial.y;
                                    for (j=0; j<toupeira_n; j++){
                                        toupeiras[j].posicao.x = toupeiras[j].posicaoInicial.x;
                                        toupeiras[j].posicao.y = toupeiras[j].posicaoInicial.y;
                                    }
                            }
                        }
                    }

                    if(tiro.visivel == 1){ //Se o tiro estiver visivel(se o jogador pressionou G), o tiro se move. Se ele colidir com algo, ele para de ser visivel e dependendo do objeto com o qual ele colidiu, a ação apropriada acontece.
                        move(&tiro);
                        if (tiroColisao(tiro, mapa, toupeiras, toupeira_n, &jogador.pontos) == 1){
                            tiro.visivel = 0;
                        }
                    }
                    if (jogador.powerup == 1){ //Se o jogador coletar um powerup, o timer do powerup aumenta
                        //funcionaldiade do powerup
                        tempoPowerup++;
                    }
                    if(tempoPowerup == 180){ //O jogo roda a 60FPS, então 3 segundos se referem a 180 frames. O contador aumenta 1 a cada frame, então quando ele chega a 180 ele tem que parar e desativar o powerup.
                        jogador.powerup = 0;
                        tempoPowerup = 0;
                    }
                    if (tempoToupeira == velocidade) tempoToupeira = 0; //Reseta o timer da velocidade das toupeiras, e aumenta uma unidade a cada ciclo do loop principal.
                        tempoToupeira++;
                    }
                    if (menu == 1) {
                        if (funcionaMenu(&menu) == 'S'){ //Salva o jogo, sobrescrevendo o save anterior (se existir) ou criando um novo
                            salvaJogo(jogador, toupeira_n, toupeiras, mapa, numeroMapa, esmeralda_n, velocidade);
                            menu = 0;
                        }
                        else if (funcionaMenu(&menu) == 'C'){ //Carrega um save
                            carregaJogo(&jogador, &toupeira_n, toupeiras, mapa, &numeroMapa, &esmeralda_n, &velocidade);
                            menu = 0;
                        }
                        else if (funcionaMenu(&menu) == 'Q'){ //Finaliza o programa, se o jogador desejar sair sem salvar
                            return 0;
                        }
                        else if (funcionaMenu(&menu) == 'N'){ //Se o jogador inicializar um novo jogo, todas as variaveis necessarias sao zeradas e o mapa 1 é carregado.
                            jogador.gameOver = 0;
                            jogador.ganhou = 0;
                            jogador.vidas = 3;
                            tiro.visivel = 0;
                            jogador.pontos = 0;
                            jogador.esmeraldas = 0;
                            velocidade = 10;
                            tempoToupeira = 0;
                            tempoPowerup = 0;
                            toupeira_n = 0;
                            esmeralda_n = 0;
                            jogador.powerup = 0;
                            numeroMapa = 1;
                            menu = 0;
                            jogador.ganhou = 0;
                            leMapa("mapa1.txt", mapa, &jogador, toupeiras, &toupeira_n, &esmeralda_n);
                            jogador.gameOver = 0;
                            menu = 0;
                        }
                    }
                    BeginDrawing(); //Inicio da interface
                    ClearBackground(RAYWHITE);
                    desenhaMapa(mapa,toupeiraText, pedraText, terraText, fundoText, powerupText, esmeraldaText, ouroText); //A função desenha todas as informações que estão na matriz
                    DrawTexture(jogadorText, jogador.posicao.x, jogador.posicao.y, WHITE); //Desenha o jogador, em função da posição dele
                    if (tiro.visivel == 1){ //Se o tiro estiver visivel, o desenha.
                        DrawTexture(tiroText, tiro.x, tiro.y, WHITE);
                    }
                    for (i=0; i<toupeira_n; i++){ //Desenha cada uma das toupeiras.
                        if (toupeiras[i].posicao.visivel == 1){ //Só desenha as toupeiras se estiverem vivas/visiveis
                            DrawTexture(toupeiraText, toupeiras[i].posicao.x, toupeiras[i].posicao.y, WHITE);
                        }
                    }
                    if (jogador.powerup == 0){ //Se o jogador não tiver o powerup, bloqueia sua visão
                        DrawRing(posicaoVisao, 3*LADO, 2000, 45, 405, 0, BLACK); //Desenha um anel ao redor do jogador, bloqueando sua visão.
                    }

                    //Desenha as informações sobre o jogo:
                    DrawTexture(baixoText, 0, 600, WHITE);
                    DrawText(TextFormat("vidas: %d",jogador.vidas), 200, 610, 30,WHITE);
                    DrawText(TextFormat("score: %d",jogador.pontos),500, 610, 30,WHITE);
                    DrawText(TextFormat("nivel: %d",numeroMapa), 200, 650, 30,WHITE);
                    DrawText(TextFormat("gemas: %d/%d",jogador.esmeraldas,esmeralda_n), 500, 650, 30,WHITE);

                    //funcionalidade do menu:
                    if (menu == 1){
                        DrawTexture(menuText, 250, 50, WHITE);
                        DrawText("Novo jogo [N]", 330, 130, 29,WHITE);
                        DrawText("Carregar jogo [C]", 330, 230, 29,WHITE);
                        DrawText("Salvar jogo [S]", 330, 330, 29,WHITE);
                        DrawText("Sair sem salvar [Q]", 330, 430, 29,WHITE);
                        DrawText("Voltar [V]", 330, 530, 29,WHITE);
                    }
                    EndDrawing();
            }else{
            //Se gameOver == 1, para o jogo e executa as funcionalidades da tela de derrota
                BeginDrawing();
                DrawTexture(gameOverText, 0, 0, WHITE);
                if (jogador.ganhou == 0){
                    switch (mensagem){ //Por diversao, a mensagem de derrota é aleatorizada :)
                        case 0: DrawText("Voce morreu! Nao estou bravo, apenas decepcionado.", 130, 400, 25,WHITE);
                                break;
                        case 1: DrawText("Voce morreu! Tente de novo ou morra tentando.", 130, 400, 25,WHITE);
                                break;
                        case 2: DrawText("Bah oh meu....", 130, 400, 25,WHITE);
                                break;
                        case 3: DrawText("As toupeiras foram mais fortes.\nMaltidos sejam os Digglets!", 130, 400, 25,WHITE);
                                break;
                        case 4: DrawText("Voce nao pode desistir ainda!\nSpace Spelunker!\nTenha determinacao!", 120, 400, 25,WHITE);
                                break;
                        case 5: DrawText("Tem uma hora pra tudo, e a sua foi agora.", 140, 400, 25,WHITE);
                                break;
                        case 6: DrawText("Foi de arrasta pra cima.", 140, 400, 25, WHITE);
                                break;
                        case 7: DrawText("Ficamos 3 semanas fazendo esse jogo e voce perdeu?!\nDesrespeitoso.", 130, 400, 25,WHITE);
                                break;
                        case 8: DrawText("Ta errado isso ai!\n- Longhi, Fernando.", 140, 400, 25,WHITE);
                                break;
                        case 9: DrawText("You tried so hard, and got so far...", 140, 400, 25,WHITE);
                                break;
                        case 10: DrawText("Meteu o pe na jaca...", 140, 400, 25,WHITE);
                                break;
                        case 11: DrawText("Peguei um besta!", 140, 400, 25,WHITE);
                                break;
                    }
                }
                else{
                    DrawText("A missao foi um sucesso!", 140, 400, 25, WHITE);
                }
                DrawText(TextFormat("Pontuacao: %d", jogador.pontos), 140, 500, 25,WHITE);
                DrawText(TextFormat("Carregar save [C]\t\t\tNovo jogo [N]", jogador.pontos), 140, 550, 25,WHITE);
                EndDrawing();
                if (IsKeyPressed(KEY_N)){ //Se o jogador inicializar um novo jogo, todas as variaveis necessarias sao zeradas e o mapa 1 é carregado.
                    jogador.gameOver = 0;
                    jogador.ganhou = 0;
                    jogador.vidas = 3;
                    tiro.visivel = 0;
                    jogador.pontos = 0;
                    jogador.esmeraldas = 0;
                    velocidade = 10;
                    tempoToupeira = 0;
                    tempoPowerup = 0;
                    toupeira_n = 0;
                    esmeralda_n = 0;
                    jogador.powerup = 0;
                    numeroMapa = 1;
                    menu = 0;
                    jogador.ganhou = 0;
                    leMapa("mapa1.txt", mapa, &jogador, toupeiras, &toupeira_n, &esmeralda_n);
                    jogador.gameOver = 0;
                }
                if (IsKeyPressed(KEY_C)){
                    carregaJogo(&jogador, &toupeira_n, toupeiras, mapa, &numeroMapa, &esmeralda_n, &velocidade); //Se o jogador quiser carregar um save, isso acontece e as condições de gameover e vitoria sao zeradas.
                    jogador.ganhou = 0;
                    jogador.gameOver = 0;
                }
            }
        }
    }
    CloseWindow();
    return 0;
}
