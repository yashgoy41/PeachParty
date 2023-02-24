#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld(){
        for (std::list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); ++it) {
            delete *it;
        }
    }
    void updateNumActors(){
        m_numActors++;
    }
    virtual int init();
    virtual int move();
    virtual void cleanUp();

private:
    std::list<Actor*> actorList;
    int m_numActors = 0;
    
};

#endif // STUDENTWORLD_H_
