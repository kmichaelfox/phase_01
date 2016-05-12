//
//  ScoreManager.hpp
//  phase_01
//
//  Created by Kelly Fox on 5/9/16.
//
//

#ifndef ScoreManager_hpp
#define ScoreManager_hpp

#include <iostream>
#include <vector>
#include "Player.hpp"

class ScoreManager {
    std::vector<Player *> players;
    Sequence seq;
    ofArduino ard;
    bool bSetupArduino;
    
    int numPlayers = 0;
    int dimensions = 1;
    float windowAspect;
    
    int activePlayer = -1;
    
public:
    ScoreManager();
    ~ScoreManager();
    
    void draw();
    void update();
    void resize(int w, int h);
    
    void addPlayer(Player * p);
    void newSequence();
    void activate(int playerNum);
    void pause();
    void resume();
    void stop();
    
    int getNumPlayers();
    void setPlayerTempo(int playerNum, float tempo);
    
    // arduino callbacks
    void setupArduino(const int & version);
    void analogPinChanged(const int & pinNum);
    void updateArduino();
    
    std::string analogVal;
};

#endif /* ScoreManager_hpp */
