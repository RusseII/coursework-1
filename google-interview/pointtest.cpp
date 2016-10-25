#include <iostream>

using namespace std;

int main() {
    int *x = new int;
    *x = 5;
    cout << "Here is just x: " <<x << endl;
    cout << "Here is &x: " << &x << endl;
    cout << "Here is *x: " << *x << endl;

    int y = 10;
    cout << "Here is just y: " <<y << endl;
    cout << "Here is &y: " << &y << endl;
    //cout << "Here is *y: " << *y << endl;

}

