//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600 
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

#define BRICKWIDTH 35
// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

//paddle
#define PADDLEWIDTH 50
#define PADDLEHEIGHT 5

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    //ball velocity
    double velocity_x = drand48()*2;
    double velocity_y = 2.0;
        
    //ball movement
   // move(ball, velocity, 0);

    // keep playing until game over
    while (lives > 0 && bricks > 0 && points<=50)
    {
        // DONE
         updateScoreboard(window,label,points);
         
         GEvent event = getNextEvent(MOUSE_EVENT);
         
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure rect follows cursor
                double x = getX(event) - getWidth(paddle) / 2;
                double y = HEIGHT-(getHeight(window)/4);
                setLocation(paddle,x,y);
            }
        }
        //ball movement
        move(ball, velocity_x,velocity_y);
        
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocity_x = -velocity_x;
        }
        else if (getX(ball) <= 0)
        {
            velocity_x = -velocity_x;
        }
        else if(getY(ball)<= 0)
        {
            velocity_y = -velocity_y;
        }
        else if(getY(ball)+getHeight(ball)>=getHeight(window))
        {
            lives--;
            setLocation(ball,getWidth(window)/2-RADIUS,getHeight(window)*1/2);
            waitForClick();
        }
        pause(10);
        
        GObject object = detectCollision(window,ball);
        if(object != NULL)
        { 
         if(object == paddle)
         {
          velocity_y = -velocity_y;
         
         }else if(strcmp(getType(object),"GRect") == 0)
         {
          removeGWindow(window,object);
          points++;
          updateScoreboard(window,label,points);
          velocity_y= -velocity_y;
         }
         
         
        }
        
    }
      if(lives<=0)
       {
           removeGWindow(window,label);
           GLabel loser = newGLabel("");
           setFont(loser, "SansSerif-36");
           add(window, loser);
           char* s = "You Lost, Sorry!";
           setLabel(loser,s);
           double x = (getWidth(window) - getWidth(loser)) / 2;
           double y = (getHeight(window) - getHeight(loser)) / 2;
           setLocation(loser, x, y);
           waitForClick();
       }
    // wait for click before exiting
     //waitForClick();
    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // DONE
    int brick_y = 30;
    char *colors[]={"GREEN","RED","CYAN","ORANGE","BLUE"};
    
    for(int i = 0; i< ROWS;i++)
    {
        int brick_x = 5;
        for(int j = 0; j<COLS;j++)
        {
            
            GRect brick = newGRect(brick_x,brick_y,BRICKWIDTH,10);
         
            if(i==0)
            {
                setColor(brick, colors[i]);
            }else if(i==1)
            {
                setColor(brick, colors[i]);
            }else if(i==2)
            {
                setColor(brick, colors[i]);
            }else if(i==3)
            {
                setColor(brick, colors[i]);
            }else if(i==4)
            {
                setColor(brick, colors[i]);
            }
            setFilled(brick,true);
            add(window, brick);
            brick_x = brick_x+40;
        }
        brick_y=brick_y+15;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // DONE
    double x = getWidth(window)/2-RADIUS;
    double y = getHeight(window)*1/2;
    GOval circle = newGOval(x,y, RADIUS*2, RADIUS*2);
    setColor(circle, "BLACK");
    setFilled(circle, true);
    add(window, circle);
    return circle;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    //DONE
    double x = (getWidth(window)/2)-(PADDLEWIDTH/2);
    double y = HEIGHT-(getHeight(window)/4);
    GRect paddle = newGRect(x,y,50,5);
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
