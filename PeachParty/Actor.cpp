#include "Actor.h"
#include "StudentWorld.h"

#include <random>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::dosomething(){
    if(m_state == 0){
//             if invalid dir
//                 select random dir that has square you can move to
//                    if ( == left){
//                        m_spriteDir = 180;
//                    }
//                    m_spriteDir = 0;

        if(getWorld()->getAction(1) == ACTION_ROLL){
//            int die_roll = randInt(1, 10);
//            int ticks_to_move = die_roll * 8;
            changeState();
        }
    }
    else if (m_state == 1){
        
    }
    
};
void  Player::changeState(){
    if(m_state == 0)
        m_state = 1;
    m_state = 0;
}
