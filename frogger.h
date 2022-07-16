
void frogJumping(double &yPos, double frogGround, sf::Sprite &frog, sf::Texture &frog1, sf::Texture &frogJump, double &yV)
{
	//when space is pressed, frog jump and change to jump texture
	//pre:spacebar is pressed, frog was on ground (yPos=frogGround)
	//Post: frog yPos=frogGround, frog texture=frog1
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)&&yPos==frogGround)
    {
        frog.setTexture(frogJump);
        yV=-15;
    }
    yV+=0.5;
    yPos+=yV;
    if(yPos>frogGround)
    {
        yPos=frogGround;
        frog.setTexture(frog1);
    }
}

void frogBranch(sf::Sprite &frog,sf::Sprite &branch,double &branchxPos,double &yPos,int &livesNum,sf::Sound &splash,sf::Clock &wetClock, sf::Time &elapsed2,sf::Texture &frogWet)
{
	//when frog collides with branch, branch moves back, frog goes to ground, frog texture wet for 1 second
	//pre: branch intersects frog
	//post:frog yPos=frogGround, branchxPos=1600, lives--, frog texture frogWet for 1 sec, then back to frog1, wet sound is played
    if(frog.getGlobalBounds().intersects(branch.getGlobalBounds()))
    {
        branchxPos=1600;
        yPos=350;
        livesNum--;
        wetClock.restart();
        splash.play();
    }
    if(elapsed2.asSeconds()<=1)
    {
        frog.setTexture(frogWet);
    }
}

void lick(sf::Sprite &tongue, double xPos, double yPos)
{
	//when L is pressed, tongue come on screen, being in sync with frog yPos
	//pre: L was pressed, tongue is out of view
	//post: tongue yPos correlates with frog yPos
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        tongue.setPosition(xPos+70,yPos+15);
    }
    else
    {
        tongue.setPosition(-100,yPos+15);
    }
}

void flyMove(sf::Sprite fly[20], double flySpeed, sf::Sprite tongue, int &scoreNum, sf::Sound &ping)
{
	//fly moves from right to left, when collides with tongue, make sound and move back
	//pre: fly is moving, fly intersects tongue or moves off left screen side
	//post: fly moves to right, scoreNum++, ping sound played (if intersects tongue)
    for(int i=0; i<20; i++)
    {
        //reseting fireflies to back
        fly[i].move(flySpeed,0);
        if(fly[i].getPosition().x<0)
        {
            fly[i].move(2500,0);
        }
        //fireflies on tongue
        if(fly[i].getGlobalBounds().intersects(tongue.getGlobalBounds()))
        {
            fly[i].move(2500,0);
            scoreNum++;
            ping.play();
        }
    }
}