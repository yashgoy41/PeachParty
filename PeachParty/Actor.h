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
    int getSpriteDirection() const{
        return getDirection();
    };
    void setSpriteDirection(int dir){
        setDirection(dir);
    }
    StudentWorld* getWorld() const{
        return m_world;
    }
    void setActive(){
        m_active = true;
    }
    void setDead(){
        m_active = false;
    }
    bool isActive() const{
        return m_active;
    }
private:
    StudentWorld* m_world;
    bool m_active = true;

};

class MovingActor: public Actor{
public:
    MovingActor(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(imageID, startX, startY, right, 0, 1.0, world){
        m_walkDir = right;
        m_ticks_to_move = 0;
    };
    ~MovingActor(){};
    virtual void doSomething() = 0;
    int getWalkDir() const{
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
    int numTicks() const{
        return m_ticks_to_move;
    }
    bool canMoveForward(int dir);
    bool isAligned() const;
private:
    int m_walkDir;
    int m_ticks_to_move;
};

class Player : public MovingActor {
public:
    Player(int imageID, int playerNumber, int startX, int startY, StudentWorld* world)
                   : MovingActor(imageID, startX, startY, world){
                       m_playerNumber = playerNumber;
                       m_stars = 0;
                       m_coins = 0;
                       m_vortex = false;
                       m_isWalking = false;
                       setWalkDir(right);
                       setSpriteDirection(right);
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
        return m_stars;
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
    int m_stars;
    int m_coins;
    bool m_vortex;
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

// Might have to make this another dericed class
class Vortex: public MovingActor { // or some subclass of Actor
public:
    Vortex(int imageID, int startX, int startY, StudentWorld* world): MovingActor(imageID, startX, startY, world){};
    ~Vortex(){};
    virtual void doSomething() { return;}
private:
};

class Square: public Actor{
public:
    Square(int imageID, int startX, int startY, StudentWorld* world) : Actor(imageID, startX, startY, right, 1, 1.0, world){
        m_IsPeachNew = true;
        m_IsYoshiNew = true;
    };
    ~Square(){};
    bool isPeachNew(){
        return m_IsPeachNew;
    }
    bool isYoshiNew(){
        return m_IsYoshiNew;
    }
    void setIsPeachNewTrue(){
        m_IsPeachNew = true;
    }
    void setIsPeachNewFalse(){
        m_IsPeachNew = false;
    }
    void setIsYoshiNewTrue(){
        m_IsYoshiNew = true;
    }
    void setIsYoshiNewFalse(){
        m_IsYoshiNew = false;
    }
    virtual void doSomething() = 0;
private:
    bool m_IsPeachNew;
    bool m_IsYoshiNew;
};

class CoinSquare: public Square {// or some subclass of Actor
public:
    CoinSquare(int imageID, int startX, int startY, StudentWorld* world, int numCoins): Square(imageID, startX, startY, world){
        m_numCoinsModified = numCoins;
    };
    ~CoinSquare(){};
    virtual void doSomething();
    int numCoinsModified(){
        return m_numCoinsModified;
    }
private:
    int m_numCoinsModified;
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
