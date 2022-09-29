/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Ending Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include <stdlib.h>>
#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static const int screenWidth = 1280;
static const int screenHeight = 720;
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------
static Font mono;

static Texture2D bg_credits;

static Music ending_music;

static void DrawTextN(const char *text, int posX, int posY, int fontSize, Color color) {
    DrawTextEx(mono, text, (Vector2) {posX, posY}, fontSize, 1, color);
}

// Ending Screen Initialization logic
void InitEndingScreen(void)
{
    // TODO: Initialize ENDING screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{
    // TODO: Update ENDING screen variables here!

    // Press enter or tap to return to TITLE screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    DrawTexture(bg_credits, 0, 0, DARKBLUE);
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return 1;
}


void credits_call(void)
{
    int i,num_cred;
    bool exit_flag = false;
    bool cred_finished_flag = false;
    char creditos[][50]={"CREDITOS","DESENVOLVIMENTO","Guilherme dos Santos Wisniewski 813319","Pedro Henrique Ghiotto 812115","Sergio Felipe Bezerra Rabelo 812205","BACKGROUND", "jeruu (itch.io)","black-canvas (itch.io)","SPRITES","craftpix.net","nyknck (itch.io)","frodoundead (itch.io)","ninjikin (itch.io)","MUSICA E SONS","Jonathan So","Cici Fyre","RAYLIB"};
    char cred_final[] = {"E OBRIGADO A VOCE POR JOGAR!"};
    Vector2 *measure_credits,*pos_credits;
    Vector2 cred_final_mea,cred_final_pos,offset_enter;
    mono = LoadFont("resources/Monocraft.otf");
    bg_credits = LoadTexture("resources/Pyramid.png");
    ending_music = LoadMusicStream("resources/NESYX.wav");
    PlayMusicStream(ending_music);
    num_cred = sizeof(creditos)/sizeof(creditos[0]);
    measure_credits = (Vector2*) malloc(num_cred*sizeof(Vector2));
    pos_credits = (Vector2*) malloc(num_cred*sizeof(Vector2));
    cred_final_mea = MeasureTextEx(mono, cred_final, 25, 1);
    cred_final_pos.x = -cred_final_mea.x;
    cred_final_pos.y = (screenHeight-cred_final_mea.y)/2;
    offset_enter = MeasureTextEx(mono, "PRESSIONE ENTER OU CLIQUE PARA VOLTAR AO MENU", 20, 1);
    for(i=0;i<num_cred;i++)
    {
        measure_credits[i] = MeasureTextEx(mono, creditos[i], 25, 1);
    }
    pos_credits[0].x = (screenWidth-measure_credits[0].x)/2;
    pos_credits[0].y = screenHeight;
    for(i=1;i<num_cred;i++)
    {
        pos_credits[i].x = (screenWidth-measure_credits[i].x)/2;
        pos_credits[i].y = pos_credits[i-1].y+measure_credits[i-1].y+20;
    }
    while(!exit_flag)
    {
        UpdateMusicStream(ending_music);
        if(WindowShouldClose())
        {
            exit(0);
        }
        if(cred_finished_flag)
        {
            if(IsKeyPressed(KEY_ENTER)||IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                exit_flag = 1;
            }
        }
        if(pos_credits[num_cred-1].y>-measure_credits[num_cred-1].y){
            for(i=0;i<num_cred;i++)
            {
                pos_credits[i].y -= 0.9f;
            }
        }
        else if (cred_final_pos.x<(screenWidth-cred_final_mea.x)/2)
        {
            cred_final_pos.x += 1.2f;
        }
        else
        {
            cred_finished_flag = 1;
        }
        BeginDrawing();
        DrawTexture(bg_credits, 0, 0, DARKBLUE);
        for(i=0;i<num_cred;i++)
        {
            
            DrawTextN(creditos[i], pos_credits[i].x, pos_credits[i].y, 25, GOLD);
        
        }
        DrawTextN(cred_final, cred_final_pos.x, cred_final_pos.y, 25, GOLD);
        if(cred_finished_flag)
        {
            DrawTextN("PRESSIONE ENTER OU CLIQUE PARA VOLTAR AO MENU", (screenWidth-offset_enter.x)/2, screenHeight-offset_enter.y-20, 20, GOLD);
        }
        EndDrawing();
    }
    UnloadFont(mono);
    UnloadTexture(bg_credits);
    UnloadMusicStream(ending_music);
    FinishEndingScreen();
}
