#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raylib.h"
#include <time.h>

const int vMin = 1;
const int vMax = 100;
const int screenWidth = 1280;
const int screenHeight = 720;

//Fonte
Font mono;
//Struct que define o conteúdo das cartas
typedef struct {
    int valor1, valor2;
    char operador;
    char pergunta[8];
    int resposta;
    char resposta_string[4];
} Carta;

//Struct que define a posição das cartas
typedef struct {
    int identificador;
    int tipo;
} Posicao;


void DrawTextB(const char *text, int posX, int posY, int fontSize, Color color) {
    DrawTextEx(mono, text, (Vector2) {posX, posY}, fontSize, 1, color);
}

//Função que cria as cartas
void CriaCartas(Carta *cartas, int pares_quantidade) {

    int cont = 0;
    int check = 0;
    int i, op;
    SetRandomSeed(time(NULL));
    while (cont < pares_quantidade) {
        cartas[cont].valor1 = GetRandomValue(vMin, vMax);
        cartas[cont].valor2 = GetRandomValue(vMin, vMax);
        op = GetRandomValue(1, 4);
        if (op == 1) {
            cartas[cont].operador = '+';
            cartas[cont].resposta = cartas[cont].valor1 + cartas[cont].valor2;
        } else if (op == 2) {
            cartas[cont].operador = '-';
            cartas[cont].resposta = cartas[cont].valor1 - cartas[cont].valor2;
            while (cartas[cont].resposta < 0) {
                cartas[cont].valor1 = GetRandomValue(vMin, vMax);
                cartas[cont].valor2 = GetRandomValue(vMin, vMax);
                cartas[cont].resposta = cartas[cont].valor1 - cartas[cont].valor2;
            }
        } else if (op == 3) {
            cartas[cont].operador = '*';
            cartas[cont].valor1 = GetRandomValue(vMin, vMax / 4);
            cartas[cont].valor2 = GetRandomValue(vMin, vMax / 4);
            cartas[cont].resposta = cartas[cont].valor1 * cartas[cont].valor2;
        } else {
            cartas[cont].operador = '/';
            cartas[cont].resposta = cartas[cont].valor1 / cartas[cont].valor2;
            while (cartas[cont].valor1 % cartas[cont].valor2 != 0) {
                cartas[cont].valor1 = GetRandomValue(vMin, vMax);
                cartas[cont].valor2 = GetRandomValue(vMin, vMax);
                cartas[cont].resposta = cartas[cont].valor1 / cartas[cont].valor2;
            }
        }
        //Verifica se é uma resposta repetida
        for (i = 0; i < cont; i++) {
            if (cartas[i].resposta == cartas[cont].resposta) {
                check = 1;
            }
        }
        if (!check) {
            cont++;
        } else {
            check = 0;
        }
    }
    for(i=0;i<pares_quantidade;i++)
    {
        sprintf(cartas[i].pergunta, "%i%c%i", cartas[i].valor1, cartas[i].operador, cartas[i].valor2);
        sprintf(cartas[i].resposta_string, "%i", cartas[i].resposta);
    }
}


void DistribuiCartas(Carta *cartas,Posicao *pos,char **cartas_string ,int pares_quantidade) {
    int cont = 0;
    int check = 0;
    int i;
    SetRandomSeed(time(NULL));
    while (cont < pares_quantidade * 2) {
        pos[cont].tipo = GetRandomValue(1, 2);
        pos[cont].identificador = GetRandomValue(0, pares_quantidade - 1);
        for (i = 0; i < cont && !check; i++) {
            if (pos[i].identificador == pos[cont].identificador && pos[i].tipo == pos[cont].tipo) {
                check = 1;
            }
        }
        if (!check) {
            cont++;
        } else {
            check = 0;
        }
    }
    for(i=0;i<pares_quantidade*2;i++)
    {
        if(pos[i].tipo == 1)
        {
            TextCopy(cartas_string[i],cartas[pos[i].identificador].pergunta);
        }
        else
        {
            TextCopy(cartas_string[i],cartas[pos[i].identificador].resposta_string);
        }
    }
}

void GeraPosicaoCartasTela(Vector2 *pos_tela, int pares_quantidade, int offset) {
    int i, j, cont = 0;
    for (i = 1; i <= 4; i++) {
        for (j = 1; j <= pares_quantidade/2; j++) {
            pos_tela[cont].x = offset + 15 * j + 120 * (j - 1);
            pos_tela[cont].y = offset + 15 * i + 100 * (i - 1);
            cont++;
        }
    }

}

void GeraPosicaoTextoTela(const char ** cartas_string, const Vector2 *pos_carta_tela, Vector2 *pos_texto_tela,
                          int pares_quantidade) {
    int i;
    Vector2 offset;
    for(i=0; i < pares_quantidade*2;i++)
    {

        offset = MeasureTextEx(mono,cartas_string[i],20,1);
        pos_texto_tela[i].x = pos_carta_tela[i].x+60-offset.x/2;
        pos_texto_tela[i].y = pos_carta_tela[i].y+50-offset.y/2;
    }
}

int main(void) {

    //Inicialização
    InitWindow(screenWidth, screenHeight, "MATENIGMA");
    SetTargetFPS(60);
    mono = LoadFont("/Users/feliperabelo/MATENIGMA/resources/Monocraft.otf");
    int pares_quantidade = pow(4, 2) / 2;
    int i;
    //Gerar as cartas
    Carta *cartas;
    cartas = (Carta *) malloc(pares_quantidade * sizeof(Carta));
    CriaCartas(cartas, pares_quantidade);
    //Gera o vetor de distribuição de cartas
    Posicao *pos;
    pos = (Posicao *) malloc((pares_quantidade * 2 * sizeof(Posicao)));
    char **cartas_string;
    cartas_string = (char**) malloc(pares_quantidade*2* sizeof(char*));
    for(i=0;i<pares_quantidade*2;i++)
    {
        cartas_string[i] = (char*) malloc(8* sizeof(char));
    }
    DistribuiCartas(cartas,pos,cartas_string, pares_quantidade);
    //Gerar posições na tela para as cartas
    Vector2 *pos_carta_tela;
    pos_carta_tela = (Vector2 *) malloc(pares_quantidade * 2 * sizeof(Vector2));
    GeraPosicaoCartasTela(pos_carta_tela, pares_quantidade, 120);
    //Gerar posições na tela para o texto
    Vector2 *pos_texto_tela;
    pos_texto_tela = (Vector2 *) malloc(pares_quantidade * 2 * sizeof(Vector2));
    GeraPosicaoTextoTela(cartas_string, pos_carta_tela, pos_texto_tela, pares_quantidade);

    //--------------------------------------------------------
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for(i = 0;i < pares_quantidade*2;i++)
        {
            DrawRectangle(pos_carta_tela[i].x,pos_carta_tela[i].y,120,100,BLACK);
            DrawTextB(cartas_string[i],pos_texto_tela[i].x,pos_texto_tela[i].y,20,WHITE);
        }
        EndDrawing();
    }

    //Desinicialização

    CloseWindow();
    UnloadFont(mono);
    return 0;
}