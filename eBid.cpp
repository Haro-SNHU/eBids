#include <iostream>
#include <climits>
#include <time.h>
#include <string>
#include <algorithm>
#include "CSVparser.hpp"
#include "CSVparser.cpp"

using namespace std;

double strToDouble(string str, char ch);

struct Bid {
    string bidId;
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

struct Node {
    Bid current;
    Node* idLeft;
    Node* idRight;
    Node* amountParent;
    Node* amountLeft;
    Node* amountRight;

    Node(Bid bid) {
        current = bid;
        idLeft = NULL;
        idRight = NULL;
        amountParent = NULL;
        amountLeft = NULL;
        amountRight = NULL;
    }

    ~Node() {
        if (idLeft) {
            delete idLeft;
        }
        if (idRight) {
            delete idRight;
        }
        if (amountParent) {
            delete amountParent;
        }
        if (amountLeft) {
            delete amountLeft;
        }
        if (amountRight) {
            delete amountRight;
        }
    }
};

void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

class BinarySearchTree {
    private:
        Node* root;
        Node* amountRoot;
        void inIdOrder(Node* node);
        void inAmountOrder(Node* node);
        void insertId(Bid bid, Node* createdNode);
        void insertAmount(Bid bid, Node* createdNode);
        void removeNode(Node* rootNode, string removeBidId);
        void amountSearch(Node* node, double lowAmount, double highAmount);

    public:
        BinarySearchTree();
        virtual ~BinarySearchTree();
        void InIdOrder();
        void InAmountOrder();
        void Insert(Bid bid);
        void Remove(string bidId);
        Bid IdSearch(string bidId);
        void AmountSearch(double lowAmount, double highAmount);
};

BinarySearchTree::BinarySearchTree() {
    root = NULL;
    amountRoot = NULL;
}

BinarySearchTree::~BinarySearchTree() {
    if (this->root) {
        delete root;
        delete amountRoot;
    }
}

void BinarySearchTree::InIdOrder() {
    this->inIdOrder(this->root);
}

void BinarySearchTree::InAmountOrder() {
    this->inAmountOrder(this->root);
}

void BinarySearchTree::Insert(Bid bid) {
    Node* createdNode = new Node(bid);
    insertId(bid, createdNode);
    insertAmount(bid, createdNode);
}

void BinarySearchTree::Remove(string bidId) {
    removeNode(this->root, bidId);
}

Bid BinarySearchTree::IdSearch(string bidId) {
    Bid bid;
    Node* current = this->root;

    while (current) {
        if (current->current.bidId == bidId) {
            bid = current->current;
            break;
        }
        if (bidId < current->current.bidId) {
            current = current->idLeft;
        } else {
            current = current->idRight;
        }
    }
    return bid;
}

void BinarySearchTree::AmountSearch(double lowAmount, double highAmount) {
    amountSearch(this->root, lowAmount, highAmount);
}

void BinarySearchTree::inIdOrder(Node* node) {
    if (node) {
        inIdOrder(node->idLeft);
        displayBid(node->current);
        inIdOrder(node->idRight);
    }
}

void BinarySearchTree::inAmountOrder(Node* node) {
    if (node) {
        inAmountOrder(node->amountLeft);
        displayBid(node->current);
        inAmountOrder(node->amountRight);
    }
}

void BinarySearchTree::insertId(Bid bid, Node* createdNode) {
    Node* prevNode = NULL;
    Node* current = this->root;
    string direction;

    while (current) {
        prevNode = current;
        if (bid.bidId < current->current.bidId) {
            direction = "L";
            current = current->idLeft;
        } else {
            direction = "R";
            current = current->idRight;
        }
    }

    if (not prevNode) {
        this->root = createdNode;
    } else {
        if (direction == "L") {
            prevNode->idLeft = createdNode;
        } else {
            prevNode->idRight = createdNode;
        }
    }
}

void BinarySearchTree::insertAmount(Bid bid, Node* createdNode) {
    Node* prevNode = NULL;
    Node* current = this->amountRoot;
    string direction;

    while (current) {
        prevNode = current;
        if (bid.amount < current->current.amount) {
            direction = "L";
            current = current->amountLeft;
        } else {
            direction = "R";
            current = current->amountRight;
        }
    }

    if (not prevNode) {
        this->amountRoot = createdNode;
    } else {
        createdNode->amountParent = prevNode;
        if (direction == "L") {
            prevNode->amountLeft = createdNode;
        } else {
            prevNode->amountRight = createdNode;
        }
    }
}

void BinarySearchTree::removeNode(Node* rootNode, string bidId) {
    Node* parentNode = NULL;
    Node* currentNode = rootNode;
    while (currentNode) {
        if (currentNode->current.bidId == bidId) {
            Node* amountParentNode = currentNode->amountParent;
            if (not currentNode->amountLeft && not currentNode->amountRight) {
                if (amountParentNode) {
                    if (amountParentNode->amountLeft == currentNode) {
                        amountParentNode->amountLeft = NULL;
                    } else {
                        amountParentNode->amountRight = NULL;
                    }
                }
            }
            else if (currentNode->amountLeft && not currentNode->amountRight) {
                if (not amountParentNode) {
                    this->amountRoot = currentNode->amountLeft;
                }
                else if (amountParentNode->amountLeft == currentNode) {
                    amountParentNode->amountLeft = currentNode->amountLeft;
                } else {
                    amountParentNode->amountRight = currentNode->amountLeft;
                }
            }
            else if (not currentNode->amountLeft && currentNode->amountRight) {
                if (not amountParentNode) {
                    this->amountRoot = currentNode->amountRight;
                }
                else if (amountParentNode->amountLeft == currentNode) {
                    amountParentNode->amountLeft = currentNode->amountRight;
                } else {
                    amountParentNode->amountRight = currentNode->amountRight;
                }
            } else {
                Node* amountSuccessorNode = currentNode->amountRight;
                Node* amountSuccessorParent = currentNode;
                while (amountSuccessorNode->amountLeft) {
                    amountSuccessorParent = amountSuccessorNode;
                    amountSuccessorNode = amountSuccessorNode->amountLeft;
                }
                amountSuccessorNode->amountLeft = currentNode->amountLeft;
                amountSuccessorNode->amountRight = currentNode->amountRight;
                amountSuccessorParent->amountLeft = NULL;
            }
            if (not currentNode->idLeft && not currentNode->idRight) {
                if (parentNode) {
                    if (parentNode->idLeft == currentNode) {
                        parentNode->idLeft = NULL;
                    } else {
                        parentNode->idRight = NULL;
                    }
                }
            }
            else if (currentNode->idLeft && not currentNode->idRight) {
                if (not parentNode) {
                    this->root = currentNode->idLeft;
                }
                else if (parentNode->idLeft == currentNode) {
                    parentNode->idLeft = currentNode->idLeft;
                } else {
                    parentNode->idRight = currentNode->idLeft;
                }
                currentNode->idLeft = NULL;
            }
            else if (currentNode->idRight) {
                if (not parentNode) {
                    this->root = currentNode->idRight;
                }
                else if (parentNode->idLeft == currentNode) {
                    parentNode->idLeft = currentNode->idRight;
                } else {
                    parentNode->idRight = currentNode->idRight;
                }
                currentNode->idRight = NULL;
            } else {
                Node* successorNode = currentNode->idRight;
                Node* successorParent = currentNode;
                while (successorNode->idLeft) {
                    successorParent = successorNode;
                    successorNode = successorNode->idLeft;
                }
                successorNode->idLeft = currentNode->idLeft;
                successorNode->idRight = currentNode->idRight;
                successorParent->idLeft = NULL;
            }

            currentNode->idLeft = NULL;
            currentNode->idRight = NULL;
            currentNode->amountParent = NULL;
            currentNode->amountLeft = NULL;
            currentNode->amountRight = NULL;
            delete currentNode;
            currentNode = NULL;
        }
        else if (currentNode->current.bidId > bidId) {
            parentNode = currentNode;
            currentNode = currentNode->idLeft;
        } else {
            parentNode = currentNode;
            currentNode = currentNode->idRight;
        }
    }
    return;
}

void BinarySearchTree::amountSearch(Node* node, double lowAmount, double highAmount) {
    if (node) {
        amountSearch(node->amountLeft, lowAmount, highAmount);
        if (node->current.amount >= lowAmount && node->current.amount <= highAmount) {
            displayBid(node->current);
        }
        amountSearch(node->amountRight, lowAmount, highAmount);
    }
}

void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    csv::Parser file = csv::Parser(csvPath);
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');
            bst->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[]) {
    string csvPath, bidKey;
    double amountHigh, amountLow;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            bidKey = "98109";
            amountLow = 200;
            amountHigh = 500;
            break;
        case 3:
            csvPath = argv[1];
            bidKey = argv[2];
            amountLow = 200;
            amountHigh = 500;
            break;
        default:
            csvPath = "/Users/haro/Desktop/BinaryTree/eBid_Monthly_Sales.csv";
            bidKey = "98109";
            amountLow = 200;
            amountHigh = 500;
    }

    clock_t ticks;
    BinarySearchTree* bst = new BinarySearchTree();
    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids by ID" << endl;
        cout << "  3. Display All Bids by Amount" << endl;
        cout << "  4. Find Bid by ID" << endl;
        cout << "  5. Find Bids in Amount Range" << endl;
        cout << "  6. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bst = new BinarySearchTree();
                ticks = clock();
                loadBids(csvPath, bst);
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;
            case 2:
                bst->InIdOrder();
                break;
            case 3:
                bst->InAmountOrder();
                break;
            case 4:
                ticks = clock();
                bid = bst->IdSearch(bidKey);
                ticks = clock() - ticks;
                if (!bid.bidId.empty()) {
                    displayBid(bid);
                } else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;
            case 5:
                bst->AmountSearch(amountLow, amountHigh);
                break;
            case 6:
                bst->Remove(bidKey);
                break;
        }
    }
    cout << "Good bye." << endl;
    return 0;
}