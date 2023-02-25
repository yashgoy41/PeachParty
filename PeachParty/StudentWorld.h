#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Player;
class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld(){

    }
    void updateNumActors(){
        m_numActors++;
    }
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Board getBoard() const{
        return bd;
    }

private:
    std::list<Actor*> actorList;
    int m_numActors = 0;
    int m_bankMoney;
    Player* m_peach;
    Player* m_yoshi;
    Board bd;
    
};

#endif // STUDENTWORLD_H_
