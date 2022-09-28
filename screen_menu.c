#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
Texture2D Background;
//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitMenuScreen(void)
{
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    Background = LoadTexture("backgroundsand-min.png");
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

    int BackgroundX = 0;
    int BackgroundY = 0;
    Vector2 BackgroundPosition = { BackgroundX, BackgroundY };
 
    Rectangle BackgroundFrameRec = { 0.0f,0.0f, (float)Background.width, (float) Background.height };
    DrawTextureRec(Background, BackgroundFrameRec, BackgroundPosition, WHITE);


    if (button_game_state == 1)     DrawRectangle((1280 / 2) - 80, 300, button_width, button_height, RED);
    else if (button_game_state == 2) DrawRectangle((1280 / 2) - 80, 300, button_width, button_height, GREEN);

    if (button_instru_state == 1)    DrawRectangle((1280 / 2) - 80, 410, button_width, button_height, RED);
    else if (button_instru_state == 2) DrawRectangle((1280 / 2) - 80, 410, button_width, button_height, GREEN);

    DrawTextEx(font, "MENU SCREEN", (Vector2){ 20, 10 }, font.baseSize*3.0f, 4, DARKGREEN);
    DrawText("INICIAR", (1280 / 2) - 66, 328, 40, BLACK);
    DrawText("DICAS", (1280 / 2) - 64, 438, 40, BLACK);

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