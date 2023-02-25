#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    startCountdownTimer(30
                        );  // this placeholder causes timeout after 5 seconds
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found) {
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format) {
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
        for(int x = 0; x < BOARD_WIDTH; x++){
            for(int y = 0; y < BOARD_HEIGHT; y++){
                Board::GridEntry ge = bd.getContentsOf(x, y);
                switch (ge) {
                    case Board::empty:
                    break;
                    case Board::boo:
                        actorList.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this, 3));
                        actorList.push_back(new Boo(IID_BOO,x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this));
                        break;
                    case Board::bowser:
                        actorList.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this, 3));
                        actorList.push_back(new Bowser(IID_BOWSER, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this));
                        break;
                    case Board::player:
                        actorList.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this, 3));
                        m_peach = new Player(IID_PEACH, 1, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this);
                        m_yoshi = new Player(IID_YOSHI, 2, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this);
                        break;
                    case Board::red_coin_square:
                        actorList.push_back(new CoinSquare(IID_RED_COIN_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this, -3));
                        break;
                    case Board::blue_coin_square:
                        actorList.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this, 3));
                        break;
                    case Board::left_dir_square:
                        actorList.push_back(new DirectionSquare(IID_DIR_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, 180, this));
                        break;
                    case Board::right_dir_square:
                        actorList.push_back(new DirectionSquare(IID_DIR_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, 0,this));
                        break;
                    case Board::up_dir_square:
                        actorList.push_back(new DirectionSquare(IID_DIR_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, 90, this));
                        break;
                    case Board::down_dir_square:
                        actorList.push_back(new DirectionSquare(IID_DIR_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, 270, this));
                        break;
                    case Board::event_square:
                        actorList.push_back(new EventSquare(IID_EVENT_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this));
                        break;
                    case Board::bank_square:
                        actorList.push_back(new BankSquare(IID_BANK_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this));
                        break;
                    case Board::star_square:
                        actorList.push_back(new StarSquare(IID_STAR_SQUARE, x*SPRITE_WIDTH,y*SPRITE_HEIGHT, this));
                        break;
                }
            }
        }
        
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_peach->doSomething();
    m_yoshi->doSomething();
    for (std::list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++) {
        if((*it)->isActive()){
            (*it)->doSomething();
        }
    }
    
    
    if(timeRemaining() <= 0){
        int winner = 0;
        if(m_peach->getStars() > m_yoshi->getStars()){
            winner = 1;
        }
        else if(m_peach->getStars() == m_yoshi->getStars() && m_peach->getCoins() > m_yoshi-> getCoins()){
            winner = 1;
        }
        else if(m_peach->getStars() == m_yoshi->getStars() && m_peach->getCoins() == m_yoshi-> getCoins()){
            winner = randInt(1, 2);
        }
        else{
            winner = 2;
        }
        playSound(SOUND_GAME_FINISHED);
        if(winner == 1){
            setFinalScore(m_peach->getStars(), m_peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else {
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        
    }
    for (std::list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); it++) {
        (*it)->doSomething();
    }
    // Delete Dead Actors
    // Update status text
    
    setGameStatText("Game will end in a few seconds");
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    for (std::list<Actor*>::iterator it = actorList.begin(); it != actorList.end(); ++it) {
        delete *it;
    }
    delete m_peach;
    delete m_yoshi;
}
