#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : GraphObject(imageID, startX, startY, dir, depth, size){
        m_world = world;
    };
    virtual ~Actor(){};
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
    bool overlaps(const Actor &a) const;
private:
    StudentWorld* m_world;
    bool m_active = true;

};

class MovingActor: public Actor{
public:
    MovingActor(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(imageID, startX, startY, right, 0, 1.0, world){
        m_walkDir = right;
        m_isWalking = false;
        m_justLanded = true;
        m_ticks_to_move = 0;
    }
    virtual ~MovingActor(){};
    virtual void doSomething() = 0;
    int getWalkDir() const{
        return m_walkDir;
    }
    void setWalkDir(int dir){
        m_walkDir = dir;
    }
    void setTicks(int num){
        m_ticks_to_move = num;
    }
    void decrementTicks(){
        m_ticks_to_move -= 1;
    }
    int numTicks() const{
        return m_ticks_to_move;
    }
    bool hasJustLanded() const{
        return m_justLanded;
    }
    void setJustLanded(bool val){
        m_justLanded = val;
    }
    bool getIsWalking() const{
        return m_isWalking;
    }
    void setIsWalking(bool val) {
        m_isWalking = val;
    }
    void teleport(MovingActor &m);
    bool canMoveForward(int dir) const;
    bool isAligned() const;
    
private:
    int m_walkDir;
    bool m_isWalking;
    int m_ticks_to_move;
    bool m_justLanded;
};

class Player : public MovingActor {
public:
    Player(int imageID, int playerNumber, int startX, int startY, StudentWorld* world);
    virtual ~Player(){}
    virtual void doSomething();
    void updateStars(int value){
        m_stars += value;
    }
    int getStars() const{
        return m_stars;
    }
    void updateCoins(int value){
        if((m_coins + value) < 0){
            m_coins = 0;
        }
        else{
            m_coins += value;
        }
    }
    void resetCoins(){
        m_coins = 0;
    }
    void resetStars(){
        m_stars = 0;
    }
    int getCoins() const{
        return m_coins;
    }
    bool hasVortex() const{
        return m_vortex;
    }
    void setVortex(bool val) {
        m_vortex = val;
    }
    void swapWithOtherPlayer();
    bool wasHurtByBaddie(){
        return m_hurtByBaddie;
    }
    void setHurtByBaddie(bool val){
        m_hurtByBaddie = val;
    }
private:
    int m_playerNumber;
    int m_stars;
    int m_coins;
    bool m_vortex;
    bool m_hurtByBaddie;
};

class Baddie: public MovingActor{
public:
    Baddie(int imageID, int startX, int startY, StudentWorld* world): MovingActor(imageID, startX, startY, world){
        m_travelDist = 0;
        m_pauseCtr = 180;
    }
    virtual ~Baddie(){}
    virtual void doSomething() { return;}
    int getPauseCtr(){
        return m_pauseCtr;
    }
    void resetPauseCtr(){
        m_pauseCtr = 180;
    }
    void decrementPauseCtr(){
        m_pauseCtr--;
    }
private:
    int m_travelDist;
    int m_pauseCtr;
};

class Bowser: public Baddie { // or some subclass of Actor
public:
    Bowser(int imageID, int startX, int startY, StudentWorld* world): Baddie(imageID, startX, startY, world){};
    virtual ~Bowser(){};
    virtual void doSomething();
private:
};

class Boo: public Baddie { // or some subclass of Actor
public:
    Boo(int imageID, int startX, int startY, StudentWorld* world): Baddie(imageID, startX, startY, world){};
    virtual ~Boo(){};
    virtual void doSomething() { return;}
private:
};

// Might have to make this another dericed class
class Vortex: public MovingActor { // or some subclass of Actor
public:
    Vortex(int imageID, int startX, int startY, StudentWorld* world): MovingActor(imageID, startX, startY, world){};
    virtual ~Vortex(){};
    virtual void doSomething() { return;}
private:
};

class Square: public Actor{
public:
    Square(int imageID, int startX, int startY, StudentWorld* world) : Actor(imageID, startX, startY, right, 1, 1.0, world){};
    virtual ~Square(){};
    virtual void doSomething(Square &p);
    virtual void doAction(Player &p) = 0;
    void changeFinances(Square &a);
private:
};

class CoinSquare: public Square {// or some subclass of Actor
public:
    CoinSquare(int imageID, int startX, int startY, StudentWorld* world, int numCoins): Square(imageID, startX, startY, world){
        m_numCoinsModified = numCoins;
    };
    virtual ~CoinSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
    int m_numCoinsModified;
};

class StarSquare: public Square {// or some subclass of Actor
public:
    StarSquare(int imageID, int startX, int startY, StudentWorld* world, int numCoins, int numStars): Square(imageID, startX, startY, world){
        m_numCoinsToDeduct = numCoins;
        m_numStarsToGive = numStars;
    };
    virtual ~StarSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
    int m_numCoinsToDeduct;
    int m_numStarsToGive;
};

class DirectionalSquare: public Square {// or some subclass of Actor
public:
    DirectionalSquare(int imageID, int startX, int startY, int dir, StudentWorld* world);
    virtual ~DirectionalSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
    int m_forceDir;
};

class BankSquare: public Square {// or some subclass of Actor
public:
    BankSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    virtual ~BankSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
//    int num_
};

class EventSquare: public Square {// or some subclass of Actor
public:
    EventSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    virtual ~EventSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
};

class DropSquare: public Square {// or some subclass of Actor
public:
    DropSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
    virtual ~DropSquare(){};
    virtual void doSomething() { return;}
    virtual void doAction(Player &p) {return;};
private:
};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
