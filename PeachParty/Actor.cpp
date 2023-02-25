#include "Actor.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
bool Actor::overlaps(Actor &a){
    if(a.getX()/16 == getX()/16 && a.getY()/16 == getY()/16){
        return true;
    }
    return false;
}

bool MovingActor::canMoveForward(int dir){
    Board::GridEntry ge = getWorld()->getBoard().getContentsOf(GraphObject::getX()/16, GraphObject::getY()/16);;
    switch (dir) {
        case right:
            ge = getWorld()->getBoard().getContentsOf(GraphObject::getX()/16+1, GraphObject::getY()/16);
            break;
        case left:
            ge = getWorld()->getBoard().getContentsOf(GraphObject::getX()/16-1, GraphObject::getY()/16);
            break;
        case up:
            ge = getWorld()->getBoard().getContentsOf(GraphObject::getX()/16, GraphObject::getY()/16+1);
            break;
        case down:
            ge = getWorld()->getBoard().getContentsOf(GraphObject::getX()/16, GraphObject::getY()/16-1);
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

void Player::doSomething(){
    if(!isWalking()){
//        if (isAligned() && !canMoveForward(getDirection())){
//            int newDir = -1;
//            while (newDir == -1) {
//                int randDir = randInt(1,4);
//                switch (randDir) {
//                    case 1:
//                        randDir = right;
//                        break;
//                    case 2:
//                        randDir = up;
//                        break;
//                    case 3:
//                        randDir = left;
//                        break;
//                    case 4:
//                        randDir = down;
//                        break;
//                }
//                if (canMoveForward(randDir)) {
//                    newDir = randDir;
//                    if(newDir == left){
//                        setDirection(left);
//                    }
//                    else{
//                        setDirection(right);
//                    }
//                }
//            }
//        }
        // Use pressed roll key
        if(getWorld()->getAction(m_playerNumber) == ACTION_ROLL){
            int die_roll = randInt(1, 10);
            setTicks(die_roll * 8);
            m_isWalking = true;
        }
        // User did not press any key
        else
            return;
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
        std::cerr << numTicks() << std::endl;
        if(numTicks() == 0){
            m_isWalking = false;            
        }
    }
    
};

void CoinSquare::doSomething(){
    if(!isActive()){
        return;
    }
    for(int i = 1; i < getWorld()->getNumPlayers()+1; i++){
        Player* player = getWorld()->getPlayer(i);
        if(overlaps(*player) && player->isWalking() == true){
            setIsPlayerNew(i, true);
            return;
        }
        if (overlaps(*player) && player->isWalking() == false){
            if(isPlayerNew(i)){
                if(numCoinsModified() > 0){
                    player->updateCoins(numCoinsModified());
                    getWorld()->playSound(SOUND_GIVE_COIN);
                    setIsPlayerNew(i, false);
                }
                else{
                    player->updateCoins(numCoinsModified());
                    getWorld()->playSound(SOUND_TAKE_COIN);
                    setIsPlayerNew(i, false);
                }
                
            }
        }
    }
}


