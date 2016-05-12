//
//  Helpers.hpp
//  phase_01
//
//  Created by Kelly Fox on 5/6/16.
//
//

#ifndef Helpers_hpp
#define Helpers_hpp

#include <iostream>
#include <vector>
#include "Note.hpp"

struct Range;

namespace PortSearch {
enum Type {
    Blocking,
    NonBlocking
};
}

namespace Helpers {
    int scanForProcessPort(std::string processName, PortSearch::Type isBlocking);
    
    std::vector<Note> rangedMidiFromPitchClass(std::vector<Note> seq, Range r, bool octaveUp = false);
}


#endif /* Helpers_hpp */
