#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

const int MAX_BOOKS = 100;
const int MAX_USERS = 50;
const int MAX_TRANSACTIONS = 200;

struct Book {
    string isbn;           
    string title;          
    string author;        
    bool isAvailable;      
};

struct User {
    string id;           
    string name;           
    string contact;       
};

struct Transaction {
    int transactionNumber; 
    string userId;         
    string userName;       
    string isbn;           
    string bookTitle;      
    string borrowDate;     
    string returnDate;     
    bool isReturned;    
};

Book books[MAX_BOOKS];
User users[MAX_USERS];
Transaction transactions[MAX_TRANSACTIONS];

int totalBooks = 0;
int totalUsers = 0;
int totalTransactions = 0;

string getCurrentDateTime() {
    time_t now = time(0);
    string dt = ctime(&now);
    return dt;
}

int findBookByISBN(string isbn) {
    for (int i = 0; i < totalBooks; i++) {
        if (books[i].isbn == isbn) {
            return i;  
        }
    }
    return -1;  
}

int findUserByID(string id) {
    for (int i = 0; i < totalUsers; i++) {
        if (users[i].id == id) {
            return i;  
        }
    }
    return -1;  
}

string toLowerCase(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

void addNewBook() {
    if (totalBooks >= MAX_BOOKS) {
        cout << "\nSorry! Library is full. Cannot add more books.\n";
        return;
    }
    
    cout << "\n   ADD NEW BOOK \n\n";
    
    Book newBook;
    
    cout << "Enter ISBN: ";
    cin >> newBook.isbn;
    
    if (findBookByISBN(newBook.isbn) != -1) {
        cout << "Error: A book with this ISBN already exists!\n";
        return;
    }
    
    cin.ignore(); 
    cout << "Enter Title: ";
    getline(cin, newBook.title);
    
    cout << "Enter Author: ";
    getline(cin, newBook.author);
    
    newBook.isAvailable = true;  
    
    books[totalBooks] = newBook;
    totalBooks++;
    
    cout << "Success! Book added to library.\n";
}

void addNewUser() {
    if (totalUsers >= MAX_USERS) {
        cout << "\nSorry! Cannot add more users.\n";
        return;
    }
    
    cout << "\n   ADD NEW USER \n\n";
    
    User newUser;

    cout << "Enter User ID: ";
    cin >> newUser.id;
    
    if (findUserByID(newUser.id) != -1) {
        cout << "Error: A user with this ID already exists!\n";
        return;
    }
    
    cin.ignore();  
    cout << "Enter Name: ";
    getline(cin, newUser.name);
    
    cout << "Enter Contact: ";
    getline(cin, newUser.contact);
    
    users[totalUsers] = newUser;
    totalUsers++;
    
    cout << "Success! User registered.\n";
}

void showAllBooks() {
    if (totalBooks == 0) {
        cout << "\nNo books in the library.\n";
        return;
    }
    
    cout << "\n\t\t\tALL BOOKS \n\n";
    cout << "---------------------------------------------------------------\n";

    for (int i = 0; i < totalBooks; i++) {
        cout << "Book #" << (i + 1) << endl;
        cout << "  ISBN: " << books[i].isbn << endl;
        cout << "  Title: " << books[i].title << endl;
        cout << "  Author: " << books[i].author << endl;
        cout << "  Status: " << (books[i].isAvailable ? "Available" : "Borrowed") << endl;
        cout << "---------------------------------------------------------------\n";
    }
}

void showAllUsers() {
    if (totalUsers == 0) {
        cout << "\nNo users registered.\n";
        return;
    }
    
    cout << "\n\t\t\tALL USERS \n";
    cout << "---------------------------------------------------------------\n";
    
    for (int i = 0; i < totalUsers; i++) {
        cout << "User #" << (i + 1) << endl;
        cout << "  ID: " << users[i].id << endl;
        cout << "  Name: " << users[i].name << endl;
        cout << "  Contact: " << users[i].contact << endl;
        cout << "---------------------------------------------------------------\n";
    }
}

void borrowBook() {
    cout << "\n\t\tBORROW BOOK \n\n";
    
    string userId, isbn;
    
    cout << "Enter User ID: ";
    cin >> userId;
    
    int userPosition = findUserByID(userId);
    if (userPosition == -1) {
        cout << "Error: User not found!\n";
        return;
    }
    
    cout << "Enter Book ISBN: ";
    cin >> isbn;
    
    int bookPosition = findBookByISBN(isbn);
    if (bookPosition == -1) {
        cout << "Error: Book not found!\n";
        return;
    }
    
    if (!books[bookPosition].isAvailable) {
        cout << "Sorry! This book is already borrowed.\n";
        return;
    }
    
    if (totalTransactions >= MAX_TRANSACTIONS) {
        cout << "Error: Transaction limit reached!\n";
        return;
    }
    
    Transaction newTransaction;
    newTransaction.transactionNumber = totalTransactions + 1;
    newTransaction.userId = users[userPosition].id;
    newTransaction.userName = users[userPosition].name;
    newTransaction.isbn = books[bookPosition].isbn;
    newTransaction.bookTitle = books[bookPosition].title;
    newTransaction.borrowDate = getCurrentDateTime();
    newTransaction.returnDate = "";
    newTransaction.isReturned = false;
    
    books[bookPosition].isAvailable = false;
    
    transactions[totalTransactions] = newTransaction;
    totalTransactions++;
    
    cout << "Success! Book borrowed.\n";
    cout << "Transaction Number: " << newTransaction.transactionNumber << endl;
}

void returnBook() {
    cout << "\n\t\tRETURN BOOK\n\n";
    
    string isbn;
    
    cout << "Enter Book ISBN: ";
    cin >> isbn;
    
    int bookPosition = findBookByISBN(isbn);
    if (bookPosition == -1) {
        cout << "Error: Book not found!\n";
        return;
    }
    
    if (books[bookPosition].isAvailable) {
        cout << "This book is not borrowed!\n";
        return;
    }
    
    for (int i = totalTransactions - 1; i >= 0; i--) {
        if (transactions[i].isbn == isbn && !transactions[i].isReturned) {
            transactions[i].returnDate = getCurrentDateTime();
            transactions[i].isReturned = true;
            
            books[bookPosition].isAvailable = true;
            
            cout << "Success! Book returned.\n";
            cout << "Transaction Number: " << transactions[i].transactionNumber << endl;
            return;
        }
    }
    
    cout << "Error: No active borrow record found for this book!\n";
}

void searchBookByTitle() {
    string searchTitle;
    
    cout << "\n   SEARCH BY TITLE \n\n";
    cout << "Enter title to search: ";
    cin.ignore();
    getline(cin, searchTitle);
    
    string searchLower = toLowerCase(searchTitle);
    
    bool found = false;
    
    for (int i = 0; i < totalBooks; i++) {
    	string bookTitleLower = toLowerCase(books[i].title);
        if (bookTitleLower.find(searchLower) != string::npos) {
            if (!found) {
                cout << "\nSearch Results:\n";
                cout << "---------------------------------------------------------------\n";
                found = true;
            }
            
            cout << "  ISBN: " << books[i].isbn << endl;
            cout << "  Title: " << books[i].title << endl;
            cout << "  Author: " << books[i].author << endl;
            cout << "  Status: " << (books[i].isAvailable ? "Available" : "Borrowed") << endl;
            cout << "---------------------------------------------------------------\n";
        }
    }
    
    if (!found) {
        cout << "No books found with that title.\n";
    }
}

void searchBookByAuthor() {
    string searchAuthor;
    
    cout << "\n   SEARCH BY AUTHOR\n\n";
    cout << "Enter author name to search: ";
    cin.ignore();
    getline(cin, searchAuthor);
    
    string searchLower = toLowerCase(searchAuthor);
    
    bool found = false;
    
    for (int i = 0; i < totalBooks; i++) {
    	string authorLower = toLowerCase(books[i].author);
        if (authorLower.find(searchLower) != string::npos) {
            if (!found) {
                cout << "\nSearch Results:\n";
                cout << "---------------------------------------------------------------\n";
                found = true;
            }
            
            cout << "  ISBN: " << books[i].isbn << endl;
            cout << "  Title: " << books[i].title << endl;
            cout << "  Author: " << books[i].author << endl;
            cout << "  Status: " << (books[i].isAvailable ? "Available" : "Borrowed") << endl;
            cout << "---------------------------------------------------------------\n";
        }
    }
    
    if (!found) {
        cout << "No books found by that author.\n";
    }
}

void searchUserByName() {
    string searchName;
    
    cout << "\n  SEARCH USER BY NAME\n\n";
    cout << "Enter name to search: ";
    cin.ignore();
    getline(cin, searchName);
    
    string searchLower = toLowerCase(searchName);
    
    bool found = false;
    
    for (int i = 0; i < totalUsers; i++) {
    	string userNameLower = toLowerCase(users[i].name);
        if (userNameLower.find(searchLower) != string::npos) {
            if (!found) {
                cout << "\nSearch Results:\n";
                cout << "---------------------------------------------------------------\n";
                found = true;
            }
            
            cout << "  ID: " << users[i].id << endl;
            cout << "  Name: " << users[i].name << endl;
            cout << "  Contact: " << users[i].contact << endl;
            cout << "---------------------------------------------------------------\n";
        }
    }
    
    if (!found) {
        cout << "No users found with that name.\n";
    }
}

void showTransactionHistory() {
    if (totalTransactions == 0) {
        cout << "\nNo transactions recorded.\n";
        return;
    }
    
    cout << "\n\t\t  TRANSACTION HISTORY \n\n";
    cout << "---------------------------------------------------------------\n";
    
    for (int i = 0; i < totalTransactions; i++) {
    	if (transactions[i].transactionNumber == 0) {
            continue;
        }
        cout << "Transaction #" << transactions[i].transactionNumber << endl;
        cout << "  User ID: " << transactions[i].userId << endl;
        cout << "  User Name: " << transactions[i].userName << endl;
        cout << "  Book ISBN: " << transactions[i].isbn << endl;
        cout << "  Book Title: " << transactions[i].bookTitle << endl;
        cout << "  Borrow Date: " << transactions[i].borrowDate;
        cout << "  Return Date: " << (transactions[i].isReturned ? transactions[i].returnDate : "Not Returned\n");
        cout << "  Status: " << (transactions[i].isReturned ? "Returned" : "Active") << endl;
        cout << "---------------------------------------------------------------\n";
    }
}

void sortBooksByTitle() {
    for (int i = 0; i < totalBooks - 1; i++) {
        for (int j = 0; j < totalBooks - i - 1; j++) {
            if (books[j].title > books[j + 1].title) {
                Book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
    
    cout << "\nBooks sorted by title!\n";
    showAllBooks();
}

void sortBooksByAuthor() {
    for (int i = 0; i < totalBooks - 1; i++) {
        for (int j = 0; j < totalBooks - i - 1; j++) {
            if (books[j].author > books[j + 1].author) {
                Book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
    
    cout << "\nBooks sorted by author!\n";
    showAllBooks();
}

void saveDataToFile() {
    ofstream bookFile("books.txt");
    for (int i = 0; i < totalBooks; i++) {
        bookFile << books[i].isbn << "|" 
                 << books[i].title << "|" 
                 << books[i].author << "|" 
                 << books[i].isAvailable << endl;
    }
    bookFile.close();
    
    ofstream userFile("users.txt");
    for (int i = 0; i < totalUsers; i++) {
        userFile << users[i].id << "|" 
                 << users[i].name << "|" 
                 << users[i].contact << endl;
    }
    userFile.close();
    
    ofstream transFile("transactions.txt");
    for (int i = 0; i < totalTransactions; i++) {
        transFile << transactions[i].transactionNumber << "|"
                  << transactions[i].userId << "|"
                  << transactions[i].userName << "|"
                  << transactions[i].isbn << "|"
                  << transactions[i].bookTitle << "|"
                  << transactions[i].borrowDate << "|"
                  << transactions[i].returnDate << "|"
                  << transactions[i].isReturned << endl;
    }
    transFile.close();
    
    cout << "\nData saved successfully!\n";
}

void loadDataFromFile() {
    ifstream bookFile("books.txt");
    if (bookFile.is_open()) {
        totalBooks = 0;
        string line;
        
        while (getline(bookFile, line)) {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);
            
            books[totalBooks].isbn = line.substr(0, pos1);
            books[totalBooks].title = line.substr(pos1 + 1, pos2 - pos1 - 1);
            books[totalBooks].author = line.substr(pos2 + 1, pos3 - pos2 - 1);
            books[totalBooks].isAvailable = (line.substr(pos3 + 1) == "1");
            
            totalBooks++;
        }
        bookFile.close();
    }
    
    ifstream userFile("users.txt");
    if (userFile.is_open()) {
        totalUsers = 0;
        string line;
        
        while (getline(userFile, line)) {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            
            users[totalUsers].id = line.substr(0, pos1);
            users[totalUsers].name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            users[totalUsers].contact = line.substr(pos2 + 1);
            
            totalUsers++;
        }
        userFile.close();
    }
    
    ifstream transFile("transactions.txt");
    if (transFile.is_open()) {
        totalTransactions = 0;
        string line;
        
        while (getline(transFile, line)) {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);
            size_t pos4 = line.find('|', pos3 + 1);
            size_t pos5 = line.find('|', pos4 + 1);
            size_t pos6 = line.find('|', pos5 + 1);
            size_t pos7 = line.find('|', pos6 + 1);
            
            transactions[totalTransactions].transactionNumber = atoi(line.substr(0, pos1).c_str());
            transactions[totalTransactions].userId = line.substr(pos1 + 1, pos2 - pos1 - 1);
            transactions[totalTransactions].userName = line.substr(pos2 + 1, pos3 - pos2 - 1);
            transactions[totalTransactions].isbn = line.substr(pos3 + 1, pos4 - pos3 - 1);
            transactions[totalTransactions].bookTitle = line.substr(pos4 + 1, pos5 - pos4 - 1);
            transactions[totalTransactions].borrowDate = line.substr(pos5 + 1, pos6 - pos5 - 1);
            transactions[totalTransactions].returnDate = line.substr(pos6 + 1, pos7 - pos6 - 1);
            transactions[totalTransactions].isReturned = (line.substr(pos7 + 1) == "1");
            
            totalTransactions++;
        }
        transFile.close();
    }
    
    cout << "\nData loaded successfully!\n";
}

void showMenu() {
    cout << "\n    LIBRARY MANAGEMENT SYSTEM\n\n";
    cout << "1.  Add New Book\n";
    cout << "2.  Add New User\n";
    cout << "3.  Show All Books\n";
    cout << "4.  Show All Users\n";
    cout << "5.  Borrow a Book\n";
    cout << "6.  Return a Book\n";
    cout << "7.  Search Book by Title\n";
    cout << "8.  Search Book by Author\n";
    cout << "9.  Search User by Name\n";
    cout << "10. Show Transaction History\n";
    cout << "11. Sort Books by Title\n";
    cout << "12. Sort Books by Author\n";
    cout << "13. Save Data to File\n";
    cout << "14. Load Data from File\n";
    cout << "0.  Exit\n";
    cout << "\n\n";
    cout << "Enter your choice: ";
}

int main() {
    int choice;
    
    cout << "\nWelcome to Library Management System!\n";
    
    loadDataFromFile();
    
    do {
        showMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                addNewBook();
                break;
            case 2:
                addNewUser();
                break;
            case 3:
                showAllBooks();
                break;
            case 4:
                showAllUsers();
                break;
            case 5:
                borrowBook();
                break;
            case 6:
                returnBook();
                break;
            case 7:
                searchBookByTitle();
                break;
            case 8:
                searchBookByAuthor();
                break;
            case 9:
                searchUserByName();
                break;
            case 10:
                showTransactionHistory();
                break;
            case 11:
                sortBooksByTitle();
                break;
            case 12:
                sortBooksByAuthor();
                break;
            case 13:
                saveDataToFile();
                break;
            case 14:
                loadDataFromFile();
                break;
            case 0:
                cout << "\nThank you for using Library Management System!\n";
                saveDataToFile();  
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
        
    } while (choice != 0);
    
    return 0;
}
