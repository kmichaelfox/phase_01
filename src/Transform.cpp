//
//  Transform.cpp
//  phase_01
//
//  Created by Kelly Fox on 3/20/16.
//
//

#include "Transform.hpp"
#include "Note.hpp"

using namespace std;
typedef vector<Note> Sequence;

static vector<int> flatNoteMapping = {
    0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6
};
static vector<int> sharpNoteMapping = {
    0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6
};

Note Transform::transpose(int interval, Note note) {
    return note;
}

Sequence Transform::transpose(int interval, Sequence seq) {
    return Sequence();
}

int Transform::pitchClassToNoteName(int note, bool preferFlat) {
    if (preferFlat) {
        return flatNoteMapping[note];
    } else {
        return sharpNoteMapping[note];
    }
}