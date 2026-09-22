# Huffman Text Compression

Huffman Text Compression is a C++ program that compresses and decompresses text using **Huffman Coding**. The project demonstrates how several core data structures can work together to build a complete compression algorithm, including a hash table, priority queue, and binary tree.

The project is designed to demonstrate the full compression process from character-frequency analysis through encoding, decoding, and compression-ratio calculation.

## Features

* Accepts text input from the user
* Builds a character frequency table
* Stores character frequencies in an `unordered_map`
* Constructs a min-priority queue from the frequency data
* Builds a Huffman binary tree
* Generates unique Huffman codes for each character
* Encodes the original text into a compressed bit sequence
* Decodes the compressed sequence back into the original text
* Displays the original 8-bit ASCII representation
* Calculates the final compression ratio
* Prints each major stage of the compression process for visualization

## How Huffman Coding Works

Huffman Coding reduces the size of text by assigning shorter bit sequences to characters that appear frequently and longer bit sequences to less common characters.

Standard ASCII characters are normally represented using 8 bits. Huffman Coding takes advantage of differences in character frequency to reduce the total number of bits required to represent the text.

The overall process is:

```text
Input Text
    ↓
Frequency Table
    ↓
Priority Queue
    ↓
Huffman Tree
    ↓
Huffman Codes
    ↓
Encoded Bit Sequence
    ↓
Decoded Text
    ↓
Compression Ratio
```

## Data Structures Used

### Hash Table

The program uses:

```cpp
using FrequencyTable = std::unordered_map<char, int>;
```

to count how many times each character occurs.

The project specification calls for a hash table because the compression algorithm needs a frequency histogram before constructing the Huffman tree.

A second hash table stores the generated Huffman codes:

```cpp
using HuffmanCodes = std::unordered_map<char, std::string>;
```

Each character is mapped to its corresponding binary code.

### Priority Queue

A minimum-priority queue stores tree nodes according to character frequency.

```cpp
std::priority_queue<Node*, std::vector<Node*>, Compare>
```

The least frequent nodes are repeatedly removed, combined into a parent node, and inserted back into the queue.

The specification describes this process as:

1. Insert all nodes into the priority queue
2. Remove the two lowest-frequency nodes
3. Combine them into a new tree
4. Add their frequencies together
5. Insert the new parent node back into the queue
6. Continue until only one node remains

The final remaining node becomes the root of the Huffman tree.

### Binary Tree

Each node stores:

```cpp
struct Node {
    std::string letters;
    int frequency;
    Node* left;
    Node* right;
};
```

The tree structure allows the program to generate binary codes based on the path from the root to each character.

The project uses the same node structure for both the priority queue and Huffman tree.

## Generating Huffman Codes

The program recursively walks through the Huffman tree.

Moving left adds:

```text
0
```

and moving right adds:

```text
1
```

When the traversal reaches a leaf node, the accumulated bit sequence becomes that character's Huffman code.

Because high-frequency characters are positioned closer to the root, they receive shorter bit sequences.

## Encoding

Encoding is performed by looking up each character in the Huffman code table and combining the resulting bit sequences.

For example:

```text
Original:
hello

Possible encoded representation:
1011100110
```

Instead of storing every character as 8 bits, the program stores each character using its generated Huffman code.

## Decoding

The decoding process begins at the root of the Huffman tree.

For every bit:

```text
0 → move left
1 → move right
```

When a leaf node is reached, its character is added to the decoded string. The traversal then returns to the root and continues with the remaining bits.

This allows the program to reconstruct the original input from the compressed representation.

## Compression Ratio

The program compares the size of the encoded sequence to the original ASCII representation.

The original size is calculated as:

```cpp
text.size() * 8
```

because each ASCII character normally requires 8 bits.

The compression ratio is then:

```text
encoded bits
────────────
original bits
```

This follows the compression-ratio calculation described in the project specification.

For example:

```text
Compressed to 25% of original size
```

means the Huffman-encoded representation requires only one quarter of the bits needed by the original text.

## Program Output

The program displays each stage of the compression algorithm, including:

* Frequency table
* Priority queue
* Huffman tree
* Huffman codes
* Uncompressed bit sequence
* Encoded bit sequence
* Decoded text
* Compression ratio

These outputs correspond directly to the required project output.

Example:

```text
Enter text: hello

Frequency table:
e: 1
h: 1
l: 2
o: 1

Priority Queue:
(e, 1) -> (o, 1) -> (h, 1) -> (l, 2) -> nullptr

Huffman encoding tree:
...

Huffman codes:
e: 111
h: 10
l: 0
o: 110

Uncompressed bit sequence:
01101000 01100101 01101100 01101100 01101111

Encoded:
1011100110

Decoded:
hello

Compressed to 25% of original size
```

## Technologies Used

* C++
* C++20
* CMake
* `std::unordered_map`
* `std::priority_queue`
* Binary trees
* Recursive tree traversal
* `std::bitset`
* `std::stringstream`

## Project Structure

```text
Huffman-Text-Compression/
│
├── main.cpp
└── CMakeLists.txt
```

### `main.cpp`

Contains the complete Huffman Coding implementation, including:

* Frequency-table construction
* Priority-queue creation
* Huffman-tree construction
* Recursive Huffman-code generation
* Text encoding
* Text decoding
* Binary conversion
* Compression-ratio calculation
* Priority-queue output
* Tree visualization

### `CMakeLists.txt`

Configures the project to build with CMake using C++20.

```cmake
set(CMAKE_CXX_STANDARD 20)
```

## Running the Project

Clone the repository:

```bash
git clone https://github.com/dentoncd/Huffman-Text-Compression
```

Navigate into the project:

```bash
cd Huffman-Text-Compression
```

### Using CMake

Create a build directory:

```bash
mkdir build
cd build
```

Generate the build files:

```bash
cmake ..
```

Compile:

```bash
cmake --build .
```

Then run the executable.

### Using g++

You can also compile the program directly:

```bash
g++ -std=c++20 main.cpp -o huffman
```

Run it with:

```bash
./huffman
```

On Windows:

```powershell
.\huffman.exe
```

## Example Workflow

After starting the program:

```text
Enter text:
```

Enter a string such as:

```text
hello world
```

The program will then generate and display every major stage of the compression process before showing the decoded text and final compression ratio.

## Concepts Practiced

This project demonstrates:

* Hash tables
* Priority queues
* Binary trees
* Min-heaps
* Tree traversal
* Recursion
* Dynamic memory
* Character-frequency analysis
* Variable-length encoding
* Data compression
* Bit representations
* C++ STL containers
* Custom comparators
* Algorithm design

## Future Improvements

Possible improvements include:

* Reading text directly from files
* Writing compressed data to a file
* Decompressing previously saved files
* Automatically freeing the entire Huffman tree after execution
* Handling single-character input more explicitly
* Adding command-line arguments
* Displaying the number of bits saved
* Comparing compression across different types of text
* Creating a graphical visualization of the Huffman tree

## About

This project implements Huffman Coding from the ground up to demonstrate how data structures can work together to solve a practical problem.

It combines hash tables, priority queues, binary trees, recursion, and binary encoding into a complete text-compression pipeline while also verifying correctness by decoding the compressed output back into the original text.
