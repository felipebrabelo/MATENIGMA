/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    DrawTextEx(font, "GAMEPLAY SCREEN", (Vector2){ 20, 10 }, font.baseSize*3, 4, MAROON);
    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return 1;
}

//---------------Tela Gameplay---------------/

//Biblioteca Utilizada
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include <time.h>

//Constante importantes como a resolução do jogo e os limites dos valores aleatórios
static const int vMin = 1;
static const int vMax = 100;
static const int screenWidth = 1280;
static const int screenHeight = 720;

//Fonte
Font mono;

//Texturas
Texture2D card_back, background, card_front, bg_pyramid, erros, clock_symb, bg_pyramid_win, game_complete;
Texture2D idle, run, attack, miss, run, death;

//Sons
Music musica_gameplay, musica_complete;
Sound card_flip_sound, victory_audio, gameover_audio;
Sound attack_audio, miss_audio;

//Struct do ranking
typedef struct{
    double pontos;
    char nome[11];
}Ranking;

//Struct que define o conteudo das cartas. As cartas de resposta e pegunta estão no mesmo struct porque dessa maneira podem ser relacionadas pelo indice.
typedef struct {
    int valor1, valor2;
    char operador;
    char pergunta[8];
    int resposta;
    char resposta_string[4];
} Carta;

//Struct que define a posição das cartas. Cada carta pode ser definida pelo seu índice(identificador) e pelo seu tipo 1- pergunta 2 - resposta. Assim, se duas cartas tem o mesmo identificador e tipos diferentes, elas formam um par.
typedef struct {
    int identificador;
    int tipo;
} Posicao;

//Struct de um relogio para contar o tempo
typedef struct {
    double inicio;
    double atual;
} Timer;

//Função que inicia a contagem de tempo do relogio
void ComecaTimer(Timer *relogio) {
    relogio->inicio = GetTime();
}

//Função que verifica quanto tempo passou desde o inicio da contagem
void VerificaTimer(Timer *relogio) {
    relogio->atual = GetTime() - relogio->inicio;
}

//Função para simplificar a escrita de texto com fonte diferente da padrão
static void DrawTextR(const char *text, int posX, int posY, int fontSize, Color color) {
    DrawTextEx(mono, text, (Vector2) {posX, posY}, fontSize, 1, color);
}

//Função que cria as cartas
void CriaCartas(Carta *cartas, int pares_quantidade) {

    int cont = 0;
    bool check = false; //Flag para indicar se a resposta de uma das cartas está repetida.
    int i;
    int op; // Variável que define qual a operação da carta. 1 - Soma 2 - Subtração 3 - Multiplicação 4 - Divisão
    SetRandomSeed(time(NULL));//Gera uma nova seed, evitando que os números aleatórios sejam sempre os mesmos
    while (cont < pares_quantidade) {
        //Valores aleatórios são atribuidos para as variáveis responsáveis pelos números da operação
        cartas[cont].valor1 = GetRandomValue(vMin, vMax);
        cartas[cont].valor2 = GetRandomValue(vMin, vMax);
        //Valor aleatório de 0 a 4 é atribuido a op
        op = GetRandomValue(1, 4);
        //op =1, define a resposta como a soma dos valores e o operador como +
        if (op == 1) {
            cartas[cont].operador = '+';
            cartas[cont].resposta = cartas[cont].valor1 + cartas[cont].valor2;
        }
            //op = 2, define a resposta como a subtração dos valores e o operador como -. O while serve para evitar que hajam valores negativos
        else if (op == 2) {
            cartas[cont].operador = '-';
            cartas[cont].resposta = cartas[cont].valor1 - cartas[cont].valor2;
            while (cartas[cont].resposta < 0) {
                cartas[cont].valor1 = GetRandomValue(vMin, vMax);
                cartas[cont].valor2 = GetRandomValue(vMin, vMax);
                cartas[cont].resposta = cartas[cont].valor1 - cartas[cont].valor2;
            }
        } //op = 3, define a resposta como a multiplicação dos valores e o operados como *. O limite superior é divido por 4 para evitar que as respostas sejam muito complicadas.
        else if (op == 3) {
            cartas[cont].operador = '*';
            cartas[cont].valor1 = GetRandomValue(vMin, vMax / 4);
            cartas[cont].valor2 = GetRandomValue(vMin, vMax / 4);
            cartas[cont].resposta = cartas[cont].valor1 * cartas[cont].valor2;
        }//else => op =4, define a resposta como a divisão de valores e o operador /. O while serve para evitar que os números sejam decimais.
        else {
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
                check = true;
            }
        }
        //Se não é repetida passa para a próxima carta
        if (!check) {
            cont++;
        }
            //Se é repetida, refaz a carta atual
        else {
            check = false;
        }
    }
    //Quando termina, converte todos os valores para string.A pergunta fica no formato valor1(op)valor2
    for (i = 0; i < pares_quantidade; i++) {
        sprintf(cartas[i].pergunta, "%i%c%i", cartas[i].valor1, cartas[i].operador, cartas[i].valor2);
        sprintf(cartas[i].resposta_string, "%i", cartas[i].resposta);
    }
}

//Função que distribui as cartas. É gerado um vetor em que cada elemento indica o identificador e tipo da carta sem elementos repetidos, efetivamente distribuindo as cartas. No fim as cartas são atribuidas a um vetor de string.
void DistribuiCartas(Carta *cartas, Posicao *pos, char **cartas_string, int pares_quantidade) {
    int cont = 0;
    bool check = false;
    int i;
    SetRandomSeed(time(NULL));
    while (cont < pares_quantidade * 2) {
        //Valores aleatórios são atribuidos para o tipo e identificador
        pos[cont].tipo = GetRandomValue(1, 2);
        pos[cont].identificador = GetRandomValue(0, pares_quantidade - 1);
        //----------Verifica se existe algum elemento com o mesmo tipo e identificado, se verdadeiro passa pro próximo elemento, se falso repete a atribuição --------------//
        for (i = 0; i < cont && !check; i++) {
            if (pos[i].identificador == pos[cont].identificador && pos[i].tipo == pos[cont].tipo) {
                check = true;
            }
        }
        if (!check) {
            cont++;
        } else {
            check = false;
        }
        //------------------------------------------------------------------------------------------------------------------------------//
    }
    //Ao terminar, salva todas as cartas na ordem definida em um vetor de string
    for (i = 0; i < pares_quantidade * 2; i++) {
        if (pos[i].tipo == 1) {
            TextCopy(cartas_string[i], cartas[pos[i].identificador].pergunta);
        } else {
            TextCopy(cartas_string[i], cartas[pos[i].identificador].resposta_string);
        }
    }
}

//Função que gera a posição das cartas na tela
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

//Função que gera a posição do texto das cartas na tela.
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

//Função que cria retângulos a serem usados para verificar colisão
void CriaRetangulos(Rectangle *cartas_objeto, const Vector2 *pos_carta_tela, int pares_quantidade) {
    int i;
    for (i = 0; i < pares_quantidade * 2; i++) {
        cartas_objeto[i].x = pos_carta_tela[i].x;
        cartas_objeto[i].y = pos_carta_tela[i].y;
        cartas_objeto[i].width = 120;
        cartas_objeto[i].height = 100;
    }
}

//Função que verifica sobre qual carta o mouse está sobre. A carta em questão é destacada com bordas vermelhas.
void ChecaSelecao(const Rectangle *cartas_objeto, const Vector2 mouse, bool *colisao, int pares_quantidade,
                  const bool *flag_remover) {
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

//Função que realiza a seleção da carta na tela. Caso a carta seja clicada, a seleção é atualizada e é salvo o o identificador da carta e seu número de posição
void SelecionaCarta(int *idcarta, int *num_carta_selec, const Posicao *pos, const bool *colisao, bool *select,
                    int pares_quantidade, int num_cara_selec_ant) {
    int i;
    for (i = 0; i < pares_quantidade * 2 && !(*select); i++) {
        if (colisao[i] && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *idcarta = pos[i].identificador;
            *select = true;
            *num_carta_selec = i;
            //Para evitar que uma mesma carta possa ser selecionada sequencialmente, nada muda se isso acontecer
            if (*num_carta_selec == num_cara_selec_ant) {
                *select = false;
                *num_carta_selec = -1;
            }
                //Se for uma cara diferente toca o som de virar
            else {
                PlaySound(card_flip_sound);
            }
        }
    }
}

//Função para esperar segundos
void EsperaSegundos(double segundos) {
    Timer relogio;
    ComecaTimer(&relogio);
    VerificaTimer(&relogio);
    while (relogio.atual < segundos) {
        UpdateMusicStream(musica_gameplay);
        VerificaTimer(&relogio);
    }
}

//Função que verifica se as duas cartas selecionadas formam um par
void VerificaSePar(const int *carta_id, bool *carta_sel, int *num_carta_selec, bool *flag_remover, int *tentativas,
                   int *acertos, int *tipo_anim) {

    if (carta_sel[0] && carta_sel[1]) {
        if (carta_id[0] == carta_id[1]) {
            //Caso as cartas possuam o mesmo identificador, elas formam um par. Assim elas são removidas da tela e o número de acertos é aumentado.
            flag_remover[num_carta_selec[0]] = true;
            flag_remover[num_carta_selec[1]] = true;
            *acertos = *acertos + 1;
            *tipo_anim = 1;
        } else {
            *tipo_anim = 2;
        }
        //Independente de ser formar par ou não, as variáveis de seleção são resetadas.
        num_carta_selec[0] = -1;
        num_carta_selec[1] = -1;
        carta_sel[0] = false;
        carta_sel[1] = false;
        *tentativas = *tentativas + 1;
        EsperaSegundos(1);
    }

}

//Função que controla a animação baseado no exemplo sprite anim. Foram usados intervalos de frames diferentes para evitar que uma influencie a outra.
void ControleAnimacao(int *framesCounter, int *framesSpeed, int *currentFrame, Rectangle *frameRec, int *tipo) {

    if (*tipo == 0) {
        if (*currentFrame > 3) {
            *currentFrame = 0;
            frameRec->x = 0.0f;
        }
        frameRec->width = (float) idle.width / 4;
        *framesCounter = *framesCounter + 1;
        *framesSpeed = 5;
        if (*framesCounter >= (60 / *framesSpeed)) {
            *framesCounter = 0;
            *currentFrame = *currentFrame + 1;
            frameRec->x = (float) *currentFrame * (float) idle.width / 4;
        }
    } else if (*tipo == 1) {
        if (*currentFrame < 4 || *currentFrame > 9) {
            *currentFrame = 4;
            frameRec->x = 0.0f;
        }
        frameRec->width = (float) attack.width / 6;
        *framesCounter = *framesCounter + 1;
        *framesSpeed = 8;
        if (*framesCounter >= (60 / *framesSpeed)) {
            *framesCounter = 0;
            *currentFrame = *currentFrame + 1;
            frameRec->x = (float) (*currentFrame - 4) * (float) attack.width / 6;
            if (frameRec->x == attack.width) {
                PlaySound(attack_audio);
                *tipo = 0;
            }
        }
    } else if (*tipo == 2) {
        if (*currentFrame < 10 || *currentFrame > 12) {
            *currentFrame = 10;
            frameRec->x = 0.0f;
        }
        frameRec->width = (float) miss.width / 3;
        *framesCounter = *framesCounter + 1;
        *framesSpeed = 6;
        if (*framesCounter >= (60 / *framesSpeed)) {
            *framesCounter = 0;
            *currentFrame = *currentFrame + 1;
            frameRec->x = (float) (*currentFrame - 10) * (float) miss.width / 3;
            if (frameRec->x == miss.width) {
                PlaySound(miss_audio);
                *tipo = 0;
            }
        }
    } else if (*tipo == 3) {
        if (*currentFrame < 13 || *currentFrame > 18) {
            *currentFrame = 13;
            frameRec->x = 0.0f;
        }
        frameRec->width = (float) run.width / 6;
        *framesCounter = *framesCounter + 1;
        *framesSpeed = 8;
        if (*framesCounter >= (60 / *framesSpeed)) {
            *framesCounter = 0;
            *currentFrame = *currentFrame + 1;
            frameRec->x = (float) (*currentFrame - 13) * (float) run.width / 6;
        }
    } else if (*tipo == 4) {
        if (*currentFrame < 19) {
            *currentFrame = 19;
            frameRec->x = 0.0f;
        }
        frameRec->width = (float) death.width / 6;
        *framesCounter = *framesCounter + 1;
        *framesSpeed = 2;
        if (*framesCounter >= (60 / *framesSpeed)) {
            *framesCounter = 0;
            *currentFrame = *currentFrame + 1;
            if (*currentFrame < 25) {
                frameRec->x = (float) (*currentFrame - 19) * (float) death.width / 6;
            }
        }
    }
}

//Função que gera a tela de vitória
void TelaVitoria(int *framesCounter, int *framesSpeed, int *currentFrame, Rectangle *frameRec, Vector2 *position_anim,
                 double pontos) {
    Timer relogio;
    int tipo = 3;
    char pontos_string[40];
    sprintf(pontos_string, "Pontos da Fase: %.0lf", pontos);
    Vector2 offset_win = MeasureTextEx(mono, "FASE CONCLUIDA", 50, 1);
    Vector2 offset_pontos = MeasureTextEx(mono, pontos_string, 35, 1);
    Vector2 offset_enter = MeasureTextEx(mono, "Pressione ENTER para continuar.", 20, 1);
    bool termina = false;
    ComecaTimer(&relogio);
    PlaySound(victory_audio);
    while (!WindowShouldClose() && !termina) {
        VerificaTimer(&relogio);
        ControleAnimacao(framesCounter, framesSpeed, currentFrame, frameRec, &tipo);
        if (position_anim->x < (float) screenWidth) {
            position_anim->x += 5.0f;
        }
        if (relogio.atual > 5.0) {
            if (IsKeyPressed(KEY_ENTER)) {
                termina = true;
            }
        }

        //--------------------------------Drawing---------------------------------------------------
        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);
        DrawTexture(bg_pyramid_win, 0, 0, GOLD);
        DrawTextR("FASE CONCLUIDA", screenWidth / 2 - offset_win.x / 2, 100, 50, WHITE);
        DrawTextR(pontos_string, (screenWidth - offset_pontos.x) / 2, 110 + offset_win.y, 35, WHITE);
        if (relogio.atual > 5.0) {
            DrawTextR("Pressione ENTER para continuar.", (screenWidth - offset_enter.x) / 2,
                      110 + offset_win.y + offset_pontos.y, 20, LIGHTGRAY);
        }
        DrawTextureRec(run, *frameRec, *position_anim, WHITE);
        EndDrawing();
        //-------------------------------------------------------------------------------------------
    }
}

//Função que gera a tela de derrota
void TelaDerrota(int *framesCounter, int *framesSpeed, int *currentFrame, Rectangle *frameRec, Vector2 *position_anim) {
    Timer relogio;
    bool termina = false;
    int tipo = 4;
    Vector2 offset_lose = MeasureTextEx(mono, "GAME OVER", 70, 1);
    Vector2 offset_enter = MeasureTextEx(mono, "Pressione ENTER para continuar.", 20, 1);
    ComecaTimer(&relogio);
    PlaySound(gameover_audio);
    while (!WindowShouldClose() && !termina) {
        VerificaTimer(&relogio);
        ControleAnimacao(framesCounter, framesSpeed, currentFrame, frameRec, &tipo);
        if (relogio.atual > 10.0) {
            if (IsKeyPressed(KEY_ENTER)) {
                termina = true;
            }
        }

        //--------------------------------Drawing---------------------------------------------------
        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);
        DrawTexture(bg_pyramid, 0, 0, GOLD);
        DrawTextR("GAME OVER", (screenWidth - offset_lose.x) / 2, 100, 70, WHITE);
        if (relogio.atual > 10.0) {
            DrawTextR("Pressione ENTER para continuar.", (screenWidth - offset_enter.x) / 2,
                      110 + offset_lose.y, 20, LIGHTGRAY);
        }
        DrawTextureRec(death, *frameRec, *position_anim, WHITE);
        EndDrawing();
        //-------------------------------------------------------------------------------------------
    }
}

//Função que gere um nível do jogo
void GerarNivelJogo(int pares_quantidade, double tempo_total, double *pontos, double pontos_peso, bool *gameover_win,
                    bool *gameover_lose,
                    Vector2 offset, char *fase_nome) {
    //--------------------------------Inicialização---------------------------------------------------

    //Variáveis
    int tentativas = 0, acertos = 0;
    Timer relogio;//Relógio que grava o tempo de jogo
    char tempo[5];//Variável para o tempo restante do jogo em string
    double timeout = tempo_total; //Variável para o tempo restante do jogo
    char erro_string[6];//Variável que guarda no número de erros em string
    int i;
    Vector2 offset_fase = MeasureTextEx(mono, fase_nome, 20, 1);
    Carta *cartas;
    Posicao *pos;
    char **cartas_string;
    Vector2 *pos_carta_tela;
    Vector2 *pos_texto_tela;
    Vector2 mouse = {0, 0}; // Vetor para gravar a posição do mouse
    Rectangle *cartas_objeto;
    bool *flag_remover; //Flag para sabe quando remover uma carta
    bool *colisao; //Flag que indica qual carta está com o mouse em cima
    //Variáveis que controlam a seleção de cartas
    bool carta_sel[2] = {false, false};
    int carta_id[2] = {-1, -1};
    int num_carta_selec[2] = {-1, -1};
    bool flag_primeira_selecao = false;
    //Variáveis que controlam a animação
    Rectangle frameRec = {0.0f, 0.0f, (float) idle.width / 4, (float) idle.height};
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 5;
    Vector2 position_anim = {(float) 256 / 2, (float) (462 - idle.height)};
    int tipo_anim = 0; // 0 - animacao padrao, 1 - animacao ataque, 2 - animacao erro, 3 - animacao correr

    //Alocação dinâmica das variáveis
    cartas = (Carta *) malloc(pares_quantidade * sizeof(Carta));
    pos = (Posicao *) malloc((pares_quantidade * 2 * sizeof(Posicao)));
    cartas_string = (char **) malloc(pares_quantidade * 2 * sizeof(char *));
    for (i = 0; i < pares_quantidade * 2; i++) {
        cartas_string[i] = (char *) malloc(8 * sizeof(char));
    }
    pos_carta_tela = (Vector2 *) malloc(pares_quantidade * 2 * sizeof(Vector2));
    pos_texto_tela = (Vector2 *) malloc(pares_quantidade * 2 * sizeof(Vector2));
    cartas_objeto = (Rectangle *) malloc(pares_quantidade * 2 * sizeof(Rectangle));
    flag_remover = (bool *) malloc(pares_quantidade * 2 * sizeof(bool));
    for (i = 0; i < pares_quantidade * 2; i++) {
        //Garante a inicialização de todos os flags como falso
        flag_remover[i] = false;
    }
    colisao = (bool *) malloc(pares_quantidade * 2 * sizeof(bool));

    //Chamada das funções de inicialização
    CriaCartas(cartas, pares_quantidade);
    DistribuiCartas(cartas, pos, cartas_string, pares_quantidade);
    GeraPosicaoCartasTela(pos_carta_tela, pares_quantidade, offset);
    GeraPosicaoTextoTela((const char **) cartas_string, pos_carta_tela, pos_texto_tela, pares_quantidade);
    CriaRetangulos(cartas_objeto, pos_carta_tela, pares_quantidade);
    ComecaTimer(&relogio);
//-------------------------------------------------------------------------------------------
    while (!*gameover_win && !*gameover_lose) {
        //--------------------------------Update---------------------------------------------------
        if(WindowShouldClose()) //Força termino do jogo ao pedir para fechar
        {
            exit(0);
        }
        ControleAnimacao(&framesCounter, &framesSpeed, &currentFrame, &frameRec, &tipo_anim);
        UpdateMusicStream(musica_gameplay);
        //Captura a posição do mouse atual
        mouse.x = (float) GetMouseX();
        mouse.y = (float) GetMouseY();
        //--------------------------------
        sprintf(erro_string, "x %i", tentativas - acertos);
        VerificaTimer(&relogio);
        timeout = tempo_total - relogio.atual; //Atualiza o tempo restante
        sprintf(tempo, "%.0lf", timeout);

        if (acertos >= pares_quantidade &&
            tipo_anim == 0) {//Verifica se todos os pares já foram virados e aciona o flag de vitória
            *gameover_win = true;
        } else if (timeout < 0 && tipo_anim == 0) {//Verifica se o tempo acabou e aciona o flag de derrota
            *gameover_lose = true;
        } else {
            VerificaSePar(carta_id, carta_sel, num_carta_selec, flag_remover, &tentativas, &acertos, &tipo_anim);
            if (!carta_sel[0]) {
                SelecionaCarta(&carta_id[0], &num_carta_selec[0], pos, colisao, &carta_sel[0], pares_quantidade,
                               -1);
            }
            if (flag_primeira_selecao) {
                SelecionaCarta(&carta_id[1], &num_carta_selec[1], pos, colisao, &carta_sel[1], pares_quantidade,
                               num_carta_selec[0]);
            }
            if (carta_sel[0]) {
                flag_primeira_selecao = true; //Flag necessário porque ao usar uma função do tipo que lê o estado do mouse, esse estado fica salvo até o fim do loop
            } else {
                flag_primeira_selecao = false;
            }

            //-------------------------------------------------------------------------------------------
            //--------------------------------Drawing---------------------------------------------------
            BeginDrawing();
            DrawTexture(background, 0, 0, WHITE);
            DrawTexture(bg_pyramid, 0, 0, GOLD);
            DrawTexture(erros, 15, 100, WHITE);
            DrawTexture(clock_symb, 15, 10, WHITE);
            if (timeout <= tempo_total * 0.1) {
                DrawTextR(tempo, 20 + clock_symb.width, 30, 20, RED);
            } else {
                DrawTextR(tempo, 20 + clock_symb.width, 30, 20, WHITE);
            }
            DrawTextR(erro_string, 20 + erros.width, 115, 20, WHITE);
            DrawTextR(fase_nome, screenWidth - offset_fase.x, 10, 20, WHITE);
            if (tipo_anim == 0) {
                DrawTextureRec(idle, frameRec, position_anim, WHITE);
            } else if (tipo_anim == 1) {
                DrawTextureRec(attack, frameRec, position_anim, WHITE);
            } else if (tipo_anim == 2) {
                DrawTextureRec(miss, frameRec, position_anim, WHITE);
            }
            for (i = 0; i < pares_quantidade * 2; i++) {
                if (!flag_remover[i]) {
                    if (num_carta_selec[0] == i) {
                        DrawTexture(card_front, pos_carta_tela[i].x, pos_carta_tela[i].y, WHITE);
                        DrawTextR(cartas_string[i], pos_texto_tela[i].x, pos_texto_tela[i].y, 20, BLACK);
                    } else if (num_carta_selec[1] == i) {
                        DrawTexture(card_front, pos_carta_tela[i].x, pos_carta_tela[i].y, WHITE);
                        DrawTextR(cartas_string[i], pos_texto_tela[i].x, pos_texto_tela[i].y, 20, BLACK);
                    } else {
                        DrawTexture(card_back, pos_carta_tela[i].x, pos_carta_tela[i].y, WHITE);
                    }
                }
            }
            ChecaSelecao(cartas_objeto, mouse, colisao, pares_quantidade, flag_remover);
            EndDrawing();
            //-------------------------------------------------------------------------------------------
        }
    }
    //--------------------------------Finalização---------------------------------------------------
    if(tentativas == 0)//Evitar divisão por zero
    {
        *pontos = 0;
    }
    else{
        *pontos = pontos_peso * (timeout * 1000) / (tentativas - acertos);
    }
    if (*gameover_win) {
        TelaVitoria(&framesCounter, &framesSpeed, &currentFrame, &frameRec, &position_anim, *pontos);
    } else if (*gameover_lose) {
        TelaDerrota(&framesCounter, &framesSpeed, &currentFrame, &frameRec, &position_anim);
    }
    //Liberação da variáveis alocadas dinamicamente
    free(cartas);
    free(pos);
    for (i = 0; i < pares_quantidade * 2; i++) {
        free(cartas_string[i]);
    }
    free(cartas_string);
    free(pos_carta_tela);
    free(pos_texto_tela);
    free(cartas_objeto);
    free(flag_remover);
    free(colisao);
    //-------------------------------------------------------------------------------------------
}

//Função que gera a tela especial se o jogador concluir todas as fases
void TelaCompleta(double pontos_total) {
    bool termina = false;
    Timer relogio;
    char pontos_string[30];
    sprintf(pontos_string, "Pontos totais: %.0lf", pontos_total);
    Vector2 offset_msg = MeasureTextEx(mono, "VITORIA!", 40, 1);
    Vector2 offset_pontos = MeasureTextEx(mono, pontos_string, 30, 1);
    Vector2 offset_enter = MeasureTextEx(mono, "Pressione ENTER para continuar.", 20, 1);
    ComecaTimer(&relogio);
    while (!WindowShouldClose() && !termina) {
        UpdateMusicStream(musica_complete);
        VerificaTimer(&relogio);
        if (relogio.atual > 5.0) {
            if (IsKeyPressed(KEY_ENTER)) {
                termina = true;
            }
        }

        //--------------------------------Drawing---------------------------------------------------
        BeginDrawing();
        DrawTexture(game_complete, 0, 0, WHITE);
        DrawTextR("VITORIA!", (screenWidth - offset_msg.x) / 2, 10, 40, WHITE);
        DrawTextR(pontos_string, (screenWidth - offset_pontos.x) / 2, offset_msg.y + 10, 30, WHITE);
        if (relogio.atual > 5.0) {
            DrawTextR("Pressione ENTER para continuar.", (screenWidth - offset_enter.x) / 2,
                      offset_msg.y + offset_pontos.y + 10, 20, LIGHTGRAY);
        }
        EndDrawing();
        //-------------------------------------------------------------------------------------------
    }

}

void gameplay(void) {
    //-----------------------------Carragamento dos assets pro gameplay-----------------------------
    card_back = LoadTexture("resources/card_back.png");
    card_front = LoadTexture("resources/card_front.png");
    bg_pyramid = LoadTexture("resources/bg_character.png");
    background = LoadTexture("resources/finalNight.png");
    idle = LoadTexture("resources/idle.png");
    run = LoadTexture("resources/run.png");
    attack = LoadTexture("resources/attack.png");
    miss = LoadTexture("resources/miss.png");
    death = LoadTexture("resources/death.png");
    erros = LoadTexture("resources/erros.png");
    clock_symb = LoadTexture("resources/clock.png");
    bg_pyramid_win = LoadTexture("resources/bg_pyramid_win.png");
    game_complete = LoadTexture("resources/game_complete.png");
    musica_gameplay = LoadMusicStream("resources/ZeroRespect.wav");
    musica_complete = LoadMusicStream("resources/Sunstrider.wav");
    card_flip_sound = LoadSound("resources/cardflip.mp3");
    attack_audio = LoadSound("resources/attack.wav");
    miss_audio = LoadSound("resources/damaged.wav");
    victory_audio = LoadSound("resources/MissionOver.wav");
    gameover_audio = LoadSound("resources/FallenInBattle.wav");
    mono = LoadFont("resources/Monocraft.otf");
    //-------------------------------------------------------------------------------------------
    SetTargetFPS(60);
    SetMusicVolume(musica_gameplay, 0.3f);
    SetMusicVolume(musica_complete, 0.5f);
    //-----------------------------Inicialização de Variáveis---------------------------------------------------
    int i;
    FILE *rank;
    int pares_quantidade[4] = {6, 8, 10, 12};
    Ranking jogador = {0,"      "};
    double tempo_total[4] = {150, 200, 250, 300};
    double pontos[4] = {0, 0, 0, 0};
    double pontos_peso[4] = {0.2, 0.75, 1, 1.45};
    char fase_nome[4][7] = {"FASE 1", "FASE 2", "FASE 3", "FASE 4"};
    bool gameover_win[4] = {false, false, false, false};
    bool gameover_lose[4] = {false, false, false, false};
    Vector2 offset = {120 + 256, 120};
    //-------------------------------------------------------------------------------------------
    PlayMusicStream(musica_gameplay);
    GerarNivelJogo(pares_quantidade[0], tempo_total[0], &pontos[0], pontos_peso[0], &gameover_win[0], &gameover_lose[0],
                   offset, fase_nome[0]);
    for (i = 1; i < 4; i++) {
        if (gameover_win[i - 1]) {
            GerarNivelJogo(pares_quantidade[i], tempo_total[i], &pontos[i], pontos_peso[i], &gameover_win[i],
                           &gameover_lose[i],
                           offset, fase_nome[i]);
        }
    }
    StopMusicStream(musica_gameplay);
    PlayMusicStream(musica_complete);
    jogador.pontos = pontos[0] + pontos[1] + pontos[2] + pontos[3] + (gameover_win[3] ? 5000 : 0);
    if (gameover_win[3]) {
        TelaCompleta(jogador.pontos);
    }
    //Salva a pontuação do jogador
    rank = fopen("resources/ranking.bin","r+b");
    if(rank == NULL)
    {
        perror("Erro na abertura de arquivo");
        exit(1);
    }
    fwrite(&jogador, sizeof(Ranking), 1, rank);
    fclose(rank);
    //-------------------------------------------------------------------------------------------
    //-----------------------------Descarragamento dos assets-----------------------------
    UnloadTexture(card_back);
    UnloadTexture(card_front);
    UnloadTexture(bg_pyramid);
    UnloadTexture(background);
    UnloadTexture(idle);
    UnloadTexture(run);
    UnloadTexture(attack);
    UnloadTexture(miss);
    UnloadTexture(death);
    UnloadTexture(erros);
    UnloadTexture(clock_symb);
    UnloadTexture(bg_pyramid_win);
    UnloadTexture(game_complete);
    UnloadMusicStream(musica_gameplay);
    UnloadMusicStream(musica_complete);
    UnloadSound(card_flip_sound);
    UnloadSound(attack_audio);
    UnloadSound(miss_audio);
    UnloadSound(victory_audio);
    UnloadSound(gameover_audio);
    UnloadFont(mono);
    //-------------------------------------------------------------------------------------------
    FinishGameplayScreen();
}
