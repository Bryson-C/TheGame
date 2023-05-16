//
// Created by Owner on 4/1/2023.
//

#ifndef SDL_SCRIPTENGINE_HPP
#define SDL_SCRIPTENGINE_HPP

#include <iostream>

#include "duktape.h"


// Uses Duktape
class ScriptEngine {
private:
    duk_context* context;
public:
    ScriptEngine();
    ~ScriptEngine();
};


#endif //SDL_SCRIPTENGINE_HPP
