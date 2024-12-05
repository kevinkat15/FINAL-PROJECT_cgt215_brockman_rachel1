// FINAL PROJECT_cgt215_brockman_rachel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <vector>

using namespace std;
using namespace sf;
using namespace sfp;

const float PADDLE_SPEED = 0.3f;
const int ROWS = 5;
const int COLUMNS = 10;

void LoadTex(Texture& tex, string filename) {
    if (!tex.loadFromFile(filename)) {
        cout << "Could not load " << filename << endl;
    }
}

int main() {
    RenderWindow window(VideoMode(800, 600), "Brick Breaker");
    World world(Vector2f(0, 0));

    bool gameEnded = false;
    bool gameWon = false;
    int bricksRemaining = ROWS * COLUMNS;

    PhysicsRectangle paddle;
    paddle.setSize(Vector2f(100, 20));
    paddle.setCenter(Vector2f(400, 550));
    world.AddPhysicsBody(paddle);

    PhysicsCircle ball;
    ball.setRadius(10);
    ball.setCenter(Vector2f(400, 300));
    ball.setVelocity(Vector2f(0.2f, -0.2f)); 
    world.AddPhysicsBody(ball);


    PhysicsRectangle leftWall, rightWall, topWall;
    leftWall.setSize(Vector2f(10, 600));
    leftWall.setCenter(Vector2f(5, 300));
    leftWall.setStatic(true);
    world.AddPhysicsBody(leftWall);

    rightWall.setSize(Vector2f(10, 600));
    rightWall.setCenter(Vector2f(795, 300));
    rightWall.setStatic(true);
    world.AddPhysicsBody(rightWall);

    topWall.setSize(Vector2f(800, 10));
    topWall.setCenter(Vector2f(400, 5));
    topWall.setStatic(true);
    world.AddPhysicsBody(topWall);

    //vector<PhysicsRectangle> bricks;
    PhysicsShapeList<PhysicsSprite> bricks;

    Texture brickTex;
    LoadTex(brickTex, "FINAL/brick.png");


    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLUMNS; ++col) {
            PhysicsSprite& brick = bricks.Create();
            brick.setTexture(brickTex);
            brick.setSize(Vector2f(6, 2));
            brick.setCenter(Vector2f(6 + col * 7, 5 + row * 2));
            //brick.setStatic(true);
            world.AddPhysicsBody(brick);
        }
    }


    ball.onCollision = [&leftWall, &rightWall, &topWall, &world, &paddle, &ball, &bricks]
    (PhysicsBodyCollisionResult result) {
        if (result.object2 == leftWall || result.object2 == rightWall) {
            ball.setVelocity(Vector2f(-ball.getVelocity().x, ball.getVelocity().y));

            //world.RemovePhysicsBody(arrow);
            //world.RemovePhysicsBody(duck);
            //ducks.QueueRemove(duck) :
            //score += 10;
        }

        if (result.object2 == topWall) {
            ball.setVelocity(Vector2f(ball.getVelocity().x, -ball.getVelocity().y));
        }

        for (PhysicsSprite& brick : bricks) {
            if (result.object2 == brick) {
                ball.setVelocity(Vector2f(ball.getVelocity().x, -ball.getVelocity().y));
                world.RemovePhysicsBody(brick);
                bricks.QueueRemove(brick);

            }
        }

        if (result.object2 == paddle) {
            cout << "collision!!" << endl;
            ball.setVelocity(Vector2f(ball.getVelocity().x, -ball.getVelocity().y));
        }
    };

    Font font;
    if (!font.loadFromFile("FINAL/arial.ttf")) {
        cout << "Could not load font." << endl;
        return -1;
    }

    Clock clock;
    Time lastTime(clock.getElapsedTime());

    while (window.isOpen()) {       
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // game ended
        if (gameEnded) {
            Text gameOverText;
            if (gameWon) {
                gameOverText.setString("You Win!");
            }
            else {
                gameOverText.setString("Game Over");
            }
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(30);
            gameOverText.setPosition(Vector2f(400 - gameOverText.getGlobalBounds().width / 2, 300));
            window.draw(gameOverText);
        }

        Time currentTime(clock.getElapsedTime());
        Time deltaTime(currentTime - lastTime);
        int deltaTimeMS(deltaTime.asMilliseconds());

        if (deltaTimeMS > 0) {
            world.UpdatePhysics(deltaTimeMS);
            lastTime = currentTime;
        }

        // key presses
        if (Keyboard::isKeyPressed(Keyboard::Left) && paddle.getCenter().x > 60) {
            window.clear();
            Vector2f oldCenter = paddle.getCenter();
            world.RemovePhysicsBody(paddle);
            paddle.setCenter(oldCenter + Vector2f(-PADDLE_SPEED, 0));
            world.AddPhysicsBody(paddle);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) && paddle.getCenter().x < 740) {
            window.clear();
            Vector2f oldCenter = paddle.getCenter();
            world.RemovePhysicsBody(paddle);
            paddle.setCenter(oldCenter + Vector2f(PADDLE_SPEED, 0));
            world.AddPhysicsBody(paddle);
        }

        if (ball.getCenter().y > 600) {
            gameEnded = true;
        }

        for (PhysicsShape& brick : bricks) {
            window.draw((PhysicsSprite&)brick);
        }

        //world.UpdatePhysics(10);
        //for (auto it = bricks.begin(); it != bricks.end();) {
            //PhysicsRectangle& brick = *it;
           /* if (brick.collidesWith(ball)) {
                ball.setVelocity(Vector2f(ball.getVelocity().x, -ball.getVelocity().y));
                world.RemovePhysicsBody(brick);
                it = bricks.erase(it);
                bricksRemaining--;

                if (bricksRemaining == 0) {
                    gameWon = true;
                    gameEnded = true;
                }
            }
            else {
                ++it;
            }*/
        //}



        // collision logic

       


        //if (paddle.collidesWith(ball)) {
            //ball.setVelocity(Vector2f(ball.getVelocity().x, -ball.getVelocity().y)); 
        //}

        //paddle.onCollision = [&ball, &world](PhysicsBodyCollisionResult result) {
            //ball.setVelocity(Vector2f(ball.getVelocity().x, -ball.getVelocity().y));
        //};

         // drawing sprites
        /*for (PhysicsRectangle& brick : bricks) {
            window.draw(brick);
        }*/

        window.clear();
        window.draw(ball);
        window.draw(paddle);
        window.display();
    }

    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
