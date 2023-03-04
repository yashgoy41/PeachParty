#include "Actor.h"
#include "GameConstants.h"

// This is complete overlap but may need to modify for partial overlap
/* =========== ACTOR =========== */
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

/* =========== MOVING ACTOR =========== */
bool MovingActor::canMoveForward(int dir) const{
    Board::GridEntry ge = Board::empty;
    switch (dir) {
        case right:
            if(getX() == BOARD_WIDTH - 1){
                ge = Board::empty;
            }
            ge = getWorld()->getBoard().getContentsOf(getX()/16+1, getY()/16);
            break;
        case left:
            if(getX() == 0){
                ge = Board::empty;
            }
            ge = getWorld()->getBoard().getContentsOf(getX()/16-1, getY()/16);
            break;
        case up:
            if(getY() == BOARD_HEIGHT - 1){
                ge = Board::empty;
            }
            ge = getWorld()->getBoard().getContentsOf(getX()/16, getY()/16+1);
            break;
        case down:
            if(getY() == 0){
                ge = Board::empty;
            }
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

void MovingActor::teleport(MovingActor &m){
    int xcoord = randInt(0, 15);
    int ycoord = randInt(0, 15);
    while(getWorld()->getBoard().getContentsOf(xcoord, ycoord) == Board::empty){
        xcoord = randInt(0, 15);
        ycoord = randInt(0, 15);
    }
    m.moveTo(xcoord*16, ycoord*16);
    m.setWalkDir(-1);
}

/* =========== PLAYER =========== */
Player::Player(int imageID, int playerNumber, int startX, int startY, StudentWorld* world)
: MovingActor(imageID, startX, startY, world){
    m_playerNumber = playerNumber;
    m_stars = 0;
    m_coins = 0;
    m_vortex = false;
    m_hurtByBaddie = false;
    world->addPlayers();
    setWalkDir(right);
    setSpriteDirection(right);
}
void Player::doSomething(){
    if(getIsWalking() == false){
        // Player has teleported
        if (getWalkDir() == -1){
            int newDir = -1;
            while (newDir == -1) {
                int randDir = 90 * randInt(0,3);
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
            setWalkDir(newDir);
        }
        // Use pressed roll key
        if(getWorld()->getAction(m_playerNumber) == ACTION_ROLL){
            int die_roll = randInt(1,1);
            setTicks(die_roll * 8);
            setIsWalking(true);
            setJustLanded(true);
            setHurtByBaddie(false);
        }
        // User did not press any key
        else{
            return;
        }
    }
    if(getIsWalking() == true){
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
            setIsWalking(false);
        }
    }
};

void Player::swapWithOtherPlayer(){
    Player* other;
    if(m_playerNumber == 1){
        other = getWorld()->getPlayer(2);
    }
    else {
        other = getWorld()->getPlayer(1);
    }
    // Temp varibles
    int t_x,t_y,t_numTicks,t_walkDir,t_spriteDir, t_isWalking;
    t_x = getX(); t_y = getY(); t_numTicks = numTicks();
    t_walkDir = getWalkDir(); t_spriteDir = getDirection();
    bool t_justLanded = hasJustLanded(); t_isWalking = getIsWalking();
    // Copy from other player into current player
    setTicks(other->numTicks());
    setWalkDir(other->getWalkDir());
    setDirection(other->getDirection());
    setIsWalking(other->getIsWalking());
    setJustLanded(other->hasJustLanded());
    moveTo(other->getX(), other->getY());
    // Copy into other player from temp
    other->setTicks(t_numTicks);
    other->setWalkDir(t_walkDir);
    other->setDirection(t_spriteDir);
    other->setIsWalking(t_isWalking);
    setJustLanded(t_justLanded);
    other->moveTo(t_x, t_y);
}

/* =========== BOWSER =========== */

void Bowser::doSomething(){
    if(getIsWalking() == false){
        for(int i = 1; i < getWorld()->getNumPlayers()+1; i++){
            Player* player = getWorld()->getPlayer(i);
            if (overlaps(*player) && player->getIsWalking() == false){
                if(player->wasHurtByBaddie() == false){
                    int event = randInt(0, 1);
                    if(event == 0){
                        std::cerr<< "player landed on BOWSER and got HURT" << std::endl;
                        player->resetCoins();
                        player->resetStars();
                        getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
                    }
                    player->setHurtByBaddie(true);
                }
            }
            decrementPauseCtr();
            if(getPauseCtr() == 0){
                int sq_to_move = randInt(1,10);
                setTicks(sq_to_move * 8);
                int newDir = -1;
                while (newDir == -1) {
                    int randDir = 90 * randInt(0,3);
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
                setWalkDir(newDir);
                setIsWalking(true);
            }
        }
    }
    if(getIsWalking() == true){
        // TODO: IMPLEMENT ALGORITHM FOR FORK
        
        // Turning Point
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
            setIsWalking(false);
            resetPauseCtr();
            // Insert dropping square;
        }
    }
}
/* =========== SQUARE =========== */
void Square::doSomething(Square &a){
    if(!a.isActive()){
        return;
    }
    for(int i = 1; i < getWorld()->getNumPlayers()+1; i++){
        Player* player = getWorld()->getPlayer(i);
        if (a.overlaps(*player)){
                a.doAction(*player);
        }
    }
}

/* =========== COIN SQUARE =========== */
void CoinSquare::doAction(Player &p){
    if(p.getIsWalking() == false){
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

/* =========== STAR SQUARE =========== */
void StarSquare::doAction(Player &p){
    // Landed on
    if(p.getIsWalking() == false){
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

/* =========== DIRECTIONAL SQUARE =========== */
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

/* =========== BANK SQUARE =========== */
void BankSquare::doAction(Player &p){
    // Landed on
    if(p.getIsWalking() == false){
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

/* =========== EVENT SQUARE =========== */
void EventSquare::doAction(Player &p){
    int option = randInt(3,3);
    if(p.getIsWalking() == false){
        if(p.hasJustLanded()){
            switch(option){
                case 1:
                    // Teleport
                    p.teleport(p);
                    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                    break;
                case 2:
                    p.swapWithOtherPlayer();
                    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                    break;
                case 3:
                    // Give Vortex
                    p.setVortex(true);
                    getWorld()->playSound(SOUND_GIVE_VORTEX);
                    break;
            }
        }
        p.setJustLanded(false);
    }
}

void EventSquare::doSomething(){
    Square::doSomething(*this);
}
