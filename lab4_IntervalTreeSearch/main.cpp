#include<iostream>
#include<fstream>

// interval tree node
struct TNode{
    bool color; // true: red, false: black
    int key;
    TNode *left, *right, *parent;
    int max_subtree;
    int max;
};

auto& out = std::cout;

struct RBTree{
    TNode* root;
    TNode* nil;
};

// char color(TNode* x){
//     if(x->color == true) return 'R';
//     else return 'B';
// }

// void mid_order(TNode* x, std::ofstream& fout){
//     if(x != nullptr and x->key != -1){
//         mid_order(x->left, fout);
//         fout << x->key << color(x) << " ";
//         mid_order(x->right, fout);
//     }
// }

// void preorder(TNode* x, std::ofstream& fout){
//     if(x != nullptr and x->key != -1){
//         fout << x->key << color(x) << " ";
//         preorder(x->left, fout);
//         preorder(x->right, fout);
//     }
// }

// // level order of transverse
// void level_order(TNode* x, std::ofstream& fout){
//     // FIFO
//     TNode* queue[100];
//     int front = 0, rear = 0;
//     queue[rear++] = x;
//     while(front != rear){
//         TNode* node = queue[front++];
//         if(node->key != -1){
//             fout << node->key << color(node) << " ";
//             if(node->left != nullptr){
//                 queue[rear++] = node->left;
//             }
//             if(node->right != nullptr){
//                 queue[rear++] = node->right;
//             }
//         }
//     }
// }

// print RBtree in level order
void print_RBTree(TNode* x){
    // FIFO
    TNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = x;
    while(front != rear){
        TNode* node = queue[front++];
        if(node->key != -1){
            std::cout << node->key << " " << node->max << " " << node->max_subtree << std::endl;
            if(node->left != nullptr){
                queue[rear++] = node->left;
            }
            if(node->right != nullptr){
                queue[rear++] = node->right;
            }
        }
    }
}


// maintain max_subtree (only rotation may change max_subtree)
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
                // printf("1 ");
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
                    // maintain max_subtree for z and parent
                    parent->max_subtree = std::max(std::max(parent->right->max_subtree, parent->left->max_subtree), parent->max);
                    z->max_subtree = std::max(std::max(z->right->max_subtree, z->left->max_subtree), z->max);
                    

                    z = parent;
                    // printf("2 ");
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
                // maintain max_subtree for z and parent
                parent->right->max_subtree = std::max(std::max(parent->right->right->max_subtree, parent->right->left->max_subtree), parent->right->max);
                parent->max_subtree = std::max(std::max(parent->right->max_subtree, parent->left->max_subtree), parent->max);
                
                // printf("3 ");
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
                // printf("4 ");
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
                    // maintain max_subtree for z and parent
                    parent->max_subtree = std::max(std::max(parent->right->max_subtree, parent->left->max_subtree), parent->max);
                    z->max_subtree = std::max(std::max(z->right->max_subtree, z->left->max_subtree), z->max);
                    
                    z = parent;   
                    // printf("5 ");
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
                // maintain max_subtree for z and parent
                parent->left->max_subtree = std::max(std::max(parent->left->right->max_subtree, parent->left->left->max_subtree), parent->left->max);
                parent->max_subtree = std::max(std::max(parent->right->max_subtree, parent->left->max_subtree), parent->max);
                
                // printf("6 ");
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
        while (y != T->nil){
            y->max_subtree = std::max(std::max(y->right->max_subtree, y->left->max_subtree), y->max);
            y = y->parent;
        }
    }else{
        z->parent = y;
        y->right = z;
        while (y != T->nil){
            y->max_subtree = std::max(std::max(y->right->max_subtree, y->left->max_subtree), y->max);
            y = y->parent;
        }
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = true;
    
    RBTree_Fix(T, z);
}

void interval_tree_search(int s_min, int s_max, TNode* x){
    // boundary condition
    if(x->key == -1){
        return;
    }
    // pruning case 1: subtree_max < s_min
    if(x->max_subtree < s_min){
        return;
    }
    // pruning case 2: min > s_max
    if(x->key > s_max){
        interval_tree_search(s_min, s_max, x->left);
        return;
    }
    // other case
    if ((x->key >= s_min and x->key <= s_max) or (x->max <= s_max and x->max >= s_min) or 
        (x->key <= s_min and x->max >= s_max) or (x->key >= s_min and x->max <= s_max)){
        printf("found: [%d %d]\n", x->key, x->max);
    }
    interval_tree_search(s_min, s_max, x->left);
    interval_tree_search(s_min, s_max, x->right);
}


int main(){
    // interval tree setup
    std::ifstream fin("insert.txt");
    int n;
    fin >> n;
    TNode nil = {false, -1, &nil, &nil, &nil, -1000, -1000};
    RBTree T = {&nil, &nil};
    for(int i = 0; i < n; i++){
        int min, max;
        fin >> min >> max;
        TNode* z = new TNode{true, min, T.nil, T.nil, T.nil, max, max};
        RBTree_Insert(&T, z);
    }
    fin.close();

    // print_RBTree(T.root);

    // search
    while (true){
        printf("search interval: \n");
        int s_min, s_max;
        std::cin >> s_min >> s_max;
        TNode* z = T.root;
        interval_tree_search(s_min, s_max, z);
    }
}
