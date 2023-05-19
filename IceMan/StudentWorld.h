// StudentWorld.h
// contains StudentWorld class declaration

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class IceMan;
class Ice;
class Boulder;
class GoldNugget;


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);

    virtual void setLives() {}

    virtual bool checkAlive();

    virtual int init();

    virtual int move();

    virtual void cleanUp() {}

    virtual void setInfo();

    Ice* getIce(int x, int y);

    void deleteIce(int x, int y);

    //GoldNugget* createGoldNugget(int x, int y);

private:
    IceMan* m_iceman;
    Ice* m_ice[64][64];
    Boulder* m_boulders[2];
    //GoldNugget* m_goldNugget;
};

#endif // STUDENTWORLD_H_
