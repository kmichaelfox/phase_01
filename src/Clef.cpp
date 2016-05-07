//
//  Clef.cpp
//  phase_01
//
//  Created by Kelly Fox on 4/3/16.
//
//

#include "Clef.hpp"

using namespace std;

unordered_map<short, Clef> Clef::ClefData = {
    {TREBLE_8VA, Clef(83)},
    {TREBLE, Clef(71)},
    {TREBLE_8VB, Clef(59)},
    {ALTO, Clef(60)},
    {BASS, Clef(50)}
};