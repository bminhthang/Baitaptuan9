 #include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <sstream>

using namespace std;

struct Node {
    string word;   // Từ trong câu
    Node* next;    // Con trỏ tới node tiếp theo
};

class WordList {
private:
    Node* head;  // Con trỏ tới node đầu tiên

public:
    WordList() : head(nullptr) {}

    // Thêm một từ vào danh sách
    void insertWord(const string& word) {
        Node* newNode = new Node;
        newNode->word = word;
        newNode->next = head;
        head = newNode;
    }

    // Tìm từ xuất hiện nhiều nhất trong câu
    string findMostFrequentWord() {
        unordered_map<string, int> wordCount;
        Node* current = head;
        
        // Đếm tần suất của các từ
        while (current != nullptr) {
            wordCount[current->word]++;
            current = current->next;
        }
        
        // Tìm từ có tần suất cao nhất
        string mostFrequentWord;
        int maxCount = 0;
        for (const auto& entry : wordCount) {
            if (entry.second > maxCount) {
                mostFrequentWord = entry.first;
                maxCount = entry.second;
            }
        }

        return mostFrequentWord;
    }

    // Loại bỏ từ láy 
    void removeDuplicateConsecutiveWords() {
        Node* current = head;
        while (current != nullptr && current->next != nullptr) {
            if (current->word == current->next->word) {
                Node* temp = current->next;
                current->next = current->next->next; // Loại bỏ node tiếp theo
                delete temp;
            } else {
                current = current->next;
            }
        }
    }

    // Đếm số từ vựng khác nhau trong câu
    int countUniqueWords() {
        set<string> uniqueWords;
        Node* current = head;

        // Lưu các từ vào set
        while (current != nullptr) {
            uniqueWords.insert(current->word);
            current = current->next;
        }

        return uniqueWords.size();
    }

    // In danh sách các từ trong câu
    void printWords() {
        Node* current = head;
        while (current != nullptr) {
            cout << current->word << " ";
            current = current->next;
        }
        cout << endl;
    }

    ~WordList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    WordList wordList;
    string sentence;

    // Nhập câu
    cout << "Enter a sentence: ";
    getline(cin, sentence);

    // Chia câu thành các từ và thêm vào danh sách
    stringstream ss(sentence);
    string word;
    while (ss >> word) {
        wordList.insertWord(word);
    }

    // In danh sách từ
    cout << "\nWords in the sentence:\n";
    wordList.printWords();

    // Tìm từ xuất hiện nhiều nhất
    string mostFrequentWord = wordList.findMostFrequentWord();
    cout << "\nMost frequent word: " << mostFrequentWord << endl;

    // Loại bỏ từ láy (các từ giống nhau)
    wordList.removeDuplicateConsecutiveWords();
    cout << "\nWords after removing consecutive duplicates:\n";
    wordList.printWords();

    // Đếm số từ vựng trong câu
    int uniqueWordCount = wordList.countUniqueWords();
    cout << "\nNumber of unique words: " << uniqueWordCount << endl;

    return 0;
}
