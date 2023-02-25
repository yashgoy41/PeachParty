#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : GraphObject(imageID, startX, startY, dir, depth, size){
        m_world = world;
    };
    ~Actor(){};
    virtual void doSomething() = 0;
    int getSpriteDirection(){
        return getDirection();
    };
    void setSpriteDirection(int dir){
        setDirection(dir);
    }
    StudentWorld* getWorld(){
        return m_world;
    }
    void setDead(){
        m_active = 0;
    }
    void setActive(){
        m_active = 1;
    }
    int isActive(){
        return m_active;
    }
private:
    StudentWorld* m_world;
    int m_active = 1;

};

class MovingActor: public Actor{
public:
    MovingActor(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(imageID, startX, startY, right, 0, 1.0, world){
        m_walkDir = right;
        m_ticks_to_move = 0;
    };
    ~MovingActor(){};

    int getWalkDir(){
        return m_walkDir;
    }
    void setWalkDir(int dir){
        m_walkDir = dir;
    };
    void setTicks(int num){
        m_ticks_to_move = num;
    }
    void decrementTicks(){
        m_ticks_to_move -= 1;
    }
    int numTicks(){
        return m_ticks_to_move;
    }
    bool canMoveForward(int dir);
    bool isAligned();
private:
    int m_walkDir;
    int m_ticks_to_move;
};

class Player : public MovingActor {
public:
    Player(int imageID, int playerNumber, int startX, int startY, StudentWorld* world)
                   : MovingActor(imageID, startX, startY, world){
                       m_playerNumber = playerNumber;
                       m_isWalking = false;
                       setWalkDir(right);
                       setSpriteDirection(right);
                       world->updateNumActors();
    }
    ~Player(){}
    virtual void doSomething();
    bool isWalking(){
        return m_isWalking;
    }
    
    void updateStars(int value){
        m_stars += value;
    }
    int getStars(){
        return m_coins;
    }
    
    void updateCoins(int value){
        m_coins += value;
    }
    int getCoins(){
        return m_coins;
    }
    
private:
    int m_playerNumber;
    bool m_isWalking;
    int m_stars = 0;
    int m_coins = 0;
};

class Baddie: public MovingActor{
public:
    Baddie(int imageID, int startX, int startY, StudentWorld* world): MovingActor(imageID, startX, startY, world){}
    ~Baddie(){}
    virtual void doSomething() { return;}
private:
};

class Bowser: public Baddie { // or some subclass of Actor
public:
    Bowser(int imageID, int startX, int startY, StudentWorld* world): Baddie(imageID, startX, startY, world){};
    ~Bowser(){};
    virtual void doSomething() { return;}
private:
};

class Boo: public Baddie { // or some subclass of Actor
public:
    Boo(int imageID, int startX, int startY, StudentWorld* world): Baddie(imageID, startX, startY, world){};
    ~Boo(){};
    virtual void doSomething() { return;}
private:
};

class Vortex: public MovingActor { // or some subclass of Actor
public:
    Vortex(int imageID, int startX, int startY, StudentWorld* world): MovingActor(imageID, startX, startY, world){};
    ~Vortex(){};
    virtual void doSomething() { return;}
private:
};

class Square: public Actor{
public:
    Square(int imageID, int startX, int startY, StudentWorld* world) : Actor(imageID, startX, startY, right, 1, 1.0, world){};
    ~Square(){};
    virtual void doSomething() { return;}
};

class CoinSquare: public Square {// or some subclass of Actor
public:
    CoinSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~CoinSquare(){};
    virtual void doSomething() { return;}
private:
};

class StarSquare: public Square {// or some subclass of Actor
public:
    StarSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~StarSquare(){};
    virtual void doSomething() { return;}
private:
};

class DirectionSquare: public Square {// or some subclass of Actor
public:
    DirectionSquare(int imageID, int startX, int startY, int dir, StudentWorld* world): Square(imageID, startX, startY, world){
        switch (dir) {
            case right:
                setDirection(right);
                break;
            case left:
                setDirection(left);
                break;
            case up:
                setDirection(up);
                break;
            case down:
                setDirection(down);
                break;
            default:
                break;
        }
    };
    ~DirectionSquare(){};
    virtual void doSomething() { return;}
private:
};

class BankSquare: public Square {// or some subclass of Actor
public:
    BankSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~BankSquare(){};
    virtual void doSomething() { return;}
private:
};

class EventSquare: public Square {// or some subclass of Actor
public:
    EventSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~EventSquare(){};
    virtual void doSomething() { return;}
private:
};

class DropSquare: public Square {// or some subclass of Actor
public:
    DropSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~DropSquare(){};
    virtual void doSomething() { return;}
private:
};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
