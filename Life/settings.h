#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
public:
    Settings();
    ~Settings();

    static int xSize;
    static int ySize;
    static int speed;

    static bool replaceMode;
    static bool xorMode;
    static bool impact;

    static int firstImpact;
    static int secondImpact;
    static int liveBegin;
    static int liveEnd;
    static int birthBegin;
    static int birthEnd;

    static int cellSize;
    static int lineWidth;

};

#endif // SETTINGS_H
