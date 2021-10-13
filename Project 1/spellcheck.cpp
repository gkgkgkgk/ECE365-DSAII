#include "hash.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <regex>

using namespace std;

string lowerCase(string &word)
{
    for (int i = 0; i < word.length(); i++)
    {
        word[i] = tolower(word[i]);
    }

    return word;
}

hashTable buildHashTable(string dictName)
{
    auto start = std::chrono::steady_clock::now();
    hashTable ht(0);
    ifstream dictFile(dictName);

    for (string word; getline(dictFile, word);)
    {
        ht.insert(lowerCase(word));
    }

    auto end = chrono::steady_clock::now();

    cout << "Total time to load dictionary: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0f
         << " seconds" << endl;

    return ht;
}

void checkWordInDict(string word, int line, hashTable &ht, ofstream &output)
{
    word = lowerCase(word);
    if (word.size() > 20)
    {
        output << "Long word at line " << line
               << ", starts: " << word.substr(0, 20) << "\n";
        return;
    }
    else if (!ht.contains(word))
    {
        output << "Unknown word at line " << line << ": " << word << endl;
    }

    return;
}

void spellCheck(string inputFile, string outputFile, hashTable &ht)
{
    auto start = std::chrono::steady_clock::now();
    ifstream input(inputFile);
    ofstream output(outputFile);
    int line = 1;

    regex r("([a-zA-Z0-9'-]+)");

    for (std::string word; getline(input, word);)
    {
        for (smatch m; regex_search(word, m, r); word = m.suffix())
        {
            if (((string)m[0]).find_first_of("1234567890") == string::npos)
            {
                checkWordInDict(m[0], line, ht, output);
            }
        }

        line++;
    }
    auto end = chrono::steady_clock::now();
    cout << "Total time to spell check: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0f
         << " seconds" << endl;
}

int main()
{
    string dictName;
    string inputFile;
    string outputFile;

    cout << "Enter path of dictionary: \n";
    cin >> dictName;

    hashTable ht = buildHashTable(dictName);

    cout << "Enter path of input file: \n";
    cin >> inputFile;
    cout << "Enter path of output file: \n";
    cin >> outputFile;

    spellCheck(inputFile, outputFile, ht);

    return 0;
}