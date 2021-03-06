#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;


unordered_map<string, int> bg_model;
void readAllWords (string allWordsFile) {
    ifstream infile(allWordsFile);
    string word;

    while (getline(infile, word)) {
        bg_model[word]++;
    }
}

void readCorpus (string corpusFile) {
    ifstream infile(corpusFile);
    string word;
    while (infile >> word) {
        bg_model[word] += 1;
    }
}

void train (string allWordsFile, string corpusFile) {
    readAllWords(allWordsFile);
    readCorpus(corpusFile);
}

vector<pair<string, string> > split(string word) {
    vector<pair<string, string> > array;
    for (unsigned int i = 0; i < word.length() + 1; i++) {
        pair<string, string> currPair(word.substr(0, i), word.substr(i, word.length() - i));
        array.push_back(currPair);
    }
    return array;
}

vector<string> inserts (vector <pair<string,string> > myPairs, string alphabet){
    vector<string> insertArr;
    for (size_t i = 0; i < myPairs.size(); i++){
        for (size_t j = 0; j < alphabet.length(); j++) {
            string temp = myPairs[i].first + alphabet.substr(j, 1) + myPairs[i].second;
            insertArr.push_back(temp);
        }
    }
    return insertArr;
}

vector<string> replaces (vector<pair<string, string> > myPairs, string alphabet) {
    vector<string> replaceArr;
    for (size_t i = 0; i < myPairs.size(); i++){
        for (size_t j = 0; j < alphabet.length(); j++) {
            if (myPairs[i].second.length() > 0) {
                string temp = myPairs[i].first + alphabet.substr(j, 1) + myPairs[i].second.substr(1);
                replaceArr.push_back(temp);
            }
        }
    }
    return replaceArr;
}

vector<string> transposes (vector<pair<string, string> > myPairs) {
    vector<string> transposeArr;
    for (size_t i = 0; i < myPairs.size(); i++){
        if (myPairs[i].second.length() > 1) {
            string a = myPairs[i].first;
            string b = myPairs[i].second;
            string temp = a + b.substr(1,1) + b.substr(0,1) + myPairs[i].second.substr(2);
            transposeArr.push_back(temp);
        }
    }
    return transposeArr;
}

vector<string> deletes(vector<pair<string, string> > splits){
    vector<string> deletes;
    for (auto pair : splits){
        if (pair.second != ""){
            string firstWord = pair.first;
            string secondWord = pair.second.substr(1);
            string finalWord = firstWord + secondWord;
            deletes.push_back(finalWord);
        }
    }
    return deletes;
}

unordered_set<string> editDistance1(string word){
    string alphabet = "abcdefghijklmnopqrstuvwxyz";

    vector<pair<string, string> > splits = split(word);
    vector<string> deleteWords = deletes(splits);
    vector<string> transposeWords = transposes(splits);
    vector<string> replaceWords = replaces(splits, alphabet);
    vector<string> insertWords = inserts(splits, alphabet);

    unordered_set<string> finalSet;
    finalSet.insert(deleteWords.begin(), deleteWords.end());
    finalSet.insert(transposeWords.begin(), transposeWords.end());
    finalSet.insert(replaceWords.begin(), replaceWords.end());
    finalSet.insert(insertWords.begin(), insertWords.end());

    return finalSet;
}

unordered_set<string> knownEditDistance2(string word){
    unordered_set<string> distanceOneWords = editDistance1(word);
    unordered_set<string> retSet;
    for (auto currWord : distanceOneWords){
        unordered_set<string> distanceTwoWords = editDistance1(currWord);
        for (auto twoWord : distanceTwoWords){
            if (bg_model.find(twoWord) != bg_model.end()){
                retSet.insert(twoWord);
            }
        }
    }
    return retSet;
}

unordered_set<string> knownWords(unordered_set<string> words){
    unordered_set<string> retSet;
    for (auto currWord : words){
        if (bg_model.find(currWord) != bg_model.end()){
            retSet.insert(currWord);
        }
    }
    return retSet;
}

string getMaxWeightWord(unordered_set<string> words){
    int maxWeight = -1;
    string wordWithMaxWeight;
    for (auto currWord : words){
        if (bg_model[currWord] > maxWeight){
            maxWeight = bg_model[currWord];
            wordWithMaxWeight = currWord;
        }
    }
    return wordWithMaxWeight;
}

string getCorrectWord(string word){
    unordered_set<string> finalSet;
    if (bg_model.find(word) != bg_model.end())
        finalSet.insert(word);
    unordered_set<string> bgModelWords = knownWords(finalSet);
    if (bgModelWords.size() != 0)
        return getMaxWeightWord(bgModelWords);
    unordered_set<string> editDistance1Words  = editDistance1(word);
    unordered_set<string> knownEditDistance1Words = knownWords(editDistance1Words);
    if (knownEditDistance1Words.size() != 0)
        return getMaxWeightWord(knownEditDistance1Words);
    unordered_set<string> knownEditDistance2Words = knownEditDistance2(word);
    return getMaxWeightWord(knownEditDistance2Words);
}

void correctFile (string input_filename, string output_filename) {
    ofstream myfile;
    myfile.open (output_filename);
    ifstream infile(input_filename);

    string line = "";
    while (getline(infile, line)) {

        istringstream iss(line);
        string word;
        string result_string = "";        

        if (line.empty()) {
            myfile << "\n";
            continue;
        }

        while (iss >> word) {
            unordered_map<string, vector<char> > additionalChars;

            int wordLen = word.length();
            int correctStart = 0;
            int correctEnd = wordLen-1;
            bool isCapitalized = false;

            if (word.at(0) < 65 || (word.at(0) >= 91 && word.at(0) <= 96) || word.at(0) > 122) {
                result_string += word.at(0);
                correctStart = 1;
            }

            if (word[correctStart] > 64 && word[correctStart] < 91) {
                isCapitalized = true;
                word[correctStart] = tolower(word[correctStart]);
            }


            while (word.at(correctEnd) < 65 || (word.at(correctEnd) > 90 && word.at(correctEnd) <= 96) || word.at(correctEnd) > 122) {
                additionalChars["end"].push_back(word.at(correctEnd));
                correctEnd--;    
            }

            string string_to_check = word.substr(correctStart, correctEnd + 1);
            string correct_word = getCorrectWord(string_to_check);

            if (isCapitalized) {
                correct_word[0] = toupper(correct_word[0]);
            }

            result_string += correct_word;

            for (int i = additionalChars["end"].size()-1; i >= 0; i--) {
                result_string += additionalChars["end"][i];
            }

            result_string += " ";
        }

        result_string[result_string.length()-1] = '\n';
        myfile << result_string;
    }

    myfile.close();
}

// to execute C++, please define "int main()"
int main() {
    train("./words.txt", "./big.txt");
    correctFile("input_test.txt", "output_test.txt");
    return 0;
}