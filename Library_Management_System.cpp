#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <string>

class Book {
public:
    std::string title;
    std::string author;
    std::string ISBN;
    bool available;

    Book(std::string t, std::string a, std::string i) : title(t), author(a), ISBN(i), available(true) {}
};

class Borrower {
public:
    std::string name;
    int id;

    Borrower(std::string n, int i) : name(n), id(i) {}
};

class Transaction {
public:
    int borrowerId;
    std::string bookISBN;
    time_t checkoutDate;
    time_t returnDate;

    Transaction(int bId, std::string bISBN) : borrowerId(bId), bookISBN(bISBN), checkoutDate(time(0)), returnDate(0) {}
};

class Library {
private:
    std::vector<Book> books;
    std::unordered_map<int, Borrower> borrowers;
    std::vector<Transaction> transactions;
    double finePerDay;

public:
    Library(double fine) : finePerDay(fine) {}

    void addBook(const std::string& title, const std::string& author, const std::string& ISBN) {
        books.emplace_back(title, author, ISBN);
        std::cout << "Book added: " << title << "\n";
    }

    void addBorrower(const std::string& name, int id) {
        borrowers.emplace(id, Borrower(name, id));
        std::cout << "Borrower added: " << name << "\n";
    }

    void searchBook(const std::string& keyword) {
        for (const auto& book : books) {
            if (book.title.find(keyword) != std::string::npos || book.author.find(keyword) != std::string::npos || book.ISBN.find(keyword) != std::string::npos) {
                std::cout << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.ISBN << ", Available: " << (book.available ? "Yes" : "No") << "\n";
            }
        }
    }

    void checkoutBook(int borrowerId, const std::string& ISBN) {
        for (auto& book : books) {
            if (book.ISBN == ISBN && book.available) {
                book.available = false;
                transactions.emplace_back(borrowerId, ISBN);
                std::cout << "Book checked out successfully.\n";
                return;
            }
        }
        std::cout << "Book not available for checkout.\n";
    }

    void returnBook(int borrowerId, const std::string& ISBN) {
        for (auto& book : books) {
            if (book.ISBN == ISBN && !book.available) {
                book.available = true;
                for (auto& transaction : transactions) {
                    if (transaction.borrowerId == borrowerId && transaction.bookISBN == ISBN && transaction.returnDate == 0) {
                        transaction.returnDate = time(0);
                        double fine = calculateFine(transaction.checkoutDate, transaction.returnDate);
                        if (fine > 0) {
                            std::cout << "Book returned with a fine of $" << fine << ".\n";
                        } else {
                            std::cout << "Book returned successfully.\n";
                        }
                        return;
                    }
                }
            }
        }
        std::cout << "Book not found or not checked out.\n";
    }

    double calculateFine(time_t checkoutDate, time_t returnDate) {
        double days = difftime(returnDate, checkoutDate) / (60 * 60 * 24);
        return days > 14 ? (days - 14) * finePerDay : 0;
    }
};

int main() {
    Library library(1.0);
    int choice;
    std::string title, author, ISBN, name;
    int id;

    while (true) {
        std::cout << "1. Add Book\n2. Add Borrower\n3. Search Book\n4. Checkout Book\n5. Return Book\n6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                std::cout << "Enter book title: ";
                std::getline(std::cin, title);
                std::cout << "Enter book author: ";
                std::getline(std::cin, author);
                std::cout << "Enter book ISBN: ";
                std::getline(std::cin, ISBN);
                library.addBook(title, author, ISBN);
                break;
            case 2:
                std::cout << "Enter borrower name: ";
                std::getline(std::cin, name);
                std::cout << "Enter borrower ID: ";
                std::cin >> id;
                library.addBorrower(name, id);
                break;
            case 3:
                std::cout << "Enter search keyword: ";
                std::getline(std::cin, title);
                library.searchBook(title);
                break;
            case 4:
                std::cout << "Enter borrower ID: ";
                std::cin >> id;
                std::cout << "Enter book ISBN: ";
                std::cin.ignore();
                std::getline(std::cin, ISBN);
                library.checkoutBook(id, ISBN);
                break;
            case 5:
                std::cout << "Enter borrower ID: ";
                std::cin >> id;
                std::cout << "Enter book ISBN: ";
                std::cin.ignore();
                std::getline(std::cin, ISBN);
                library.returnBook(id, ISBN);
                break;
            case 6:
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}
