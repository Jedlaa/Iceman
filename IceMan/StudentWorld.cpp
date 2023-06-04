#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>    // std::min

using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir) : GameWorld(assetDir) {}

Ice* StudentWorld::getIce(int x, int y) {
    return m_ice[x][y];
}

void StudentWorld::deleteIce(int x, int y) {
    if (x >= 0 && x < 64 && y >= 0 && y < 60) {
        delete m_ice[x][y];
        m_ice[x][y] = nullptr;
    }
}

//void StudentWorld::deleteNug() {
//    if (m_goldNugget != nullptr) {
//        delete m_goldNugget;
//        m_goldNugget = nullptr;
//    }
//}

GoldNugget* StudentWorld::createGoldNugget(int x, int y) {
    m_goldNugget = new GoldNugget(this, x, y, true);
    return m_goldNugget;
}

int StudentWorld::init() {
    
    int x,y;
    m_iceman = new IceMan(this);
    
    
    // Creating Ice arena
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 60; j++) {
            m_ice[i][j] = new Ice(i, j);
        }
    }
    
    // Removing Ice for player starting center area
    for (int i = 30; i < 34; i++)
        for (int j = 4; j < 60; j++)
        {
            deleteIce(i, j);
        }
    
    
    // Creating boulders as well as deleing ice around
    int m_level = getLevel();
    int B = min(m_level/2+2, 9);
    for (int i = 0; i < B; i++){
        x = rand() % 28;
        y = rand() % 59;
        Boulder* boulder = new Boulder(this, x, y);
        m_boulders.push_back(boulder);
        Actors.push_back(boulder);
        for (int i = x; i < x + 4; i++) {
            for (int j = y; j < y + 4; j++) {
                deleteIce(i, j);
            }
        }
    }
    
    setInfo();
    
    C=getLevel()*25+300; // chance of adding goodies
    int protester = 2+getLevel()*1.5;
    P=min(15, protester); // num protesters

    // Creating gold nuggets
    int tmp = 5 - getLevel() / 2;
    G = max(tmp, 2);
    
    for (int i = 0; i<G ;i++){
        x = rand()%61;
        y = rand()%57;
        Actors.push_back(createGoldNugget(x, y));
    }
    
    
    auto* Pro = new RegularProtester(this);
    Actors.push_back(Pro);
    
    
    // Creating oil barrels
    tmp = getLevel();
    L = min(2 + tmp, 21);
    for (int i = 0; i < L; i++){
        x = rand()%61;
        y = rand()%57;
        OilBarrel* oil = new OilBarrel(this,30,30);
        Actors.push_back(oil);
        setBarrels(L);
    }
    
    
    
    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::checkAlive() {
    if (getLives() > 0) {
        return true;
    }
    return false;
}

int StudentWorld::move() {
    // Updating display text
    setInfo();
    
    // IceMan's actions as long as player is alive
    while (checkAlive()) {
        m_iceman->doSomething();
        
        //creating a traversal iterator
        auto tmp = Actors.begin();
        
        //while we aren't at the end call each actors doSomething() reduce typing
        while(tmp != Actors.end()){auto it = *tmp; it->doSomething(); tmp++;}
        
        if (rand() % C == 0){
            if(rand() % 5 == 0) {
                bool isArea = true;
                for (const auto& actor : Actors) {
                    if (actor->getX() == 0 && actor->getY() == 60) {
                        isArea = false;
                        break;
                    }
                }
                if (isArea) {
                    SonarKit* sonar = new SonarKit(this, 0, 60);
                    Actors.push_back(sonar);
                }
            }
            else
            {
                int x=rand()%61; int y=rand()%61;
                while (!randompostionclear(x, y)) //keep generating new position if not clear.
                {
                    x=rand()%61; y=rand()%61;
                }
                WaterPool *water= new WaterPool(this, x,y);
                Actors.push_back(water);
            }
        }
            
        
        if (barrelsLeft() == 0){
            playSound(SOUND_FINISHED_LEVEL);
            cleanUp();
            return GWSTATUS_FINISHED_LEVEL;
            
        }
        
        auto dead = Actors.begin();
        while (dead != Actors.end())
        {
            if ((*dead)->isDead())
            {
                delete (*dead);
                dead= Actors.erase(dead);
                continue;
            }
            dead++;
        }
        
        
        return GWSTATUS_CONTINUE_GAME;
        
    }
    
    
    // If not alive, decrement lives
    decLives();
    return GWSTATUS_CONTINUE_GAME;
}

//Getting the iceman's X position
int StudentWorld::icemanPosX() {
    return m_iceman->getX();
}


//Getting the iceman's Y positition
int StudentWorld::icemanPosY() {
    return m_iceman->getY();
}


void StudentWorld::setInfo() {
    int level = getLevel();
    int lives = getLives();
    int health = m_iceman->getHealth();
    int water = m_iceman->getWater();
    int sonar = m_iceman->getSonar();
    int nuggets = m_iceman->getNug();
    int score = getScore();
    
    ostringstream oss;
    oss << "Lvl: " << setw(2) << level << "  Lives: " << setw(1) << lives << "  Hlth: " << setw(3) << health * 10 << "%  Wtr: " << setw(2) << water << "  Gld: " << setw(2) << nuggets << "  Oil Left: " << "  Sonar: " << setw(2) << sonar << "  Scr: ";
    oss.fill('0');
    oss << setw(6) << score << endl;
    string s = oss.str();
    setGameStatText(s);
}

bool StudentWorld::checkBoulder(int x, int y) {
    // Checking if boulder at specified position
    for (int i = 0; i < 2; i++) {
        auto it = m_boulders.begin();
        if (*it != nullptr) {
            int boulderX = (*it)->getX();
            int boulderY = (*it)->getY();
            if (x >= boulderX - 3 && x <= boulderX + 3 && y >= boulderY - 3 && y <= boulderY + 3) {
                return true;
            }
        }
    }
    return false;
}

double StudentWorld::distance(int x1, int y1, int x2, int y2) {
    double xx = x2 - x1;
    double yy = y2 - y1;
    return sqrt((xx * xx) + (yy * yy));
}

void StudentWorld::revealHidden(int x, int y, int radius){
    auto it = Actors.begin();
    for (;it!=Actors.end(); it++)
    {
        if ((*it)->revealablebysonar()==true && (*it)->getState() != dead )
        {
            if (sqrt(((*it)->getX()-x)*((*it)->getX()-x) + ((*it)->getY()-y)*((*it)->getY()-y))<12.0)
                (*it)->setVisible(true);
        }
    }
}

void StudentWorld::cleanUp()
{
    //delete the ice vector
    for(int i=0;i<64;i++)
        for (int j=0;j<60;j++)   // Changed from 64 to 60
            if (m_ice[i][j]!=nullptr)
                deleteIce(i, j);
    if (m_iceman != nullptr) {
        delete m_iceman;
        m_iceman = nullptr;
    }

    list<Actor*>::iterator p=Actors.begin();
    while (p!= Actors.end())
    {
        delete *p;
        p=Actors.erase(p);
    }
}

bool StudentWorld::randompostionclear(int x, int y)
{
    
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
        {
            if (m_ice[x+i][y+j]!=nullptr)
                return false;
        }
    return true;
}


int StudentWorld::randInt(int min, int max) {
    return (min + (std::rand() % (max - min + 1)));
}

bool StudentWorld::isFacingIceman() {
    int icemanX = m_iceman->getX();
    int icemanY = m_iceman->getY();

    int protesterX = m_regprotester->getX();
    int protesterY = m_regprotester->getY();

    // check if regular protester facing Iceman
    if (protesterX == icemanX) {
        // in same column
        if (protesterY < icemanY && m_regprotester->getDirection() == GraphObject::up) {
            return true;
        }
        if (protesterY > icemanY && m_regprotester->getDirection() == GraphObject::down) {
            return true;
        }
    }
    if (protesterY == icemanY) {
        // in same row
        if (protesterX < icemanX && m_regprotester->getDirection() == GraphObject::right) {
            return true;
        }
        if (protesterX > icemanX && m_regprotester->getDirection() == GraphObject::left) {
            return true;
        }
    }

    return false;
}

bool StudentWorld::recentlyShouted() {
    return m_ticksSinceLastShout <= 15;
}

void StudentWorld::flagRecentlyShouted() {
    m_ticksSinceLastShout = 0;      // reset to 0 and increment each tick again
}

//void StudentWorld::createProtester(int level) {
//    // probability of hardcore protester
//
//    // create reg protester
//    RegularProtester* regprotester = new RegularProtester(this);
//    Actors.push_back(regprotester);
//
//}

void StudentWorld::CreateSquirt(int x, int y, GraphObject::Direction dir)
{
    auto squirt = new Squirt (this,x, y, dir);
    Actors.push_back(squirt);
}

bool StudentWorld::squirtAnnoy(int x, int y){
    bool b = false;
    auto p = Actors.begin();
    for (;p!=Actors.end(); p++){
        if ((*p)->annoyable()==true )
        {
            if (sqrt(((*p)->getX()-x)*((*p)->getX()-x) + ((*p)->getY()-y)*((*p)->getY()-y))<=3.0)
            {
                (*p)->decHealth(2);
                //if ((*p)->gethealth()>0) playSound(SOUND_PROTESTER_ANNOYED);
                b=true;
            }
        }
    }
    return b;
}
