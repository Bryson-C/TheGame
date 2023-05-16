//
// Created by Owner on 5/2/2023.
//

#ifndef SDL_SAVELOAD_HPP
#define SDL_SAVELOAD_HPP

#include <iostream>
#include <map>

class SaveLoad {
private:
    void split(std::string& code);
public:
    SaveLoad(std::string filePath);
};


#endif //SDL_SAVELOAD_HPP
