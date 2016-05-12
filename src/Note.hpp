//
//  Note.hpp
//  phase_01
//
//  Created by Kelly Fox on 3/4/16.
//
//

#ifndef Note_hpp
#define Note_hpp

#include <iostream>

enum NoteLength {
    WHOLE = 0,
    HALF = 1,
    QUARTER = 2,
    EIGHTH = 3,
    SIXTEENTH = 4,
};

enum NoteType {
    PIZZ = 0,
    ARCO = 1
};

class Note {
public:
    int note;
    NoteLength dur;
    NoteType type;
    int dots;
    Note(int n, NoteLength d, NoteType t, int dots = 0);
    Note(Note & n, int newNote);
};

#endif /* Note_hpp */
