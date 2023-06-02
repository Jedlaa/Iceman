#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <vector>
#include <algorithm>    // std::max


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
    //Setting the state for the actor
    virtual void setState(State s) { state = s; }
    // Retrieval function for state
    virtual State getState() { return state; }
    //Can an actor reveal goodies by sonar
    virtual bool revealablebysonar() const { return false; } // it can't

    virtual void doSomething() {};

private:
    State state;
    StudentWorld* m_studentworld;
};

class Agent : public Actor {
public:
    Agent(StudentWorld* p, int startX, int startY, Direction dir, int imageID, unsigned int hitpoints)
        : Actor(p, imageID, startX, startY, dir, 1.0, 0) {
        m_health = 10;

    }
    virtual int getHealth() { return m_health; }
private:
    int m_health;
};

class IceMan : public Agent {
public:
    IceMan(StudentWorld* p);                                // Constructor with StudentWorld params
    virtual void doSomething();                             // IceMan's actions
    virtual void destroyIce(int x, int y, Direction dir);   // Destroys Ice in specified coords & direction

    // Pick ups
    void addSonar() { m_sonar++; }
    void addWater() { m_water++; }
    void addNug() { m_nuggets++; }

    // Accessors for inventory
    virtual int getSonar() { return m_sonar; }
    virtual int getWater() { return m_water; }
    virtual int getNug() { return m_nuggets; }

    // Using items
    void decSonar() { m_sonar--; }
    void decWater() { m_water--; }
    void decNug() { m_nuggets--; }

private:
    int m_sonar;
    int m_water;
    int m_nuggets;

};

class RegularProtester : public Agent {
public:
    RegularProtester(StudentWorld* p);
    virtual void doSomething();

private:
    int numSquaresToMoveInCurrentDirection; // between 8 - 60
    int m_health;                           // 5 hp
    bool leaveOilFieldState;                // false
    bool isAlive;
    int ticksToWaitBetweenMoves;
};


class Boulder : public Actor {
public:
    Boulder(StudentWorld* p, int startX, int startY);
    virtual void doSomething();
    virtual void decCount() { countDown--; }
    virtual void setCount() { countDown = 30; }
private:
    int countDown = 30;
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


class GoldNugget : public Actor {
public:
    GoldNugget(StudentWorld* p, int startX, int startY, bool flag);
    virtual void doSomething();
    virtual bool revealablebysonar() const { return true; }
private:
    std::vector<std::pair<int, int>> pos;
    bool isAlive;
    bool isVisible;
    bool soundPlayed;
    bool isTemporary;
    bool pickUpStatus;  // True if Iceman can pickup, False if protester can pickup
    int lifetimeTicks;  // dropped GoldNuggets are temp and have a lifetime of 100
};


class ActivatingObject : public Actor {
public:
    ActivatingObject(StudentWorld* p, int startX, int startY, int imageID, int soundToPlay, bool activateOnPlayer, bool activateOnProtestor, bool initiallyActive)
        : Actor(p, imageID, startX, startY, right, 1.0, 2) {
        isAlive = activateOnPlayer;
        setState(waiting);
    }
    virtual void move() {}
    void setTicksToLive();
    void setDead() { isAlive = false; }
    bool getStatus() { return isAlive; }
    int getTicks() { return ticks; }
    void decTicks() { ticks--; }
private:
    bool isAlive;
    int ticks;
};


class OilBarrel : public ActivatingObject {
public:
    OilBarrel(StudentWorld* p, int startX, int startY);
    virtual void doSomething() {}
    virtual void move() {}
    virtual bool revealablebysonar() const { return true; }
    virtual bool needsToBePikcedUpToFinishLevel() const { return true; }
};

class SonarKit : public ActivatingObject {
public:
    SonarKit(StudentWorld* p, int startX, int startY);
    virtual void doSomething();
    virtual void move() {}
};

class WaterPool : public ActivatingObject {
public:
    WaterPool(StudentWorld* p, int startX, int startY);
    virtual void doSomething() {}
    virtual void move() {}
};


#endif // ACTOR_H_