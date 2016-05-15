//
//  ScoreScene.hpp
//  phase_01
//
//  Created by Kelly Fox on 5/14/16.
//
//

#ifndef ScoreScene_hpp
#define ScoreScene_hpp

#include <iostream>
#include <vector>
#include "Note.hpp"
#include "Staff.hpp"

struct Timer {
    int player;
    time_t time;
    float duration;
};

class ScoreScene {
    NoteType type;
    std::vector<Sequence> playerSequences;
    std::vector<Timer> timers;
    std::vector<int> triggers;
public:
    ScoreScene();
    
    void update();
    
    void addPlayerSequence(Sequence s);
    void generateTimers();
    std::vector<int> getTriggers();
    bool hasAvailableTriggers();
    Sequence getSequence(int playerNumber);
    void setNoteType(NoteType t);
    NoteType getNoteType();
    bool isEmpty();
};

#endif /* ScoreScene_hpp */
