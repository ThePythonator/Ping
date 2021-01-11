#include "game.hpp"

using namespace blit;

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

#define PADDLE_WIDTH 4
#define PADDLE_HEIGHT 16

#define BALL_START_SPEED 0.5

#define PADDLE_SPEED 1

#define MAX_SCORE 5

int state = 0;
float paddleOne, paddleTwo;
float ballX, ballY;
float ballVelX, ballVelY;
int scoreOne, scoreTwo;
float ballSpeed;

void reset_ball(int startDirection)
{
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballVelX = startDirection;
    ballVelY = 0;
    ballSpeed = BALL_START_SPEED;
}

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {
    set_screen_mode(ScreenMode::lores);
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();

    // draw some text at the top of the screen
    screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen = Pen(255, 255, 255);

    if (state == 0 || state == 1)
    {
        screen.text("Ping!", minimal_font, Point(SCREEN_WIDTH / 2, 16), true, TextAlign::center_center);
        screen.text("1 player", minimal_font, Point(SCREEN_WIDTH / 2, 56), true, TextAlign::center_center);
        screen.text("2 player", minimal_font, Point(SCREEN_WIDTH / 2, 72), true, TextAlign::center_center);
    }
    else if (state == 2 || state == 3)
    {
        screen.rectangle(Rect(2, paddleOne - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT));
        screen.rectangle(Rect(SCREEN_WIDTH - 2 - PADDLE_WIDTH, paddleTwo - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT));
        screen.rectangle(Rect(ballX - PADDLE_WIDTH / 2, ballY - PADDLE_WIDTH / 2, PADDLE_WIDTH, PADDLE_WIDTH));

        screen.text(std::to_string(scoreOne), minimal_font, Point(60, 8), true, TextAlign::center_center);
        screen.text(std::to_string(scoreTwo), minimal_font, Point(SCREEN_WIDTH - 60, 8), true, TextAlign::center_center);
    }
    else if (state == 4) {
        //screen.text("Press A", minimal_font, Point(SCREEN_WIDTH / 2, 92), true, TextAlign::center_center);
    }

    if (state == 0)
    {
        screen.rectangle(Rect(50, 54, 4, 4));
    }
    else if (state == 1)
    {
        screen.rectangle(Rect(50, 70, 4, 4));
    }
    else if (state == 2)
    {

    }
    else if (state == 3)
    {

    }
    else if (state == 4)
    {
        if (scoreOne == MAX_SCORE)
        {
            screen.text("Player 1 wins!", minimal_font, Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), true, TextAlign::center_center);
        }
        else if (scoreTwo == MAX_SCORE)
        {
            screen.text("Player 2 wins!", minimal_font, Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), true, TextAlign::center_center);
        }
    }

    screen.pen = Pen(0, 0, 0);

}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
    if (state == 0 || state == 1)
    {
        if ((buttons.pressed & Button::DPAD_DOWN) || (buttons.pressed & Button::DPAD_UP))
        {
            state = 1 - state;
        }
        else if (buttons.pressed & Button::A)
        {
            state += 2;
            paddleOne = paddleTwo = SCREEN_HEIGHT / 2;
            scoreOne = scoreTwo = 0;
            reset_ball(1); // Maybe random later
        }
    }
    else if (state == 2 || state == 3)
    {
        ballX += ballVelX * ballSpeed;
        ballY += ballVelY * ballSpeed;

        if (buttons & Button::DPAD_UP)
        {
            paddleOne -= PADDLE_SPEED;
        }
        if (buttons & Button::DPAD_DOWN)
        {
            paddleOne += PADDLE_SPEED;
        }

        if (state == 3)
        {
            if (buttons & Button::X)
            {
                paddleTwo -= PADDLE_SPEED;
            }
            if (buttons & Button::B)
            {
                paddleTwo += PADDLE_SPEED;
            }
        }
        else
        {
            if (ballVelX > 0)
            {
                if (paddleTwo > ballY)
                {
                    paddleTwo -= PADDLE_SPEED;
                }
                else
                {
                    paddleTwo += PADDLE_SPEED;
                }
            }
            else
            {
                if (paddleTwo > SCREEN_HEIGHT / 2)
                {
                    paddleTwo -= PADDLE_SPEED;
                }
                else
                {
                    paddleTwo += PADDLE_SPEED;
                }
            }
        }

        if (paddleOne < PADDLE_HEIGHT / 2)
        {
            paddleOne = PADDLE_HEIGHT / 2;
        }
        else if (paddleOne > SCREEN_HEIGHT - PADDLE_HEIGHT / 2)
        {
            paddleOne = SCREEN_HEIGHT - PADDLE_HEIGHT / 2;
        }

        if (paddleTwo < PADDLE_HEIGHT / 2)
        {
            paddleTwo = PADDLE_HEIGHT / 2;
        }
        else if (paddleTwo > SCREEN_HEIGHT - PADDLE_HEIGHT / 2)
        {
            paddleTwo = SCREEN_HEIGHT - PADDLE_HEIGHT / 2;
        }

        if (ballX - PADDLE_WIDTH / 2 < 2 + PADDLE_WIDTH && ballY + PADDLE_WIDTH / 2 > paddleOne - PADDLE_HEIGHT / 2 && ballY - PADDLE_WIDTH / 2 < paddleOne + PADDLE_HEIGHT / 2)
        {
            // collided with left paddle
            ballVelY = ballVelY - ((paddleOne - ballY) / (PADDLE_HEIGHT / 2));
            if (ballVelY < -1)
            {
                ballVelY = -1;
            }
            else if (ballVelY > 1)
            {
                ballVelY = 1;
            }
            ballVelX = std::sqrt(1 - std::pow(ballVelY, 2));
            //ballVelX = -ballVelX;
            ballSpeed += 0.1;
        }
        else if (ballX + PADDLE_WIDTH / 2 > SCREEN_WIDTH - 2 - PADDLE_WIDTH && ballY + PADDLE_WIDTH / 2 > paddleTwo - PADDLE_HEIGHT / 2 && ballY - PADDLE_WIDTH / 2 < paddleTwo + PADDLE_HEIGHT / 2)
        {
            // collided with right paddle
            ballVelY = ballVelY - ((paddleTwo - ballY) / (PADDLE_HEIGHT / 2));
            if (ballVelY < -1)
            {
                ballVelY = -1;
            }
            else if (ballVelY > 1)
            {
                ballVelY = 1;
            }
            ballVelX = -std::sqrt(1 - std::pow(ballVelY, 2));
            //ballVelX = -ballVelX;
            ballSpeed += 0.1;
        }

        if (ballX < 0)
        {
            scoreTwo += 1;
            reset_ball(-1);
        }
        else if (ballX > SCREEN_WIDTH)
        {
            scoreOne += 1;
            reset_ball(1);
        }

        if (ballY < 0 || ballY > SCREEN_HEIGHT)
        {
            ballVelY = -ballVelY;
        }

        if (scoreOne == MAX_SCORE || scoreTwo == MAX_SCORE)
        {
            state = 4; // display winner
        }
    }
    else if (state == 4)
    {
        if (buttons.pressed & Button::A)
        {
            state = 0;
        }
    }
}