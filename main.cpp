#include "raylib.h"
#include <iostream>

using namespace std;

int screenHeight;
int screenWidth;

int ballRadius = 10;
Vector2 ballPosition;
Vector2 ballSpeed;

Vector2 leftPaddlePosition;
Vector2 rightPaddlePosition;

const int paddleWidth = 10;
const int paddleHeight = 80;
const int paddleSpeed = 5;

int leftPlayerScore = 0;
int rightPlayerScore = 0;


void ResetBall() {
    ballPosition = { static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2) };
    ballSpeed = { 2, 3 };
}


int main() {
    cout << "Hello Worrrld" << endl;

    //Font ft = LoadFont("resources/fonts/setback.png");

    screenHeight = 600;
    screenWidth = 900;

    InitWindow(screenWidth, screenHeight, "Pong Ultimate");
    SetTargetFPS(60);

    // Ball position & speed
    ballPosition = { static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2) };
    ballSpeed = { 2,3};


    // Initialize paddle positions
    ResetBall();
    leftPaddlePosition = { 10, static_cast<float>(screenHeight / 2 - paddleHeight / 2) };
    rightPaddlePosition = { static_cast<float>(screenWidth - paddleWidth - 10), static_cast<float>(screenHeight / 2 - paddleHeight / 2) };


    while (!WindowShouldClose()) {
        // Update
        ballPosition.x += ballSpeed.x;
        ballPosition.y += ballSpeed.y;

        // Bounce off the walls
        if ((ballPosition.y + ballRadius > screenHeight) || (ballPosition.y - ballRadius < 0)) {
            ballSpeed.y = -ballSpeed.y;
        }

        // Bounce off the paddles
        if (CheckCollisionCircleRec(ballPosition, ballRadius, { leftPaddlePosition.x, leftPaddlePosition.y, static_cast<float>(paddleWidth), static_cast<float>(paddleHeight) }) ||
            CheckCollisionCircleRec(ballPosition, ballRadius, { rightPaddlePosition.x, rightPaddlePosition.y, static_cast<float>(paddleWidth), static_cast<float>(paddleHeight) })) {
            ballSpeed.x = -ballSpeed.x;
        }

        // Score points
        if (ballPosition.x - ballRadius < 0) {
            // Right player scores a point
            rightPlayerScore++;
            ResetBall();
        }
        else if (ballPosition.x + ballRadius > screenWidth) {
            // Left player scores a point
            leftPlayerScore++;
            ResetBall();
        }



        // Control paddles
        if (IsKeyDown(KEY_W) && leftPaddlePosition.y > 0) {
            leftPaddlePosition.y -= paddleSpeed;
        }
        if (IsKeyDown(KEY_S) && (leftPaddlePosition.y + paddleHeight) < screenHeight) {
            leftPaddlePosition.y += paddleSpeed;
        }

        if (IsKeyDown(KEY_UP) && rightPaddlePosition.y > 0) {
            rightPaddlePosition.y -= paddleSpeed;
        }
        if (IsKeyDown(KEY_DOWN) && (rightPaddlePosition.y + paddleHeight) < screenHeight) {
            rightPaddlePosition.y += paddleSpeed;
        }











        // Draw

        BeginDrawing();
        ClearBackground(DARKBLUE);

        // Draw ball
        DrawCircleV(ballPosition, ballRadius, WHITE);



        // Draw paddles
        DrawRectangleV(leftPaddlePosition, { static_cast<float>(paddleWidth), static_cast<float>(paddleHeight) }, WHITE);
        DrawRectangleV(rightPaddlePosition, { static_cast<float>(paddleWidth), static_cast<float>(paddleHeight) }, WHITE);


        // Draw text

        //DrawTextEx(ft, "Pong Ultimate", ballPosition, 30,20,WHITE);
        DrawText("Pong        Ultimate", 325,10, 30,WHITE);
        

        // Drawing three lines to make them thicker

        for (int i = 0; i < screenHeight; i += 15) {
            DrawLineV({ static_cast<float>(screenWidth / 2), static_cast<float>(i) },
                { static_cast<float>(screenWidth / 2), static_cast<float>(i + 10) }, WHITE);
        }

        for (int i = 0; i < screenHeight; i += 15) {
            DrawLineV({ static_cast<float>((screenWidth / 2)+1), static_cast<float>(i) },
                { static_cast<float>((screenWidth / 2)+1), static_cast<float>(i + 10) }, WHITE);
        }

        for (int i = 0; i < screenHeight; i += 15) {
            DrawLineV({ static_cast<float>((screenWidth / 2) - 1), static_cast<float>(i) },
                { static_cast<float>((screenWidth / 2) - 1), static_cast<float>(i + 10) }, WHITE);
        }


        // Draw scores
        DrawText(TextFormat("%d", leftPlayerScore), screenWidth / 4, 10, 30, WHITE);
        DrawText(TextFormat("%d", rightPlayerScore), screenWidth * 3 / 4 - MeasureText(TextFormat("%d", rightPlayerScore), 30), 10, 30, WHITE);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}



