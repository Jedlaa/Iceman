// Actor.cpp
// implementation of classes from Actor.h

#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>           // cmath?
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
    if (d == down) {
        setDirection(down);
        moveTo(getX(), getY() - 1);

    } else if (d == up) {
        setDirection(up);
        moveTo(getX(), getY() + 1);

    } else if (d == left) {
        setDirection(left);
        moveTo(getX() - 1, getY());

    } else if (d == right) {
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
Boulder::Boulder(StudentWorld* p, int startX, int startY) : Actor(p, IID_BOULDER, startX, startY, down, 1.0, 10.0) {}

// WaterGun constructor
WaterGun::WaterGun(StudentWorld* p, int x, int y, Direction d)
    : Actor(p, IID_WATER_SPURT, x, y, d, 1.0, 1) {}

//// GoldNugget constructor
GoldNugget::GoldNugget(StudentWorld* p, int startX, int startY)
    : Actor(p, IID_GOLD, startX, startY, right, 1.0, 2) {
        pos.push_back({startX, startY});
    setVisible(false);
}


// Ice constructor
Ice::Ice(int startX, int startY)
    : Actor(IID_ICE, startX, startY, right, 0.25, 3) {}

// Check if there's Ice at specified coords & direction
bool Actor::checkIce(int x, int y, Direction dir) {
    if (getWorld()->getIce(x, y) != nullptr) {
        return true;
    }
    return false;
}

void IceMan::destroyIce(int x, int y, Direction dir) {
    if (dir == down) {
        for (int i = 0; i < 4; i++) {
            getWorld()->deleteIce(x + i, y);
        }
        //getworld()->deleteIce(x, y);
        //getworld()->deleteIce(x + 1, y);
        //getworld()->deleteIce(x + 2, y);
        //getworld()->deleteIce(x + 3, y);

    } else if (dir == up) {
        for (int i = 0; i < 4; i++) {
            getWorld()->deleteIce(x + i, y + 3);
        }

    } else if (dir == left) {
        for (int i = 0; i < 4; i++) {
            getWorld()->deleteIce(x, y + i);
        }

    } else if (dir == right) {
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
                destroyIce(getX(), getY() + 1, up);
                moveTo(getX(), getY() + 1);
                setDirection(up);
            }

        } else if (move == KEY_PRESS_DOWN) {
            if (checkBoundary(getX(), getY() - 1)) {
                destroyIce(getX(), getY() - 1, down);
                moveTo(getX(), getY() - 1);
                setDirection(down);
            }

        } else if (move == KEY_PRESS_LEFT) {
            if (checkBoundary(getX() - 1, getY())) {
                destroyIce(getX() - 1, getY(), left);
                moveTo(getX() - 1, getY());
                setDirection(left);
            }

        } else if (move == KEY_PRESS_RIGHT) {
            if (checkBoundary(getX() + 1, getY())) {
                destroyIce(getX() + 1, getY(), right);
                moveTo(getX() + 1, getY());
                setDirection(right);
            }
        }
    }
}

//nuggets do something
void GoldNugget::doSomething(){
    for (const auto& p : pos) {
        if (getWorld()->icemanPosX() == p.first && getWorld()->icemanPosY() == p.second){
            setVisible(true);
            
        }
    }
    
}
