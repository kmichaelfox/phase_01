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

class Note {
public:
    int note;
    NoteLength dur;
//public:
    Note(int n, NoteLength d);
    //Note(Note & n);
};

#endif /* Note_hpp */
