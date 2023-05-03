#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir) : GameWorld(assetDir){
}

Ice* StudentWorld::getice(int x, int y){
    return m_ice[x][y];
}

void StudentWorld::deleteIce(int x, int y){
    delete m_ice[x][y];
    m_ice[x][y] = nullptr;
}

int StudentWorld::init(){
    m_iceman = new IceMan(this);

    
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 60; j++){
            m_ice[i][j] = new Ice(i,j);
        }
    }
    for (int i=30; i<34; i++)
        for (int j=4; j<60; j++)
        {delete m_ice[i][j];
            m_ice[i][j]=nullptr;
        }

    
    int tmpX = rand() % 28;
    int tmpY =rand() % 60;
    m_boulders[0] = new boulder(this, tmpX , tmpY);
    for (int i = tmpX; i != tmpX+4; i++){
        for (int j = tmpY; j != tmpY+4; j++){
            deleteIce(i, j);
        }
    }
    tmpX =rand() % 29 + 32;
    tmpY = rand() % 60;
    m_boulders[1] = new boulder(this, tmpX, tmpY );
    for (int i = tmpX; i != tmpX+4; i++){
        for (int j = tmpY; j != tmpY+4; j++){
            deleteIce(i, j);
        }
    }
    
    
    setInfo();
    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::checkAlive(){
    if (getLives()>0){
        return true;
    }
    return false;
}

int StudentWorld::move(){
    while(checkAlive()){
        m_iceman->doSomething();
        return GWSTATUS_CONTINUE_GAME;
        
    }
    decLives();
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::setInfo(){
    setGameStatText("this->getLives()");
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
