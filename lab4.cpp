#include <iostream>
#include <vector>

struct Node{
    int data;
    Node* left;
    Node* right;

    Node(int value): data(value), left(nullptr), right(nullptr) {}
};

class Tree{
private:
    Node* root;

    Node* copy(Node* current_node){
        if (current_node == nullptr) return nullptr;

        Node* node = new Node(current_node->data);
        node->left = copy(current_node->left);
        node->right = copy(current_node->right);

        return node;
    }

    void destroy(Node* current){
        if (current == nullptr) return;
        
        destroy(current->left);  
        destroy(current->right);
        delete current;
    }

    void printNode(Node* node){

        if (node == nullptr) return;
        std::cout << node->data << " ";
    
        printNode(node->left);
        
        printNode(node->right);
    }

    int get_height(Node* node){
        if (node == nullptr) return 0;

        int left_height = get_height(node->left);
        int right_height = get_height(node->right);

        if(left_height >= right_height){
            return left_height + 1;
        }
        else{
            return right_height + 1;
        }

    }

    int diff(Node* node){
        if(node == nullptr) return 0;
        
        return get_height(node -> left ) - get_height(node -> right);
    }

    //балансировка ветвей
    Node* rotateRight(Node* root) {
        Node* newRoot = root->left;   
        root->left = newRoot->right;  
        newRoot->right = root;        
        return newRoot;               
    }

    Node* rotateLeft(Node* root) {
        Node* newRoot = root->right;  
        root->right = newRoot->left;  
        newRoot->left = root;         
        return newRoot;
    }

    Node* balance(Node* node) {
        int dif = diff(node);

      
        if (dif >= 2) {
            
            if (diff(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node); 
        }

       
        if (dif <= -2) {
           
            if (diff(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node); 
        }

        return node; 
    }


    Node* insertNode(Node* node, int key, bool& flag) {
        if (node == nullptr) {
            flag = true; 

            return new Node(key);
        }

        if (key < node->data) {
            node->left = insertNode(node->left, key, flag);
        } else if (key > node->data) {
            node->right = insertNode(node->right, key, flag);
        } else {
            flag = false; 
            return node;
        }

        return balance(node);
    }

    bool containsNode(Node* node, int key){
        if(node == nullptr) return false;
        if(node -> data == key) return true;

        if(key < node ->data){
            return containsNode(node -> left, key);
        }
        else{
            return containsNode(node -> right, key);
        }
    }

    Node* eraseNode(Node* node, int key, bool& flag){
        if (node == nullptr) {
            flag = false; 

            return nullptr;
        }

        if(key < node -> data){
            node->left = eraseNode(node->left, key, flag);
        }
        else if(key > node -> data){
            node->right = eraseNode(node->right, key, flag);
        }
        else{
            flag = true;

            //нет правой ветки
            if(node -> right == nullptr){
                Node* temp = node -> left;
                delete node;
                return temp;
            }
            //нет левой ветки
            else if(node -> left == nullptr){
                Node* temp = node -> right;
                delete node;
                return temp;
            }
            else{
                if (get_height(node->left) > get_height(node->right)) {
                    
                    Node* maxInLeft = node->left;
                    while (maxInLeft->right != nullptr) {
                        maxInLeft = maxInLeft->right;
                    }
                    node->data = maxInLeft->data;
                    node->left = eraseNode(node->left, maxInLeft->data, flag);
                } 
                else {
                    
                    Node* minInRight = node->right;
                    while (minInRight->left != nullptr) {
                        minInRight = minInRight->left;
                    }
                    node->data = minInRight->data;
                    node->right = eraseNode(node->right, minInRight->data, flag);
                }
            
                
            }
        }
        return balance(node);
    }

    bool isstrictly_balanced(Node* node){
        if(node == nullptr) return true;

        int dif = diff(node);

        if(dif != 0) return false;

        bool left = isstrictly_balanced(node -> left);
        bool right = isstrictly_balanced(node -> right);

        if(left && right){ 
            return true;
        }
        else{
            return false;
        }
    }

public:
    
    Tree(){
        root = nullptr;
    }

    Tree(const Tree& other_tree){
        root = copy(other_tree.root);
    }

    ~Tree(){
        destroy(root);
    }

    Tree& operator=(const Tree& other_tree){
        if(this == &other_tree){
            return *this;
        }

        destroy(root);
        root = copy(other_tree.root);
        return *this;
    }

    void print(){
        printNode(root);
        std::cout << std::endl;
    }

    bool insert(int key) {
        bool flag = false;
        root = insertNode(root, key, flag);
        return flag;
    }

    bool contains(int key){
        return containsNode(root, key);
    }

    bool erase(int key){
        bool flag = false;
        root = eraseNode(root, key, flag);
        return flag;
    }

    bool strictly_balanced(){
        return isstrictly_balanced(root);
    }

    void fillVector(Node* node, std::vector<int>& vect) const {
        if (node == nullptr) return;
        vect.push_back(node->data);
        fillVector(node->left, vect);
        fillVector(node->right, vect);
    }

    std::vector<int> getElements() const {
        std::vector<int> vect;
        fillVector(root, vect);
        return vect;
    }
};

bool treeintree(Tree& tree1, Tree& tree2){
    std::vector<int> elem1 = tree1.getElements();
    std::vector<int> elem2 = tree2.getElements();

    //1 во 2
    bool fl1 = true;
    for(int i = 0; i < elem1.size() ; i++){
        if(tree2.contains(elem1[i]) == false){
           fl1 =  false;
        } 
    }

    if(fl1) return true;

    //2 в 1
    bool fl2 = true;
    for(int i = 0; i < elem2.size() ; i++){
        if(tree1.contains(elem2[i]) == false){
            return false;
        } 
    }
    if(fl2) return true;

    return false;

}

int main(){
    Tree t1;
    Tree t2;

    t1.insert(30);
    t1.insert(20);
    t1.insert(40);
    t1.insert(45);
    
    t2.insert(10);
   
    
    bool fl3 = treeintree(t1, t2);

    // std::cout << t1.get_height();
}