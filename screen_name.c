#include "raylib.h"
#include "screens.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"

#define MAX_INPUT_CHARS     20
#define NOME_ARQUIVO "resources/rank.bin"
#define PONTOS_ATUAL "resources/ponto_individual.bin"
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
int letterCount = 0;
char name[MAX_INPUT_CHARS + 1] = "\0";

typedef struct {
    double score;
    char name[21];
}player;

player* list;
//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitNameScreen(void)
{
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

FILE* fopen_e_teste(char* caminho, char* modo) {
    FILE* f;
    f = fopen(caminho, modo);
    if (f == NULL) {
        perror("Erro ao tentar abrir o arquivo. \n");
        exit(1);
    }
    return f;
}

int compara(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

void criaranking(player* list) {

    int i;
    FILE* pf = fopen_e_teste(NOME_ARQUIVO, "wb+");

    qsort(list, 11, sizeof(player), compara);

    for (i = 0; i < 10 && list[i].score > 0; i++) {
        fwrite(&list[i], sizeof(player), 1, pf);
    }
    fclose(pf);
}

int seleciona(){
    //player* list;
    player teste;
    double x;
    int i;
    int cont = 0;
    int zerado = 0;

    FILE* fp = fopen_e_teste(NOME_ARQUIVO, "rb");
    FILE* scoreatual = fopen_e_teste(PONTOS_ATUAL, "rb");
    
    list = (player*)malloc(11 * sizeof(player));
    if (list == NULL) {
        perror("Erro ao alocar. \n");
        exit(1);
    }
    fread(&x, sizeof(double), 1, scoreatual);

    for (i = 0; i < 11; i++) {
        list[i].score = 0;
    }
    list[10].score = x;

    while (fread(&teste, sizeof(player), 1, fp) > 0 && zerado<10) {
        list[zerado] = teste;
        if (list[10].score > list[zerado].score) {
            cont++;
        }
        zerado++;
    }
    /*if (cont || !zerado) {
        scanf("%s", user);
        strcpy(list[10].name, user);
        criaranking(list);
    }*/
    fclose(fp);
    fclose(scoreatual);
    return (cont || !zerado);
}

// Title Screen Update logic
void UpdateNameScreen(void)
{
    // TODO: Update TITLE screen variables here
    // Press enter or tap to change to GAMEPLAY screen

    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1;   // ENDING
        PlaySound(fxCoin);
    }
}

// Title Screen Draw logic
void DrawNameScreen(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    //player* list;

    Rectangle textBox = { screenWidth / 2.0f - 100, screenHeight / 2 - 25, 500, 60 };
    bool mouseOnText = false;
    // Update
    //----------------------------------------------------------------------------------
    if (seleciona()) {
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    name[letterCount] = (char)key;
                    name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText) framesCounter++;
        else framesCounter = 0;

        // Draw

        ClearBackground(RAYWHITE);

        DrawText("FIM DE JOGO", screenWidth / 2 - 200, screenHeight / 2 - 200, 50, BLACK);
        DrawText(TextFormat("Sua pontuacao foi de: x pontos"), screenWidth / 2 - 300, screenHeight / 2 - 100, 40, GRAY);
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

        DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

        DrawText(TextFormat("Digite seu Nome: %i/%i", letterCount, MAX_INPUT_CHARS), screenWidth / 2 - 500, screenHeight / 2, 30, DARKGRAY);

        if (mouseOnText)
        {
            if (letterCount < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore char
                if (((framesCounter / 20) % 2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
            }
            else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
        }
        strcpy(list[10].name, name);
    }
    criaranking(list);

    DrawText(TextFormat("rank   nome                  pontos \n"), screenWidth / 2 + 100, screenHeight / 2 + 100, 25, GRAY);
    DrawText(TextFormat("rank   nome                  pontos \n"), screenWidth / 2 - 500, screenHeight / 2 + 100, 25, GRAY);

    for (int i = 0; i < 1; i++) {
        DrawText(TextFormat(" %d    %s\n", i + 1, list[i].name), screenWidth / 2 - 300, screenHeight / 2 + 50, 15, GRAY);
    }
    for (int i = 0; i < 1; i++) {
        DrawText(TextFormat("                             %lf\n", list[i].score), screenWidth / 2 - 300, screenHeight / 2 + 50, 15, GRAY);
    }
    /*for (int i = 5; i < 10; i++) {
        DrawText(TextFormat(" %d    %s\n", i + 1, list[i].name), screenWidth / 2 + 300, screenHeight / 2 + 50, 15, GRAY);
    }
    for (int i = 5; i < 10; i++) {
        DrawText(TextFormat("                             %lf\n", list[i].score), screenWidth / 2 + 300, screenHeight / 2 + 50, 15, GRAY);
    }*/
}

// Title Screen Unload logic
void UnloadNameScreen(void)
{
    // TODO: Unload TITLE screen variables here!
}

// Title Screen should finish?
int FinishNameScreen(void)
{
    return finishScreen;
}