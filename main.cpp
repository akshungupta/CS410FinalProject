#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>
using namespace std;

vector<pair<string, string>> split(string word) {
    vector<pair<string, string>> array;
    for (unsigned int i = 0; i < word.length() + 1; i++) {
        pair<string, string> currPair(word.substr(0, i), word.substr(i, word.length() - i));
        array.push_back(currPair);
    }
    return array;
}

vector<string> inserts (vector <pair<string,string>> myPairs, string alphabet){
    vector<string> insertArr;
    for (size_t i = 0; i < myPairs.size(); i++){
        for (size_t j = 0; j < alphabet.length(); j++) {
            string temp = myPairs[i].first + alphabet.substr(j, 1) + myPairs[i].second;
            insertArr.push_back(temp);
        }
    }
    return insertArr;
}

vector<string> replaces (vector<pair<string, string>> myPairs, string alphabet) {
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

vector<string> transposes (vector<pair<string, string>> myPairs) {
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

vector<string> deletes(vector<pair<string, string>> splits){
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

    vector<pair<string, string>> splits = split(word);
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

// To execute C++, please define "int main()"
int main() {
    unordered_set<string> finalSet = editDistance1("hello");
    cout << finalSet.size() << endl;
    for (auto it: finalSet) {
        if (it.find("\n") != 0){
            cout << it << endl;
            cout << "WTF______________________________________________" << endl;
        }
    }
    return 0;
}