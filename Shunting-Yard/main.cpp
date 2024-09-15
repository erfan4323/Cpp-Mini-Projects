#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <raylib.h>

#include "lib/EvalCal.h"

void Solver(std::string exp, char* answer);

void GUILayout(Rectangle  layoutRecs[7], char  expression[128], bool& expressionEditMode, char  greet[128], char  answer[128]);

int main()
{
    int screenWidth = 264;
    int screenHeight = 550;

    InitWindow(screenWidth, screenHeight, "Shunt");
    GuiLoadStyle("style_bluish.rgs");
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    bool expressionEditMode = false;
    char expression[128] = "";
    char greet[128] = "Greetings!!!\nThis is the Shunting Yard \nalgorithm demonstration.\nEnter your expression below...";
    char answer[128] = "";

    Rectangle layoutRecs[8] = {
        Rectangle{0, 0, 264, 600},
        Rectangle{24, 50 - 33, 216, 216},
        Rectangle{24, 288 - 33, 216, 96},
        Rectangle{24, 408 - 33, 216, 96},
        Rectangle{48, 528 - 33, 168, 48},
        Rectangle{24 + 8, 288 - 33 + 8, 200, 80},
        Rectangle{24 + 8, 50 - 33 + 8, 200, 200},
        Rectangle{24 + 8, 408 - 33 + 8, 200, 80},
    };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        //GUILayout(layoutRecs, expression, expressionEditMode, greet, answer);

        if (GuiTextBox(layoutRecs[5], expression, 128, expressionEditMode)) expressionEditMode = !expressionEditMode;
        if (GuiButton(layoutRecs[4], "Solve")) Solver(std::string(expression), answer);
        GuiTextBox(layoutRecs[6], greet, 128, false);
        GuiTextBox(layoutRecs[7], answer, 128, false);
        GuiGroupBox(layoutRecs[1], "Greetings");
        GuiGroupBox(layoutRecs[2], "Expression");
        GuiGroupBox(layoutRecs[3], "Answer");

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void GUILayout(Rectangle  layoutRecs[7], char  expression[128], bool& expressionEditMode, char  greet[128], char  answer[128])
{
    if (GuiTextBox(layoutRecs[5], expression, 128, expressionEditMode)) expressionEditMode = !expressionEditMode;
    if (GuiButton(layoutRecs[4], "Button")) Solver(std::string(expression), answer);
    GuiTextBox(layoutRecs[6], greet, 128, false);
    GuiTextBox(layoutRecs[7], answer, 128, false);
    GuiGroupBox(layoutRecs[1], "Greetings");
    GuiGroupBox(layoutRecs[2], "Expression");
    GuiGroupBox(layoutRecs[3], "Answer");
}

void Solver(std::string exp, char* answer)
{
    // if (exp.compare("")) return;

    // double result = Calculate(Evaluate(exp)); // Assuming Calculate returns a double

    // std::ostringstream sstream;
    // sstream << result;
    // std::string resultStr = sstream.str();

    // std::cout << result << std::endl;

    // strncpy(answer, resultStr.c_str(), 128);
    // answer[127] = '\0'; // Ensure null-termination
}

