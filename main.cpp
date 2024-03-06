#include "raylib.h"
#include <iostream>
#include <vector>

using namespace std;



enum Scene {
    MENU,
    GAME,
    SCORE,
    AI,
    SCOREAI,
};

float screenHeight;
float screenWidth;

int ballRadius = 10;
Vector2 ballPosition;
Vector2 ballSpeed;

Vector2 leftPaddlePosition;
Vector2 rightPaddlePosition;

Vector2 titlePosition{ 220,20 };

const int paddleWidth = 10;
const int paddleHeight = 80;
const int paddleSpeed = 5.7;

int leftPlayerScore = 0;
int rightPlayerScore = 0;

Scene currentScene = MENU;







class Button {
public:
    Rectangle bounds;
    std::string text;
    Scene sceneID;
    Texture2D image;  // New member for the button image

    
    Button(Vector2 position, Vector2 size, std::string text, Scene sceneID, Texture2D image) {
        bounds = { position.x, position.y, size.x, size.y };
        this->text = text;
        this->sceneID = sceneID;
        this->image = image;
    }

    bool isClicked() const {
        return CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    }

    void draw() const {
        // Draw if available found, otherwise draw white rectangle (but it works yay)
        if (image.id > 0) {
            DrawTextureRec(image, { 0, 0, static_cast<float>(image.width), static_cast<float>(image.height) }, { bounds.x, bounds.y }, WHITE);

        }
        else {
            DrawRectangleRec(bounds, WHITE);
            DrawText(text.c_str(), static_cast<int>(bounds.x + bounds.width / 4), static_cast<int>(bounds.y + bounds.height / 4), 20, BLACK);
        }
    }

};


std::vector<Button> buttons;

void launchScene(Scene newScene) {
    currentScene = newScene; 
}

// check for button clicks and launch corresponding scenes
void checkButtonClicks() {
    for (const Button& button : buttons) {
        if (button.isClicked()) {
            launchScene(button.sceneID);
            break; // Stop checking buttons after
        }
    }
}

void ResetBall() {
    ballPosition = { static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2) };

    // Generate a random angle between 45 and 50 degrees (so the ball doesn't always start with the same path)
    float angle = GetRandomValue(45, 60);

    // Calculate the corresponding x and y components of the speed
    ballSpeed.x = cos(angle * DEG2RAD) * 10;
    ballSpeed.y = sin(angle * DEG2RAD) * 10;
}

int main() {
    cout << "Hello Worrrld" << endl;



    Font ft = LoadFont("resources/fonts/jupiter_crash");

    screenHeight = 600;
    screenWidth = 800;

    InitWindow(screenWidth, screenHeight, "Pong Ultimate");
    SetTargetFPS(60);

    // Ball position & speed
    ballPosition = { static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2) };
    ballSpeed = { 2,3};


    // Initialize paddle positions
    ResetBall();

    leftPaddlePosition = { 10, static_cast<float>(screenHeight / 2 - paddleHeight / 2) };
    rightPaddlePosition = { static_cast<float>(screenWidth - paddleWidth - 30), static_cast<float>(screenHeight / 2 - paddleHeight / 2) };


    // Create buttons

    Image img = LoadImage("Images/Courtdetennis.png");

    Image Solo = LoadImage("Images/DUO.png");

    Image Versus = LoadImage("Images/SOL.png");


    Image ballImage = LoadImage("Images/bal.png");

    Texture2D ballTexture = LoadTextureFromImage(ballImage);

    UnloadImage(ballImage);

    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    Texture2D textureSolo = LoadTextureFromImage(Solo);
    UnloadImage(Solo);

    Texture2D textureVersus = LoadTextureFromImage(Versus);
    UnloadImage(Versus);


    //images of the paddles

    Image leftPaddleImage = LoadImage("Images/padl.png");
    Image rightPaddleImage = LoadImage("Images/padl.png");

    Texture2D leftPaddleTexture = LoadTextureFromImage(leftPaddleImage);
    Texture2D rightPaddleTexture = LoadTextureFromImage(rightPaddleImage);

    UnloadImage(leftPaddleImage);
    UnloadImage(rightPaddleImage);





    // Create buttons
    buttons.push_back(Button({ 270, 150 }, { 200, 100 }, "Solo Mode", GAME, textureSolo));


    buttons.push_back(Button({ 270, 300 }, { 200, 100 }, "VS Mode", AI, textureVersus));


    while (!WindowShouldClose()) {

        switch (currentScene) {
        case MENU:

            // Draw menu
            BeginDrawing();
            ClearBackground(DARKBLUE);

            DrawTextureEx(texture, { 0, 0 }, 0, 0.302, WHITE);

            DrawTextEx(ft, "Pong Ultimate", titlePosition, 40, 15, WHITE);


            //DrawText("Press any key to start", (screenWidth / 4)+100, screenHeight / 2, 20, WHITE);

            // Draw menu buttons
            for (const Button& button : buttons) {
                button.draw();
            }

            leftPlayerScore = 0;
            rightPlayerScore = 0;


            EndDrawing();

            // Check for key press to start the game
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_D)) {

                leftPlayerScore = 0;
                rightPlayerScore = 0;

                currentScene = GAME;

            }

            if (IsKeyPressed(KEY_S)) {

                currentScene = AI;
            }

            checkButtonClicks();

            break;


        case GAME:

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

            DrawTextureEx(texture, { 0, 0 }, 0, 0.302, WHITE);

            // Draw ball (simple circle)
            //DrawCircleV(ballPosition, ballRadius, WHITE);

            // Draw ball image
            DrawTexture(ballTexture, static_cast<int>(ballPosition.x - ballRadius), static_cast<int>(ballPosition.y - ballRadius), WHITE);


            // Draw paddles (rectangles at first)
            //DrawRectangleV(leftPaddlePosition, { static_cast<float>(paddleWidth), static_cast<float>(paddleHeight) }, WHITE);
            //DrawRectangleV(rightPaddlePosition, { static_cast<float>(paddleWidth), static_cast<float>(paddleHeight) }, WHITE);


            // Draw paddles images
            DrawTexture(leftPaddleTexture, static_cast<int>(leftPaddlePosition.x), static_cast<int>(leftPaddlePosition.y), WHITE);
            DrawTexture(rightPaddleTexture, static_cast<int>(rightPaddlePosition.x), static_cast<int>(rightPaddlePosition.y), WHITE);



            // Draw text (abandonned)
            //DrawTextEx(ft, "Pong Ultimate", titlePosition, 30, 10, WHITE);
            //Previous text code idea
            //DrawText("Pong        Ultimate", 325,10, 30,WHITE);
            // Drawing three lines to make them thicker

            for (int i = 0; i < screenHeight; i += 15) {
                DrawLineV({ static_cast<float>(screenWidth / 2), static_cast<float>(i) },
                    { static_cast<float>(screenWidth / 2), static_cast<float>(i + 10) }, WHITE);
            }

            for (int i = 0; i < screenHeight; i += 15) {
                DrawLineV({ static_cast<float>((screenWidth / 2) + 1), static_cast<float>(i) },
                    { static_cast<float>((screenWidth / 2) + 1), static_cast<float>(i + 10) }, WHITE);
            }

            for (int i = 0; i < screenHeight; i += 15) {
                DrawLineV({ static_cast<float>((screenWidth / 2) - 1), static_cast<float>(i) },
                    { static_cast<float>((screenWidth / 2) - 1), static_cast<float>(i + 10) }, WHITE);
            }


            // Draw scores
            DrawTextEx(ft, TextFormat("%d", leftPlayerScore), { screenWidth / 4, 10 }, 30, 2, WHITE);
            DrawTextEx(ft, TextFormat("%d", rightPlayerScore), { screenWidth * 3 / 4 - MeasureText(TextFormat("%d", rightPlayerScore), 30), 10 }, 30, 2, WHITE);

            EndDrawing();

            // Check for key press to start the game
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
                currentScene = MENU;
            }

            if (leftPlayerScore == 10 || rightPlayerScore == 10) {
                currentScene = SCORE;
            }

            break;


        case SCORE:
            
            // Draw menu
            BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawTextEx(ft, "Pong Ultimate", titlePosition, 30, 15, WHITE);

            if (leftPlayerScore == 10) {

                DrawText("Left", (screenWidth / 4) + 50, screenHeight / 2, 20, WHITE);

            }

            if (rightPlayerScore == 10) {

                DrawText("Right", (screenWidth / 4) + 40, screenHeight / 2, 20, WHITE);

            }

            DrawText("player wins ! Congrats !", (screenWidth / 4) + 100, screenHeight / 2, 20, WHITE);


            DrawText("Press SPACE to restart :)", (screenWidth / 4) + 60, (screenHeight / 2)+50, 20, WHITE);
            DrawText("Press P to get back to menu", (screenWidth / 4) + 50, (screenHeight / 2) + 150, 20, WHITE);

            EndDrawing();

            // Check if the player wants to replay the game
            if (IsKeyPressed(KEY_SPACE)) {

                leftPlayerScore = 0;
                rightPlayerScore = 0;

                currentScene = GAME;
            }

            // Check if the player wants to go back to the menu
            if (IsKeyPressed(KEY_P)) {

                currentScene = MENU;
            }

            break;

        case AI:

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

            // AI controls
            // Simple AI: Match the height of the ball

            if (rightPaddlePosition.y + paddleHeight / 2 < ballPosition.y) {
                rightPaddlePosition.y += paddleSpeed;
            }
            else if (rightPaddlePosition.y + paddleHeight / 2 > ballPosition.y) {
                rightPaddlePosition.y -= paddleSpeed;
            }

            // Draw
            BeginDrawing();
            ClearBackground(DARKBLUE);

            DrawTextureEx(texture, { 0, 0 }, 0, 0.302, WHITE);

            // Draw ball
            //DrawCircleV(ballPosition, ballRadius, WHITE);

            // Draw ball image
            DrawTexture(ballTexture, static_cast<int>(ballPosition.x - ballRadius), static_cast<int>(ballPosition.y - ballRadius), WHITE);

            // Draw paddles
            //DrawRectangleV(leftPaddlePosition, { static_cast<float>(paddleWidth), static_cast<float>(paddleHeight) }, WHITE);
            //DrawRectangleV(rightPaddlePosition, { static_cast<float>(paddleWidth), static_cast<float>(paddleHeight) }, WHITE);


            // Draw paddles images
            DrawTexture(leftPaddleTexture, static_cast<int>(leftPaddlePosition.x), static_cast<int>(leftPaddlePosition.y), WHITE);
            DrawTexture(rightPaddleTexture, static_cast<int>(rightPaddlePosition.x), static_cast<int>(rightPaddlePosition.y), WHITE);

            // Draw text
            //DrawTextEx(ft, "Pong Ultimate", titlePosition, 30, 10, WHITE);
            //Previous text code idea
            //DrawText("Pong        Ultimate", 325,10, 30,WHITE);
            // Drawing three lines to make them thicker

            for (int i = 0; i < screenHeight; i += 15) {
                DrawLineV({ static_cast<float>(screenWidth / 2), static_cast<float>(i) },
                    { static_cast<float>(screenWidth / 2), static_cast<float>(i + 10) }, WHITE);
            }

            for (int i = 0; i < screenHeight; i += 15) {
                DrawLineV({ static_cast<float>((screenWidth / 2) + 1), static_cast<float>(i) },
                    { static_cast<float>((screenWidth / 2) + 1), static_cast<float>(i + 10) }, WHITE);
            }

            for (int i = 0; i < screenHeight; i += 15) {
                DrawLineV({ static_cast<float>((screenWidth / 2) - 1), static_cast<float>(i) },
                    { static_cast<float>((screenWidth / 2) - 1), static_cast<float>(i + 10) }, WHITE);
            }

            // Draw scores
            DrawTextEx(ft, TextFormat("%d", leftPlayerScore), { screenWidth / 4, 10 }, 30, 2, WHITE);
            DrawTextEx(ft, TextFormat("%d", rightPlayerScore), { screenWidth * 3 / 4 - MeasureText(TextFormat("%d", rightPlayerScore), 30), 10 }, 30, 2, RED);

            EndDrawing();

            // Check for key press to start the game
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
                currentScene = MENU;
            }

            if (leftPlayerScore == 10 || rightPlayerScore == 10) {
                currentScene = SCOREAI;
            }

            break;

        case SCOREAI :

            // Draw menu
            BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawTextEx(ft, "Pong Ultimate", titlePosition, 30, 15, WHITE);

            if (leftPlayerScore == 10) {

                DrawText("Left", (screenWidth / 4) + 50, screenHeight / 2, 20, WHITE);

            }

            if (rightPlayerScore == 10) {

                DrawText("Right", (screenWidth / 4) + 40, screenHeight / 2, 20, RED);

            }

            DrawText("player wins ! Congrats !", (screenWidth / 4) + 100, screenHeight / 2, 20, WHITE);

            DrawText("Press SPACE to restart :)", (screenWidth / 4) + 60, (screenHeight / 2) + 50, 20, WHITE);
            DrawText("Press P to get back to menu", (screenWidth / 4) + 50, (screenHeight / 2) + 150, 20, WHITE);

            EndDrawing();

            // Check if the player wants to replay the game
            if (IsKeyPressed(KEY_SPACE)) {

                leftPlayerScore = 0;
                rightPlayerScore = 0;

                currentScene = AI;
            }

            // Check if the player wants to go back to the menu
            if (IsKeyPressed(KEY_P)) {

                currentScene = MENU;
            }

            break;

        }

    }

    CloseWindow();
    return 0;
}



