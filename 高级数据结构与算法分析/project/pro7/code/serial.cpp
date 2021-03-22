#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>
using namespace std;

//WordCount contains the main class
class WordCount{
public:
    explicit WordCount(string new_word) { word = std::move(new_word); num = 1; };
    string get_word() const { return word; }          //return the string of wht word
    int get_number() const { return num; }      //return the frequency of the word
    void add_number() { num++; }                //add one more word
private:
    string word;
    int num;
};

//this function is used when sorting
//if a appears earlier than b, then a.num > b.num
//or, a.num == b.num and a.word < b.word
bool compare(const WordCount& a, const WordCount& b) {
    if (a.get_number() > b.get_number())  return true;
    string A(a.get_word()),B(b.get_word());
    //If two or more words have the same number of occurrences, they must be printed in lexicographical order
    transform(A.begin(),A.end(),A.begin(),::tolower);
    transform(B.begin(),B.end(),B.begin(),::tolower);
    return a.get_number() == b.get_number() && A <= B;
}

int main (int argc, char* argv[]){
    //start timing
    clock_t start, total;
    start = clock();

    //word_count is used to save the information of a word
    vector<WordCount> word_count;
    string temp_line, temp_word;

    //start traverse the input files
    for (int i = 1; i < argc; i++) {
        //get the input stream
        ifstream input_file(argv[i]);
        while (getline(input_file, temp_line)) {
            //deliver one word each time
            istringstream is_word(temp_line);
            while (is_word >> temp_word){
                //check duplication
                if (word_count.empty()) {
                    WordCount new_word(temp_word);
                    word_count.push_back(new_word);
                    continue;
                }
                for (int j = 0; j < word_count.size(); j++) {
                    if (temp_word == word_count[j].get_word()) { //if find the word, then add one more time
                        word_count[j].add_number();
                        break;
                    } else if (j == word_count.size() - 1) { //otherwise, create a new item for the vector
                        WordCount new_word(temp_word);
                        word_count.push_back(new_word);
                        break;
                    }
                }
            }
        }
        input_file.close();
    }

    //sort the vector
    sort(word_count.begin(), word_count.end(), compare);

    //finally traverse all the words in the vector and put them into the result files
    ofstream output_file("result.txt");

    for (auto & i : word_count) {
        output_file << i.get_word() << "         " << i.get_number() << endl;
    }
    //end timing
    total = clock();
    output_file << endl << "Total time : " << (double)(total - start)/(double)CLOCKS_PER_SEC <<" s"<< endl;
    output_file.close();
}