#include <iostream>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

struct File {
    string name;          // Tên file
    time_t created_time;  // Thời gian tạo file
    long size;            // Kích thước file (đơn vị byte)
};

struct Node {
    File file;       // Dữ liệu của file
    Node* next;      // Con trỏ tới node tiếp theo
};

class FileList {
private:
    Node* head;  // Con trỏ tới node đầu tiên

public:
    FileList() : head(nullptr) {}

    // Thêm một file vào danh sách, theo thứ tự thời gian 
    void insertFile(const File& file) {
        Node* newNode = new Node;
        newNode->file = file;
        newNode->next = nullptr;

        // Chèn file vào danh sách theo thứ tự thời gian 
        if (head == nullptr || difftime(file.created_time, head->file.created_time) > 0) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr && difftime(file.created_time, current->next->file.created_time) <= 0) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    // Hàm sao chép một file từ thư mục khác 
    void copyFile(const File& file) {
        insertFile(file);
        cout << "File copied: " << file.name << endl;
    }

    // Tính tổng kích thước của các file trong danh sách
    long calculateTotalSize() {
        long totalSize = 0;
        Node* current = head;
        while (current != nullptr) {
            totalSize += current->file.size;
            current = current->next;
        }
        return totalSize;
    }

    // Xóa các file có kích thước nhỏ nhất cho đến khi vừa với dung lượng ổ
    void freeUpSpace(long usbSize) {
        long totalSize = calculateTotalSize();
        cout << "Total size before freeing space: " << totalSize << " bytes" << endl;
        
        while (totalSize > usbSize && head != nullptr) {
            cout << "Removing file: " << head->file.name << ", size: " << head->file.size << " bytes" << endl;
            totalSize -= head->file.size;
            Node* temp = head;
            head = head->next;
            delete temp;
        }

        cout << "Total size after freeing space: " << totalSize << " bytes" << endl;
    }

    // In danh sách các file trong thư mục
    void printFiles() {
        Node* current = head;
        while (current != nullptr) {
            cout << "File name: " << current->file.name
                 << ", Size: " << current->file.size
                 << " bytes, Created: " << ctime(&current->file.created_time);
            current = current->next;
        }
    }

    ~FileList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

time_t convertToTimeT(const string& dateTimeStr) {
    struct tm tm = {};
    strptime(dateTimeStr.c_str(), "%Y-%m-%d %H:%M:%S", &tm);  
    // Dùng định dạng thời gian: yyyy-mm-dd hh:mm:ss
    return mktime(&tm);
}

int main() {
    FileList fileList;
    int n;

    cout << "Enter number of files: ";
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Làm sạch bộ đệm sau khi nhập số lượng file

    for (int i = 0; i < n; ++i) {
        File newFile;

        // Nhập tên file
        cout << "Enter name of file " << (i + 1) << ": ";
        getline(cin, newFile.name);

        // Nhập thời gian tạo file
        string dateTimeStr;
        cout << "Enter creation time of file (format: yyyy-mm-dd hh:mm:ss): ";
        getline(cin, dateTimeStr);
        newFile.created_time = convertToTimeT(dateTimeStr);

        // Nhập kích thước file
        cout << "Enter size of file (in bytes): ";
        cin >> newFile.size;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Làm sạch bộ đệm sau khi nhập kích thước

        // Sao chép file vào danh sách
        fileList.copyFile(newFile);
    }

    // In danh sách các file
    cout << "\nFile list in the folder:\n";
    fileList.printFiles();

    // Tính tổng kích thước của các file
    long totalSize = fileList.calculateTotalSize();
    cout << "Total size of files: " << totalSize << " bytes\n";

    // Thực hiện sao lưu vào ổ (32GB = 32 * 1024 * 1024 * 1024 bytes)
    long usbSize = 32L * 1024 * 1024 * 1024;
    fileList.freeUpSpace(usbSize);

    return 0;
}
