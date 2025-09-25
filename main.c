#include <iostream>
#include <fstream>
#include <cstdio>

// Forward declaration of Deque class
class Deque;

// Global pointers to main deque and pattern deque
Deque *deq = nullptr;      // Main data deque
Deque *img_deq = nullptr;  // Pattern deque for search

// Node class for deque elements
class Node {
public:
    float data;    // Stored value
    Node* next;    // Pointer to next node
    Node* prev;    // Pointer to previous node

public:
    // Node constructor
    Node(float data) {
        this->data = data;
        this->next = this->prev = nullptr;  // Initialize pointers
    }
};

// Double-ended queue (deque) class
class Deque {
public:
    Node *head;  // Pointer to deque start
    Node *tail;  // Pointer to deque end

public:
    // Constructor - initialize empty deque
    Deque() {
        this->head = nullptr;
        this->tail = nullptr;
    }

    // Destructor - free memory
    ~Deque() {
        // Sequentially remove all elements from the beginning
        while (head != nullptr) {
            pop_front();
        }
    }

    // Add element to the front of deque
    Node* push_front(float data) {
        // Allocate memory for new node
        Node *ptr = new(std::nothrow) Node(data);
        
        // Check memory allocation
        if (ptr == nullptr) {
            std::cout << "Memory allocation error\n";
            delete deq;
            delete img_deq;
            exit(EXIT_FAILURE);
        }

        // Set up node connections
        ptr->next = head;
        
        if (head != nullptr)
            head->prev = ptr;
            
        if (tail == nullptr)  // If deque was empty
            tail = ptr;
            
        head = ptr;  // Update deque head
        return ptr;
    }

    // Add element to the back of deque
    Node* push_back(float data) {
        // Allocate memory for new node
        Node *ptr = new(std::nothrow) Node(data);
        
        // Check memory allocation
        if (ptr == nullptr) {
            std::cout << "Memory allocation error\n";
            delete deq;
            delete img_deq;
            exit(EXIT_FAILURE);
        }

        // Set up node connections
        ptr->prev = tail;
        
        if (tail != nullptr)
            tail->next = ptr;
            
        if (head == nullptr)  // If deque was empty
            head = ptr;
            
        tail = ptr;  // Update deque tail
        return ptr;
    }

    // Remove element from the front of deque
    void pop_front() {
        if (head == nullptr) return;  // If deque is empty

        Node* ptr = head->next;
        
        if (ptr != nullptr)
            ptr->prev = nullptr;
        else
            tail = nullptr;  // Deque became empty

        delete head;  // Free memory
        head = ptr;   // Update deque head
    }

    // Remove element from the back of deque
    void pop_back() {
        if (tail == nullptr) return;  // If deque is empty

        Node *ptr = tail->prev;
        
        if (ptr != nullptr)
            ptr->next = nullptr;
        else
            head = nullptr;  // Deque became empty

        delete tail;  // Free memory
        tail = ptr;   // Update deque tail
    }
};

// Function to calculate deque size
int size_deq(Deque *deq) {
    int n = 0;
    Node *ptr = deq->head;
    
    // Traverse deque and count elements
    while (ptr != nullptr) {
        ptr = ptr->next;
        n++;
    }
    return n;
}

// Function to get node by index
Node* getLn(Deque *deq, int index) {
    int n = 0;
    Node *ptr = deq->head;
    
    // Traverse to the specified index
    while (n != index && ptr != nullptr) {
        ptr = ptr->next;
        n++;
    }
    return ptr;
}

// Function to print deque contents
void print_deck(Deque *deq) {
    Node *ptr = deq->head;
    
    // Print all elements sequentially
    while (ptr != nullptr) {
        printf("%0.2f ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

// Function to insert element at specific index
Node* insert(Deque *deq, int index, double x) {
    Node* right = getLn(deq, index);
    
    if (right == nullptr)  // If index is at the end
        return deq->push_back(x);
        
    Node* left = right->prev;
    
    if (left == nullptr)  // If index is at the beginning
        return deq->push_front(x);

    // Create new node
    Node* ptr = new(std::nothrow) Node(x);
    
    if (ptr == nullptr) {
        std::cout << "Memory allocation error\n";
        delete deq;
        delete img_deq;
        exit(EXIT_FAILURE);
    }

    // Insert between left and right
    ptr->prev = left;
    ptr->next = right;
    left->next = ptr;
    right->prev = ptr;
    
    return ptr;
}

// Function to build max-heap for heap sort
void max_heap(Deque *deq, int n_size) {
    float value, temp;
    float pre_value;
    Node *ptr1 = nullptr;
    Node *ptr2 = nullptr;
    Node *ptr3 = nullptr;
    bool check_heap = true;

    // Continue until heap is properly ordered
    while (check_heap == true) {
        check_heap = false;
        float value_leaf;

        // Process all parent nodes
        for (int i = n_size / 2 - 1; i > -1; i--) {
            ptr1 = getLn(deq, i);
            value = ptr1->data;
            value_leaf = value;

            // Check left child
            if (i * 2 + 1 < n_size) {
                ptr2 = getLn(deq, i * 2 + 1);
                pre_value = ptr2->data;

                if (pre_value > value) {
                    value = pre_value;
                }

                // Check right child
                if (i * 2 + 2 < n_size) {
                    ptr3 = getLn(deq, i * 2 + 2);
                    pre_value = ptr3->data;

                    if (pre_value > value) {
                        ptr2 = ptr3;
                        value = pre_value;
                    }
                }
            }

            // If larger value found - swap
            if (value > value_leaf) {
                temp = value_leaf;
                ptr1->data = value;
                ptr2->data = temp;
                check_heap = true;  // Another pass required
            }
        }
    }
}

// Heap sort function
void pyramid_sort(Deque *deq) {
    // Temporary node for value swapping
    Node *ptr_tmp = new(std::nothrow) Node(0);
    
    if (ptr_tmp == nullptr) {
        std::cout << "Memory allocation error!" << std::endl;
        delete deq;
        delete img_deq;
        exit(EXIT_FAILURE);
    }

    int n_size = size_deq(deq);

    // Main sorting loop
    while (n_size > 1) {
        max_heap(deq, n_size);  // Build heap
        
        Node *ptr = getLn(deq, n_size - 1);
        
        // Swap maximum element with last
        ptr_tmp->data = deq->head->data;
        deq->head->data = ptr->data;
        ptr->data = ptr_tmp->data;

        n_size--;  // Reduce unsorted portion size
    }

    delete ptr_tmp;  // Free temporary node
}

// Knuth-Morris-Pratt substring search algorithm
void KMP_SEQ(Deque *sequence, Deque *pattern) {
    Node *ptr1 = nullptr;
    Node *ptr2 = nullptr;

    // Create deque for prefix function storage
    Deque *prefix = new(std::nothrow) Deque();
    
    if (prefix == nullptr) {
        delete deq;
        delete img_deq;
        exit(EXIT_FAILURE);
    }

    int m = size_deq(pattern);   // Pattern length
    int n = size_deq(sequence);  // Sequence length

    // Initialize prefix function with zeros
    for (int i = 0; i < m; i++)
        prefix->push_back(0);

    // Calculate prefix function for pattern
    int j = 0, i = 1;
    
    while (i < m) {
        ptr1 = getLn(pattern, i);
        ptr2 = getLn(pattern, j);
        
        if (ptr1->data == ptr2->data) {
            insert(prefix, i, j + 1);
            j++;
            i++;
        } else if (j == 0) {
            insert(prefix, i, 0);
            i++;
        } else {
            ptr1 = getLn(prefix, j - 1);
            j = ptr1->data;
        }
    }

    // Search for pattern in sequence
    i = 0;
    j = 0;
    
    while (i < n) {
        ptr1 = getLn(sequence, i);
        ptr2 = getLn(pattern, j);
        
        if (ptr1->data == ptr2->data) {
            i += 1;
            j += 1;
            
            if (j == m) {  // Pattern fully found
                printf("Pattern found at position %d\n", i - j);
                delete prefix;
                return;
            }
        } else if (j > 0) {
            ptr1 = getLn(prefix, j - 1);
            j = ptr1->data;  // Shift using prefix function
        } else {
            i++;  // Shift sequence
        }
    }

    // Pattern not found
    if (i == n)
        printf("Pattern not found\n");
        
    delete prefix;
}

int main() {
    // Initialize deques
    deq = new(std::nothrow) Deque();     // Data deque
    img_deq = new(std::nothrow) Deque(); // Pattern deque

    // Check memory allocation
    if (deq == nullptr || img_deq == nullptr) {
        std::cerr << "Memory allocation error!" << std::endl;
        if (deq != nullptr) delete deq;
        if (img_deq != nullptr) delete img_deq;
        exit(EXIT_FAILURE);
    }

    float x;
    int mode;

    // Choose data input source
    std::cout << "Enter 0 for keyboard input, 1 for file input: ";
    std::cin >> mode;

    // Check input validity
    if (std::cin.fail()) {
        std::cerr << "Error: Invalid input!" << std::endl;
        return 0;
    }

    // File input
    if (mode > 0) {
        char file_name[256];
        std::cout << "Enter filename: ";
        std::cin >> file_name;

        if (std::cin.fail()) {
            std::cerr << "Error: Invalid filename!" << std::endl;
        }

        std::ifstream ifs(file_name);
        
        if (ifs.is_open()) {
            // Read data from file
            while (ifs >> x) {
                if (ifs.fail()) {
                    std::cerr << "Error: Invalid data in file!" << std::endl;
                    delete deq;
                    delete img_deq;
                    exit(EXIT_FAILURE);
                }
                deq->push_back(x);  // Add to deque
            }
        } else {
            std::cerr << "Error: Cannot open file!" << std::endl;
            delete deq;
            delete img_deq;
            exit(EXIT_FAILURE);
        }

        // Display input data
        std::cout << "Input deque:" << std::endl;
        print_deck(deq);
    } else {
        // Keyboard input
        std::cout << "Enter deque elements (Ctrl+D/Ctrl+Z to finish):" << std::endl;
        
        while (std::cin >> x) {
            if (std::cin.fail()) {
                std::cerr << "Error: Invalid input!" << std::endl;
                delete deq;
                delete img_deq;
                exit(EXIT_FAILURE);
            }
            deq->push_back(x);  // Add to deque
        }
        std::cin.clear();  // Clear EOF state
    }

    // Input search pattern
    std::cout << "Enter pattern to search for:" << std::endl;
    
    while (std::cin >> x) {
        if (std::cin.fail()) {
            std::cerr << "Error: Invalid input!" << std::endl;
            delete deq;
            delete img_deq;
            exit(EXIT_FAILURE);
        }
        img_deq->push_back(x);  // Add to pattern deque
    }

    // Check for empty deques
    if (size_deq(deq) == 0) {
        fprintf(stderr, "Error: Main deque is empty!");
        delete deq;
        delete img_deq;
        exit(EXIT_FAILURE);
    } else if (size_deq(img_deq) == 0) {
        fprintf(stderr, "Error: Pattern deque is empty!");
        delete deq;
        delete img_deq;
        exit(EXIT_FAILURE);
    }

    // Search pattern in original and sorted sequences
    std::cout << "--- Search Results ---" << std::endl;
    
    std::cout << "Search in original deque: ";
    KMP_SEQ(deq, img_deq);  // Search in original deque

    pyramid_sort(deq);  // Sort deque

    std::cout << "Search in sorted deque: ";
    KMP_SEQ(deq, img_deq);  // Search in sorted deque

    std::cout << "Sorted deque: ";
    print_deck(deq);  // Display sorted deque

    // Free memory
    delete deq;
    delete img_deq;

    return 0;
}