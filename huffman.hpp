#pragma once

#include <list>
#include <map>
#include <string>
#include <stdint.h>

struct Node {
    uint32_t weight;
    char ch;
    Node *left;
    Node *right;

    Node(uint32_t w, char c);
    Node(uint32_t w, Node* l, Node* r);
    ~Node();
};

struct NodeComparer {
    bool operator() (const Node* l, const Node* r) const {
        return l->weight < r->weight;
    }
};

class StringDecomposer {
    std::string text;
    std::map<char, uint32_t> decomposedString;
private:
    void addCharToDecomposedMap(char& c);
public:
    StringDecomposer(const std::string& t); 
    void printText(); 
    void printStatistic();
    std::map<char, uint32_t>& getMap(); 
};

class HuffmanCode {
    std::list<Node*> nodesToHandle;
    std::map<char, std::string>codeTable;
    Node* codeTree;

    std::string packingData(const std::string& text);
    void deleteNodes(Node* node, size_t level);
    void fillTable(Node* root, const std::string& text);
public:
    HuffmanCode(const std::map<char, uint32_t>& map);
    ~HuffmanCode();
    std::string HuffmanEncoder(const std::string& text);
    std::string HuffmanDecoder(const std::string& encoded);
    void printNodes(); 
};

