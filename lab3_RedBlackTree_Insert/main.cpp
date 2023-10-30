#include<iostream>
#include<fstream>

auto& out = std::cout;

// Red Black Tree Insertion
struct TNode{
    bool color; // true: red, false: black
    int key;
    TNode *left, *right, *parent;
};

struct RBTree{
    TNode* root;
    TNode* nil;
};

char color(TNode* x){
    if(x->color == true) return 'R';
    else return 'B';
}

void mid_order(TNode* x, std::ofstream& fout){
    if(x != nullptr and x->key != -1){
        mid_order(x->left, fout);
        fout << x->key << color(x) << " ";
        mid_order(x->right, fout);
    }
}

void preorder(TNode* x, std::ofstream& fout){
    if(x != nullptr and x->key != -1){
        fout << x->key << color(x) << " ";
        preorder(x->left, fout);
        preorder(x->right, fout);
    }
}

// level order of transverse
void level_order(TNode* x, std::ofstream& fout){
    // FIFO
    TNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = x;
    while(front != rear){
        TNode* node = queue[front++];
        if(node->key != -1){
            fout << node->key << color(node) << " ";
            if(node->left != nullptr){
                queue[rear++] = node->left;
            }
            if(node->right != nullptr){
                queue[rear++] = node->right;
            }
        }
    }
}

// print RBtree in level order
void print_RBTree(TNode* x){
    // FIFO
    TNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = x;
    while(front != rear){
        TNode* node = queue[front++];
        if(node->key != -1){
            std::cout << node->key << " " << node->color << std::endl;
            if(node->left != nullptr){
                queue[rear++] = node->left;
            }
            if(node->right != nullptr){
                queue[rear++] = node->right;
            }
        }
    }
}



void RBTree_Fix(RBTree* T, TNode* z){
    while(z->parent->color == true){
        // left case
        if (z->parent == z->parent->parent->left){
            TNode* y = z->parent->parent->right;
            if(y->color == true){
                // case 1: uncle is red
                z->parent->color = false;
                y->color = false;
                z->parent->parent->color = true;
                z = z->parent->parent; // recursive
                printf("1 ");
            }
            else{
                // case 2: uncle is black and z is right child
                if(z == z->parent->right){
                    // left rotate
                    TNode* child = z->left;
                    TNode* parent = z->parent;
                    if (parent->parent != T->nil) parent->parent->left = z;
                    z->parent = parent->parent;
                    z->left = parent;
                    parent->right = child;
                    if (child != T->nil) child->parent = parent;
                    parent->parent = z;
                    z = parent;   
                    printf("2 ");
                }
                // case 3: uncle is black and z is left child   
                // right rotate
                TNode* child = z->parent->right;
                TNode* parent = z->parent;
                if (parent->parent != T->nil) parent->parent->left = parent->right;
                parent->right->parent = parent->parent;
                if (parent->parent->parent != T->nil) 
                    if (parent->parent->parent->left == parent->parent) parent->parent->parent->left = parent;
                    else parent->parent->parent->right = parent;
                else T->root = parent;
                parent->right = parent->parent;
                parent->parent = parent->parent->parent;
                parent->right->parent = parent;
                z->parent->color = false;
                z->parent->right->color = true;
                printf("3 ");
                }
            }
        else
        {
            // right case
            TNode* y = z->parent->parent->left;
            if(y->color == true){
                // case 4: uncle is red
                z->parent->color = false;
                y->color = false;
                z->parent->parent->color = true;
                z = z->parent->parent; // recursive
                printf("4 ");
            }
            else{
                // case 5: uncle is black and z is left child
                if(z == z->parent->left){
                    // right rotate
                    TNode* child = z->right;
                    TNode* parent = z->parent;
                    if (parent->parent != T->nil) parent->parent->right = z;
                    z->parent = parent->parent;
                    z->right = parent;
                    parent->left = child;
                    if (child != T->nil)  child->parent = parent;
                    parent->parent = z;
                    z = parent;   
                    printf("5 ");
                }
                // case 6: uncle is black and z is right child
                // left rotate
                TNode* child = z->left;
                TNode* parent = z->parent;
                if (parent->parent != T->nil) parent->parent->right = parent->left;
                parent->left->parent = parent->parent;
                if (parent->parent->parent != T->nil) 
                    if (parent->parent->parent->left == parent->parent) parent->parent->parent->left = parent;
                    else parent->parent->parent->right = parent;
                else T->root = parent;
                parent->left = parent->parent;
                parent->parent = parent->parent->parent;
                parent->left->parent = parent;
                z->parent->color = false;
                z->parent->left->color = true;
                printf("6 ");
            }
        }
    }
    
    T->root->color = false;
}


void RBTree_Insert(RBTree* T, TNode* z){
    TNode* y = nullptr;
    TNode* x = T->root;
    while(x != nullptr and x->key != -1){
        y = x;
        if(z->key < x->key){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    
    if(y == nullptr){
        z->parent = T->nil;
        T->root = z;
    }else if(z->key < y->key){
        z->parent = y;
        y->left = z;
    }else{
        z->parent = y;
        y->right = z;
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = true;
    RBTree_Fix(T, z);
}


int main(){
    std::ifstream fin("insert.txt");
    int n;
    fin >> n;
    TNode nil = {false, -1, &nil, &nil, &nil};
    // nil.parent = &nil;
    // nil.left = &nil;
    // nil.right = &nil;
    RBTree T = {&nil, &nil};
    printf("sequence:\n");
    for(int i = 0; i < n; i++){
        int key;
        fin >> key;
        TNode* z = new TNode{true, key, T.nil, T.nil, T.nil};
        RBTree_Insert(&T, z);
        // std::cout << i << ": " << key << std::endl;
        // print_RBTree(T.root);
    }
    fin.close();

    // print
    // print_RBTree(T.root);

    // output
    // mid order
    std::ofstream fout("LNR.txt");
    fout << "mid order:\n";
    TNode* x = T.root;
    mid_order(x, fout);
    fout.close();

    // preorder
    fout.open("NLR.txt");
    fout << "preorder:\n";
    preorder(x, fout);
    fout.close();

    // right order
    fout.open("LOT.txt");
    fout << "level order:\n";
    level_order(x, fout);
    fout.close();

    putchar('\n');

}