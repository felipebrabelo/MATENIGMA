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
Texture2D card_back, background, card_front, bg_pyramid, erros, clock_symb, bg_pyramid_win;
Texture2D idle, run, attack, miss, run, death;

//Sons

Music musica_gameplay;
Sound card_flip_sound, victory_audio, gameover_audio;
Sound attack_audio, miss_audio;
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
            } else {
                PlaySound(card_flip_sound);
            }
        }
    }
}

void EsperaSegundos(double segundos) {
    double inicio, fim;
    inicio = GetTime();
    while ((fim = GetTime() - inicio) < segundos) {
        UpdateMusicStream(musica_gameplay);
    }
}


void VerificaSePar(const int *carta_id, bool *carta_sel, int *num_carta_selec, bool *flag_remover, int *tentativas,
                   int *acertos, int *tipo_anim) {

    if (carta_sel[0] && carta_sel[1]) {
        if (carta_id[0] == carta_id[1]) {
            flag_remover[num_carta_selec[0]] = true;
            flag_remover[num_carta_selec[1]] = true;
            *acertos = *acertos + 1;
            *tipo_anim = 1;
        } else {
            *tipo_anim = 2;
        }
        num_carta_selec[0] = -1;
        num_carta_selec[1] = -1;
        carta_sel[0] = false;
        carta_sel[1] = false;
        *tentativas = *tentativas + 1;
        EsperaSegundos(1);
    }

}

void ControleAnimacao(int *framesCounter, int *framesSpeed, int *currentFrame, Rectangle *frameRec, int *tipo) {

    if (*tipo == 0) {
        if (*currentFrame > 3) {
            *currentFrame = 0;
            frameRec->x =0.0f;
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
            frameRec->x =0.0f;
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
            frameRec->x =0.0f;
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
            frameRec->x =0.0f;
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
            frameRec->x =0.0f;
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

        //Drawing

        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);
        DrawTexture(bg_pyramid_win, 0, 0, GOLD);
        DrawTextB("FASE CONCLUIDA", screenWidth / 2 - offset_win.x / 2, 100, 50, WHITE);
        DrawTextB(pontos_string, (screenWidth - offset_pontos.x) / 2, 110 + offset_win.y, 35, WHITE);
        if (relogio.atual > 5.0) {
            DrawTextB("Pressione ENTER para continuar.", (screenWidth - offset_enter.x) / 2,
                      110 + offset_win.y + offset_pontos.y, 20, LIGHTGRAY);
        }
        DrawTextureRec(run, *frameRec, *position_anim, WHITE);
        EndDrawing();
    }
}

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

        //Drawing
        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);
        DrawTexture(bg_pyramid, 0, 0, GOLD);
        DrawTextB("GAME OVER", (screenWidth - offset_lose.x) / 2, 100, 70, WHITE);
        if (relogio.atual > 10.0) {
            DrawTextB("Pressione ENTER para continuar.", (screenWidth - offset_enter.x) / 2,
                      110 + offset_lose.y, 20, LIGHTGRAY);
        }
        DrawTextureRec(death, *frameRec, *position_anim, WHITE);
        EndDrawing();

    }
}

void GerarNivelJogo(int pares_quantidade, double tempo_total, double *pontos, double pontos_peso, bool *gameover_win,
                    bool *gameover_lose,
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

    //
    Rectangle frameRec = {0.0f, 0.0f, (float) idle.width / 4, (float) idle.height};
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 5;
    Vector2 position_anim = {(float) 256 / 2, (float) (462 - idle.height)};
    int tipo_anim = 0; // 0 - animacao padrao, 1 - animacao ataque, 2 - animacao erro, 3 - animacao correr

    //
    ComecaTimer(&relogio);

    while (!WindowShouldClose() && !*gameover_win && !*gameover_lose) {

        //Animacao

        ControleAnimacao(&framesCounter, &framesSpeed, &currentFrame, &frameRec, &tipo_anim);
        //Update to retangulo que segue o mouse

        UpdateMusicStream(musica_gameplay);
        mouse.x = (float) GetMouseX();
        mouse.y = (float) GetMouseY();
        sprintf(tentativas_string, "x %i", tentativas - acertos);
        VerificaTimer(&relogio);
        timeout = tempo_total - relogio.atual;
        sprintf(tempo, "%.0lf", timeout);

        if (acertos >= pares_quantidade && tipo_anim == 0) {
            *gameover_win = true;
        } else if (timeout < 0 && tipo_anim == 0) {
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

            //Drawing---------------
            BeginDrawing();
            DrawTexture(background, 0, 0, WHITE);
            DrawTexture(bg_pyramid, 0, 0, GOLD);
            DrawTexture(erros, 15, 100, WHITE);
            DrawTexture(clock_symb, 15, 10, WHITE);
            if (timeout <= tempo_total * 0.1) {
                DrawTextB(tempo, 20 + clock_symb.width, 30, 20, RED);
            } else {
                DrawTextB(tempo, 20 + clock_symb.width, 30, 20, WHITE);
            }

            DrawTextB(tentativas_string, 20 + erros.width, 115, 20, WHITE);
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
                        DrawTextB(cartas_string[i], pos_texto_tela[i].x, pos_texto_tela[i].y, 20, BLACK);
                    } else if (num_carta_selec[1] == i) {
                        DrawTexture(card_front, pos_carta_tela[i].x, pos_carta_tela[i].y, WHITE);
                        DrawTextB(cartas_string[i], pos_texto_tela[i].x, pos_texto_tela[i].y, 20, BLACK);
                    } else {
                        DrawTexture(card_back, pos_carta_tela[i].x, pos_carta_tela[i].y, WHITE);
                    }
                }
            }
            ChecaSelecao(cartas_objeto, mouse, colisao, pares_quantidade, flag_remover);

            EndDrawing();
        }
    }
    *pontos = pontos_peso * (timeout * 1000) / (tentativas - acertos);
    if (*gameover_win) {
        TelaVitoria(&framesCounter, &framesSpeed, &currentFrame, &frameRec, &position_anim, *pontos);
    } else if (*gameover_lose) {
        TelaDerrota(&framesCounter, &framesSpeed, &currentFrame, &frameRec, &position_anim);
    }
}

int main(void) {

    //Inicialização
    InitWindow(screenWidth, screenHeight, "MATENIGMA");
    InitAudioDevice();
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
    musica_gameplay = LoadMusicStream("resources/ZeroRespect.wav");
    card_flip_sound = LoadSound("resources/cardflip.mp3");
    attack_audio = LoadSound("resources/attack.wav");
    miss_audio = LoadSound("resources/damaged.wav");
    victory_audio = LoadSound("resources/MissionOver.wav");
    gameover_audio = LoadSound("resources/FallenInBattle.wav");
    mono = LoadFont("resources/Monocraft.otf");
    SetTargetFPS(60);
    SetMusicVolume(musica_gameplay, 0.3);
    int i;
    int pares_quantidade[4] = {6, 8, 10, 12};
    double tempo_total[4] = {300, 250, 200, 150};
    double pontos[4] = {0, 0, 0, 0};
    double pontos_peso[4] = {0.2, 0.35, 0.65, 1.5};
    double pontos_total;
    bool gameover_win[4] = {false, false, false, false};
    bool gameover_lose[4] = {false, false, false, false};
    Vector2 offset = {120 + 256, 120};
    PlayMusicStream(musica_gameplay);
    GerarNivelJogo(pares_quantidade[0], tempo_total[0], &pontos[0], pontos_peso[0], &gameover_win[0], &gameover_lose[0],
                   offset);
    for (i = 1; i < 4; i++) {
        if (gameover_win[i - 1]) {
            GerarNivelJogo(pares_quantidade[i], tempo_total[i], &pontos[i], pontos_peso[i], &gameover_win[i],
                           &gameover_lose[i],
                           offset);
        }
    }
    pontos_total = pontos[0] + pontos[1] + pontos[2] + pontos[3];
    SaveFileData("resources/ponto_individual.bin", &pontos_total, sizeof(double));
    //Desinicialização
    CloseAudioDevice();
    CloseWindow();
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
    UnloadMusicStream(musica_gameplay);
    UnloadSound(card_flip_sound);
    UnloadSound(attack_audio);
    UnloadSound(miss_audio);
    UnloadSound(victory_audio);
    UnloadSound(gameover_audio);
    UnloadFont(mono);
    return 0;
}