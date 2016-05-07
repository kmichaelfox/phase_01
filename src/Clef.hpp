//
//  Clef.hpp
//  phase_01
//
//  Created by Kelly Fox on 4/3/16.
//
//

#ifndef Clef_hpp
#define Clef_hpp

#include <iostream>
#include <unordered_map>

enum ClefType : short {
    TREBLE_8VA = 0,
    TREBLE,
    TREBLE_8VB,
    ALTO,
    BASS
};

class Clef {
public:
    const int centerNote;
    
    Clef(int center)
    : centerNote(center)
    {}
    
    static std::unordered_map<short, Clef> ClefData;
};

#endif /* Clef_hpp */
