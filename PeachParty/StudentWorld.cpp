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
    /*
     Hint: You will want to use our Board class to load the board specification in your StudentWorld class’s init() method. The assetPath() and getBoardNumbers() methods your StudentWorld class inherits from GameWorld might also be useful too!
     */
    Board bd;
    string board_file = assetPath() + "board01.txt"; Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found) {
        cerr << "Could not find board01.txt data file\n";
    }
    else if (result == Board::load_fail_bad_format) {
        cerr << "Your board was improperly formatted\n";
    }
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
        for(int i = 0; i < BOARD_WIDTH; i++){
            for(int j = 0; j < BOARD_HEIGHT; j++){
                Board::GridEntry ge = bd.getContentsOf(i, j);
                switch (ge) {
                    case Board::empty:
                    cout << "Location 5,10 is empty\n"; break;
                    case Board::boo:
                        actorList.push_back(new Boo(IID_BOO,i*16,j*16, this));
                    case Board::bowser:
                        actorList.push_back(new Bowser(IID_BOWSER, i*16,j*16, this));
                        break;
                    case Board::player:
                        actorList.push_back(new Player(IID_PEACH, 1, i*16,j*16, this));
                        break;
                    case Board::red_coin_square:
                        actorList.push_back(new CoinSquare(IID_RED_COIN_SQUARE, i*16,j*16, this));
                        break;
                    case Board::blue_coin_square:
                        actorList.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, i*16,j*16, this));
                        break;
                    case Board::left_dir_square:
                        actorList.push_back(new DirectionSquare(IID_BOWSER, i*16,j*16, this));
                        break;
                    case Board::right_dir_square:
                        actorList.push_back(new DirectionSquare(IID_BOWSER, i*16,j*16, this));
                        break;
                    case Board::up_dir_square:
                        actorList.push_back(new DirectionSquare(IID_BOWSER, i*16,j*16, this));
                        break;
                    case Board::down_dir_square:
                        actorList.push_back(new DirectionSquare(IID_BOWSER, i*16,j*16, this));
                        break;
                    case Board::event_square:
                        actorList.push_back(new EventSquare(IID_BOWSER, i*16,j*16, this));
                        break;
                    case Board::bank_square:
                        actorList.push_back(new BankSquare(IID_BOWSER, i*16,j*16, this));
                        break;
                    case Board::star_square:
                        actorList.push_back(new StarSquare(IID_BOWSER, i*16,j*16, this));
                        break;
                     // etc...
                }
            }
        }
        
    }
    
    startCountdownTimer(5);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
        
    
    return 0;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    setGameStatText("Game will end in a few seconds");
    
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}
