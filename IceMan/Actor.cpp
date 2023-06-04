#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>
#include <algorithm>
using namespace std;

// Constructor with default params
Actor::Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) 
    : GraphObject(imageID, startX, startY, dir, size, depth) {
    setVisible(true);
}

// Constructor with StudentWorld params
Actor::Actor(StudentWorld* p, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) 
    : GraphObject(imageID, startX, startY, dir, size, depth) {
    setVisible(true);
    m_studentworld = p;
}

// Move in specified direction
void Actor::move(Direction d) {
    setDirection(d);
    int xMove = 0;
    int yMove = 0;
    if (d == down) yMove = -1;
    else if (d == up) yMove = 1;
    else if (d == left) xMove = -1;
    else if (d == right) xMove = 1;
    moveTo(getX() + xMove, getY() + yMove);
}

// Check coords are within game bounds
bool Actor::checkBoundary(int x, int y) {
    return x >= 0 && x < 62 && y >= 0 && y < 60;
}


// IceMan constructor
IceMan::IceMan(StudentWorld* p) 
    : Agent(p, 30, 60, right, IID_PLAYER, 10) {
    m_sonar = 3;
    m_water = 5;
    m_nuggets = 0;
}


RegularProtester::RegularProtester(StudentWorld* p)
    : Agent(p, 60, 60, left, IID_PROTESTER, 5) {
    setVisible(true);
    numSquaresToMoveInCurrentDirection = getWorld()->randInt(8, 60); // random int between 8 - 60
    setHealth(5);// 5 hp
    leaveOilFieldState = false;                // false
    isAlive = true;
    ticksToWaitBetweenMoves = max(0, static_cast<int>(3 - getWorld()->getLevel() / 4)); // static_cast bc max() uses int and unsigned int
}

// Boulder constructor
Boulder::Boulder(StudentWorld* p, int startX, int startY)
    : Actor(p, IID_BOULDER, startX, startY, down, 1.0, 10.0) {
        setState(stable);
    }

// WaterGun constructor
Squirt::Squirt(StudentWorld * pointer, int x, int y, Direction d):
    Actor(pointer, IID_WATER_SPURT,x,y,d,1.0,1){
    SetCountdown(4);
    setState(stable);
    if (checkIce(x, y, d)){
        setState(dead);
    }
}

// Ice constructor
Ice::Ice(int startX, int startY) 
    : Actor(IID_ICE, startX, startY, right, 0.25, 3) {}


//SonarKit
SonarKit::SonarKit(StudentWorld* p, int startX, int startY)
    : ActivatingObject(p, startX, startY, IID_SONAR, SOUND_SONAR, true, false, false){
        setTicksToLive();
        setVisible(true);
    }

//Barrel of oil Constructor
OilBarrel::OilBarrel(StudentWorld* p, int startX, int startY)
    : ActivatingObject(p, startX, startY, IID_BARREL, SOUND_FOUND_OIL, true, false, true){
        setVisible(false);
    }


// Check if there's Ice at specified coords & direction
bool Actor::checkIce(int x, int y, Direction dir) {
    if(dir == right){
        for (int i= 0; i < 4; i++){
            if(getWorld()->getIce(x+4, y+i) != nullptr){
                return true;
            }
        }
        return false;
    }
    if(dir == left){
        for (int i = 0; i < 4; i++){
            if(getWorld()->getIce(x-1, y+i)){
                return true;
            }
        }
        return false;
    }
    if(dir == up){
        for (int i = 0; i < 4; i++){
            if(getWorld()->getIce(x+i, y+4) != nullptr){
                return true;
            }
        }
        return false;
    }
    if (dir == down){
        for (int i = 0; i < 4; i++){
            if (getWorld()->getIce(x+i, y-1) != nullptr){
                return true;
            }
        }
        return false;
    }
    return false;
}

void IceMan::destroyIce(int x, int y, Direction dir) {
    if (dir == down) {
        for (int i = 0; i < 4; i++) {
            getWorld()->deleteIce(x + i, y);
        }
    }
    else if (dir == up) {
        for (int i = 0; i < 4; i++) {
            getWorld()->deleteIce(x + i, y + 3);
        }
    }
    else if (dir == left) {
        for (int i = 0; i < 4; i++) {
            getWorld()->deleteIce(x, y + i);
        }
    }
    else if (dir == right) {
        for (int i = 0; i < 4; i++) {
            getWorld()->deleteIce(x + 3, y + i);
        }
    }
}

// IceMan's actions
void IceMan::doSomething() {
    int move;

    if (getWorld()->getKey(move) == true) {
        if (move == KEY_PRESS_UP) {
            if (checkBoundary(getX(), getY() + 1)) {
                if (!getWorld()->checkBoulder(getX(), getY() + 1)) {
                    destroyIce(getX(), getY() + 1, up);
                    moveTo(getX(), getY() + 1);
                    setDirection(up);
                }
            }

        }
        else if (move == KEY_PRESS_DOWN) {
            if (checkBoundary(getX(), getY() - 1)) {
                if (!getWorld()->checkBoulder(getX(), getY() - 1)) {
                    destroyIce(getX(), getY() - 1, down);
                    moveTo(getX(), getY() - 1);
                    setDirection(down);
                }
            }

        }
        else if (move == KEY_PRESS_LEFT) {
            if (checkBoundary(getX() - 1, getY())) {
                if (!getWorld()->checkBoulder(getX() - 1, getY())) {
                    destroyIce(getX() - 1, getY(), left);
                    moveTo(getX() - 1, getY());
                    setDirection(left);
                }
            }

        }
        else if (move == KEY_PRESS_RIGHT) {
            if (checkBoundary(getX() + 1, getY())) {
                if (!getWorld()->checkBoulder(getX() + 1, getY())) {
                    destroyIce(getX() + 1, getY(), right);
                    moveTo(getX() + 1, getY());
                    setDirection(right);
                }
            }
        }
        else if (move == 'z'){
            getWorld()->revealHidden(getX(), getY(), 12);
            decSonar();
        }
        else if (move == KEY_PRESS_SPACE){
            if (getWater() > 0){
                decWater();
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                if (getDirection()==down)
                {
                    getWorld()->CreateSquirt(getX(),getY()-4, getDirection());
                }
                else if (getDirection()==up)
                {
                    getWorld()->CreateSquirt(getX(),getY()+4, getDirection());
                }
                else if (getDirection()==right)
                {
                    getWorld()->CreateSquirt(getX()+4,getY(), getDirection());
                }
                else if (getDirection()==left)
                {
                    getWorld()->CreateSquirt(getX()-4,getY(), getDirection());
                }
            }
        }
    }
}

// GoldNugget constructor
GoldNugget::GoldNugget(StudentWorld* p, int startX, int startY, bool flag)
    : Actor(p, IID_GOLD, startX, startY, right, 1.0, 2) {
    pos.push_back({ startX, startY });

    setVisible(true);

    // Initializations
    isAlive = true;
    isVisible = false;
    soundPlayed = false;
    isTemporary = false;
    pickUpStatus = flag;
    lifetimeTicks = 100;

}

// Nuggets do something
void GoldNugget::doSomething() {
    // 1. Check if GoldNugget is alive, if not, then return
    if (!isAlive) {
        return;
    }

    // 2. If gold nugget not currently visable and iceman is within 4 units, then
        // gold nugget must make itself visible & return
    if (!isVisible && getWorld()->distance(getX(), getY(), getWorld()->icemanPosY(), getWorld()->icemanPosY()) <= 4.0) {
        setVisible(true);
        //return;
    }

    // 3. Check if Gold Nugget is pickupable by Iceman (PickupStatus = true) & within a radius of 3.0
    if (pickUpStatus == true && getWorld()->distance(getX(), getY(), getWorld()->icemanPosX(), getWorld()->icemanPosY()) <= 3.0) {
        // GoldNugget state to dead (to be removed from game at end of current tick)
        isAlive = false; //setState(dead);
        setState(dead);
        // play sound effect on pickup
        if (soundPlayed == false) {
            getWorld()->playSound(SOUND_GOT_GOODIE);
            soundPlayed = true;
        }
        // increase player score by 10 points
        getWorld()->increaseScore(10);
        setVisible(false);
        // tell iceman object that it received a nugget (update inv)
        getWorld()->getIceMan()->
        addNug();
        return;
    }
    //// 4. Check if Gold Nugget is pickupable by Protesters (PickupStatus = false) & is within a radius of 3.0 from Protester
    //if (PickupStatus == false) {
    //    Protester* protester = getWorld()->getProtesterInRadius(getX(), getY(), 3.0);
    //    if (protester != nullptr) {
    //        // state to dead
    //        isAlive = false;setState(dead);
    //        // play sound effect that protester found gold
    //        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    //        // tell protester that it received a nuggest (act bribed)
    //        protester->addGoldNugget();
    //        // increase player's score by 25
    //        getWorld()->increaseScore(25);  
    //        // NOTE: 1 gold per protester, if multiple protesters in range, only 1 gets bribed
    //    }
    //}

    // 5. Check temp state. If temp, check tick lifetime and if so, set state to dead to be removed at end of current tick
    if (isTemporary) {
        lifetimeTicks--;
    }

    if (isTemporary && lifetimeTicks <= 0) {
        // state to dead
        isAlive = false; //setState(dead);
        return;
    }
}



void Boulder::doSomething(){
    if(getState() == waiting){
        if (countDown == 0){
            setState(falling);
            return;
        }
        decCount();
    }
    
    if(getState() == falling){
        if(!checkIce(getX(), getY(), down)){
            getWorld()->playSound(SOUND_FALLING_ROCK);
            //checking icrementally on the way down allowing boulder to "fall"
            while(!checkIce(getX(), getY(), down)){
                //moving down keeping x cause boulders to shimmy
                moveTo(getX(), getY()-1);
            }
            setState(stable);
            setCount();
        }
    }
    
    if(checkIce(getX(), getY(), down)){return;}
    else{setState(waiting);return;}
}


void OilBarrel::doSomething(){
    if (getWorld()->distance(getX(), getY(), getWorld()->icemanPosX(), getWorld()->icemanPosY()) == 4.0 && getState() != dead) {
        setVisible(true);
        return;
    }
    if (getWorld()->distance(getX(), getY(), getWorld()->icemanPosX(), getWorld()->icemanPosY()) <= 3.0 && getState() != dead) {
        setDead();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->decBarrels();
    }
    
}

void RegularProtester::doSomething() {
    // 1. check alive, if not, return
    if (!isAlive) {
        return;
    }

    // 2. check rest state (if waiting for turn to move),
        // decr and return until 0 (meaning is protester's turn)
    if (ticksToWaitBetweenMoves > 0) {
        ticksToWaitBetweenMoves--;
        return;
    }

    // 3. if in leaveOilFieldState (bc hp = 0)
    if (leaveOilFieldState == true) {
        // already at exit point (60,60), removes end of tick
        if (getX() == 60 && getY() == 60) {
            setState(dead);
            return;
        }
        else {
            //moveTowardsExit();      // DEFINE -----------------------------------------------------------
            return;
        }
    }

    // 4. check if within distance of 4 from Iceman
        // AND currently facing Iceman's direction
        // if both true and regular protester hasn't shouted within 15 non-resting ticks, shouts
    if ((getWorld()->distance(getX(), getY(), getWorld()->icemanPosX(), getWorld()->icemanPosY()) <= 3.0)
        && (getWorld()->isFacingIceman()) && (getWorld()->recentlyShouted() != true)) {
        // 4.a. protester shouts
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        // 4.b. iceman annoyed for 2 annoyance points (iceman -2 hp)
        // annoyIceman(2);
        // 4.c. flag to say protester shouted and needs to wait at least 15 non-resting seconds
        getWorld()->flagRecentlyShouted();
        return;
    }

    // 5.
}


void SonarKit::doSomething(){
    //if ticks are == 0 then we set the sonar kit to 0 and dead
    if (getTicks() == 0){
        setDead();
        return;
    }
    //if iceman is within 3 units of the sonarkit then we setvisibility to false as well as set dead while incrementing values accordingly
    if (getWorld() != nullptr && getWorld()->icemanPosY() - getY() <= 3 && getWorld() != nullptr && getWorld()->icemanPosX() - getX() <= 3 && getState() != dead) {
        setState(dead);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(75);
        getWorld()->getIceMan()->addSonar();
    }
    //if essentially nothing happens then we decticks to decrease time it is alive
    else {decTicks();}
}

void Squirt::doSomething()
{
    if (getWorld()->squirtAnnoy(getX(), getY())){
        setState(dead);
    }
    if (getCount() == 0) {
        setState(dead);
        setVisible(false);
    }
    if (checkIce(getX(), getY(), getDirection())){
        setState(dead);
        setVisible(false);
    }
    move(getDirection());
    CountminusOne();
    return;
}


bool RegularProtester::decHealth(int v)
{
    Agent::decHealth(v);
    
    if (getHealth()<=0 && v==2)//hit by squirt
    {
        getWorld()->increaseScore(100);
    }
    return true;
}

void ActivatingObject::setTicksToLive(){
    //formula given in documentation for number of ticks to live
    int tmp = getWorld()->getLevel();
    ticks = max(100, 300 - 10*tmp);
}
