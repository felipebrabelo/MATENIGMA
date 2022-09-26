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

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitMenuScreen(void)
{
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Title Screen Update logic
void UpdateMenuScreen(void)
{
    // TODO: Update TITLE screen variables here!

    // Press enter or tap to change to GAMEPLAY screen
    float button_width = 180;
    float button_height = 90;
    Vector2 mousePoint = { 0.0f, 0.0f };
    Rectangle button_game_bound = { (1280/2)-80, 300, button_width, button_height };
    Rectangle button_instru_bound = { (1280 / 2) - 80, 410, button_width, button_height };
    mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, button_game_bound)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            finishScreen = 1;
            PlaySound(fxCoin);
        }
    }
    if (CheckCollisionPointRec(mousePoint, button_instru_bound)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            finishScreen = 2;
            PlaySound(fxCoin);
        }
    }
 /*   else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        finishScreen = 2;
        PlaySound(fxCoin);
    }*/
}

// Title Screen Draw logic
void DrawMenuScreen(void)
{
    Vector2 mousePoint = { 0.0f, 0.0f };
    mousePoint = GetMousePosition();

    // TODO: Draw TITLE screen here!
    int button_game_state = 1;
    int button_instru_state = 1;
    float button_width = 180;
    float button_height = 90;
    Rectangle button_game_bound = { (1280 / 2) - 80, 300, button_width, button_height };
    Rectangle button_instru_bound = { (1280 / 2) - 80, 410, button_width, button_height };
    if (CheckCollisionPointRec(mousePoint, button_game_bound)) {
        button_game_state = 2;
    }
    else button_game_state = 1;

    if (CheckCollisionPointRec(mousePoint, button_instru_bound)) {
        button_instru_state = 2;
    }
    else button_instru_state = 1;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), YELLOW);
    if (button_game_state == 1)     DrawRectangle((1280 / 2) - 80, 300, button_width, button_height, RED);
    else if (button_game_state == 2) DrawRectangle((1280 / 2) - 80, 300, button_width, button_height, GREEN);

    if (button_instru_state == 1)    DrawRectangle((1280 / 2) - 80, 410, button_width, button_height, RED);
    else if (button_instru_state == 2) DrawRectangle((1280 / 2) - 80, 410, button_width, button_height, GREEN);

    DrawTextEx(font, "MENU SCREEN", (Vector2){ 20, 10 }, font.baseSize*3.0f, 4, DARKGREEN);
    DrawText("INICIAR", (1280 / 2) - 66, 328, 40, BLACK);
    DrawText("DICAS", (1280 / 2) - 64, 438, 40, BLACK);
    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
}

// Title Screen Unload logic
void UnloadMenuScreen(void)
{
    // TODO: Unload TITLE screen variables here!
}

// Title Screen should finish?
int FinishMenuScreen(void)
{
    return finishScreen;
}