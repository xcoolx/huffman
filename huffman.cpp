#include <iostream>
#include "huffman.hpp"

Node::Node(uint32_t w, char c) : weight(w), ch(c), left(nullptr), right(nullptr) {}

Node::Node(uint32_t w, Node* l, Node* r) : weight(w), ch(0), left(l), right(r) {}

Node::~Node() {
    return;
    std::cout << "delete node: "
              << weight
              << " : "
              << ch
              << std::endl;
}

HuffmanCode::HuffmanCode(const std::map<char, uint32_t>& map) : codeTree(nullptr) {
    for (auto const& el : map) {
        nodesToHandle.push_back(new Node(el.second, el.first));
    }
}

void HuffmanCode::deleteNodes(Node* node, size_t level = 0) {
    level++;
    if(node->left) deleteNodes(node->left, level);
    if(node->right) deleteNodes(node->right, level);
    //std::cout << level << ' ';
    delete node;
}

HuffmanCode::~HuffmanCode() {
    if (codeTree == nullptr) return;

    deleteNodes(codeTree);
}

void HuffmanCode::fillTable(Node* node, const std::string& code) {
    if(node->left) {
        fillTable(node->left, code + "0" );
    }
    if(node->right) {
        fillTable(node->right, code + "1" );
    }
    if(node->ch) {
        codeTable[node->ch] = code;
    }
}

std::string HuffmanCode::packingData(const std::string& text) {
    std::string output;
    std::string temp;
    for(auto const& ch : text) {
        temp.append(codeTable[ch]);
    }
    if(temp.size() % 8) temp.append(std::string( 8 - temp.size() % 8, '0'));
    for (size_t i = 0; i < temp.size(); i += 8) {
        char ch = std::stoi(temp.substr (i,8), nullptr, 2);
        output += ch;
    }
    std::cout << temp << std::endl;
    std::cout << output << std::endl;
    return output;
}

std::string HuffmanCode::HuffmanEncoder(const std::string& text) {
    while(nodesToHandle.size() > 1) {
        nodesToHandle.sort(NodeComparer());
	Node* left = nodesToHandle.front();
	nodesToHandle.pop_front();
        Node* right = nodesToHandle.front();
	nodesToHandle.pop_front();

        nodesToHandle.push_front(new Node(left->weight + right->weight, left, right));
    }
    codeTree = nodesToHandle.front();    
    fillTable(codeTree, "");

    return packingData(text); 
}

std::string HuffmanCode::HuffmanDecoder(const std::string& encoded) {
    std::string output;
    Node* currentNode = codeTree;
    for(auto const& ch : encoded) {
        for(int8_t i = 7; i >= 0; i--) {
	    bool direction = (ch >> i) & 1;
	    if(direction) currentNode = currentNode->right;
	    else currentNode = currentNode->left;
	    if(currentNode->ch) {
                output += currentNode->ch;
		currentNode = codeTree;
	    }
	}
    }
    std::cout << output << std::endl;
    return output; 
}

void HuffmanCode::printNodes() {
    nodesToHandle.sort(NodeComparer());
    for (auto const& el : nodesToHandle) {
        std::cout << el->ch
                  << " : "
                  << el->weight
                  << std::endl;
    }
    for (auto const& el : codeTable) {
        std::cout << el.first
                  << " : "
                  << el.second
                  << std::endl;
    }
}

void StringDecomposer::addCharToDecomposedMap(char& c) {
    auto search = decomposedString.find(c);
    if(search != decomposedString.end()) {
        ++search->second;
    } else {
        decomposedString[c] = 1;
    }
}

StringDecomposer::StringDecomposer(const std::string& t) : text(t) {
    for(char& c : text) {
        addCharToDecomposedMap(c);
    }
}

void StringDecomposer::printText() {
    std::cout << text << std::endl;
}

void StringDecomposer::printStatistic() {
    for (auto const& el : decomposedString) {
        std::cout << el.first
                  << " : "
                  << el.second
                  << std::endl;
    }
}

std::map<char, uint32_t>& StringDecomposer::getMap() {
    return decomposedString;
}

int main() {
    std::string text;
    getline(std::cin, text);
    StringDecomposer decomposedText(text);
    decomposedText.printStatistic();
    HuffmanCode code(decomposedText.getMap());
    std::cout << std::endl;
    code.printNodes();
    std::string encoded = code.HuffmanEncoder(text); 
    code.printNodes();
    std::string decoded = code.HuffmanDecoder(encoded); 
    return 0;
}
