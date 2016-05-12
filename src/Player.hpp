//
//  Player.hpp
//  phase_01
//
//  Created by Kelly Fox on 3/20/16.
//
//

#ifndef Player_hpp
#define Player_hpp

#include "ofMain.h"
#include "Staff.hpp"
#include "Clef.hpp"

class Staff;

class Player {
public:
    Player();
    Player(Instrument instr, std::string name = "");
    Player(ClefType clef, std::string name = "");
    ~Player();
    
    void update();
    void draw();
    void draw(int x, int y);
    
    std::string getName();
    void setName(std::string n);
    
    int getWidth();
    void setWidth(int width);
    
    int getHeight();
    void setHeight(int height);
    
    float getTempo();
    void setTempo(float tempo);
    
    Range getRange();
    
    bool isFirstChair();
    
    void idle(bool immediately=false);
    bool isActive();
    void activate(bool state);
    
    void queueSequence(Sequence seq, float tempo);
    void createSingleNote(int note);
    
    float getAspectRatio();
    
private:
    ofFbo fbo;
    Staff *staff;
    
    int drawWidth;
    int drawHeight;
    
    float tempo = 120;
};

#endif /* Player_hpp */
