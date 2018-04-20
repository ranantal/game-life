#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <iostream>

enum State {
    ALIVE,
    DEAD
};

class Hex {
public:
    Hex(State state) {
        this->state = state;
        impact = 0.0;
    }

    Hex() {
        state = DEAD;
        impact = 0.0;
    }

    int impact;

    State state;
};

class GameModel {
public:
    std::vector<std::vector<Hex> > field;

    GameModel();
    void init(int x, int y);

    void calculateImpacts();
    void nextStep();
    void setHexState(int y, int x, State state);

    Hex getHex(int y, int x);

    void printField();

    void operator=(GameModel& model);

    bool isActual;

private:

};

std::ostream& operator<<(std::ostream& os, GameModel& model);
std::istream& operator>> (std::istream& is, GameModel& model);

#endif // GAMEMODEL_H
