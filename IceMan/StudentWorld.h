#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include "GraphObject.h"


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class IceMan;
class Ice;
class Boulder;
class GoldNugget;
class Actor;
class SonarKit;
class OilBarrel;
class RegularProtester;


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);

    virtual void setLives() {}
    
    virtual void cleanUp();
    
    virtual void revealHidden(int X, int Y, int radius);

    virtual bool checkAlive();

    virtual int init();

    virtual int move();


    virtual int icemanPosX();

    virtual int icemanPosY();

    virtual void setInfo();

    Ice* getIce(int x, int y);

    void deleteIce(int x, int y);

    GoldNugget* createGoldNugget(int x, int y);

    bool checkBoulder(int x, int y);

    IceMan* getIceMan() { return m_iceman; }

//    GoldNugget* getGoldNugget() { return m_goldNugget; }

    double distance(int x1, int y1, int x2, int y2);    // used for checking radius of gold nugget, iceman, protesters

    void setBarrels(int n){barrels = n;}

    void decBarrels(){barrels--;}
    
    int barrelsLeft(){return barrels;}
    //void deleteNug();
    
    int randInt(int min, int max);

    bool isFacingIceman();

    bool recentlyShouted();

    void flagRecentlyShouted();
    
    void CreateSquirt(int x, int y, GraphObject::Direction dir);
    
    bool squirtAnnoy(int x, int y);



    //void createProtester(int level);

private:
    std::list<Actor*> Actors;
    IceMan* m_iceman;
    Ice* m_ice[64][64];
    std::vector<Boulder*> m_boulders;
    GoldNugget* m_goldNugget;
    SonarKit* m_sonar;
    OilBarrel* m_oil;
    int G; //number of nuggets
    int L; //number of barrels
    int barrels;
    RegularProtester* m_regprotester;
    int m_ticksSinceLastShout;

//    std::list<GoldNugget*> m_goldNuggets;
    
    //Protester* m_protester;
};

#endif // STUDENTWORLD_H_
