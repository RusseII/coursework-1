#include <iostream>
#include <vector>
using namespace std;

int fib(int n) {
    if(n == 1 || n == 2){
        return 1;
    }
        return fib(n-1) + fib(n-2);
}

vector<int> memo(100, 0); 
int f;
int memofib(int n) {
    if(memo[n] == 0)
        f = memo[n];
    if(n==1 || n==2)
        f = 1;
    else
        f = memofib(n-1) + memofib(n-2);
    memo[n] = f;
    return f;
}


int main() {

    int out;
    cout << "Starting fib\n";
    out = fib(20);
    cout << "output: "<< out;
    cout << "finished fib\n";
    cout << "finished fib\n";
    cout << "finished fib\n";
    cout << "finished fib\n";
    cout << "finished fib\n";
    cout << "finished fib\n";
    cout << "finished fib\n";
    cout << "finished fib\n";
    cout << "finished fib\n";
    cout << "finished fib\n";
    cout << "staring memofib";
    out = memofib(20);
    cout << "output:" << out;
    cout << endl;
}

