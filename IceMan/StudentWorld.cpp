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

//    // Randomly place 2 boulders
//    int tmpX = rand() % 28;
//    int tmpY = rand() % 59;
//    m_boulders[0] = new Boulder(this, tmpX, tmpY);
//    for (int i = tmpX; i < tmpX + 4; i++) {
//        for (int j = tmpY; j < tmpY + 4; j++) {
//            deleteIce(i, j);
//        }
//    }
//    //Randomly place second boulder
//    tmpX = rand() % 29 + 32;
//    tmpY = rand() % 59;
//    m_boulders[1] = new Boulder(this, tmpX, tmpY);
//    Actors.push_back(m_boulders[0]);
//    Actors.push_back(m_boulders[1]);
//    for (int i = tmpX; i < tmpX + 4; i++) {
//        for (int j = tmpY; j < tmpY + 4; j++) {
//            deleteIce(i, j);
//        }
//    }
    
    int m_level = getLevel();
    int B = min(m_level/2+2, 9);
    for (int i = 0; i < B; i++){
        int tmpX = rand() % 28;
        int tmpY = rand() % 59;
        Boulder* boulder = new Boulder(this, tmpX, tmpY);
        m_boulders.push_back(boulder);
        Actors.push_back(boulder);
        for (int i = tmpX; i < tmpX + 4; i++) {
            for (int j = tmpY; j < tmpY + 4; j++) {
                deleteIce(i, j);
            }
        }
    }

    setInfo();
    int tmp = 5 - getLevel() / 2;
    G = max(tmp, 2);

    for (int i = 0; i<G ;i++){
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
        
        
        //creating a traversal iterator
        list<Actor*>::iterator tmp = Actors.begin();
        
        //while we aren't at the end call each actors doSomething() reduce typing
        while(tmp != Actors.end()){auto it = *tmp; it->doSomething(); tmp++;}
                
        //problem here seems like sonars get called very often causing multiple sonars to exist in one location
        if (rand() % G == 0 && rand() % 5 == 0) {
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
            if ((*it)->revealablebysonar()==true )
            {
                if (sqrt(((*it)->getX()-x)*((*it)->getX()-x) + ((*it)->getY()-y)*((*it)->getY()-y))<12.0)
                    (*it)->setVisible(true);
            }
        }
    }

