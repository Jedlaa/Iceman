#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

enum State {stable=1, waiting=2, falling=3, dead=0, active=4, pickablebyiceman=5, pickablebyprotesters=6, bribed=7};


class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    Actor(StudentWorld* p, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual StudentWorld* getworld(){
        return m_studentworld;
    }
    virtual bool checkBoundary(int x, int y);
    virtual bool checkIce(int x, int y, Direction dir);
    
private:
    State state;
    StudentWorld* m_studentworld;
};

class Agent : public Actor {
public:
    Agent(StudentWorld* p, int startX, int startY, Direction dir, int imageID, unsigned int hitpoints): Actor(p, imageID, startX, startY, dir, 1.0, 0){
        
    }
    virtual void setHealth(){
        m_health = 100; 
    }
private:
    int m_health;
};

class IceMan : public Agent {
public:
    IceMan(StudentWorld* p);
    virtual void doSomething();
    virtual void destroyIce(int x, int y, Direction dir);
    

private:
    int m_sonar;
    int m_water;
    int m_nuggets;
    
};

class items : public Actor{
    
private:
    
};

class boulder : public Actor{
public:
    boulder(StudentWorld* p, int startX, int startY);
};

class Ice : public Actor{
public:
    Ice(int startX, int startY);
private:
};
#endif // ACTOR_H_
