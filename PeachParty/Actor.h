#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : GraphObject(imageID, startX, startY, dir, depth, size){
        m_world = world;
    };
    ~Actor(){};
    virtual void dosomething() = 0;
    
    StudentWorld* getWorld(){
        return m_world;
    }
private:
    StudentWorld* m_world;

};

class MovingActor: public Actor{
public:
    MovingActor(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(imageID, startX, startY, right, 0, 1.0, world){
        m_spriteDir = right;
        m_walkDir = right;
    };
    ~MovingActor(){};

    int getWalkDir(){
        return m_walkDir;
    }
    void setWalkDirection(int dir){
        m_walkDir = dir;
    };
    int getSpriteDirection(){
        return getDirection();
    };
    void setSpriteDirection(int dir){
        setDirection(dir);
    }
private:
    int m_spriteDir;
    int m_walkDir;
};

class Player : public MovingActor {
public:
    Player(int imageID, int playerNumber, int startX, int startY, StudentWorld* world)
                   : MovingActor(imageID, startX, startY, world){
                       m_playerNumber = 1;
                       m_state = 0;
                       setWalkDirection(right);
                       setSpriteDirection(right);
    };
    virtual void dosomething();
    void changeState();
    ~Player(){};
private:
    int m_playerNumber;
    int m_state;
};

class Baddie: public MovingActor{
public:
    Baddie(int imageID, int startX, int startY, StudentWorld* world): MovingActor(imageID, startX, startY, world){};
    ~Baddie(){};
    virtual void dosomething() { return;}
private:
};

class Bowser: public Baddie { // or some subclass of Actor
public:
    Bowser(int imageID, int startX, int startY, StudentWorld* world): Baddie(imageID, startX, startY, world){};
    ~Bowser(){};
    virtual void dosomething() { return;}
private:
};

class Boo: public Baddie { // or some subclass of Actor
public:
    Boo(int imageID, int startX, int startY, StudentWorld* world): Baddie(imageID, startX, startY, world){};
    ~Boo(){};
    virtual void dosomething() { return;}
private:
};

class Vortex: public MovingActor { // or some subclass of Actor
public:
    Vortex(int imageID, int startX, int startY, StudentWorld* world): MovingActor(imageID, startX, startY, world){};
    ~Vortex(){};
    virtual void dosomething() { return;}
private:
};

class Square: public Actor{
public:
    Square(int imageID, int startX, int startY, StudentWorld* world) : Actor(imageID, startX, startY, right, 1, 1.0, world){};
    ~Square(){};
    virtual void dosomething() { return;}
};

class CoinSquare: public Square {// or some subclass of Actor
public:
    CoinSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~CoinSquare(){};
    virtual void dosomething() { return;}
private:
};

class StarSquare: public Square {// or some subclass of Actor
public:
    StarSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~StarSquare(){};
    virtual void dosomething() { return;}
private:
};

class DirectionSquare: public Square {// or some subclass of Actor
public:
    DirectionSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~DirectionSquare(){};
    virtual void dosomething() { return;}
private:
};

class BankSquare: public Square {// or some subclass of Actor
public:
    BankSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~BankSquare(){};
    virtual void dosomething() { return;}
private:
};

class EventSquare: public Square {// or some subclass of Actor
public:
    EventSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~EventSquare(){};
    virtual void dosomething() { return;}
private:
};

class DropSquare: public Square {// or some subclass of Actor
public:
    DropSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    ~DropSquare(){};
    virtual void dosomething() { return;}
private:
};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
