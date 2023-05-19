#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

enum State {
    stable = 1,
    waiting = 2,
    falling = 3,
    dead = 0,
    active = 4,
    pickablebyiceman = 5,
    pickablebyprotesters = 6,
    bribed = 7
};


class Actor : public GraphObject {
public:
    // Constructor with explicit params
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);

    // Constructor with StudentWorld params
    Actor(StudentWorld* p, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);

    // Accessor for StudentWorld ptr
    virtual StudentWorld* getWorld() { return m_studentworld; }

    // Check if coords are within game bounds
    virtual bool checkBoundary(int x, int y);

    // Check if there's Ice at specified coords & direction
    virtual bool checkIce(int x, int y, Direction dir);

    // Move in specified direction
    virtual void move(Direction dir);

private:
    State state;
    StudentWorld* m_studentworld;
};

class Agent : public Actor {
public:
    // Constructor with explicit params
    Agent(StudentWorld* p, int startX, int startY, Direction dir, int imageID, unsigned int hitpoints)
        : Actor(p, imageID, startX, startY, dir, 1.0, 0) {      // m_health(hitpoints)?
        m_health = 10;
    }
    // Accessor for hp
    virtual int getHealth() { return m_health; }

private:
    int m_health;
};

class IceMan : public Agent {
public:
    // Constructor with StudentWorld params
    IceMan(StudentWorld* p);

    // IceMan's actions
    virtual void doSomething();

    // Destroys Ice in specified coords & direction
    virtual void destroyIce(int x, int y, Direction dir);

    // Accessors for inventory
    virtual int getSonar() { return m_sonar; }
    virtual int getWater() { return m_water; }
    virtual int getNug() { return m_nuggets; }

private:
    int m_sonar;
    int m_water;
    int m_nuggets;

};

class Boulder : public Actor {
public:
    Boulder(StudentWorld* p, int startX, int startY);
};

class Ice : public Actor {
public:
    Ice(int startX, int startY);
private:
};

class WaterGun : public Actor {
public:
    WaterGun(StudentWorld* pointer, int x, int y, Direction d);
};

//class GoldNugget : public Actor {
//public:
//    GoldNugget(StudentWorld* pointer, int x, int y, bool isVisible, bool isPickableByIceman, bool isPermanent);
//    virtual void doSomething();
//
//private:
//    bool isVisible;
//    bool isPickableByIceman;
//    bool isPermanent;
//};

#endif // ACTOR_H_
