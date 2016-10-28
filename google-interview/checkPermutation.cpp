

bool checkPermutation(string str1, string str2) {

    if(str1.length() != str2.length()){
        return false;
    }
    
    //make a counter array. size 256. Count the chaaracters in oen string
    //and put htem all into the array
    //Take those coutns and subtarcat the counts of the second strings ints
    //If any are non-zero return fasle
    //

    //sort both strings and then compare them elemtn wise adn if they differ return fasle
    //
