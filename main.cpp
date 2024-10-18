// COMSC-210 | Midterm 2 | Dan Pokhrel
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;
const int TIME_PERIODS = 20; // number of times simulation runs
const int INITAL_CUSTOMERS = 5; // number of customers added at start of simulation
const int NAMES_COUNT = 99; // number of names in names.txt
const string NAMES_FILE = "names.txt";
// Probabilities
const int ORDERING = 40, JOINING = 60, LEAVING = 20, ANY_LEAVING = 10, VIP = 10;

class DoublyLinkedList {
private:
    struct Node {
        string name;
        Node* prev;
        Node* next;
        Node(string val, Node* p = nullptr, Node* n = nullptr) {
            name = val; 
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    string get_front() const {
        if (head)
            return head->name;
        else
            return "<Line is empty>";
    }
    string get_back() const {
        if (tail)
            return tail->name;
        else
            return "<Line is empty>";
    }

    void insert_after(string value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }

    void delete_val(string value) {
        if (!head) return;

        Node* temp = head;
        
        while (temp && temp->name != value)
            temp = temp->next;

        if (!temp) return; 

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next; 

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev; 

        delete temp;
    }

    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
    
        if (pos == 1) {
            pop_front();
            return;
        }
    
        Node* temp = head;
    
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }
    
        if (!temp->next) {
            pop_back();
            return;
        }
    
        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    void push_back(string v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    
    void push_front(string v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    
    void pop_front() {

        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node * temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node * temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void print() {
        Node* current = head;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->name << " ";
            current = current->next;
        }
        cout << endl;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) { 
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->name << " ";
            current = current->prev;
        }
        cout << endl;
    }
};

// prototypes
bool prob(int x); // returns true randomly based on the probability stated with x
string getRandomName();

int main() {
    srand(time(0));
    DoublyLinkedList line;

    // Starting Customers
    cout << "Store opens:\n";
    for (int i = 0; i < INITAL_CUSTOMERS; i++){
        string name = getRandomName();
        line.push_back(name);
        cout << "\t" << name << " joins the line.\n";
    }
    cout << " >  Resulting Line: "; line.print();

    // Simulation Loop
    for (int step = 2; step <= TIME_PERIODS; step++){
        cout << "\nTime step #" << step << endl;

        if (prob(ORDERING)){
            cout << "\t" << line.get_front() << " is served\n";
            line.pop_front();
        }
        if (prob(JOINING)){
            string name = getRandomName();
            line.push_back(name);
            cout << "\t" << name << " joined the line.\n";
        }
        if (prob(LEAVING)){
            cout << "\t" << line.get_back() << " (at the rear) left the line";
            line.pop_back();
        }
        if (prob(ANY_LEAVING)){

        }
        if (prob(VIP)){

        }

        cout << "  > Resulting Line: "; line.print();
    }
    
    return 0;
}

bool prob(int x){
    int p = rand() % 100 + 1;
    return p <= x;
}

string getRandomName(){
    ifstream fin;
    fin.open(NAMES_FILE);
    if (!fin){
        cout << "Names file not found.";
        return "<Error>";
    }

    int i = 0, x = rand() % NAMES_COUNT; // rand number between 0 and 98
    string name;
    while (fin && i <= x){ // loop through lines until we get to the x'th line
        getline(fin, name);
        i++;
    }

    return name;
}