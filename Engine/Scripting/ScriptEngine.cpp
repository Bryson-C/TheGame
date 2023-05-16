//
// Created by Owner on 4/1/2023.
//

#include "ScriptEngine.hpp"

ScriptEngine::ScriptEngine() {
    if (!(context = duk_create_heap_default())) {
        throw std::runtime_error("Error Creating Scripting Engine");
    }
}

ScriptEngine::~ScriptEngine() {
    duk_destroy_heap(context);
}