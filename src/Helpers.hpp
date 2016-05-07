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

namespace PortSearch {
enum Type {
    Blocking,
    NonBlocking
};
}

namespace Helpers {
    int scanForProcessPort(std::string processName, PortSearch::Type isBlocking);
}


#endif /* Helpers_hpp */
