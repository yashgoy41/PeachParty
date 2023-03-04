#include "Actor.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
bool Actor::overlaps(const Actor &other) const{
    if(other.getX() == getX() && other.getY() == getY()){
        return true;
    }
    return false;
//    int thisLeft = getX();1
//        int thisRight = thisLeft + SPRITE_WIDTH - 1;
//        int thisBottom = getY();
//        int thisTop = thisBottom + SPRITE_HEIGHT - 1;
//
//        // Get the bounding box of the other actor
//        int otherLeft = other.getX();
//        int otherRight = otherLeft + SPRITE_WIDTH - 1;
//        int otherBottom = other.getY();
//        int otherTop = otherBottom + SPRITE_HEIGHT - 1;
//
//        // Check if the bounding boxes overlap
//        return (thisLeft <= otherRight && thisRight >= otherLeft &&
//                thisBottom <= otherTop && thisTop >= otherBottom);
}

bool MovingActor::canMoveForward(int dir) const{
    Board::GridEntry ge = getWorld()->getBoard().getContentsOf(getX()/16, getY()/16);;
    switch (dir) {
        case right:
            ge = getWorld()->getBoard().getContentsOf(getX()/16+1, getY()/16);
            break;
        case left:
            ge = getWorld()->getBoard().getContentsOf(getX()/16-1, getY()/16);
            break;
        case up:
            ge = getWorld()->getBoard().getContentsOf(getX()/16, getY()/16+1);
            break;
        case down:
            ge = getWorld()->getBoard().getContentsOf(getX()/16, getY()/16-1);
            break;
    }
    if(ge == Board::empty){
        return false;
    }
    return true;
}

bool MovingActor::isAligned() const{
    if(getX()% 16 == 0 && getY()%16 == 0){
        return true;
    }
    return false;
}

Player::Player(int imageID, int playerNumber, int startX, int startY, StudentWorld* world)
: MovingActor(imageID, startX, startY, world){
    m_playerNumber = playerNumber;
    m_stars = 0;
    m_coins = 0;
    m_vortex = false;
    m_isWalking = false;
    world->addPlayers();
    setWalkDir(right);
    setSpriteDirection(right);
}
void Player::doSomething(){
    if(!isWalking()){
        if (!canMoveForward(getDirection()) && hasTeleported() == true){
            int newDir = -1;
            while (newDir == -1) {
                int randDir = randInt(1,4);
                switch (randDir) {
                    case 1:
                        randDir = right;
                        break;
                    case 2:
                        randDir = up;
                        break;
                    case 3:
                        randDir = left;
                        break;
                    case 4:
                        randDir = down;
                        break;
                }
                if (canMoveForward(randDir)) {
                    newDir = randDir;
                }
            }
            if(newDir == left){
                setDirection(left);
            }
            else{
                setDirection(right);
            }
        }

        // Use pressed roll key
        if(getWorld()->getAction(m_playerNumber) == ACTION_ROLL){
            int die_roll = randInt(1, 2);
            setTicks(die_roll * 8);
            m_isWalking = true;
            setJustLanded(true);
        }
        // User did not press any key
        else{
            return;
            
        }
    }
    if(isWalking()){
        if (isAligned() && canMoveForward(getWalkDir()) == false){
            //change direction
            if (getWalkDir() == right || getWalkDir() == left) {
                   // check if up is available
                   if (canMoveForward(up)) {
                       setWalkDir(up);
                       setDirection(right);
                   }
                   else if (canMoveForward(down)) {
                       setWalkDir(down);
                       setDirection(right);
                   }
            }
            else if (getWalkDir() == up || getWalkDir() == down) {
                   // check if right is available
                   if (canMoveForward(right)) {
                       setWalkDir(right);
                       setDirection(right);
                   }
                   else if (canMoveForward(left)) {
                       setWalkDir(left);
                       setDirection(left);
                   }
            }
        }
        moveAtAngle(getWalkDir(), 2);
        decrementTicks();
        if(numTicks() == 0){
            m_isWalking = false;            
        }
    }
};

void Square::doSomething(Square &a){
    if(!a.isActive()){
        return;
    }
    for(int i = 1; i < a.getWorld()->getNumPlayers()+1; i++){
        Player* player = a.getWorld()->getPlayer(i);
        if (a.overlaps(*player)){
                a.doAction(*player);
        }
    }
}

void CoinSquare::doAction(Player &p){
    if(p.isWalking() == false){
        if(p.hasJustLanded()){
            p.updateCoins(m_numCoinsModified);
            if(m_numCoinsModified > 0){
                getWorld()->playSound(SOUND_GIVE_COIN);
            }
            else{
                getWorld()->playSound(SOUND_TAKE_COIN);
            }
            p.setJustLanded(false);
        }
    }
}
void CoinSquare::doSomething(){
    Square::doSomething(*this);
}

void StarSquare::doAction(Player &p){
    // Landed on
    if(p.isWalking() == false){
        if(p.hasJustLanded()){
            if(p.getCoins() >= 20){
                p.updateCoins(m_numCoinsToDeduct);
                p.updateStars(m_numStarsToGive);
                getWorld()->playSound(SOUND_GIVE_STAR);
            }
            p.setJustLanded(false);
        }
    }
    // Passsing By
    else {
        if(p.getCoins() >= 20){
            p.updateCoins(m_numCoinsToDeduct);
            p.updateStars(m_numStarsToGive);
            getWorld()->playSound(SOUND_GIVE_STAR);
        }
    }
}
void StarSquare::doSomething(){
    Square::doSomething(*this);
}

DirectionalSquare::DirectionalSquare(int imageID, int startX, int startY, int dir, StudentWorld* world): Square(imageID, startX, startY, world){
    m_forceDir = dir;
    switch (m_forceDir) {
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

void DirectionalSquare::doAction(Player &p) {
    p.setWalkDir(m_forceDir);
    if(m_forceDir == left){
        p.setDirection(left);
    }
    else{
        p.setDirection(right);
    }
};

void DirectionalSquare::doSomething(){
    Square::doSomething(*this);
}

void BankSquare::doAction(Player &p){
    // Landed on
    if(p.isWalking() == false){
        if(p.hasJustLanded()){
            p.updateCoins(getWorld()->getBankCoins());
            getWorld()->resetBankCoins();
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
        }
        p.setJustLanded(false);
    }
    // Passsing By
    else {
        // If player has less than 5 coins
        if(p.getCoins() < 5){
            int tmp = p.getCoins();
            p.updateCoins(-5);
            getWorld()->updateBankCoins(tmp);
        }
        else{
            p.updateCoins(-5);
            getWorld()->updateBankCoins(5);
        }
        getWorld()->playSound(SOUND_TAKE_COIN);
    }
}

void BankSquare::doSomething(){
        Square::doSomething(*this);
}

void EventSquare::doAction(Player &p){
    int option = randInt(1,3);
    if(p.isWalking() == false){
        if(p.hasJustLanded()){
            switch(option){
                case 1:
                    // Teleport
                    p.timeToTeleport();
                    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                    break;
                case 2:
                    // Swap Position w P2
                    break;
                case 3:
                    // Give Vortex
                    p.giveVortex();
                    getWorld()->playSound(SOUND_GIVE_VORTEX);
                    break;
            }
        }
        p.setJustLanded(false);
    }
}
