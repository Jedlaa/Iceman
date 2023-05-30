#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

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
    m_goldNugget = new GoldNugget(this, x, y, true); //temporary testing position
    return m_goldNugget;
}

int StudentWorld::init() {
    m_iceman = new IceMan(this);

    // Creating Ice
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 60; j++) {
            m_ice[i][j] = new Ice(i, j);
        }
    }

    // Removing Ice for player starting area
    for (int i = 30; i < 34; i++)
        for (int j = 4; j < 60; j++)
        {
            deleteIce(i, j);
        }

    // Randomly place 2 boulders
    int tmpX = rand() % 28;
    int tmpY = rand() % 60;
    m_boulders[0] = new Boulder(this, tmpX, tmpY);
    for (int i = tmpX; i < tmpX + 4; i++) {
        for (int j = tmpY; j < tmpY + 4; j++) {
            deleteIce(i, j);
        }
    }

    tmpX = rand() % 29 + 32;
    tmpY = rand() % 60;
    m_boulders[1] = new Boulder(this, tmpX, tmpY);
    for (int i = tmpX; i < tmpX + 4; i++) {
        for (int j = tmpY; j < tmpY + 4; j++) {
            deleteIce(i, j);
        }
    }
    setInfo();
    int tmp = getLevel() / 2;
    int numNug = max(tmp, 2);

    for (int i = 0; i<numNug;i++){
        int x = rand()%61;
        int y = rand()%57;
        Actors.push_back(createGoldNugget(x, y));

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
        //        m_goldNugget->doSomething();
        list<Actor*>::iterator tmp = Actors.begin();
        while(tmp != Actors.end()){auto it = *tmp; it->doSomething(); tmp++;}
        for(int i = 0;i < 2;i++){m_boulders[i]->doSomething();}
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
        if (m_boulders[i] != nullptr) {
            int boulderX = m_boulders[i]->getX();
            int boulderY = m_boulders[i]->getY();
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
