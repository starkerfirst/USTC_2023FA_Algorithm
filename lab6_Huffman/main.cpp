#include<iostream>
#include<fstream>
#include<queue>
#include<vector>
#include<cassert>
#include<utility>
#include<string>
#include<cmath>
#include<algorithm>
using namespace std;

class Node{
    public:
    int freq;
    char ch;
    Node *left, *right;

    Node(int freq, char ch){
        this->freq = freq;
        this->ch = ch;
        left = right = NULL;
    }

    bool operator > (const Node &node) const{
        return freq < node.freq;
    }
};

class Compare{
    public:
    bool operator() (Node* a, Node* b){
        return a->freq > b->freq;
    }
};

// store final encoding scheme
vector<pair<char, string>> scheme(128);

Node* Huffman(priority_queue<Node*, vector<Node*>, Compare>& Queue){
    int n = Queue.size();
    for (int i = 0; i < n-1; i++){
        Node* node_min = Queue.top();
        Queue.pop();
        Node* node_sec = Queue.top();
        Queue.pop();

        auto node = new Node(node_min->freq + node_sec->freq, '\0');
        node->left = node_min;
        node->right = node_sec;
        Queue.push(node);
    }

    return Queue.top();
}

void output(Node* root, ofstream& fout, string code){
    Node* ptr = root;
    if (ptr == NULL) return;
    if (ptr->ch != '\0') {
        fout << ptr->ch << "\t" << ptr->freq << "\t" << code << endl;
        scheme.push_back(make_pair(ptr->ch, code));
    }
    output(ptr->left, fout, code + "0");
    output(ptr->right, fout, code + "1");
}

int main(){
    ifstream fin("original.txt");
    if (!fin.is_open()){
        cout << "Error opening file" << endl;
        return 0;
    }
    
    // statistics
    int freq[128] = {0}; // ASCII
    char ch;
    while(fin.get(ch)){
        freq[ch]++;
    }
    fin.close();

    // create Huffman tree
    priority_queue<Node*, vector<Node*>, Compare> Queue;
    int total = 0;
    int length = 0;
    // push all numbers, characters and delimiters into the queue
    for (int i = 0; i < 128; i++){
        if (freq[i] > 0 and i > 32){
            Queue.push(new Node(freq[i], i));
            total += freq[i];
            length++;
        }
    }

    // Huffman
    auto root = Huffman(Queue);
    assert(root->freq == total);

    // output
    ofstream fout("table.txt");
    fout << "char\tfreq\tcode" << endl;
    output(root, fout, "");

    // calculate compression ratio compared to fixed length encoding
    double ratio = 0;
    for (auto i : scheme){
        ratio += i.second.length() * freq[i.first];
    }
    int fixed = int(ceil(log2(length))) * total;
    ratio /= fixed;
    cout << "Compression ratio: " << ratio << endl;

    return 0;
}