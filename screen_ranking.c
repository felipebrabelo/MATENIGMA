#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

static const int screenWidth_r = 1280;
static const int screenHeight_r = 720;
static int finishScreen = 0;

static Font mono;

static Texture2D bg_ranking;
static Music musica_complete;

typedef struct {
    double pontos;
    char nome[11];
} Ranking;

static void DrawTextN(const char *text, int posX, int posY, int fontSize, Color color) {
    DrawTextEx(mono, text, (Vector2) {posX, posY}, fontSize, 1, color);
}

static void CaixaTexto(Ranking *novo) {
    char name[11]="\0";
    int letterCount = 0;
    bool exit_flag = false;
    while(!exit_flag) {
        if(WindowShouldClose())
        {
            exit(0);
        }
        UpdateMusicStream(musica_complete);
        if(IsKeyPressed(KEY_ENTER)||IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            exit_flag = true;
        }
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < 10)) {
                name[letterCount] = (char) key;
                name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                letterCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = '\0';
        }


        //Draw
        Vector2 offset = MeasureTextEx(mono,name,30,1);
        Vector2 offset_msg = MeasureTextEx(mono, "Parabéns, seu score esta entre os 10 melhores!", 20, 1);
        Vector2 offset_enter = MeasureTextEx(mono, "Digite seu nome e pressione ENTER ou clique para continuar.", 20, 1);
        BeginDrawing();
        DrawTexture(bg_ranking, 0, 0, DARKBLUE);
        DrawRectangleLines((screenWidth_r-230)/2,(screenHeight_r-1.5*offset.y)/2,230,offset.y*1.5,GOLD);
        DrawTextN(name,(screenWidth_r-offset.x)/2,(screenHeight_r-offset.y)/2,30,GOLD);
        DrawTextN("Parabens, seu score esta entre os 10 melhores!", (screenWidth_r-offset_msg.x)/2, (screenHeight_r-1.5*offset.y)/2-offset_msg.y-20, 20, GOLD);
        DrawTextN("Digite seu nome e pressione ENTER ou clique para continuar.", (screenWidth_r-offset_enter.x)/2, screenHeight_r/2+offset.y*1.5+20, 20, GOLD);
        EndDrawing();
    }
    strcpy(novo->nome,name);
}

static int compara(const void *a, const void *b) {
    Ranking *r1 = (Ranking *) a;
    Ranking *r2 = (Ranking *) b;
    return ((int) (r2->pontos - r1->pontos));
}

static void AtualizaRanking(FILE *rank, Ranking *lista) {
    Ranking novo;
    rewind(rank);
    fread(&novo, sizeof(Ranking), 1, rank);
    fread(lista, sizeof(Ranking), 10, rank);
    lista[10] = novo;
    qsort(lista, 11, sizeof(Ranking), compara);
    if(lista[10].nome!=novo.nome&&lista[10].pontos!=novo.pontos)
    {
        rewind(rank);
        fread(&novo, sizeof(Ranking), 1, rank);
        fread(lista, sizeof(Ranking), 10, rank);
        CaixaTexto(&novo);
        lista[10] = novo;
        qsort(lista, 11, sizeof(Ranking), compara);
    }
    fseek(rank, sizeof(Ranking), SEEK_SET);
    fwrite(lista, sizeof(Ranking), 10, rank);
    fclose(rank); //Fechar para garantir que o arquivo atualize
}

static void LeRanking(FILE *rank, Ranking *lista) {
    fseek(rank, sizeof(Ranking), SEEK_SET);
    fread(lista, sizeof(Ranking), 10, rank);
}

static void CriaStringRanking(Ranking *lista, char ranking_string[][41], int num_linhas) {
    int i, j;
    for (i = 0; i < num_linhas; i++) {
        if (strlen(lista[i].nome) < 10) {
            for (j = strlen(lista[i].nome); j < 10; j++) {
                lista[i].nome[j] = ' ';
            }
            lista[i].nome[10] = '\0';
        }
        if (i < num_linhas - 1) {
            sprintf(ranking_string[i], "%i \t%s\t%.0lf", (i + 1), lista[i].nome, lista[i].pontos);
        } else {
            sprintf(ranking_string[i], "%i\t%s\t%.0lf", (i + 1), lista[i].nome, lista[i].pontos);
        }
    }
}

static void GeraPosicaoTextoRanking(char ranking_string[][41], int num_linhas, Vector2 *pos_texto_ranking) {

    int i;
    Vector2 offset = MeasureTextEx(mono, ranking_string[0], 30, 1);
    for (i = 0; i < num_linhas; i++) {
        pos_texto_ranking[i].x = (screenWidth_r - offset.x) / 2;
        pos_texto_ranking[i].y = 230 + offset.y * i;
    }

}



int FinishRankingScreen(void)
{
    return finishScreen;
}

static FILE *fopen_e_teste(FILE *f,char *caminho, char*modo)
{
    FILE *f_return;
    f_return = fopen(caminho, modo);
    if(f_return  == NULL)
    {
        perror("Erro na abertura do arquivo");
        exit(1);
    }
    return f_return;
}

void ranking_call(void) {
    finishScreen = 0;
    int i;
    FILE *rank;
    Ranking lista[11], lista_in[10];
    char ranking_string[10][41];
    Vector2 pos_texto_ranking[10];
    mono = LoadFont("resources/Monocraft.otf");
    bg_ranking = LoadTexture("resources/Dune4.png");
    musica_complete = LoadMusicStream("resources/Sunstrider.wav");
    
    Vector2 offset_ranking = MeasureTextEx(mono, "RANKING", 80, 1);
    Vector2 offset_enter = MeasureTextEx(mono, "Pressione a tecla ENTER ou clique para continuar.", 20, 1);
     
    PlayMusicStream(musica_complete);
    fopen_e_teste(rank, "resources/ranking.bin", "r+b");
    AtualizaRanking(rank, lista);
    fopen_e_teste(rank, "resources/ranking.bin", "rb");;
    LeRanking(rank, lista_in);
    fclose(rank);
    CriaStringRanking(lista_in, ranking_string, 10);
    GeraPosicaoTextoRanking(ranking_string, 10, pos_texto_ranking);
    while (!finishScreen) {
        UpdateMusicStream(musica_complete);
        if(IsKeyPressed(KEY_ENTER))
        {
            finishScreen = 1;
        }
        if(WindowShouldClose())
        {
            exit(0);
        }
        BeginDrawing();
        DrawTexture(bg_ranking, 0, 0, DARKBLUE);
        DrawTextN("RANKING", (screenWidth_r-offset_ranking.x)/2, 100, 80, GOLD);
        DrawTextN("Pressione a tecla ENTER ou clique para continuar.", (screenWidth_r-offset_enter.x)/2, screenHeight_r-offset_ranking.y-10, 20, GOLD);
        for (i = 0; i < 10; i++) {
            DrawTextN(ranking_string[i], pos_texto_ranking[i].x, pos_texto_ranking[i].y, 30, GOLD);
        }
        EndDrawing();
    }
    UnloadFont(mono);
    FinishRankingScreen();
}

void ranking_call_menu(void)
{
    finishScreen = 0;
    int i;
    FILE *rank;
    Ranking lista[10];
    char ranking_string[10][41];
    Vector2 pos_texto_ranking[10];
    
    
    mono = LoadFont("resources/Monocraft.otf");
    bg_ranking = LoadTexture("resources/Dune4.png");
    musica_complete = LoadMusicStream("resources/Sunstrider.wav");
    
    Vector2 offset_ranking = MeasureTextEx(mono, "RANKING", 80, 1);
    Vector2 offset_enter = MeasureTextEx(mono, "Pressione a tecla ENTER ou clique para voltar ao menu.", 20, 1);
    
    PlayMusicStream(musica_complete);
    
    rank = fopen_e_teste(rank,"resources/ranking.bin", "rb");
    LeRanking(rank, lista);
    CriaStringRanking(lista,ranking_string,10);
    GeraPosicaoTextoRanking(ranking_string, 10, pos_texto_ranking);
    
    while(!finishScreen)
    {
        UpdateMusicStream(musica_complete);
        if(WindowShouldClose())
        {
            exit(0);
        }
        if(IsKeyPressed(KEY_ENTER)||IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            finishScreen = 2;
        }
        BeginDrawing();
        DrawTexture(bg_ranking, 0, 0, DARKBLUE);
        DrawTextN("RANKING", (screenWidth_r-offset_ranking.x)/2, 100, 80, GOLD);
        for (i = 0; i < 10; i++) {
            DrawTextN(ranking_string[i], pos_texto_ranking[i].x, pos_texto_ranking[i].y, 30, GOLD);
        }
        DrawTextN("Pressione a tecla ENTER ou clique para voltar ao menu.", (screenWidth_r-offset_enter.x)/2, screenHeight_r-offset_ranking.y-10, 20, GOLD);
        EndDrawing();
        
    }
    UnloadFont(mono);
    UnloadMusicStream(musica_complete);
    UnloadTexture(bg_ranking);
    FinishRankingScreen();
}
