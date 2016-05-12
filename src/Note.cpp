//
//  Note.cpp
//  phase_01
//
//  Created by Kelly Fox on 3/4/16.
//
//

#include "Note.hpp"

Note::Note(int n, NoteLength d, NoteType t, int dots)
: note(n)
, dur(d)
, dots(dots)
, type(t)
{}

Note::Note(Note &n, int newNote)
: note(newNote)
, dur(n.dur)
, dots(n.dots)
, type(n.type)
{}