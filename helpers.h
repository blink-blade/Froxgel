#ifndef HELPERS_H
#define HELPERS_H
#include <bits/stdc++.h>
#include <vector>
#include "window.h"
using namespace std;
extern float timeValue;

extern vector<string> splitString(string s, char delim);
extern void engineInits();
extern unsigned int loadTexture(char const * path, bool linearFiltering);
extern void engineUpdates();
#endif