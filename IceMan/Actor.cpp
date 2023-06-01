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
void Actor::move(Direction d)
{
    if (d == down)
    {
        setDirection(down); 
        moveTo(getX(), getY() - 1);
    }
    else if (d == up)
    {
        setDirection(up); 
        moveTo(getX(), getY() + 1);
    }
    else if (d == left)
    {
        setDirection(left); 
        moveTo(getX() - 1, getY());
    }
    else if (d == right)
    {
        setDirection(right); 
        moveTo(getX() + 1, getY());
    }
}

// Check coords are within game bounds
bool Actor::checkBoundary(int x, int y) {
    if (x >= 0 && x < 62 && y >= 0 && y < 60) {
        return true;
    }
    return false;
}


// IceMan constructor
IceMan::IceMan(StudentWorld* p) 
    : Agent(p, 30, 60, right, IID_PLAYER, 10) {
    m_sonar = 3;
    m_water = 5;
    m_nuggets = 0;
}

// Boulder constructor
Boulder::Boulder(StudentWorld* p, int startX, int startY)
    : Actor(p, IID_BOULDER, startX, startY, down, 1.0, 10.0) {
        setState(stable);
    }

// WaterGun constructor
WaterGun::WaterGun(StudentWorld* p, int x, int y, Direction d) 
    : Actor(p, IID_WATER_SPURT, x, y, d, 1.0, 1) {}

// Ice constructor
Ice::Ice(int startX, int startY) 
    : Actor(IID_ICE, startX, startY, right, 0.25, 3) {}

SonarKit::SonarKit(StudentWorld* p, int startX, int startY)
    : ActivatingObject(p, startX, startY, IID_SONAR, SOUND_SONAR, true, false, false){
        setTicksToLive();
        setVisible(true);
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
    }
}

// GoldNugget constructor
GoldNugget::GoldNugget(StudentWorld* p, int startX, int startY, bool flag)
    : Actor(p, IID_GOLD, startX, startY, right, 1.0, 2) {
    pos.push_back({ startX, startY });

    setVisible(false);

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
    if (!isVisible && getWorld()->distance(getX(), getY(), getWorld()->icemanPosY(), getWorld()->icemanPosY()) <= 10.0) {
        setVisible(true);
        //return;
    }

    // 3. Check if Gold Nugget is pickupable by Iceman (PickupStatus = true) & within a radius of 3.0
    if (pickUpStatus == true && getWorld()->distance(getX(), getY(), getWorld()->icemanPosX(), getWorld()->icemanPosY()) <= 3.0) {
        // GoldNugget state to dead (to be removed from game at end of current tick)
        isAlive = false; //setState(dead);
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



//fixed boulder problem
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


void SonarKit::doSomething(){
    //if ticks are == 0 then we set the sonar kit to 0 and dead
    if (getTicks() == 0){
        setVisible(false);
        setDead();
        return;
    }
    //if iceman is within 3 units of the sonarkit then we setvisibility to false as well as set dead while incrementing values accordingly
    if (getWorld() != nullptr && getWorld()->icemanPosY() - getY() <= 3 && getWorld() != nullptr && (getWorld()->icemanPosX() - getX() <= 3) && getState() != dead) {
        setVisible(false);
        setState(dead);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(75);
        getWorld()->getIceMan()->addSonar();
    }
    //if essentially nothing happens then we decticks to decrease time it is alive
    else {decTicks();}
}


void ActivatingObject::setTicksToLive(){
    //formula given in documentation for number of ticks to live
    int tmp = getWorld()->getLevel();
    ticks = max(100, 300 - 10*tmp);
}
