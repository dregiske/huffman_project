#include "HCTree.hpp"

void HCTree::delete_tree(HCNode* node){
    if(!node) return;
    delete_tree(node->c0);
    delete_tree(node->c1);
    delete node;
}

HCTree::~HCTree(){
    if(root){
        delete_tree(root);
    }
}

void HCTree::build(const vector<int>& freqs){
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    for(size_t i = 0; i < freqs.size(); i++){
        if(freqs[i] > 0){
            HCNode* new_node = new HCNode(freqs[i], (unsigned char)i);
            leaves[i] = new_node;
            pq.push(new_node);
        }
    }

    while(pq.size() > 1){
        HCNode* left = pq.top();
        pq.pop();
        HCNode* right = pq.top();
        pq.pop();

        HCNode* parent = new HCNode(left->count + right->count, left->symbol);
        parent->c0 = left;
        parent->c1 = right;
        left->p = parent;
        right->p = parent;
        pq.push(parent);
    }

    if(!pq.empty()){
        root = pq.top();
    }
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    HCNode* node = leaves[symbol];
    vector<int> bits;

    while(node->p){
        if(node->p->c0 == node){
            bits.push_back(0);
        }
        else{
            bits.push_back(1);
        }
        node = node->p;
    }
    
    reverse(bits.begin(), bits.end());
    
    for(int bit : bits){
        out.write_bit(bit);
    }
}

int HCTree::decode(FancyInputStream & in) const{
    if(!root) return EOF;

    HCNode* node = root;

    while(node->c0 || node->c1){
        int bit = in.read_bit();
        if(bit == -1){
            cerr << "EOF while decoding." << endl;
            return EOF;
        }
        if(bit == 0){
            if(!node->c0){
                cerr << "EOF, missing left child." << endl;
                return EOF;
            }
            node = node->c0;
        }
        else{
            if(!node->c1){
                cerr << "EOF, missing right child." << endl;
                return EOF;
            }
            node = node->c1;
        }
    }
    return static_cast<int>(node->symbol);
}