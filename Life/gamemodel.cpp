#include "settings.h"
#include "gamemodel.h"

#include <iostream>
#include <string>
#include <strstream>
#include <sstream>

std::ostream& operator<<(std::ostream& os, GameModel& model) {
    os << Settings::xSize << " " << Settings::ySize << std::endl;
    os << 1 << std::endl;
    os << Settings::cellSize << std::endl;

    std::string aliveCells = "";
    int aliveCellsCount = 0;

    for (int y = 0; y < Settings::ySize; ++y) {
        for (int x = 0; x < Settings::xSize - (y % 2); ++x) {
            if (model.getHex(y, x).state == ALIVE) {
                ++aliveCellsCount;
                aliveCells += '\n';
                aliveCells += '0' + x;
                aliveCells += ' ';
                aliveCells += '0' + y;
            }
        }
    }

    os << aliveCellsCount;
    os << aliveCells << std::endl;

    return os;
}

std::istream& operator>> (std::istream& is, GameModel& model) {
    char c;

    const std::string formatError = "Wrong file format";
    const std::string dataError = "Wrong data";

    std::string str = "";

    while (!is.eof()) {
        is.get(c);
        if ((c >= '0' && c <= '9') || c == ' ' || c == '\n') {
            str += c;
            continue;
        }
        else if (c == '/') {
            is.get(c);
            if (c == '/') {
                while (c != '\n')
                    is.get(c);
                str += c;
            } else throw formatError;
        } else throw formatError;
    }

    std::stringstream ss(str);

    GameModel _model;

    int xSize;
    int ySize;
    int lineWidth;
    int cellSize;
    int cellsCount;
    int x;
    int y;

    ss >> xSize;
    if (ss.eof())
        throw dataError;

    ss >> ySize;
    if (ss.eof())
        throw dataError;

    _model.init(xSize, ySize);

    ss >> lineWidth;
    if (ss.eof())
        throw dataError;

    ss >> cellSize;
    if (ss.eof())
        throw dataError;

    ss >> cellsCount;
    if (ss.eof())
        throw dataError;

    for (int i = 0; i < cellsCount; ++i) {
        if (ss.eof())
            throw dataError;

        ss >> x;

        if (ss.eof())
            throw dataError;

        ss >> y;

        _model.setHexState(y, x, ALIVE);
    }

    if (!ss.eof())
        throw dataError;

    Settings::xSize = xSize;
    Settings::ySize = ySize;
    Settings::cellSize = cellSize;

    model = _model;

    return is;
}

GameModel::GameModel()
{
}

void GameModel::init(int x, int y) {
    field.resize(y);

    for (int i = 0; i < y; ++i) {
        field[i].resize(x - (i % 2));
    }

    isActual = true;
}

void GameModel::nextStep() {
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[i].size(); ++j) {
            if (field[i][j].state == ALIVE) {
                if (field[i][j].impact > Settings::liveEnd || field[i][j].impact < Settings::liveBegin)
                    field[i][j].state = DEAD;
            } else {
                if (field[i][j].impact <= Settings::birthEnd && field[i][j].impact >= Settings::birthBegin)
                    field[i][j].state = ALIVE;
            }
        }
    }

    isActual = false;
}

void GameModel::calculateImpacts() {
    for (int y = 0; y < field.size(); ++y) {
        for (int x = 0; x < field[y].size(); ++x) {
            int firstImpactCount = 0;
            int secondImpactCount = 0;

            if (x - 1 > -1)
                firstImpactCount += field[y][x - 1].state == ALIVE ? 1 : 0;

            if (x + 1 < field[y].size())
                firstImpactCount += field[y][x + 1].state == ALIVE ? 1 : 0;

            if (y + 1 < field.size()) {
                if ((x + (y % 2)) < field[y + 1].size())
                    firstImpactCount += field[y + 1][x + (y % 2)].state == ALIVE ? 1 : 0;

                if ((x - ((y + 1) % 2)) > -1)
                    firstImpactCount += field[y + 1][x - ((y + 1) % 2)].state == ALIVE ? 1 : 0;
            }

            if (y - 1 > -1) {
                if ((x + (y % 2)) < field[y - 1].size())
                    firstImpactCount += field[y - 1][x + (y % 2)].state == ALIVE ? 1 : 0;

                if ((x - ((y + 1) % 2)) > -1)
                    firstImpactCount += field[y - 1][x - ((y + 1) % 2)].state == ALIVE ? 1 : 0;
            }

            if (y + 2 < field.size())
                secondImpactCount += field[y + 2][x].state == ALIVE ? 1 : 0;

            if (y - 2 > -1)
                secondImpactCount += field[y - 2][x].state == ALIVE ? 1 : 0;

            if (y + 1 < field.size()) {
                if ((x + 1 + (y % 2)) < field[y + 1].size())
                    secondImpactCount += field[y + 1][x + 1 + (y % 2)].state == ALIVE ? 1 : 0;
                if ((x - 1 - ((y + 1) % 2)) > -1)
                    secondImpactCount += field[y + 1][x - 1 - ((y + 1) % 2)].state == ALIVE ? 1 : 0;
            }

            if (y - 1 > -1) {
                if ((x + 1 + (y % 2)) < field[y - 1].size())
                    secondImpactCount += field[y - 1][x + 1 + (y % 2)].state == ALIVE ? 1 : 0;
                if ((x - 1 - ((y + 1) % 2)) > -1)
                    secondImpactCount += field[y - 1][x - 1 - ((y + 1) % 2)].state == ALIVE ? 1 : 0;
            }

            field[y][x].impact = firstImpactCount * Settings::firstImpact + secondImpactCount * Settings::secondImpact;
        }
    }
}

void GameModel::setHexState(int y, int x, State state) {
    field[y][x].state = state;
    isActual = false;
}

Hex GameModel::getHex(int y, int x) {
    return field[y][x];
}

void GameModel::printField() {
    for (int i = 0; i < field.size(); ++i) {
        if (i % 2 == 1)
            std::cout << " ";

        for (int j = 0; j < field[i].size(); ++j) {
            std::cout << field[i][j].state << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void GameModel::operator=(GameModel& model) {
    this->field = model.field;
    this->isActual = model.isActual;
}
