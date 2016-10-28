//Given a string, write a funciton to check if it is a permutation of a palindrome. 


//racecar
//raecarc
//
//Check all permutations of the sting and see if they are palindromes
//
//Palindrome
//      same forward as it is backwarsd
//      car
//      racecar
//      aaccerr
//      each has two letters but one can have one letter
//
//      if it is odd it must have all two letters and one one letter
//      if even it must have all two letters
//
#include <iostream>
using namespace std;


bool isPermPalindrome(string str){
    int array[256] = {0};
    int numOdd = 0;
    for(int i = 0; i < str.length(); i++){
        array[str[i]]++;
    }

    if(str.length() % 2 != 0){
        for(int i = 0; i < 256; i++){
            if(array[i] == 1)
                numOdd++;
            else if(array[i] != 0 && array[i]!= 2)
                return false;

            if(numOdd > 1)
                return false;
        }
    }
    return true;

}
 
int main() {
    string str = "iraecar";

    cout << isPermPalindrome(str);
}
