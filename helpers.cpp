#include "helpers.h"
#include <bits/stdc++.h>
#include <vector>
using namespace std;

vector<string> splitString(string s, char delim) {
    stringstream ss(s);
    vector<string> output;

    while (getline(ss, s, delim)) {

        // store token string in the vector
        output.push_back(s);
    }
    return output;
}
