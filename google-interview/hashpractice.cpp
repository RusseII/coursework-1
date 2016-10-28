#include<iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;

bool allUnique(string str) {
    for(int i = 0; i < str.length(); i++) {
        char check = str[i];

        for(int j = i+1; j < str.length(); j++) {
            if(str[j] == check){
                return false;
            }
        }
    }
    return true;
}

bool betterUnique(string str) {
    std::sort(str.begin(), str.end());

    for(int i = 0; i < str.length()-1; i++){
        if(str[i] == str[i+1])
            return false;
    }
    return true;
}


bool hashUnique(string str) {
    std::unordered_map<int, string> map;
    int map2[128] = {0};

    for(int i = 0; i < str.length(); i++){
        map2[str[i]]++;
        if(map2[str[i]] > 1)
            return false;
    }
    return true;

}

            
int main() {
    string st = "emerson";

    cout << allUnique(st);
    cout << betterUnique(st);
    cout << hashUnique(st);
}
