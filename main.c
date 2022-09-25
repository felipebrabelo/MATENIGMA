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

//Texturas
Texture2D card_back, cardfront;

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


typedef struct {
    double inicio;
    double atual;
} Timer;

void ComecaTimer(Timer *relogio) {
    relogio->inicio = GetTime();
}

void VerificaTimer(Timer *relogio) {
    relogio->atual = GetTime() - relogio->inicio;
}

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
    for (i = 0; i < pares_quantidade; i++) {
        sprintf(cartas[i].pergunta, "%i%c%i", cartas[i].valor1, cartas[i].operador, cartas[i].valor2);
        sprintf(cartas[i].resposta_string, "%i", cartas[i].resposta);
    }
}


void DistribuiCartas(Carta *cartas, Posicao *pos, char **cartas_string, int pares_quantidade) {
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
    for (i = 0; i < pares_quantidade * 2; i++) {
        if (pos[i].tipo == 1) {
            TextCopy(cartas_string[i], cartas[pos[i].identificador].pergunta);
        } else {
            TextCopy(cartas_string[i], cartas[pos[i].identificador].resposta_string);
        }
    }
}

void GeraPosicaoCartasTela(Vector2 *pos_tela, int pares_quantidade, Vector2 offset) {
    int i, j, cont = 0;
    for (i = 1; i <= 4; i++) {
        for (j = 1; j <= pares_quantidade / 2; j++) {
            pos_tela[cont].x = offset.x + 15 * j + 120 * (j - 1);
            pos_tela[cont].y = offset.y + 15 * i + 100 * (i - 1);
            cont++;
        }
    }

}

void GeraPosicaoTextoTela(const char **cartas_string, const Vector2 *pos_carta_tela, Vector2 *pos_texto_tela,
                          int pares_quantidade) {
    int i;
    Vector2 offset;
    for (i = 0; i < pares_quantidade * 2; i++) {

        offset = MeasureTextEx(mono, cartas_string[i], 20, 1);
        pos_texto_tela[i].x = pos_carta_tela[i].x + 60 - offset.x / 2;
        pos_texto_tela[i].y = pos_carta_tela[i].y + 50 - offset.y / 2;
    }
}

void CriaRetangulos(Rectangle *cartas_objeto, const Vector2 *pos_carta_tela, int pares_quantidade) {
    int i;
    for (i = 0; i < pares_quantidade * 2; i++) {
        cartas_objeto[i].x = pos_carta_tela[i].x;
        cartas_objeto[i].y = pos_carta_tela[i].y;
        cartas_objeto[i].width = 120;
        cartas_objeto[i].height = 100;
    }
}

void ChecaSelecao(const Rectangle *cartas_objeto, const Vector2 mouse, bool *colisao, int pares_quantidade,
                  bool *flag_remover) {
    int i;
    for (i = 0; i < pares_quantidade * 2; i++) {
        if (!flag_remover[i]) {
            colisao[i] = CheckCollisionPointRec(mouse, cartas_objeto[i]);
            if (colisao[i]) {
                DrawRectangleLinesEx(cartas_objeto[i], 2, RED);
            }
        } else {
            colisao[i] = false;
        }
    }
}

void SelecionaCarta(int *idcarta, int *num_carta_selec, const Posicao *pos, const bool *colisao, bool *select,
                    int pares_quantidade, int num_cara_selec_ant) {
    int i;
    for (i = 0; i < pares_quantidade * 2 && !(*select); i++) {
        if (colisao[i] && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *idcarta = pos[i].identificador;
            *select = true;
            *num_carta_selec = i;
            if (*num_carta_selec == num_cara_selec_ant) {
                *select = false;
                *num_carta_selec = -1;
            }
        }
    }
}

void EsperaSegundos(int segundos) {
    double inicio, fim;
    inicio = GetTime();
    while ((fim = GetTime() - inicio) < segundos) {

    }
}


void VerificaSePar(const int *carta_id, bool *carta_sel, int *num_carta_selec, bool *flag_remover, int *tentativas,
                   int *acertos) {

    if (carta_sel[0] && carta_sel[1]) {
        if (carta_id[0] == carta_id[1]) {
            flag_remover[num_carta_selec[0]] = true;
            flag_remover[num_carta_selec[1]] = true;
            *acertos = *acertos + 1;
        }
        num_carta_selec[0] = -1;
        num_carta_selec[1] = -1;
        carta_sel[0] = false;
        carta_sel[1] = false;
        *tentativas = *tentativas + 1;
        EsperaSegundos(1);
    }

}

void GerarNivelJogo(int pares_quantidade, double tempo_total, double *pontos, bool *gameover_win, bool *gameover_lose,
                    Vector2 offset) {
    int tentativas = 0, acertos = 0;
    Timer relogio;
    char tempo[5];
    double timeout = tempo_total;
    char tentativas_string[20];
    int i;
    //Gerar as cartas
    Carta *cartas;
    cartas = (Carta *) malloc(pares_quantidade * sizeof(Carta));
    CriaCartas(cartas, pares_quantidade);
    //Gera o vetor de distribuição de cartas
    Posicao *pos;
    pos = (Posicao *) malloc((pares_quantidade * 2 * sizeof(Posicao)));
    char **cartas_string;
    cartas_string = (char **) malloc(pares_quantidade * 2 * sizeof(char *));
    for (i = 0; i < pares_quantidade * 2; i++) {
        cartas_string[i] = (char *) malloc(8 * sizeof(char));
    }
    DistribuiCartas(cartas, pos, cartas_string, pares_quantidade);
    //Gerar posições na tela para as cartas
    Vector2 *pos_carta_tela;
    pos_carta_tela = (Vector2 *) malloc(pares_quantidade * 2 * sizeof(Vector2));
    GeraPosicaoCartasTela(pos_carta_tela, pares_quantidade, offset);
    //Gerar posições na tela para o texto
    Vector2 *pos_texto_tela;
    pos_texto_tela = (Vector2 *) malloc(pares_quantidade * 2 * sizeof(Vector2));
    GeraPosicaoTextoTela((const char **) cartas_string, pos_carta_tela, pos_texto_tela, pares_quantidade);

    //Ponto para representar a posição do mouse
    Vector2 mouse = {0, 0};

    //Retangulos para as cartas
    Rectangle *cartas_objeto;
    cartas_objeto = (Rectangle *) malloc(pares_quantidade * 2 * sizeof(Rectangle));
    CriaRetangulos(cartas_objeto, pos_carta_tela, pares_quantidade);

    //Flag para saber se a carta em questão dever ser removida da tela ou não
    bool *flag_remover;
    flag_remover = (bool *) malloc(pares_quantidade * 2 * sizeof(bool));
    for (i = 0; i < pares_quantidade * 2; i++) {
        flag_remover[i] = false;
    }
    //
    bool *colisao;
    colisao = (bool *) malloc(pares_quantidade * 2 * sizeof(bool));

    bool carta_sel[2] = {false, false};
    int carta_id[2] = {-1, -1};
    int num_carta_selec[2] = {-1, -1};
    bool flag_primeira_selecao = false;

    ComecaTimer(&relogio);

    while (!WindowShouldClose() && !*gameover_win && !*gameover_lose) {

        //Update to retangulo que segue o mouse

        mouse.x = GetMouseX();
        mouse.y = GetMouseY();
        sprintf(tentativas_string, "Tentativas: %i", tentativas);
        VerificaTimer(&relogio);
        timeout = tempo_total - relogio.atual;
        sprintf(tempo, "%.0lf", timeout);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (acertos >= pares_quantidade) {
            BeginDrawing();
            DrawTextB("VOCE VENCEU!", screenWidth / 2 - 200, screenHeight / 2 - 100, 50, BLACK);
            EndDrawing();
            EsperaSegundos(5);
            *gameover_win = true;
        }
        if (timeout < 0) {
            BeginDrawing();
            DrawTextB("VOCE PERDEU!", screenWidth / 2 - 200, screenHeight / 2 - 100, 50, BLACK);
            EndDrawing();
            EsperaSegundos(5);
            *gameover_lose = true;
        }
        for (i = 0; i < pares_quantidade * 2; i++) {
            if (!flag_remover[i]) {
                DrawRectangleRec(cartas_objeto[i], BLACK);
            }
        }
        ChecaSelecao(cartas_objeto, mouse, colisao, pares_quantidade, flag_remover);
        if (!carta_sel[0]) {
            SelecionaCarta(&carta_id[0], &num_carta_selec[0], pos, colisao, &carta_sel[0], pares_quantidade, -1);
        }
        if (num_carta_selec[0] != -1) {
            DrawTextB(cartas_string[num_carta_selec[0]], pos_texto_tela[num_carta_selec[0]].x,
                      pos_texto_tela[num_carta_selec[0]].y, 20, WHITE);
        }
        if (flag_primeira_selecao) {
            SelecionaCarta(&carta_id[1], &num_carta_selec[1], pos, colisao, &carta_sel[1], pares_quantidade,
                           num_carta_selec[0]);
        }
        if (num_carta_selec[1] != -1) {
            DrawTextB(cartas_string[num_carta_selec[1]], pos_texto_tela[num_carta_selec[1]].x,
                      pos_texto_tela[num_carta_selec[1]].y, 20, WHITE);
        }
        if (carta_sel[0]) {
            flag_primeira_selecao = true; //Flag necessário porque ao usar uma função do tipo que lê o estado do mouse, esse estado fica salvo até o fim do loop
        } else {
            flag_primeira_selecao = false;
        }
        DrawTextB(tentativas_string, screenWidth - 200, screenHeight - 100, 20, BLACK);
        DrawTextB(tempo, screenWidth - 60, 30, 20, BLACK);
        EndDrawing();
        VerificaSePar(carta_id, carta_sel, num_carta_selec, flag_remover, &tentativas, &acertos);
    }
    *pontos = (timeout * 1000) / (tentativas - acertos);
}

int main(void) {

    //Inicialização
    InitWindow(screenWidth, screenHeight, "MATENIGMA");
    SetTargetFPS(60);
    mono = LoadFont("resources/Monocraft.otf"); // OBS: mudar parametro para resources/Monocraft.otf
    int i;
    int pares_quantidade[4] = {6, 8, 10, 12};
    double tempo_total[4] = {300, 250, 200, 150};
    double pontos[4] = {0, 0, 0, 0};
    double pontos_total;
    bool gameover_win[4] = {false, false, false, false};
    bool gameover_lose[4] = {false, false, false, false};
    Vector2 offset = {120, 120};
    GerarNivelJogo(pares_quantidade[0], tempo_total[0], &pontos[0], &gameover_win[0], &gameover_lose[0], offset);
    for (i = 1; i < 4; i++) {
        if (gameover_win[i - 1]) {
            GerarNivelJogo(pares_quantidade[i], tempo_total[i], &pontos[i], &gameover_win[i], &gameover_lose[i],
                           offset);
        }
    }
    pontos_total = pontos[0]*0.2+pontos[1]*0.35+pontos[2]*0.65+pontos[3]*1.5;
    SaveFileData("resources/ponto_individual.bin",&pontos_total, sizeof(double ));
    //Desinicialização
    CloseWindow();
    UnloadFont(mono);
    return 0;
}