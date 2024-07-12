#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

class Node{
    public:
        char ch;
        int freq;
        Node* left, *right;
        Node(char c, int f, Node *l, Node *r){
            this->ch = c;
            this->freq = f;
            this->left = l;
            this->right = r;
        }
};

struct compare{
    bool operator()(Node *l, Node *r){
        return l->freq > r->freq;
    }
};

void encode(Node *root, string str, unordered_map<char, string> &huffmanCode){
    if(root==NULL) return;
    if(!root->left && !root->right){
        huffmanCode[root->ch] = str;
    }

    encode(root->left, str + '0' , huffmanCode);
    encode(root->right, str + '1' , huffmanCode);
}

void buildHuffmanTree(string text){
    unordered_map<char, int> freq;
    for(char ch: text){
        freq[ch]++;
    }

    priority_queue<Node*, vector<Node*>, compare> pq;
    for(auto pair: freq){
        pq.push(new Node(pair.first, pair.second, NULL, NULL));
    }

    while(pq.size()>1){
        Node *l = pq.top();
        pq.pop();
        Node *r = pq.top();
        pq.pop();

        int sum = l->freq + r->freq;
        pq.push(new Node('\0', sum, l, r));
    }

    Node *root = pq.top();
    unordered_map<char, string> huffmancode;
    encode(root, "", huffmancode);

    ofstream file("sample-compressed.txt");
    if(!file){
        perror("Open Failed for the Output File");
        exit(1);
    }
    for(auto pair: huffmancode){
        file<<pair.first<<": "<<pair.second<<"\n";
    }
    file.close();
}

int main(){
    ifstream inFile("sample.txt");
    if(!inFile){
        cerr<<"Open Failed for Input File"<<endl;
        exit(1);
    }

    string line;
    string text;
    while(getline(inFile, line)){
        text += line;
    }

    inFile.close();

    buildHuffmanTree(text);
    return 0;
}