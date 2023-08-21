//
// Created by Owner on 7/19/2023.
//

#ifndef SDL_GRIDACCESSOR_HPP
#define SDL_GRIDACCESSOR_HPP

class World;
struct SparseGrid;

struct GridAccessor {
    SparseGrid* grid;
    int depth;
    int section;
    // These Functions Are Defined In The World.cpp File,
    // That Is Because They Need Access To The `world` Member

    GridAccessor loadNew(std::vector<SparseGrid*>& gridStorage, int depth, int section);
    std::array<GridAccessor, 4> loadAll(std::vector<SparseGrid*>& gridStorage);

    void print() {
        std::string msg = "Grid |\n";
        for (int i = 0; i < 4; i++) {
            msg += "     | Quad " + std::to_string(i);
            if (section == i) msg += " <\n";
            else msg += "\n";
        }
        printf("%s", msg.c_str());
    }
};


#endif //SDL_GRIDACCESSOR_HPP
