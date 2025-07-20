#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 700;

// The window we are going to render our snake in
SDL_Window *window = NULL;

// The renderer we are going to use
SDL_Renderer *renderer = NULL;

// helps logging the error
void coutError(const char *str);

// helps initializing the SDL
bool init();

// the main game loop runs here frame-by-frame
bool renderLoop();

// checks if two cubes collide
bool isColliding();

// cleanup, preventing any memory leaks
void close();

class Snake
{
public:
    // constructor
    Snake();

    // helps the parts of snake to move
    void moveCubes(SDL_Event &e);

    // helps rendering the part of the snake
    void renderCubes();

    // index of the box
    static int index;

    // the collision rectangle of the box
    SDL_Rect collider;

    void setVelocity(int x, int y);
    int getWidth();
    int getHeight();
    SDL_Rect getCollider();
    int getXPos();
    int getYPos();
    void move();

private:
    // height and width of the snake cube:
    int height;
    int width;
    int xPos, yPos, xVel, yVel;
    // the data member storing the index value
    int number;
};

void coutError(const char *str)
{
    cout << str << endl;
}

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        coutError(SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        coutError(SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        coutError(SDL_GetError());
        return false;
    }
    return true;
}

bool renderLoop()
{
    SDL_Event e;
    bool quit = false;
    Snake s;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;

            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    s.setVelocity(0, -35);
                    break;
                case SDLK_DOWN:
                    s.setVelocity(0, 35);
                    break;
                case SDLK_LEFT:
                    s.setVelocity(-35, 0);
                    break;
                case SDLK_RIGHT:
                    s.setVelocity(35, 0);
                    break;
                }
            }
        }
        s.move();

        SDL_RenderClear(renderer);

        // create new surface every frame
        SDL_Surface *loadedSurface = SDL_CreateRGBSurface(
            0,
            s.getWidth(),
            s.getHeight(),
            32,
            0x00FF0000,
            0x0000FF00,
            0x000000FF,
            0xFF000000);

        if (!loadedSurface)
        {
            coutError("Failed to create surface!");
            return false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Background: black
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Snake: green
        SDL_Rect destRect = s.getCollider();
        SDL_RenderFillRect(renderer, &destRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(250);
    }

    return true;
}

void close()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

bool isColliding()
{
    return false;
}

Snake::Snake()
{
    width = 35; 
    height = 35;
    collider.h = 35;
    collider.w = 35;
    xPos = 0;
    yPos = 0;
    xVel = 0;
    yVel = 0;
}
void Snake::setVelocity(int x, int y)
{
    xVel = x;
    yVel = y;
}

void Snake::move()
{
    xPos += xVel;
    collider.x += xVel;
    if ((xPos < 0) || (xPos + width) > SCREEN_WIDTH || isColliding())
    {
        cout << "game over!" << endl;
    }

    yPos += yVel;
    collider.y += yVel;
    if ((yPos < 0) || (yPos + height) > SCREEN_HEIGHT || isColliding())
    {
        cout << "game over!" << endl;
    }
}

int Snake::getWidth()
{
    return width;
}

int Snake::getHeight()
{
    return height;
}

SDL_Rect Snake::getCollider()
{
    return collider;
}

int Snake::getXPos()
{
    return xPos;
}

int Snake::getYPos()
{
    return yPos;
}

int main(int argc, char *args[])
{
    if (!init())
    {
        coutError("Could not initialize!");
        return -1;
    }

    if (!renderLoop())
    {
        coutError("loop failed to run!");
        return -1;
    }
    close();
}
