#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
/* =========== ACTOR =========== */
class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual ~Actor(){};
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const{return m_world;}
    void setDead(){m_active = false;}
    bool isActive() const{return m_active;}
    bool overlaps(const Actor &a) const;
    bool partiallyOverlaps(const Actor &a) const;
    virtual bool isASquare() const = 0;
    virtual bool canBeHitByVortex() const = 0;
    virtual void gotHitByVortex(){return;};
private:
    StudentWorld* m_world;
    bool m_active;

};

/* =========== MOVING ACTOR =========== */
class MovingActor: public Actor{
public:
    MovingActor(int imageID, int startX, int startY, StudentWorld* world);
    virtual ~MovingActor(){};
    virtual bool isASquare() const {return false;};
    virtual bool canBeHitByVortex() const {return false;}
    virtual void doSomething() = 0;
    int getWalkDir() const{return m_walkDir;}
    void setWalkDir(int dir){m_walkDir = dir;}
    void setTicks(int num){m_ticks_to_move = num;}
    void decrementTicks(){m_ticks_to_move -= 1;}
    int numTicks() const{return m_ticks_to_move;}
    bool hasJustLanded() const{return m_justLanded;}
    void setJustLanded(bool val){m_justLanded = val;}
    bool getIsWalking() const{return m_isWalking;}
    void setIsWalking(bool val) {m_isWalking = val;}
    void teleport(MovingActor &m);
    bool canMoveForward(int dir) const;
    bool isAligned() const;
    bool atAFork() const;
    void atTurningPoint(MovingActor &object);
    void pickRandDir();
private:
    int m_walkDir;
    bool m_isWalking;
    int m_ticks_to_move;
    bool m_justLanded;
};

/* =========== PLAYER =========== */
class Player : public MovingActor {
public:
    Player(int imageID, int playerNumber, int startX, int startY, StudentWorld* world);
    virtual ~Player(){}
    virtual bool isASquare() const {return false;};
    virtual void doSomething();
    virtual bool canBeHitByVortex() const {return false;}
    void createVortex(int dir);
    void resetCoins(){m_coins = 0;}
    void updateCoins(int value);
    void setCoins(int num){m_coins  = num;}
    int getCoins() const{return m_coins;}
    void resetStars(){m_stars = 0;}
    void updateStars(int value){m_stars += value;}
    int getStars() const{return m_stars;}
    void setStars(int num){m_stars  = num;}
    bool hasVortex() const{return m_vortex;}
    void setVortex(bool val){m_vortex = val;}
    void swapWithOtherPlayer();
    bool wasHurtByBaddie() const{return m_hurtByBaddie;}
    void setHurtByBaddie(bool val){m_hurtByBaddie = val;}
    Player* getOtherPlayer() const;
    bool setForkDirection(int action);
private:
    int m_playerNumber;
    int m_stars;
    int m_coins;
    bool m_vortex;
    bool m_hurtByBaddie;
};

/* =========== BADDIE =========== */
class Baddie: public MovingActor{
public:
    Baddie(int imageID, int startX, int startY, StudentWorld* world);
    virtual ~Baddie(){}
    virtual bool isASquare() const {return false;};
    virtual bool canBeHitByVortex() const {return true;}
    virtual void gotHitByVortex(){m_gotHitByVortex = true;};
    virtual void doSomething(Baddie &b);
    virtual void hurtPlayer(Player &a) = 0;
    virtual void specialMove() = 0;
    int getPauseCtr(){return m_pauseCtr;}
    void resetPauseCtr(){m_pauseCtr = 180;}
    void decrementPauseCtr(){m_pauseCtr--;}
private:
    int m_travelDist;
    int m_pauseCtr;
    int m_gotHitByVortex;
};

/* =========== BOWSER =========== */
class Bowser: public Baddie { // or some subclass of Actor
public:
    Bowser(int imageID, int startX, int startY, StudentWorld* world);
    virtual ~Bowser(){};
    virtual bool isASquare() const {return false;};
    virtual void doSomething();
    virtual void specialMove();
    virtual void hurtPlayer(Player &a);
    
private:
};

/* =========== BOO =========== */
class Boo: public Baddie { // or some subclass of Actor
public:
    Boo(int imageID, int startX, int startY, StudentWorld* world);
    virtual ~Boo(){};
    virtual bool isASquare() const {return false;};
    virtual void doSomething();
    virtual void hurtPlayer(Player &a);
    virtual void specialMove(){return;};
private:
};

/* =========== VORTEX =========== */
class Vortex: public MovingActor { // or some subclass of Actor
public:
    Vortex(int imageID, int startX, int startY, StudentWorld* world, int dir);
    virtual ~Vortex(){};
    virtual bool isASquare() const {return false;};
    virtual void doSomething();
private:
};

/* =========== SQUARE =========== */
class Square: public Actor{
public:
    Square(int imageID, int startX, int startY, StudentWorld* world);
    virtual ~Square(){};
    virtual void doSomething(Square &p);
    virtual void doAction(Player &p) = 0;
    virtual bool isASquare() const {return true;};
    virtual bool canBeHitByVortex() const {return false;};
private:
};
 
/* =========== COIN SQUARE =========== */
class CoinSquare: public Square {// or some subclass of Actor
public:
    CoinSquare(int imageID, int startX, int startY, StudentWorld* world, int numCoins);
    virtual ~CoinSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
    int m_numCoinsModified;
};

/* =========== STAR SQUARE =========== */
class StarSquare: public Square {// or some subclass of Actor
public:
    StarSquare(int imageID, int startX, int startY, StudentWorld* world, int numCoins, int numStars);
    virtual ~StarSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
    int m_numCoinsToDeduct;
    int m_numStarsToGive;
};

/* =========== DIRECTIONAL SQUARE =========== */
class DirectionalSquare: public Square {// or some subclass of Actor
public:
    DirectionalSquare(int imageID, int startX, int startY, int dir, StudentWorld* world);
    virtual ~DirectionalSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
    int m_forceDir;
};

/* =========== BANK SQUARE =========== */
class BankSquare: public Square {// or some subclass of Actor
public:
    BankSquare(int imageID, int startX, int startY, StudentWorld* world);
    virtual ~BankSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
//    int num_
};

/* =========== EVENT SQUARE =========== */
class EventSquare: public Square {// or some subclass of Actor
public:
    EventSquare(int imageID, int startX, int startY, StudentWorld* world);
    virtual ~EventSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
};

/* =========== DROPPING SQUARE =========== */
class DroppingSquare: public Square {// or some subclass of Actor
public:
    DroppingSquare(int imageID, int startX, int startY, StudentWorld* world);
    virtual ~DroppingSquare(){};
    virtual void doSomething();
    virtual void doAction(Player &p);
private:
};

#endif // ACTOR_H_
