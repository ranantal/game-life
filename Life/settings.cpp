#include "settings.h"

Settings::Settings() {

}

Settings::~Settings() {

}

int Settings::xSize = 10;
int Settings::ySize = 10;
int Settings::speed = 1;

bool Settings::replaceMode = true;
bool Settings::xorMode = false;
bool Settings::impact = false;

int Settings::firstImpact = 10;
int Settings::secondImpact = 03;
int Settings::liveBegin = 20;
int Settings::liveEnd = 33;
int Settings::birthBegin = 23;
int Settings::birthEnd = 29;

int Settings::cellSize = 35;
int Settings::lineWidth = 1;
