//
//  Transform.hpp
//  phase_01
//
//  Created by Kelly Fox on 3/20/16.
//
//

#ifndef Transform_hpp
#define Transform_hpp

#include <iostream>
#include <vector>

class Note;

namespace Transform {
    std::vector<Note> transpose(int interval, std::vector<Note> sequence);
    Note transpose(int interval, Note note);
    
    int pitchClassToNoteName(int note, bool preferFlat);
}

#endif /* Transform_hpp */
