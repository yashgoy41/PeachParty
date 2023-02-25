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
    ~StudentWorld(){};
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Board getBoard() {
        return bd;
    }
    Player* getPeach(){
        return m_peach;
    }
    Player* getYoshi(){
        return m_yoshi;
    }
    Player* getPlayer(int playerNum){
        switch (playerNum) {
            case 1:
                return m_peach;
                break;
            case 2:
                return m_yoshi;
                break;
            default:
                return nullptr;
                break;
        }
    }
    void addPlayers(){
        m_numPlayers++;
    }
    int getNumPlayers(){
        return m_numPlayers;
    }

private:
    std::list<Actor*> actorList;
    int m_numPlayers;
    int m_bankMoney;
    Player* m_peach;
    Player* m_yoshi;
    Board bd;
    
};

#endif // STUDENTWORLD_H_
