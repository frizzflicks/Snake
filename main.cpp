#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <set>
#include <vector>
using namespace std;

const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 700;

SDL_Rect appleRect;

bool game_start = false;
bool isAlive = true;

// The window we are going to render our snake in
SDL_Window *window = NULL;

// The renderer we are going to use
SDL_Renderer *renderer = NULL;

// helps logging the error
void coutError(const char *str);

// helps initializing the SDL
bool init();

// the font we will use in our game
TTF_Font *gFont = NULL;

// the main game loop runs here frame-by-frame
bool renderLoop();

// returns texture for a string
SDL_Texture *textTex(const std::string &textureText, SDL_Color textColor);

// checks if two cubes collide
bool isColliding();

void restartGame();

// cleanup, preventing any memory leaks
void close();

class Snake
{
public:
    // constructor
    Snake();

    // another constructor with params
    Snake(int xPosP, int yPosP);

    // prev locaton
    int prevX, prevY;

    // set location
    void setLocation(int x, int y);

    // set collider location
    void setColloc(int x, int y);

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
    SDL_Rect &getCollider();
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
// The objects array for snake squares
vector<Snake> SnakeParts = {};

// score of the player
int score = SnakeParts.size() - 1;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        coutError(SDL_GetError());
        return false;
    }
    if (TTF_Init() == -1)
    {
        std::cout << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    gFont = TTF_OpenFont("text.ttf", 10);
    if (!gFont)
    {
        cout << "Error Loading font: " << TTF_GetError() << endl;
    }
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
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
int mRandom(int low, int high, const std::set<int> &exclude)
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(low, high);

    int result;
    do
    {
        result = dist(gen);
    } while (exclude.count(result));

    return result;
}

SDL_Texture *textTex(const std::string &textureText, SDL_Color textColor)
{
    SDL_Surface *textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture *mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (mTexture == NULL)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(textSurface);

    return mTexture;
}

void generateApple(Snake &s)
{
    set<pair<int, int>> used;
    for (auto &part : SnakeParts)
    {
        used.insert({part.getXPos(), part.getYPos()});
    }

    int xPosA, yPosA;
    do
    {
        xPosA = mRandom(0, SCREEN_WIDTH - 35, {});
        yPosA = mRandom(0, SCREEN_HEIGHT - 35, {});
        xPosA -= (xPosA % 35);
        yPosA -= (yPosA % 35);
    } while (used.count({xPosA, yPosA}));

    appleRect = {xPosA, yPosA, s.getWidth(), s.getHeight()};
}
bool checkCollision(const SDL_Rect &a, const SDL_Rect &b)
{
    return (
        a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y);
}

Snake::Snake(int xPosP, int yPosP)
{

    width = 35;
    height = 35;
    collider.h = 35;
    collider.w = 35;
    xVel = 0;
    yVel = 0;
    xPos = xPosP;
    yPos = yPosP;
    collider.x = xPosP;
    collider.y = yPosP;
    Snake::index++;
}

// set location
void Snake::setLocation(int x, int y)
{
    xPos = x;
    yPos = y;
    setColloc(x, y);
}

// set collider location
void Snake::setColloc(int x, int y)
{
    collider.x = x;
    collider.y = y;
}
void manageGrowth(Snake &s)
{
    generateApple(s);
    Snake sn(s.prevX, s.prevY);
    SnakeParts.push_back(sn);
}

void updateLoc(Snake &s, int index)
{
    if (index > 0 && isAlive)
    {
        s.prevX = s.getXPos();
        s.prevY = s.getYPos();
        s.setLocation(SnakeParts[index - 1].prevX, SnakeParts[index - 1].prevY);
    }
}
bool renderLoop()
{
    SDL_Event e;
    bool quit = false;
    Snake s;
    SnakeParts.push_back(s);
    generateApple(SnakeParts[0]);
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;

            if (e.type == SDL_KEYDOWN && game_start)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    SnakeParts[0].setVelocity(0, -35);
                    break;
                case SDLK_DOWN:
                    SnakeParts[0].setVelocity(0, 35);
                    break;
                case SDLK_LEFT:
                    SnakeParts[0].setVelocity(-35, 0);
                    break;
                case SDLK_RIGHT:
                    SnakeParts[0].setVelocity(35, 0);
                    break;
                }
            }
            if (e.type == SDL_KEYDOWN && !game_start)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    game_start = true;
                    break;
                }
            }
        }
        score = SnakeParts.size() - 1;

        if (isAlive)
            SnakeParts[0].move();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Background: black
        SDL_RenderClear(renderer);
        if (!game_start)
        {
            if (gFont == NULL)
            {
                printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
            }
            SDL_Color green = {0, 255, 0, 255};
            SDL_Texture *myTex = textTex("Slither", green);
            SDL_Texture *myTex01 = textTex("Press Enter", green);
            SDL_Texture *myTex02 = textTex("to Continue", green);
            SDL_Rect destRect = {0, 100, 700, 150};
            SDL_Rect destRect01 = {0, 300, 700, 150};
            SDL_Rect destRect02 = {0, 450, 700, 150};
            SDL_RenderCopy(renderer, myTex, NULL, &destRect);
            SDL_RenderCopy(renderer, myTex01, NULL, &destRect01);
            SDL_RenderCopy(renderer, myTex02, NULL, &destRect02);
            SDL_RenderPresent(renderer);
        }
        else
        {
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Background: white
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Snake: green

            SDL_Color yellow = {255, 255, 0, 255};
            SDL_Texture *scoreTex = textTex(to_string(score), yellow);
            SDL_Rect destRectScore = {600, 600, 100, 100};
            SDL_RenderCopy(renderer, scoreTex, NULL, &destRectScore);
            SDL_DestroyTexture(scoreTex);

            int index = 0;
            for (auto &part : SnakeParts)
            {
                updateLoc(part, index);
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &part.getCollider());
                index++;
            }
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red apple
            SDL_RenderFillRect(renderer, &appleRect);
            SDL_RenderPresent(renderer);
            bool iscol = checkCollision(SnakeParts[0].getCollider(), appleRect);
            if (iscol)
                manageGrowth(SnakeParts[index - 1]);
        }
        SDL_Delay(149);
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
    if (gFont)
    {
        TTF_CloseFont(gFont);
        gFont = nullptr;
    }
    TTF_Quit(); 
    SDL_Quit();
} 

bool isColliding()
{
    bool returnvalue = false;
    int i = 0;
    for (auto part : SnakeParts)
    {
        if (i != 0)
        {
            returnvalue = returnvalue || checkCollision(SnakeParts[0].getCollider(), part.getCollider());
        }
        if (returnvalue)
        {
            break;
        }
        i++;
    }

    return returnvalue;
}

Snake::Snake()
{
    width = 35;
    height = 35;
    collider.h = 35;
    collider.w = 35;
    xPos = 105;
    yPos = 105;
    collider.x = 105;
    collider.y = 105;
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
    prevX = xPos;
    prevY = yPos;
    xPos += xVel;
    collider.x += xVel;
    if ((xPos < 0) || (xPos + width) > SCREEN_WIDTH || isColliding())
    {
        restartGame();
    }

    yPos += yVel;
    collider.y += yVel;
    if ((yPos < 0) || (yPos + height) > SCREEN_HEIGHT || isColliding())
    {
        restartGame();
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

SDL_Rect &Snake::getCollider()
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

int Snake::index = 0;

void restartGame()
{
    SnakeParts.clear(); // Clean all
    Snake s;            // New head
    SnakeParts.push_back(s);

    generateApple(SnakeParts[0]);
    isAlive = true;
    game_start = false;
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