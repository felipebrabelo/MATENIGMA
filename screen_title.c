/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Title Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "screens.h"

static Font mono;

static Texture2D bg_title;
static Texture2D bg_novojogo;

static Music title_music;
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static const int screenWidth = 1280;
static const int screenHeight = 720;
//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

static void DrawTextR(const char *text, int posX, int posY, int fontSize, Color color) {
    DrawTextEx(mono, text, (Vector2) {posX, posY}, fontSize, 1, color);
}

// Title Screen Initialization logic

int FinishTitleScreen(void)
{
    return finishScreen;
}

void SelecionaBotao(Rectangle *bt_rec,Vector2 mouse)
{
    int i;
    bool selec[3] = {false,false,false};
    for(i=0;i<3;i++)
    {
        if(CheckCollisionPointRec(mouse, bt_rec[i]))
        {
            DrawRectangleLinesEx(bt_rec[i], 2, RED);
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                selec[i] = true;
            }
        }
    }
    if(selec[0])
    {
        finishScreen = 1;
    }
    else if (selec[1])
    {
        finishScreen = 2;
    }
    else if (selec[2])
    {
        finishScreen = 3;
    }
}

void NovoJogoTela(void)
{
    bool exit_flag = false;
    char enter[] = "PRESSIONE ENTER OU CLIQUE PARA COME??AR!";
    char narrativa[] = "\tVoc?? ?? uma famosa arque??loga em busca da sua pr??xima descoberta, e por isso visita a misteriosa pir??mide de Maahtotep. Dentro dela, voc?? descobre uma passagem\n\tsecreta que te leva a uma ??rea nunca vista, mas antes que possa explorar, uma esfinge se coloca no meio do caminho de diz: Voc?? que ousa desvendar os segredos\n\tmais antigos deve primeiro pelos desafios avan??ar!.\n\n\tV??rias t??buas aparecem na sua frente e as instru????es s??o ditas:\n\n\t1 - Clique em uma t??bua para vir??-la.\n\n\t2 - As t??buas possuem express??es matem??ticas e suas repostas.\n\n\t3 - Procure virar duas t??buas que correspondem ao par: express??o e sua resposta.\n\n\t 4 - N??o existem respostas repetidas.";
    int offset = MeasureText(enter, 15);
    while(!exit_flag)
    {
        UpdateMusicStream(title_music);
        if(WindowShouldClose())
        {
            exit(1);
        }
        if(IsKeyPressed(KEY_ENTER)||IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            exit_flag = true;
        }
        BeginDrawing();
        DrawTexture(bg_novojogo, 0, 0, DARKBLUE);
        DrawText(narrativa, 10, 100, 15, GOLD);
        DrawText(enter, (screenWidth-offset)/2, screenHeight-20, 15, GOLD);
        EndDrawing();
    }
}


void title_call(void)
{
   
    finishScreen = 0;

    Vector2 mouse = {0.0f,0.0f};
    
    mono = LoadFont("resources/Monocraft.otf");
    bg_title = LoadTexture("resources/Sphinx.png");
    bg_novojogo = LoadTexture("resources/newgame.png");
    title_music = LoadMusicStream("resources/FieldsofIce.wav");

    
    Vector2 offset_title = MeasureTextEx(mono,"ENIGMATE", 100, 1);
    Vector2 offset_bt1 = MeasureTextEx(mono, "NOVO JOGO", 25, 1);
    Vector2 offset_bt2 = MeasureTextEx(mono, "RANKING", 25, 1);
    Vector2 offset_bt3 = MeasureTextEx(mono, "CREDITOS", 25, 1);
    Rectangle bt_rec[3]={{(screenWidth-offset_bt1.x*1.25)/2,200+offset_title.y-(offset_bt1.y*1.25)/8,offset_bt1.x*1.25,offset_bt1.y*1.25},
        {(screenWidth-offset_bt2.x*1.25)/2,250+offset_title.y+offset_bt1.y-offset_bt2.y*1.25/8,offset_bt2.x*1.25,offset_bt2.y*1.25},
        {(screenWidth-offset_bt3.x*1.25)/2,300+offset_title.y+offset_bt1.y+offset_bt2.y-offset_bt3.y*1.25/8,offset_bt3.x*1.25,offset_bt3.y*1.25}
    };

    PlayMusicStream(title_music);
    while(!finishScreen)
    {
        UpdateMusicStream(title_music);
        mouse.x = (float) GetMouseX();
        mouse.y = (float) GetMouseY();
        if(WindowShouldClose())
        {
            exit(0);
        }
        //---------------------
        BeginDrawing();
        DrawTexture(bg_title,0,0, DARKBLUE);
        DrawRectangleRec(bt_rec[0], BLANK);
        DrawRectangleRec(bt_rec[1], BLANK);
        DrawRectangleRec(bt_rec[2], BLANK);
        DrawTextR("ENIGMATE", screenWidth/2-offset_title.x/2, 100, 100, GOLD);
        DrawTextR("NOVO JOGO", (screenWidth-offset_bt1.x)/2, 200+offset_title.y, 25, GOLD);
        DrawTextR("RANKING", (screenWidth-offset_bt2.x)/2, 250+offset_title.y+offset_bt1.y, 25, GOLD);
        DrawTextR("CREDITOS", (screenWidth-offset_bt3.x)/2, 300+offset_title.y+offset_bt1.y+offset_bt2.y, 25, GOLD);
        SelecionaBotao(bt_rec, mouse);
        EndDrawing();
    }
    if(finishScreen == 1)
    {
        NovoJogoTela();
    }
    UnloadFont(mono);
    UnloadTexture(bg_title);
    UnloadTexture(bg_novojogo);
    UnloadMusicStream(title_music);
}
