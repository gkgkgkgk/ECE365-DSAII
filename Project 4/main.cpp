#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

int mat[1001][1001];

int checkEquality(char a, char b){
    return tolower(a) == tolower(b);
}

void checkMerge(string input, string output){
    ifstream inFile;
    ofstream outFile;
    string w1, w2, m;
    int w1len, w2len, mlen;

    inFile.open(input);
    outFile.open(output);

    while(inFile >> w1 && inFile >> w2 && inFile >> m){
        w1len = w1.length();
        w2len = w2.length();
        mlen = m.length();

        memset(mat, 0, sizeof(mat));
    
        if(w1len + w2len != mlen){
            outFile << "*** NOT A MERGE ***" << endl;
            continue;
        }
        
        for(int i = 0; i <= w1len; i++){
            for(int j = 0; j <= w2len; j++){
                if (i == 0 && j == 0){
                    // set first cell to 1
                    mat[i][j] = 1;
                }
                else if(i == 0){
                     // if current char in w2 matches current char in m and previous char in w2 matches previous char in m
                    mat[i][j] = mat[0][j-1] && checkEquality(w2[j - 1], m[j-1]);
                }
                else if(j == 0){
                    // if current char in w1 matches current char in m and previous char in w1 matches previous char in m
                    mat[i][j] = mat[i-1][0] && checkEquality(w1[i - 1], m[i-1]);
                }
                else {
                    // check if either string has a matching char and previous char matches
                    mat[i][j] = (mat[i - 1][j] && checkEquality(w1[i-1], m[i+j-1])) || (mat[i][j - 1] && checkEquality(w2[j-1], m[i+j-1])); 
                }

            }
        }

        if(mat[w1len][w2len] == 1){
            while (w1len >= 0 && w2len >= 0){
                if(mat[w1len][w2len]){ // if the current cell is 1
                    if(w2len == 0 || !mat[w1len][w2len - 1]){ // check if it is part of the first word by checking if it is in the first row or if the cell above it is 0
                        m[w1len + w2len -1] = toupper(m[w1len + w2len -1]);
                        w1len--;
                    }
                    else {
                        w2len--;
                    }
                }
            }

            outFile << m << endl;
        }
        else{
            outFile << "*** NOT A MERGE ***" << endl;
        }
    }
}

int main(int argc, char * argv[]){
    cout << "Enter name of input file:" << endl;
    string inputFile;
    cin >> inputFile;
    cout << "Enter name of output file:" << endl;
    string outputFile;
    cin >> outputFile;

    checkMerge(inputFile, outputFile);

    return 0;
}