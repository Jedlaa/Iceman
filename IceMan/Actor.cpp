#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>
#include <algorithm>
using namespace std;


Actor::Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0): GraphObject(imageID, startX, startY, dir, size ,depth){
    setVisible(true);
}
Actor::Actor(StudentWorld* p, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0): GraphObject(imageID, startX, startY, dir, size ,depth){
    setVisible(true);
    m_studentworld = p;
}


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
IceMan::IceMan(StudentWorld* p) : Agent(p, 30, 60, right, IID_PLAYER, 10){
}

boulder::boulder(StudentWorld*p, int startX, int startY) : Actor(p, IID_BOULDER, startX, startY, down, 1.0, 10.0){
}

bool Actor::checkBoundary(int x, int y){
    cout << "Checking position (" << x << ", " << y << ")" << endl;
    if (x >= 0 && x < 60 && y >= 0 && y < 60){
        cout << "Position (" << x << ", " << y << ") is valid" << endl;
        return true;
    }
    cout << "Position (" << x << ", " << y << ") is invalid" << endl;
    return false;
}
//
Ice::Ice(int startX, int startY) : Actor(IID_ICE, startX,startY,right,0.25, 3){
}

bool Actor::checkIce(int x, int y, Direction dir){
    if (getworld()->getice(x, y) != nullptr){
        return true;
    }
    return false;
}

void IceMan::destroyIce(int x, int y, Direction dir){
    if(dir == down){
            getworld()->deleteIce(x, y);
            getworld()->deleteIce(x+1, y);
            getworld()->deleteIce(x+2, y);
            getworld()->deleteIce(x+3, y);
    }
    if (dir == up){
            getworld()->deleteIce(x, y+2);
            getworld()->deleteIce(x+1, y+2);
            getworld()->deleteIce(x+2, y+2);
            getworld()->deleteIce(x+3, y+2);
        
    }
    if (dir == left){
            getworld()->deleteIce(x, y);
            getworld()->deleteIce(x, y+1);
            getworld()->deleteIce(x, y+2);
            getworld()->deleteIce(x, y+3);
    }
}

void IceMan::doSomething(){
    int move;
    
    if(getworld()->getKey(move) == true){
        if ( move == KEY_PRESS_UP){
            if(checkBoundary(getX(), getY()+1)){
                destroyIce(getX(), getY()+1, up);
                moveTo(getX(), getY()+1);
                setDirection(up);
            }

            
        }
        if ( move == KEY_PRESS_DOWN){
            if(checkBoundary(getX(), getY()-1)){
                destroyIce(getX(), getY()-1, down);
                moveTo(getX(), getY()-1);
                setDirection(down);
            }
        }
        if ( move == KEY_PRESS_LEFT){
            if(checkBoundary(getX()-1, getY())){
                destroyIce(getX()-1, getY(), left);
                moveTo(getX()-1, getY());
                setDirection(left);
            }
        }
        if ( move == KEY_PRESS_RIGHT){
            if(checkBoundary(getX()+1, getY())){
                destroyIce(getX()+1, getY(), right);
                moveTo(getX()+1, getY());
                setDirection(right);
            }
        }
    }
    
}
