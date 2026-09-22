#include <bitset>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <iomanip>

// Aliases
using FrequencyTable = std::unordered_map<char, int>;
using HuffmanCodes = std::unordered_map<char, std::string>;

// Step 3: Create a Node class that is convenient to use in both data structures
struct Node {
    Node(std::string letters, int frequency, Node* left = nullptr, Node* right = nullptr)
        : letters{letters}, frequency{frequency}, left{left}, right{right} {}

    std::string letters;
    int frequency;
    Node* left, *right;
};

// Min-heap comparison
struct Compare {
    bool operator()(Node* lhs, Node* rhs) {
        return lhs->frequency > rhs->frequency;
    }
};

// Functions go here:
FrequencyTable build_frequency_table(const std::string& text);
Node* build_huffman_tree(const FrequencyTable& frequency_table);
void assign_huffman_codes(Node* root, HuffmanCodes& huffman_codes, const std::string& current = "");
std::string encode_text(const std::string& text, HuffmanCodes& huffman_codes);
std::string decode_text(Node* root, std::string& encoded_text);
std::string binary(const std::string& text);
double compression(const std::string& text, const std::string& encoded_text);
void print_priority_queue(std::priority_queue<Node*, std::vector<Node*>, Compare> pq);
void print_huffman_tree(Node* n, int level = 0, std::ostream& os = std::cout);

int main() {
    // --- User Input ---
    std::string text;
    std::cout << "Enter text: ";
    std::getline(std::cin, text);

    if (text.empty()) {
        throw std::runtime_error("Text is empty");
    }

    // --- Frequency Table ---
    FrequencyTable frequency_table;

    std::cout << "Frequency table: \n";
    frequency_table = build_frequency_table(text);
    for (const auto& pair : frequency_table) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    // --- Priority Queue ---
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (const auto& pair : frequency_table) {
        Node* node = (new Node{std::string(1, pair.first), pair.second, nullptr, nullptr});
        pq.push(node);
    }
    std::cout << "\nPriority Queue: \n";
    print_priority_queue(pq);
    while (!pq.empty()) {
        delete pq.top(); // Prevent memory leaks
        pq.pop();
    }

    // --- Huffman Tree ---
    std::cout << "Huffman encoding tree: \n";
    Node* root = build_huffman_tree(frequency_table);
    print_huffman_tree(root, 0, std::cout);

    // --- Huffman Codes ---
    std::cout << "\nHuffman codes: \n";
    HuffmanCodes huffman_codes;
    assign_huffman_codes(root, huffman_codes);
    for (const auto& pair : huffman_codes) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    // --- Uncompressed ---
    std::cout << "\nUncompressed bit sequence: \n";
    for (char c : text) {
        std::cout << std::bitset<8>(c) << " ";
    }
    std::cout << "\n\n";

    // --- Encoded / Compressed ---
    std::string encoded_text = encode_text(text, huffman_codes);
    std::cout << "Encoded: \n";
    std::cout << encoded_text << "\n\n";

    // --- Decoded ---
    std::cout << "Decoded: \n";
    std::cout << decode_text(root, encoded_text) << "\n\n";

    // --- Compression Ratio ---
    const double compression_ratio = compression(text, encoded_text);
    std::cout << "Compressed to " << compression_ratio * 100 << "% of original size" << "\n";
}

// Step 2: Construct a frequency table
FrequencyTable build_frequency_table(const std::string& text) {
    FrequencyTable frequency_table; // unordered_map<char, int>
    // Look through each character in the string and add it to the frequency table
    for (char c : text) {
        frequency_table[c]++;
    }
    return frequency_table;
}

// Step 3: Construct a Huffman Tree
Node* build_huffman_tree(const FrequencyTable& frequency_table) {
    // Place all the nodes in the PQ sorted by letter frequency
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (const auto& pair : frequency_table) {
        std::string letter(1, pair.first); // str to char
        int frequency = pair.second;
        pq.push(new Node(letter, frequency));
    }

    // Until there is only one node in the PQ:
    // Remove two smallest nodes (pq reversed so smallest is at the top)
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        // Combine them into  a tree where the parent holds the string of letters, and the sum of letter frequencies
        std::string new_letters = left->letters + right->letters;
        int sum = left->frequency + right->frequency;
        auto parent = new Node(new_letters, sum, left, right); // auto = Node*

        // Insert tree node back into PQ
        pq.push(parent);
    }
    return pq.top();
}

// Step 4: Assigning Huffman Codes
void assign_huffman_codes(Node* root, HuffmanCodes& huffman_codes, const std::string& current) {
    // Traverse recursively, base case
    if (root == nullptr) { // if null, do nothing
        return;
    }

    // If is leaf, store char and assigned code
    if (!root->left && !root->right) {
        huffman_codes[root->letters[0]] = current;
        return;
    }

    // Traverse left and right nodes
    assign_huffman_codes(root->left, huffman_codes, current + "0");
    assign_huffman_codes(root->right, huffman_codes, current + "1");
}

// Step 5: Encoding
std::string encode_text(const std::string& text, HuffmanCodes& huffman_codes) {
    std::string encoded;

    // Combine into large string
    for (char c : text) {
        encoded += huffman_codes[c];
    }
    return encoded;
}

// Step 6: Decoding
std::string decode_text(Node* root, std::string& encoded_text) {
    std::string decoded_text;
    // Start at root
    Node* current = root;

    // Go through each char in encoded text
    for (char c : encoded_text) {
        // Left for 0...
        if (c == '0') {
            current = current->left;
        }
        // ...right for 1
        else if (c == '1') {
            current = current->right;
        }
        // If is leaf, DECODE
        if (!current->left && !current->right) {
            // Add char to decoded text
            decoded_text += current->letters;
            // Go back to root for next char
            current = root;
        }
    }
    return decoded_text;
}

// Step 7.1: Binary
std::string binary(const std::string& text) {
    std::stringstream ss;
    for (char c : text) {
        ss << std::bitset<8>(c);
    }
    return ss.str();
}

// Step 7.2: Compression
double compression(const std::string& text, const std::string& encoded_text) {
    // Size of bit is 8
    int size = text.size() * 8;
    int encoded_size = encoded_text.size();

    // Check if size is 0 because cannot divide by zero
    if (size == 0) {
        throw std::runtime_error("Cannot divide by zero");
    }

    // Calculate compression ratio
    double compression_ratio = static_cast<double>(encoded_size) / size;
    return compression_ratio;
}

// Print priority queue
void print_priority_queue(std::priority_queue<Node*, std::vector<Node*>, Compare> pq) {
    while (!pq.empty()) {
        Node* node = pq.top();
        std::cout << "(" << node->letters << ", " << node->frequency << ") -> ";
        pq.pop();
    }
    std::cout << "nullptr\n\n";
}

// Pretty print tree
void print_huffman_tree(Node* n, int level, std::ostream& os) { // code from trees_part_two
    if (n) {
        if (n->right) {
            print_huffman_tree(n->right, level+4, os);
            os << std::setw(level+2) << "/\n";
        }

        os << std::setw(level) << " " << n->letters << "(" << n->frequency << ")" << '\n';

        if (n->left) {
            os << std::setw(level+2) << "\\\n";
            print_huffman_tree(n->left, level+4, os);
        }
    }
}