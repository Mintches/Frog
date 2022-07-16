#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include "frogger.h"

using namespace std;

void updateBranch(double &branchxPos,double &branchyPos, double &branchWidth, double &branchHeight, double branchSpeed)
{
    //moves branch across screen, generates different size when it goes back
    //pre: branchxPos<0-branchWidth, branch is out of screen on left
    //post: branchxPos=800, back at right side, with new, random dimensions
    branchxPos-=branchSpeed;
    if(branchxPos<0-branchWidth)
    {
        branchxPos=800;
        branchWidth=rand()%50+10;
        branchHeight=rand()%50+100;
        branchyPos=400-branchHeight;
    }
}
void flyAnimation(sf::Clock &flyClock, sf::Time &elapsed1, sf::Sprite fly[20], sf::Texture &fly1, sf::Texture &fly2)
{
    //fireflies are bright, then dim every 0.5 second
    //pre: when elapsed1 is less than 0.5sec, bright, more? dim.
    //post: fireflies are flashing, flyClock is reset
    if(elapsed1.asMilliseconds()<=500)
    {
        for(int i=0; i<20; i++)
        {
            fly[i].setTexture(fly2);
        }
    }
    else if(elapsed1.asMilliseconds()>500&&elapsed1.asMilliseconds()<=1000)
    {
        for(int i=0; i<20; i++)
        {
            fly[i].setTexture(fly1);
        }
    }
    else
    {
        flyClock.restart();
    }
}
void gameStop(bool gameOver, double &branchxPos, double &branchSpeed, double &flySpeed, sf::Sprite fly[20], sf::Text &result, sf::RectangleShape &startButt, sf::Text &startText, int livesNum, int scoreNum)
{
    //stopping all parts (except frog can still jump and lick)
    //pre:gameOver=true
    //post: branch, flies aren't moving and are off the screen, start button is on
    if(gameOver==true)
    {
        //if livesNum=0 or scoreNum=100, the game ends (gameOver==true)
        int num=0;
        branchxPos=800;
        branchSpeed=0;
        flySpeed=0;
        result.setPosition(350,125);
        startButt.setPosition(400-75, 200-20);
        startText.setPosition(400-33, 200-12);
        for(int j=0; j<4; j++)
        {
            for(int i=0; i<5; i++)
            {
                fly[num].setPosition(1000,150);
                fly[num].move(i*30+5, j*30+5);
                num++;
            }
        }
        if(scoreNum>=100)
        {
            result.setString("You Won!");
            startText.setString("Play!");
        }
        else if(livesNum==0)
        {
            result.setString("You Lost");
            startText.setString("Again");
        }
    }
}
void gameStart(bool gameOver, double &branchSpeed, double &flySpeed, int &livesNum, int &scoreNum, sf::RectangleShape &startButt, sf::Text &startText, sf::Text &result)
{
    //game starts, everything is running, start button is off the screen
    //pre: gameOver=false (start button is pressed)
    //post: branch, fly and frog are on screen and moving, start button is out
    if(gameOver==false)
    {
        branchSpeed=12.5;
        flySpeed=-7.5;
        livesNum=3;
        scoreNum=0;
        startButt.setPosition(0,1000);
        startText.setPosition(0,1000);
        result.setPosition(0,1000);
    }
}

int main()
{
    //vars:

    sf::RenderWindow window(sf::VideoMode(800, 400), "Frog Jump!");
    window.setFramerateLimit(60);

    //frog stuff
    sf::Texture frog1;
    sf::Texture frogJump;
    sf::Texture frogWet;
    frog1.loadFromFile("frog1.png");
    frogJump.loadFromFile("frogJump.png");
    frogWet.loadFromFile("frogWet.png");
    sf::Sprite frog;
    frog.setTexture(frog1);
    frog.scale(5,5);
    double yPos=350;
    double xPos=50;
    double yV=0;
    double frogGround=350;
    frog.setPosition(xPos,yPos);

    //frog tongue stuff
    sf::Texture tongueTexture;
    tongueTexture.loadFromFile("frogLick.png");
    sf::Sprite tongue;
    tongue.setTexture(tongueTexture);
    tongue.scale(5,5);
    tongue.setPosition(120,360);

    //branch
    sf::Texture branchTexture;
    branchTexture.loadFromFile("branch.png");
    sf::Sprite branch;
    branch.setTexture(branchTexture);
    double branchWidth=25;
    double branchHeight=50;
    double branchxPos=800-branchWidth;
    double branchyPos=400-branchHeight;
    branch.setScale(sf::Vector2f(branchWidth,branchHeight));
    branch.setPosition(branchxPos,branchyPos);
    double branchSpeed=12.5;

    //fireflies
    sf::Texture fly1;
    sf::Texture fly2;
    fly1.loadFromFile("fly1.png");
    fly2.loadFromFile("fly2.png");
    int rows=4;
    int cols=5;
    int num=0;
    sf::Sprite fly[20];
    double flySpeed=-7.5;
    for(int j=0; j<rows; j++)
    {
        for(int i=0; i<cols; i++)
        {
            fly[num].setTexture(fly1);
            fly[num].setPosition(800,150);
            fly[num].move(i*30+5, j*30+5);
            fly[num].scale(5,5);
            num++;
        }
    }

    //clocks for animations
    sf::Clock flyClock;
    sf::Clock wetClock;

    //sound
    sf::SoundBuffer sBuff1;
    sBuff1.loadFromFile("splash.wav");
    sf::Sound splash;
    splash.setBuffer(sBuff1);

    sf::SoundBuffer sBuff2;
    sBuff2.loadFromFile("ping.wav");
    sf::Sound ping;
    ping.setBuffer(sBuff2);
    ping.setVolume(25.f);

    sf::SoundBuffer sBuff3;
    sBuff3.loadFromFile("buttonPress.wav");
    sf::Sound click;
    click.setBuffer(sBuff3);

    //font
    sf::Font font;
    font.loadFromFile("EightBitDragon.ttf");

    //start button
    sf::RectangleShape startButt;
    startButt.setSize(sf::Vector2f(150,40));
    startButt.setFillColor(sf::Color::White);
    startButt.setPosition(400-75, 200-20);

    sf::Text startText;
    startText.setFont(font);
    startText.setCharacterSize(20);
    startText.setString("start");
    startText.setFillColor(sf::Color::Black);
    startText.setPosition(400-33, 200-12);

    //lives
    sf::Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(18);
    livesText.setString("Lives: ");
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(150, 25);
    //lives number
    sf::Text lives;
    int livesNum=3;
    lives.setString(std::to_string(livesNum));
    lives.setFont(font);
    lives.setFillColor(sf::Color::White);
    lives.setCharacterSize(18);
    lives.setPosition(225,25);

    //score text
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setString("Score: ");
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(25, 25);
    //score number
    sf::Text score;
    int scoreNum=0;
    score.setString(std::to_string(scoreNum));
    score.setFont(font);
    score.setFillColor(sf::Color::White);
    score.setCharacterSize(18);
    score.setPosition(100,25);

    sf::Text result;
    result.setString("Play Now!");
    result.setFont(font);
    result.setFillColor(sf::Color::White);
    result.setCharacterSize(18);
    result.setPosition(350,125);

    bool gameOver=true;

    while (window.isOpen())
    {
        //events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(event.type==sf::Event::MouseButtonPressed)
            {
                double tempx=sf::Mouse::getPosition(window).x;
                double tempy=sf::Mouse::getPosition(window).y;
                if(tempx<475&&tempx>325&&tempy>180&&tempy<220)
                {
                    click.play();
                    gameOver=false;
                    gameStart(gameOver, branchSpeed, flySpeed, livesNum, scoreNum, startButt, startText, result);
                }
            }
        }
        //game logic
        if(livesNum==0||scoreNum>=100)
        {
            gameOver=true;
        }

        gameStop(gameOver, branchxPos, branchSpeed, flySpeed, fly, result, startButt, startText, livesNum, scoreNum);

        //frog jumping
        frogJumping(yPos, frogGround, frog, frog1, frogJump, yV);
        frog.setPosition(xPos, yPos);
        //tongue out
        lick(tongue, xPos, yPos);

        //branch moving
        updateBranch(branchxPos,branchyPos,branchWidth,branchHeight,branchSpeed);
        branch.setScale(sf::Vector2f(branchWidth,branchHeight));
        branch.setPosition(branchxPos,branchyPos);

        //frog hits branch
        sf::Time elapsed2=wetClock.getElapsedTime();
        lives.setString(std::to_string(livesNum));
        frogBranch(frog,branch,branchxPos,yPos,livesNum,splash,wetClock, elapsed2,frogWet);

        //fireflies moving, tongue catches fireflies
        flyMove(fly,flySpeed,tongue,scoreNum,ping);
        score.setString(std::to_string(scoreNum));
        //fireflies animation
        sf::Time elapsed1=flyClock.getElapsedTime();
        flyAnimation(flyClock, elapsed1, fly, fly1, fly2);

        window.clear();
        window.draw(frog);
        window.draw(tongue);
        window.draw(branch);
        window.draw(startButt);
        window.draw(startText);
        window.draw(result);
        window.draw(livesText);
        window.draw(lives);
        window.draw(scoreText);
        window.draw(score);
        for(int i=0; i<20; i++)
        {
            window.draw(fly[i]);
        }
        window.display();
    }

    return 0;
}
