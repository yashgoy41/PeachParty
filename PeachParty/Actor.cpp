#include "Actor.h"
#include "GameConstants.h"

// This is complete overlap but may need to modify for partial overlap
/* =========== ACTOR =========== */
Actor::Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : GraphObject(imageID, startX, startY, dir, depth, size){
    m_world = world;
    m_active = true;
};
bool Actor::overlaps(const Actor &other) const{
    if(other.getX() == getX() && other.getY() == getY()){
        return true;
    }
    return false;
}
bool Actor::partiallyOverlaps(const Actor &other) const {
    int thisLeft = getX();
    int thisRight = thisLeft + SPRITE_WIDTH - 1;
    int thisBottom = getY();
    int thisTop = thisBottom + SPRITE_HEIGHT - 1;

    // Get the bounding box of the other actor
    int otherLeft = other.getX();
    int otherRight = otherLeft + SPRITE_WIDTH - 1;
    int otherBottom = other.getY();
    int otherTop = otherBottom + SPRITE_HEIGHT - 1;

    // Check if the bounding boxes overlap
    return (thisLeft < otherRight && thisRight > otherLeft &&
                thisBottom < otherTop && thisTop > otherBottom);
}

/* =========== MOVING ACTOR =========== */
MovingActor::MovingActor(int imageID, int startX, int startY, StudentWorld* world)
: Actor(imageID, startX, startY, right, 0, 1.0, world){
    m_walkDir = right;
    m_isWalking = false;
    m_justLanded = true;
    m_ticks_to_move = 0;
}

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

bool MovingActor::atAFork() const{
    int dirs = 0;
    Board::GridEntry ge = getWorld()->getBoard().getContentsOf(getX() / BOARD_WIDTH, getY() / BOARD_HEIGHT);
    // If you are on top of a directional square, it is not a fork.
    if (ge == Board::up_dir_square || ge == Board::down_dir_square || ge == Board::left_dir_square || ge == Board::right_dir_square){
        return false;
    }
    if(getWorld()->getBoard().getContentsOf(getX()/16+1, getY()/16) != Board::empty)
        dirs++;
    if(getWorld()->getBoard().getContentsOf(getX()/16-1, getY()/16) != Board::empty)
        dirs++;
    if(getWorld()->getBoard().getContentsOf(getX()/16, getY()/16+1) != Board::empty)
        dirs++;
    if(getWorld()->getBoard().getContentsOf(getX()/16, getY()/16-1) != Board::empty)
        dirs++;
    if(dirs >= 3){
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

void MovingActor::pickRandDir() {
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

void MovingActor::atTurningPoint(MovingActor &object) {
    if (object.isAligned() && object.canMoveForward(object.getWalkDir()) == false){
        //change direction
        if (object.getWalkDir() == right || object.getWalkDir() == left) {
            // check if up is available
            if (object.canMoveForward(up)) {
                object.setWalkDir(up);
                object.setDirection(right);
            }
            else if (object.canMoveForward(down)) {
                object.setWalkDir(down);
                object.setDirection(right);
            }
        }
        else if (object.getWalkDir() == up || object.getWalkDir() == down) {
            // check if right is available
            if (object.canMoveForward(right)) {
                object.setWalkDir(right);
                object.setDirection(right);
            }
            else if (object.canMoveForward(left)) {
                object.setWalkDir(left);
                object.setDirection(left);
            }
        }
    }
}

/* =========== PLAYER =========== */
Player::Player(int imageID, int playerNumber, int startX, int startY, StudentWorld* world)
: MovingActor(imageID, startX, startY, world){
    m_playerNumber = playerNumber;
    m_stars = 0;
    m_coins = 0;
    m_vortex = true;
    m_hurtByBaddie = false;
    world->addPlayers();
    setWalkDir(right);
    setDirection(right);
}
void Player::updateCoins(int value){
    if((m_coins + value) < 0){
        m_coins = 0;
    }
    else{
        m_coins += value;
    }
}
Player* Player::getOtherPlayer() const{
    if (m_playerNumber == 1){
        return getWorld()->getPlayer(2);
    }
    else {
        return getWorld()->getPlayer(1);
    }
}

void Player::doSomething(){
    if(getIsWalking() == false){
        // Player has teleported
        if (getWalkDir() == -1){
            pickRandDir();
        }
        // Use pressed roll key
        int action = getWorld()->getAction(m_playerNumber);
        if(action == ACTION_ROLL){
            int die_roll = randInt(1,10);
            setTicks(die_roll * 8);
            setIsWalking(true);
            setJustLanded(true);
            setHurtByBaddie(false);
        }
        // Pressed fire
        if(action == ACTION_FIRE && m_vortex == true){
            createVortex(getWalkDir());
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            m_vortex = false;
        }
        // User did not press any key
        else{
            return;
        }
    }
    if(getIsWalking() == true){
        // Fork
        if(atAFork() && isAligned()){
            int action  = getWorld()->getAction(m_playerNumber);
            bool validOption = setForkDirection(action);
            if (validOption == false){
                return;
            }
        }
        // Turning Point
        atTurningPoint(*this);
        moveAtAngle(getWalkDir(), 2);
        decrementTicks();
        if(numTicks() == 0){
            setIsWalking(false);
        }
    }
};
bool Player::setForkDirection(int action){
    switch (action) {
        case ACTION_NONE:
            return false;
            break;
        case ACTION_LEFT:
            if(canMoveForward(left)){
                setWalkDir(left);
                setDirection(left);
                return true;
            }
            return false;
            break;
        case ACTION_RIGHT:
            if(canMoveForward(right)){
                setWalkDir(right);
                setDirection(right);
                return true;
            }
            return false;
            break;
        case ACTION_UP:
            if(canMoveForward(up)){
                setWalkDir(up);
                setDirection(right);
                return true;
            }
            return false;
            break;
        case ACTION_DOWN:
            if(canMoveForward(down)){
                setWalkDir(down);
                setDirection(right);
                return true;
            }
            return false;
            break;
        default:
            return false;
            break;
    }
}
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
void Player::createVortex(int dir){
    int x = getX();
    int y = getY();
    switch (dir) {
        case right:
            x += 16;
            break;
        case left:
            x -= 16;
            break;
        case up:
            y += 16;
            break;
        case down:
            y -= 16;
            break;
        default:
            break;
    }
    getWorld()->createNewVortex(x, y, dir);
    
}
/* =========== BADDIE =========== */
Baddie::Baddie(int imageID, int startX, int startY, StudentWorld* world): MovingActor(imageID, startX, startY, world){
    m_travelDist = 0;
    m_pauseCtr = 180;
    m_gotHitByVortex = false;
}
void Baddie::doSomething(Baddie &b){
    if(m_gotHitByVortex == true){
        teleport(*this);
        m_gotHitByVortex = false;
    }
    if(b.getIsWalking() == false){
        for(int i = 1; i < getWorld()->getNumPlayers()+1; i++){
            Player* player = getWorld()->getPlayer(i);
            if (b.overlaps(*player) && player->getIsWalking() == false){
                b.hurtPlayer(*player);
            }
        }
        b.decrementPauseCtr();
        if(b.getPauseCtr() == 0){
            int sq_to_move = randInt(1,10);
            b.setTicks(sq_to_move * 8);
            b.pickRandDir();
            b.setIsWalking(true);
        }
    }
    if(b.getIsWalking() == true){
        // Fork Point
        if (b.isAligned() && b.atAFork()){
            b.pickRandDir();
        }
        // Turning Point
        b.atTurningPoint(*this);
        b.moveAtAngle(getWalkDir(), 2);
        b.decrementTicks();
        if(b.numTicks() == 0){
            b.setIsWalking(false);
            b.resetPauseCtr();
            b.specialMove();
        }
    }
}

/* =========== BOWSER =========== */
Bowser::Bowser(int imageID, int startX, int startY, StudentWorld* world): Baddie(imageID, startX, startY, world){};
void Bowser::hurtPlayer(Player &player){
    if(player.wasHurtByBaddie() == false){
        int event = randInt(0, 1);
        if(event == 0){
            player.resetCoins();
            player.resetStars();
            getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
        }
        player.setHurtByBaddie(true);
    }
}

void Bowser::specialMove(){
    int chance = randInt(1, 4);
    if(chance == 1){
        getWorld()->addDroppingSquare(getX(), getY());
        getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
    }
}
void Bowser::doSomething(){
    Baddie::doSomething(*this);
}


/* =========== BOO =========== */
Boo::Boo(int imageID, int startX, int startY, StudentWorld* world): Baddie(imageID, startX, startY, world){};
void Boo::hurtPlayer(Player &player){
    if(player.wasHurtByBaddie() == false){
        int event = randInt(0, 1);
        if(event == 0){
            Player* other = player.getOtherPlayer();
            int tmp = player.getCoins();
            player.setCoins(other->getCoins());
            other->setCoins(tmp);
        }
        if(event == 1){
            Player* other = player.getOtherPlayer();
            int tmp = player.getStars();
            player.setStars(other->getStars());
            other->setStars(tmp);
        }
        getWorld()->playSound(SOUND_BOO_ACTIVATE);
        player.setHurtByBaddie(true);
    }
}

void Boo::doSomething(){
    Baddie::doSomething(*this);
}

/* =========== VORTEX =========== */
Vortex::Vortex(int imageID, int startX, int startY, StudentWorld* world, int dir): MovingActor(imageID, startX, startY, world){
    setWalkDir(dir);
};

void Vortex::doSomething(){
    if(!isActive()){
        return;
    }
    moveAtAngle(getWalkDir(), 2);
    if(getX() > VIEW_WIDTH || getX() < 0 || getY() > VIEW_HEIGHT || getY() < 0){
        setDead();
    }
    if(getWorld()->hitWithVortex(getX(), getY()) == true){
        setDead();
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
}
/* =========== SQUARE =========== */
Square::Square(int imageID, int startX, int startY, StudentWorld* world) : Actor(imageID, startX, startY, right, 1, 1.0, world){
};
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
CoinSquare::CoinSquare(int imageID, int startX, int startY, StudentWorld* world, int numCoins): Square(imageID, startX, startY, world){
    m_numCoinsModified = numCoins;
};
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
StarSquare::StarSquare(int imageID, int startX, int startY, StudentWorld* world, int numCoins, int numStars): Square(imageID, startX, startY, world){
    m_numCoinsToDeduct = numCoins;
    m_numStarsToGive = numStars;
};
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
}
void DirectionalSquare::doAction(Player &p) {
    p.setWalkDir(m_forceDir);
    if(m_forceDir == left){
        p.setDirection(left);
    }
    else{
        p.setDirection(right);
    }
}

void DirectionalSquare::doSomething(){
    Square::doSomething(*this);
}

/* =========== BANK SQUARE =========== */
BankSquare::BankSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};

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
EventSquare::EventSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};

void EventSquare::doAction(Player &p){
    int option = randInt(1,3);
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

/* =========== DROPPING SQUARE =========== */
DroppingSquare::DroppingSquare(int imageID, int startX, int startY, StudentWorld* world): Square(imageID, startX, startY, world){};
void DroppingSquare::doAction(Player &p){
    if(p.getIsWalking() == false){
        if(p.hasJustLanded()){
            int event  = randInt(0, 1);
            if(event == 0){
                p.updateCoins(-10);
            }
            if(event == 1){
                if(p.getStars() > 0){
                    p.updateStars(-1);
                }
            }
            getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
        }
        p.setJustLanded(false);
    }
}

void DroppingSquare::doSomething(){
    Square::doSomething(*this);
}


