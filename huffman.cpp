 #include <iostream>
 #include <climits>
 #include <utility>
 #include <vector>
 #include <map>
 #include <algorithm>
 
 using std::string;
 
 #define COUNT 15
 

class node {
public:
    int value;
    string symbols;
    char code;
    node* left;
    node* right;
    node();
    node(int);
    node(int, string);
    node(node*, node*);
};


class tree {
public:
    node* head;
    tree();
    tree(int);
    tree(node*);
    void addElement(int);
    std::pair<bool, node*> inTree(int);
    void removeElement(int);
    std::vector<int> traverse();
    void traverseHelper(node*, std::vector<int>&);
    void traverseApplyCode();
    void applyCodeHelper(node*);
    std::map<string, string> traverseGetCode();
    void getCodeHelper(node*, string, string, std::map<string, string>&);
};


node::node(){
    this->value = INT_MAX;
    this->symbols = '\0';
    this->code = '\0';
    this->left = nullptr;
    this->right = nullptr;
};

node::node(int num) {
    this->value = num;
    this->symbols = '\0';
    this->code = '\0';
    this->left = nullptr;
    this->right = nullptr;    
};


node::node(int num, string c) {
    this->value = num;
    this->symbols = c;
    this->code = '\0';
    this->left = nullptr;
    this->right = nullptr;    
};

node::node(node* left, node* right) {
    this->value = left->value + right->value;
    this->symbols = left->symbols + right ->symbols;
    this->code = '\0';
    this->left = left;
    this->right = right;
}
tree::tree() {
    this->head = new node();
};

tree::tree(int first) {
    this->head = new node(first);
};

tree::tree(node* first) {
    this->head = first;
};

void tree::addElement(int num) {
    node* current_node = this->head;
    node* vanguard_node = this->head;
    
    if (this->head->value == INT_MAX) {
        this->head->value = num;
    } else {
        if (this->head->value >= num) {
            vanguard_node = current_node->left;
        } else {
            vanguard_node = current_node->right;
        };
        
        while(vanguard_node != nullptr) {
            if (vanguard_node->value >= num) {
                current_node = vanguard_node;
                vanguard_node = vanguard_node->left;
            } else {
                current_node = vanguard_node;
                vanguard_node = vanguard_node->right;
            };
        };
        
        if (current_node->value >= num) {
            current_node->left = new node(num);
        } else {
            current_node->right = new node(num);
        };        
    };
};

std::pair<bool, node*> tree::inTree(int num) {
    node* current_node = this->head;
    node* vanguard_node = this->head;
    
    if (current_node->value == num) {
        return { true, current_node };
    };
        
    if (current_node != nullptr) {
       if (this->head->value >= num) {
            vanguard_node = current_node->left;
        } else {
            vanguard_node = current_node->right;
        };       
    } else {
        return { false, nullptr };
    }
    
    while(vanguard_node != nullptr) {
        if (vanguard_node->value > num) {
            current_node = vanguard_node;
            vanguard_node = vanguard_node->left;
        } else if (vanguard_node->value < num) {
            current_node = vanguard_node;
            vanguard_node = vanguard_node->right;
        } else if (vanguard_node->value == num) {
            return { false, current_node };            
        };
    };
    return { false, nullptr };
    
    
};

void tree::removeElement(int num) {
    std::pair<bool, node*> result = this->inTree(num);  
    if (result.second == nullptr) {
        std::cout << "Element not found!" << std::endl;
        return;
    }
    
    // special case: remove head
    if (result.first == true) {
        node* element = result.second;
        
        if ((element->left != nullptr) && (element->right != nullptr)) {        
        // find smallest node to the right of the element to be removed i.e furthest left
        node* before_current = element->right;
        node* current_node = before_current->left;
        node* vanguard_node = (current_node == nullptr) ? nullptr : current_node->left;
        
        while (vanguard_node != nullptr) {
            before_current = current_node;
            current_node = vanguard_node;
            vanguard_node = vanguard_node->left;
        };
        // point new node's left/right to the children of the element to be removed
        node* newnode = new node(current_node->value);
        newnode->left = element->left;
        newnode->right = element->right;
        this->head = newnode;
        before_current->left = nullptr;
        delete element;
        //this->removeElement(current_node->value);    
        };
        return;
    };
    
    
    bool onleft = (result.second->left->value == num) ? true : false;
    node* element = (result.second->left->value == num) ? result.second->left : result.second->right;
    
    // simplest case: no child nodes
    if (element->left == nullptr && element->right == nullptr) {
        if (onleft) {
            result.second->left = nullptr;
        } else {
            result.second->right = nullptr;
        };
        delete element;
        return;
    };

    // one child node
    if ((element->left == nullptr) != (element->right == nullptr)) {        
        if (onleft) {
            node* child = (result.second->left->left != nullptr) ? result.second->left->left : result.second->left->right;
            result.second->left = child;
        } else {
            node* child = (result.second->right->left != nullptr) ? result.second->right->left : result.second->right->right;
            result.second->right = child;
        };
        delete element;        
        return;
    };
    
    // two child nodes
    
    if ((element->left != nullptr) && (element->right != nullptr)) {
        // find smallest node to the right of the element to be removed i.e furthest left
        node* before_current = element->right;
        node* current_node = before_current->left;
        node* vanguard_node = (current_node == nullptr) ? nullptr : current_node->left;
        while (vanguard_node != nullptr) {
            before_current = current_node;
            current_node = vanguard_node;
            vanguard_node = vanguard_node->left;
        };
        // point new node's left/right to the children of the element to be removed
        node* newnode = new node(current_node->value);
        newnode->left = element->left;
        newnode->right = element->right;
        if (onleft) {
            result.second->left = newnode;
        } else {
            result.second->right = newnode;
        };
        
        before_current->left = nullptr;
        
        //this->removeElement(current_node->value);
        return;
    };
};

std::vector<int> tree::traverse() {
    std::vector<int> result;
    
    traverseHelper(this->head, result);
    
    return result;
    
};

void tree::traverseHelper(node* next, std::vector<int>& order) {
    if (next != nullptr) {
        traverseHelper(next->left, order);
        order.push_back(next->value);
        traverseHelper(next->right, order);
    };
};

void tree::traverseApplyCode() {
    applyCodeHelper(this->head);
}

void tree::applyCodeHelper(node* next) {
    if (next != nullptr) {
        if (next->left != nullptr) {
            next->left->code = '0';
        }
        applyCodeHelper(next->left);
        if (next->right != nullptr) {
            next->right->code = '1';
        }
        applyCodeHelper(next->right);
    }
}

std::map<string, string> tree::traverseGetCode() {
    std::map<string, string> ret;
    getCodeHelper(this->head, std::string(), std::string(), ret);
    
    return ret;
}

void tree::getCodeHelper(node* next, string currentCode, string currentSymbol, std::map<string, string> &ret) {
    if (next != nullptr) {
        currentCode += next->code;
        getCodeHelper(next->left, currentCode, next->symbols, ret);
        getCodeHelper(next->right, currentCode, next->symbols, ret);
    } else {
        ret[currentSymbol] = currentCode;
    
    }
}

// Rebuild tree

node* balanceTreeHelper(std::vector<int> order, int start, int end) {
    if (start > end){
        return nullptr;
    };
    
    int mid = (start + end) / 2;
    node* newnode = new node(order[mid]);
    
    newnode->left = balanceTreeHelper(order, start, mid-1);
    newnode->right = balanceTreeHelper(order, mid+1, end);
    
    return newnode;
  
};

tree* balanceTree(tree unbalanced) {
    std::vector<int> in_order = unbalanced.traverse();
    tree* newtree = new tree();
    
    newtree->head = balanceTreeHelper(in_order, 0, in_order.size() - 1);
    
    return newtree;
};


void printHuffman2DUtil(node *root, int space)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    printHuffman2DUtil(root->right, space);
 
    // Print current node after space
    // count
    
    
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    //printf("%d\n", root->value);
    
    
    
    printf("%c", '\'');
    printf("%s", root->symbols.c_str());
    printf("%s", "'(");
    printf("%i", root->value);
    printf("%s", "): ");
    printf("%c\n", root->code);
    
 
    // Process left child
    printHuffman2DUtil(root->left, space);
}
 
// Prints huffman tree
void printHuffman2D(node *root)
{
    printHuffman2DUtil(root, 0);
}

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(node *root, int space)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    print2DUtil(root->right, space);
 
    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->value);
 
    // Process left child
    print2DUtil(root->left, space);
}
 
// Wrapper over print2DUtil()
void print2D(node *root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}


// Example tree insertions and balancing
void testTree()
{
    tree Tree = *new tree();
    Tree.addElement(50);
    Tree.addElement(100);
    Tree.addElement(150);
    Tree.addElement(200);
    Tree.addElement(250);
    Tree.addElement(75);
    Tree.addElement(74);
    Tree.addElement(99);
    Tree.addElement(125);
    Tree.addElement(175);
    Tree.addElement(225);
    Tree.addElement(49);
    
    print2D(Tree.head);

    std::vector<int> result = Tree.traverse();
    std::cout << "Elements: [";
    for (auto& element : result) {
        std::cout << ' ' << element << ' ';
    }
    std::cout << "]\n";
    
    tree balanced = *balanceTree(Tree);
    print2D(balanced.head);
    
    result = balanced.traverse();
    std::cout << "Elements: [";
    for (auto& element : result) {
        std::cout << ' ' << element << ' ';
    }
    std::cout << "]\n";
}

void testHuffman()
{
    string input;
    std::map<string, int> freqMap;
    std::vector<node*> nodelist;
    
    std::cout << "Enter string: " << '\n';
    getline(std::cin, input);
    
    for (auto& i : input) {
        freqMap[string(1,i)] += 1;
    }
    
    for (auto& i : freqMap) {
        nodelist.emplace_back(new node(i.second, i.first));
    }
    
    std::sort(nodelist.begin(), nodelist.end(), [](const node* left, const node* right) {
        return left->value > right->value;
    });

    do
    {
        node* left = nodelist.back();
        nodelist.pop_back();
        node* right = nodelist.back();
        nodelist.pop_back();
        nodelist.insert(std::upper_bound(nodelist.begin(), nodelist.end(), left->value + right->value, [](const int &left, const node* right) { return left > right->value; }), new node(left, right));
    } while (nodelist.size() > 1);

    
    tree Tree = *new tree(nodelist[0]);
    
    Tree.traverseApplyCode();
    
    printHuffman2D(nodelist[0]);
    
    std::cout << '\n';
    std::cout << "Keys: " << '\n';
    
    std::map<string, string> codes = Tree.traverseGetCode();
    
    for (auto& i : codes) {
        std::cout << i.first << ": " << i.second << '\n';
    }
    
    std::cout << '\n';
    std::cout << "Message: " << '\n';
    
    for (auto& i : input) {
        std::cout << codes[string(1, i)];
    }
    std::cout << '\n';
}



int main(void) {
    testHuffman();    
    return 0;
}
