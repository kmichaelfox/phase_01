//
//  ScoreScene.cpp
//  phase_01
//
//  Created by Kelly Fox on 5/14/16.
//
//

#include "ScoreScene.hpp"
#include <time.h>

using namespace std;

ScoreScene::ScoreScene() {}

void ScoreScene::update() {
    for (auto it = timers.begin(); it != timers.end();) {
        if (difftime(time(NULL), (*it).time) > (*it).duration) {
            triggers.push_back((*it).player);
            it = timers.erase(it);
        } else {
            ++it;
        }
    }
}

void ScoreScene::addPlayerSequence(Sequence s) {
    playerSequences.push_back(s);
}

void ScoreScene::generateTimers() {
    vector<int> p_id;
    for (int i = 0; i < playerSequences.size(); i++) {
        p_id.push_back(i);
    }
    
    random_shuffle(p_id.begin(), p_id.end());
    
    for (int i = 0; i < playerSequences.size(); i++) {
        timers.push_back({i, 0, p_id[i] * 7.0f});
        time(&timers.back().time);
    }
}

vector<int> ScoreScene::getTriggers() {
    vector<int> currentTriggers;
    
    for (int i = 0; i < triggers.size(); i++) {
        currentTriggers.push_back(triggers[i]);
    }
    triggers.clear();
    
    return currentTriggers;
}

bool ScoreScene::hasAvailableTriggers() {
    return !triggers.empty();
}

Sequence ScoreScene::getSequence(int playerNumber) {
    return playerSequences[playerNumber];
}

void ScoreScene::setNoteType(NoteType t) {
    type = t;
}

NoteType ScoreScene::getNoteType() {
    return type;
}

bool ScoreScene::isEmpty() {
    return playerSequences.empty();
}