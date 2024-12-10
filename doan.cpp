#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
using namespace std::chrono;
using namespace std;


struct ShowInfo {
    int id;              // STT
    string title;        // Ten phim
    int rank;            // Xep hang
    string type;         // Loai 
    int episodes;        // So tap
    string aired;        // Thoi gian phat song 
    float score;         // Diem so (thang diem 10)
};

struct Node {
    ShowInfo data;
    Node* pNext;
};

// Hàm thêm một đối tượng ShowInfo vào cuối danh sách liên kết
void addShowToEnd(Node*& head, const ShowInfo& show) {
    Node* newNode = new Node;
    newNode->data = show;
    newNode->pNext = nullptr;

    if (head == nullptr) {
        head = newNode;
    }
    else {
        Node* temp = head;
        while (temp->pNext != nullptr) {
            temp = temp->pNext;
        }
        temp->pNext = newNode;
    }
}
void clearList(Node*& head) {
    Node* temp;
    while (head != nullptr) {
        temp = head;
        head = head->pNext;
        delete temp;
    }
    head = nullptr;  // Đảm bảo danh sách trống
}
//Hàm đọc dữ liệu từ file
void readFromCSV(const string& filename, Node*& head) {
    // Giải phóng danh sách cũ trước khi đọc dữ liệu mới
    clearList(head);

    head = nullptr;  // Đảm bảo danh sách trống

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Khong the mo tep CSV!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        ShowInfo show;

        // Tách và đọc các trường trong CSV
        ss >> show.id;
        ss.ignore();  // Bỏ qua dấu phẩy
        getline(ss, show.title, ',');
        ss >> show.rank;
        ss.ignore();
        getline(ss, show.type, ',');
        ss >> show.episodes;
        ss.ignore();
        getline(ss, show.aired, ',');
        ss >> show.score;

        // Thêm đối tượng ShowInfo vào cuối danh sách liên kết
        addShowToEnd(head, show);
    }

    cout << "Da tai du lieu tu file: " << filename << endl;
    file.close();
}
//hàm làm mới danh sách
void freeList(Node*& head) {
    Node* temp;
    while (head != nullptr) {
        temp = head;
        head = head->pNext;
        delete temp;
    }
    head = nullptr;  // Đảm bảo danh sách trống
}
//Hàm hiển thị có phân trang
void displayPaginated(Node* head, int itemsPerPage) {
    // Đếm tổng số mục
    int totalItems = 0;
    Node* temp = head;

    while (temp != nullptr) {
        totalItems++;
        temp = temp->pNext;
    }

    // Nếu không có dữ liệu, thông báo và thoát sớm
    if (totalItems == 0) {
        cout << "Khong co ket qua de hien thi." << endl;
        return;
    }

    // Tính tổng số trang
    int totalPages = (totalItems + itemsPerPage - 1) / itemsPerPage;

    // Hiển thị tổng số dữ liệu và số trang
    cout << "\nTong so ket qua: " << totalItems << endl;
    cout << "Moi trang hien thi: " << itemsPerPage << " muc." << endl;
    cout << "Tong so trang: " << totalPages << endl;

    int currentPage = 1; // Bắt đầu từ trang đầu tiên
    while (true) {
        // Xác định phạm vi dữ liệu trên trang hiện tại
        int startIndex = (currentPage - 1) * itemsPerPage;
        int endIndex = min(startIndex + itemsPerPage, totalItems);

        // Hiển thị các mục trên trang hiện tại
        temp = head;
        int currentIndex = 0;
        cout << "\n========== Trang " << currentPage << " ==========" << endl;
        while (temp != nullptr) {
            if (currentIndex >= startIndex && currentIndex < endIndex) {
                cout << "ID: " << temp->data.id << endl;
                cout << "Title: " << temp->data.title << endl;
                cout << "Rank: " << temp->data.rank << endl;
                cout << "Type: " << temp->data.type << endl;
                cout << "Episodes: " << temp->data.episodes << endl;
                cout << "Aired: " << temp->data.aired << endl;
                cout << "Score: " << temp->data.score << endl;
                cout << "------------------------" << endl;
            }
            currentIndex++;
            temp = temp->pNext;
        }

        // Hiển thị menu điều hướng
        cout << "=====================" << endl;
        cout << "Trang " << currentPage << "/" << totalPages << endl;
        cout << "Nhap 'n' de xem trang tiep theo, 'p' de quay lai trang truoc, 'g' de den trang cu the, 'q' de thoat." << endl;
        string choice;
        cin >> choice;

        if (choice == "n" && currentPage < totalPages) {
            currentPage++;
        }
        else if (choice == "p" && currentPage > 1) {
            currentPage--;
        }
        else if (choice == "g") {
            int targetPage;
            cout << "Nhap so trang (1-" << totalPages << "): ";
            cin >> targetPage;
            if (targetPage >= 1 && targetPage <= totalPages) {
                currentPage = targetPage;
            }
            else {
                cout << "So trang khong hop le. Vui long thu lai." << endl;
            }
        }
        else if (choice == "q") {
            break;
        }
        else {
            cout << "Lua chon khong hop le. Vui long thu lai." << endl;
        }
    }
}
//Hàm tìm kiếm theo tiêu chí
Node* searchByCriteria(Node* head, const string& criteria, const string& value) {
    Node* resultHead = nullptr;
    Node* resultTail = nullptr;
    int count = 0; // Biến đếm số lượng kết quả

    Node* temp = head;
    while (temp != nullptr) {
        bool match = false;

        // Kiểm tra theo tiêu chí
        if (criteria == "title" && temp->data.title.find(value) != string::npos) {
            match = true;
        }
        else if (criteria == "aired" && temp->data.aired.find(value) != string::npos) {
            match = true;
        }
        else if (criteria == "id" && to_string(temp->data.id) == value) {
            match = true;
        }
        else if (criteria == "type" && temp->data.type.find(value) != string::npos) {
            match = true;
        }
        else if (criteria == "episodes" && to_string(temp->data.episodes) == value) {
            match = true;
        }
        else if (criteria == "score" && to_string(temp->data.score) == value) {
            match = true;
        }

        // Nếu có kết quả khớp, thêm vào danh sách kết quả
        if (match) {
            Node* newNode = new Node;
            newNode->data = temp->data;
            newNode->pNext = nullptr;

            if (resultHead == nullptr) {
                resultHead = newNode;
                resultTail = newNode;
            }
            else {
                resultTail->pNext = newNode;
                resultTail = newNode;
            }
            count++; // Tăng biến đếm
        }
        temp = temp->pNext;
    }

    // Hiển thị số lượng kết quả
    cout << "Tim thay " << count << " ket qua voi tieu chi \"" << criteria << "\" gia tri \"" << value << "\"." << endl;

    return resultHead;
}

//hàm ghi dữ liệu vào file
void writeListToCSV(Node* head, const string& filename) {
    ofstream file(filename, ios::out); // Mở file ở chế độ ghi đè
    if (!file.is_open()) {
        cerr << "Khong the mo tep CSV de ghi lai!" << endl;
        return;
    }

    Node* temp = head;
    while (temp != nullptr) {
        file << temp->data.id << ","
            << temp->data.title << ","
            << temp->data.rank << ","
            << temp->data.type << ","
            << temp->data.episodes << ","
            << temp->data.aired << ","
            << temp->data.score << endl;
        temp = temp->pNext;
    }

    file.close();
    cout << "Da ghi du lieu vao file CSV: " << filename << endl;
}

// Hàm xóa chương trình theo ID và cập nhật lại tệp CSV
void deleteShowByID(Node*& head, int id, const string& filename) {
    Node* temp = head;
    Node* prev = nullptr;

    // Nếu chương trình đầu tiên cần xóa
    if (temp != nullptr && temp->data.id == id) {
        head = temp->pNext;
        delete temp;
        cout << "Da xoa chuong trinh co ID: " << id << endl;
        writeListToCSV(head, filename);  // Cập nhật lại tệp CSV sau khi xóa
        return;
    }

    // Tìm kiếm chương trình cần xóa
    while (temp != nullptr && temp->data.id != id) {
        prev = temp;
        temp = temp->pNext;
    }

    // Nếu không tìm thấy chương trình
    if (temp == nullptr) {
        cout << "Khong tim thay chuong trinh co ID: " << id << endl;
        return;
    }

    // Xóa chương trình
    prev->pNext = temp->pNext;
    delete temp;
    cout << "Da xoa chuong trinh co ID: " << id << endl;
    writeListToCSV(head, "animerank.csv");
}

// Hàm kiểm tra ID trùng lặp trong danh sách
bool isDuplicateID(Node* head, int id) {
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data.id == id) {
            return true; // Trùng ID
        }
        temp = temp->pNext;
    }
    return false; // Không trùng
}

void addNewShow(Node*& head) {
    ShowInfo newShow;

    // Nhập ID và kiểm tra trùng lặp
    while (true) {
        cout << "Nhap ID: ";
        cin >> newShow.id;
        if (cin.fail() || newShow.id <= 0) {
            cout << "ID phai la so nguyen duong. Vui long thu lai!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (isDuplicateID(head, newShow.id)) {
            cout << "ID da ton tai trong danh sach. Vui long nhap ID khac!" << endl;
        }
        else {
            break;
        }
    }
    cin.ignore();

    // Nhập tên phim
    while (true) {
        cout << "Nhap ten phim: ";
        getline(cin, newShow.title);
        if (newShow.title.empty()) {
            cout << "Ten phim khong duoc de trong. Vui long thu lai!" << endl;
        }
        else {
            break;
        }
    }

    // Nhập xếp hạng
    while (true) {
        cout << "Nhap xep hang (1-100000): ";
        cin >> newShow.rank;
        if (cin.fail() || newShow.rank <= 0 || newShow.rank > 100000) {
            cout << "Xep hang phai la so nguyen trong khoang 1-100000. Vui long thu lai!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }
    cin.ignore();

    // Nhập loại
    while (true) {
        cout << "Nhap loai: ";
        getline(cin, newShow.type);
        if (newShow.type.empty()) {
            cout << "Loai khong duoc de trong. Vui long thu lai!" << endl;
        }
        else {
            break;
        }
    }

    // Nhập số tập
    while (true) {
        cout << "Nhap so tap (>= 1): ";
        cin >> newShow.episodes;
        if (cin.fail() || newShow.episodes <= 0) {
            cout << "So tap phai la so nguyen duong. Vui long thu lai!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }
    cin.ignore();

    // Nhập thời gian phát sóng
    while (true) {
        cout << "Nhap thoi gian phat song (VD: 2022-04-01): ";
        getline(cin, newShow.aired);
        if (newShow.aired.empty()) {
            cout << "Thoi gian phat song khong duoc de trong. Vui long thu lai!" << endl;
        }
        else {
            break;
        }
    }

    // Nhập điểm số
    while (true) {
        cout << "Nhap diem so (0.0 - 10.0): ";
        cin >> newShow.score;
        if (cin.fail() || newShow.score < 0.0 || newShow.score > 10.0) {
            cout << "Diem so phai la so thuc trong khoang 0.0 - 10.0. Vui long thu lai!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }

    // Thêm chương trình mới vào cuối danh sách
    addShowToEnd(head, newShow);
    cout << "Chuong trinh da duoc them thanh cong!" << endl;

    // Ghi danh sách mới vào CSV mà không ghi đè dữ liệu cũ
    writeListToCSV(head, "animerank.csv");
}


// Hàm lọc danh sách theo nhiều tiêu chí
Node* locTheoNhieuTieuChi(Node* head, const string& ten = "", const string& ngay = "", int soThuTu = -1,
    const string& loai = "", int soTap = -1, float diem = -1) {
    Node* ketQuaHead = nullptr;
    Node* ketQuaTail = nullptr;

    Node* temp = head;
    while (temp != nullptr) {
        bool thoaMan = true;

        // Kiem tra tung tieu chi
        if (!ten.empty() && temp->data.title.find(ten) == string::npos) thoaMan = false;
        if (!ngay.empty() && temp->data.aired != ngay) thoaMan = false;
        if (soThuTu != -1 && temp->data.id != soThuTu) thoaMan = false;
        if (!loai.empty() && temp->data.type != loai) thoaMan = false;
        if (soTap != -1 && temp->data.episodes != soTap) thoaMan = false;
       

        if (thoaMan) {
            Node* newNode = new Node;
            newNode->data = temp->data;
            newNode->pNext = nullptr;

            if (ketQuaHead == nullptr) {
                ketQuaHead = newNode;
                ketQuaTail = newNode;
            }
            else {
                ketQuaTail->pNext = newNode;
                ketQuaTail = newNode;
            }
        }
        temp = temp->pNext;
    }

    return ketQuaHead;
}

// Hàm sửa thông tin chương trình theo ID
void updateShow(Node* head, int id) {
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data.id == id) {
            cout << "Nhap ten phim moi: ";
            cin.ignore();
            getline(cin, temp->data.title);
            cout << "Nhap xep hang moi: ";
            cin >> temp->data.rank;
            cin.ignore();
            cout << "Nhap loai moi: ";
            getline(cin, temp->data.type);
            cout << "Nhap so tap moi: ";
            cin >> temp->data.episodes;
            cin.ignore();
            cout << "Nhap thoi gian phat song moi: ";
            getline(cin, temp->data.aired);
            cout << "Nhap diem so moi: ";
            cin >> temp->data.score;
            cout << "Thong tin chuong trinh da duoc cap nhat!" << endl;
            writeListToCSV(head, "animerank.csv");
            return;
        }
        temp = temp->pNext;
    }
    cout << "Khong tim thay chuong trinh co ID: " << id << endl;
}
// Ve bieu do bieu dien du lieu
void displayBarChart(Node* head) {
    if (head == nullptr) {
        cout << "Danh sach rong!" << endl;
        return;
    }

    // Tính toán max điểm số để chuẩn hóa tỷ lệ
    float maxScore = 0;
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data.score > maxScore) {
            maxScore = temp->data.score;
        }
        temp = temp->pNext;
    }

    // Vẽ biểu đồ cột
    temp = head;
    while (temp != nullptr) {
        int barHeight = static_cast<int>(temp->data.score / maxScore * 30); // Chuẩn hóa với chiều cao tối đa 30
        cout << left<<setw(5) << temp->data.id << " | " << setw(95) << temp->data.title
            << " | Diem: " << fixed << setprecision(1) << temp->data.score;
        // Vẽ dấu sao trên cùng một dòng
        for (int i = 0; i < barHeight; ++i) {
            cout << "*";
        }
        cout << endl;
        temp = temp->pNext;
    }
}

// Hàm sắp xếp bằng bubbleSort (tăng dần hoặc giảm dần)
void bubbleSort(Node*& head, bool ascending) {
    bool swapped;
    Node* temp;
    Node* last = nullptr;

    if (head == nullptr) return;

    do {
        swapped = false;
        temp = head;
        while (temp->pNext != last) {
            if ((ascending && temp->data.id > temp->pNext->data.id) ||
                (!ascending && temp->data.id < temp->pNext->data.id)) {
                swap(temp->data, temp->pNext->data);
                swapped = true;
            }
            temp = temp->pNext;
        }
        last = temp;
    } while (swapped);
}

// Hàm sắp xếp selectionSort (tăng dần hoặc giảm dần)
void selectionSort(Node*& head, bool ascending) {
    Node* temp = head;
    Node* minNode = nullptr;
    Node* it = nullptr;

    while (temp != nullptr) {
        minNode = temp;
        it = temp->pNext;

        while (it != nullptr) {
            if ((ascending && it->data.score < minNode->data.score) ||
                (!ascending && it->data.score > minNode->data.score)) {
                minNode = it;
            }
            it = it->pNext;
        }

        if (minNode != temp) {
            swap(minNode->data, temp->data);
        }

        temp = temp->pNext;
    }
}

// Hàm sắp xếp insertionSort(tăng dần hoặc giảm dần)
void insertionSort(Node*& head, bool ascending) {
    if (head == nullptr) return;

    Node* sorted = nullptr;
    Node* current = head;

    while (current != nullptr) {
        Node* next = current->pNext;
        if (sorted == nullptr || 
            (ascending && sorted->data.title >= current->data.title) || 
            (!ascending && sorted->data.title <= current->data.title)) {
            current->pNext = sorted;
            sorted = current;
        }
        else {
            Node* temp = sorted;
            while (temp->pNext != nullptr && 
                   ((ascending && temp->pNext->data.title < current->data.title) || 
                    (!ascending && temp->pNext->data.title > current->data.title))) {
                temp = temp->pNext;
            }
            current->pNext = temp->pNext;
            temp->pNext = current;
        }
        current = next;
    }
    head = sorted;
}
void sortList(Node*& head) {
    int choice, orderChoice;
    bool ascending;

    cout << "Chon Loai Sap Xep:" << endl;
    cout << "1. Sap xep theo ID" << endl;
    cout << "2. Sap xep theo diem so" << endl;
    cout << "3. Sap xep theo tieu de" << endl;
    cout << "Nhap lua chon (1-3): ";
    cin >> choice;

    if (choice < 1 || choice > 3) {  // Đúng logic của loại sắp xếp
        cout << "Lua chon khong hop le!" << endl;
        return;
    }

    cout << "Chon thu tu sap xep:" << endl;
    cout << "1. Tu thap den cao (Tang dan)" << endl;
    cout << "2. Tu cao den thap (Giam dan)" << endl;
    cout << "Nhap lua chon (1-2): ";
    cin >> orderChoice;

    if (orderChoice == 1) {
        ascending = true;
    }
    else if (orderChoice == 2) {
        ascending = false;
    }
    else {
        cout << "Lua chon khong hop le!!" << endl;
        return;
    }

    switch (choice) {
    case 1:
        bubbleSort(head, ascending);
        cout << "Danh sach da duoc sap xep theo ID!" << endl;
        break;
    case 2:
        selectionSort(head, ascending);
        cout << "Danh sach da duoc sap xep theo diem so!" << endl;
        break;
    case 3:
        insertionSort(head, ascending);
        cout << "Danh sach da duoc sap xep theo tieu de!" << endl;
        break;
    default:
        cout << "Lua chon khong hop le!" << endl;
    }
}

//Chương trình chính
int main() {
    Node* showList = nullptr;
    int choice;
    const string filename = "animerank.csv";  // Đặt tên tệp CSV
    int itemsPerPage = 5;  // Mặc định 5 mục trên mỗi trang

    do {
        cout << "\n========== MENU ==========" << endl;
        cout << "1. Tai du lieu tu file CSV" << endl;
        cout << "2. Hien thi danh sach chuong trinh (phan trang)" << endl;
        cout << "3. Tim kiem chuong trinh theo cac tieu chi" << endl;
        cout << "4. Them chuong trinh moi" << endl;
        cout << "5. Sua thong tin chuong trinh theo ID" << endl;
        cout << "6. Xoa chuong trinh theo ID" << endl;
        cout << "7. Loc chuong trinh cac tieu chi" << endl;
        cout << "8. Bieu Do bieu dien du lieu theo diem so" << endl;
        cout << "9. Sap xep kieu Bubble Sort" << endl;
        cout << "10. Sap xep kieu Selection Sort" << endl;
        cout << "11. Sap xep kieu Insertion Sort" << endl;
        cout << "12. Thoat" << endl;
        cout << "Nhap lua chon (1 - 12): ";
        cin >> choice;

       
        switch (choice) {
        case 1:
            {
                // Đo thời gian thực thi khi đọc dữ liệu từ CSV
                auto start = high_resolution_clock::now();
                readFromCSV(filename, showList);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "\nThoi gian tai du lieu tu file CSV: " << duration.count() << " microseconds\n";
            }
            break;

        case 2:
            {
                // Đo thời gian thực thi khi hiển thị danh sách theo phân trang
                auto start = high_resolution_clock::now();
                displayPaginated(showList, itemsPerPage);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "\nThoi gian hien thi danh sach chuong trinh (phan trang): " << duration.count() << " microseconds\n";
            }
            break;

        case 3: {
            string criteria[] = { "title", "aired", "id", "type", "episodes" };
            string value;
            bool backToMenu = false;

            do {
                cout << "\nChon tieu chi tim kiem:" << endl;
                cout << "1. Tieu de" << endl;
                cout << "2. Ngay len song" << endl;
                cout << "3. So thu tu (ID)" << endl;
                cout << "4. Loai" << endl;
                cout << "5. So tap" << endl;
                cout << "0. Quay ve menu chinh" << endl;
                cout << "Nhap lua chon (0-5): ";
                int subChoice;
                cin >> subChoice;

                if (subChoice == 0) {
                    backToMenu = true;
                    break;
                }
                else if (subChoice < 1 || subChoice > 5) {
                    cout << "Lua chon khong hop le, vui long thu lai!" << endl;
                    continue;
                }

                cout << "Nhap gia tri can tim: ";
                cin.ignore();
                getline(cin, value);

                auto start = high_resolution_clock::now();
                Node* searchResult = searchByCriteria(showList, criteria[subChoice - 1], value);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);

                if (searchResult == nullptr) {
                    cout << "Khong tim thay ket qua!" << endl;
                }
                else {
                    displayPaginated(searchResult, itemsPerPage);
                    freeList(searchResult);
                }
                cout << "Thoi gian tim kiem: " << duration.count() << " microseconds\n";
            } while (!backToMenu);
        }
              break;


        case 4: 
            {
                // Đo thời gian thực thi khi thêm chương trình mới
                auto start = high_resolution_clock::now();
                addNewShow(showList);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "\nThoi gian them chuong trinh moi: " << duration.count() << " microseconds\n";
            }
            break;

        case 5:
            {
                int id;
                cout << "Nhap ID chuong trinh can sua (hoac nhap -1 de quay ve menu): ";
                cin >> id;
                if (id == -1) break;
                auto start = high_resolution_clock::now();
                updateShow(showList, id);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "\nThoi gian sua thong tin chuong trinh: " << duration.count() << " microseconds\n";
            }
            break;

        case 6:
            {
                int id;
                cout << "Nhap ID chuong trinh can xoa (hoac nhap -1 de quay ve menu): ";
                cin >> id;
                if (id == -1) break;
                auto start = high_resolution_clock::now();
                deleteShowByID(showList, id, filename);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "\nThoi gian xoa chuong trinh: " << duration.count() << " microseconds\n";
            }
            break;

        case 7:
            {
                // Đo thời gian thực thi khi lọc chương trình theo nhiều tiêu chí
                string ten, ngay, loai;
                int soThuTu = -1, soTap = -1;
                float diem = -1;
                char confirm;

                cout << "Ban co muon thuc hien loc chuong trinh? (y/n): ";
                cin >> confirm;
                if (tolower(confirm) != 'y') {
                    cout << "Quay ve menu chinh..." << endl;
                    break; // Quay về menu
                }

                cin.ignore(); // Xóa ký tự xuống dòng còn sót
                cout << "Nhap ten (bo trong neu khong loc theo tieu chi nay): ";
                getline(cin, ten);

                cout << "Nhap ngay (bo trong neu khong loc theo tieu chi nay): ";
                getline(cin, ngay);

                cout << "Nhap so thu tu (-1 neu khong loc theo tieu chi nay): ";
                cin >> soThuTu;

                cout << "Nhap loai (bo trong neu khong loc theo tieu chi nay): ";
                cin.ignore(); // Xóa ký tự xuống dòng còn sót
                getline(cin, loai);

                cout << "Nhap so tap (-1 neu khong loc theo tieu chi nay): ";
                cin >> soTap;

                cout << "Nhap diem (-1 neu khong loc theo tieu chi nay): ";
                cin >> diem;

                auto start = high_resolution_clock::now();
                Node* danhSachLoc = locTheoNhieuTieuChi(showList, ten, ngay, soThuTu, loai, soTap, diem);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);

                if (danhSachLoc == nullptr) {
                    cout << "Khong co chuong trinh nao thoa man cac tieu chi loc!" << endl;
                }
                else {
                    cout << "Danh sach cac chuong trinh thoa man tieu chi loc:\n";
                    displayPaginated(danhSachLoc, itemsPerPage);
                    freeList(danhSachLoc);
                }
                cout << "\nThoi gian loc chuong trinh: " << duration.count() << " microseconds\n";
            }
            break;

        case 8:
            {
                // Đo thời gian thực thi khi hiển thị biểu đồ
                auto start = high_resolution_clock::now();
                displayBarChart(showList);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "\nThoi gian hien thi bieu do: " << duration.count() << " microseconds\n";
            }
            break;

        case 9:
        case 10:
        case 11:
            {
                // Đo thời gian sắp xếp danh sách
                auto start = high_resolution_clock::now();
                sortList(showList);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "\nThoi gian sap xep danh sach: " << duration.count() << " microseconds\n";
            }
            break;

        case 12:
            freeList(showList);
            cout << "Ket thuc chuong trinh." << endl;
            break;

        default:
            cout << "Lua chon khong hop le, vui long nhap lai!" << endl;
        }
    } while (choice != 12);

    return 0;
}
