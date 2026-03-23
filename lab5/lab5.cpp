#include <cmath>
#include <random>
#include <iostream>
#include <algorithm>

template<typename T>
class Hash{
private:
    struct Node{
        T data;
        Node* next;
        int key; //k

        Node(int key, T data): data(data), key(key), next(nullptr){}
    };

    Node** table;
    const unsigned int a=2654435769; //a
    int capacity;    //M = 2^l
    int size;
    const int W = 32;  //w
    int l;

    //h(k) = (k * a mod 2^w) >> (w - l)
    int hash_function(int key){
        unsigned int uns_key = static_cast<unsigned int>(key);
        unsigned int hash = (uns_key * a); 
        return hash % capacity;
    }

    void clear(){
        if(table == nullptr) return;
        for(int i = 0; i < capacity; i++){
            Node* current = table[i];
            while(current){
                Node* temp = current;
                current = current -> next;
                delete temp;
            }
            table[i] = nullptr;
        }
    }

    void rehash() {
        int old_capacity = capacity;
        Node** old_table = table;

        capacity = old_capacity * 2; // Удваиваем
        l = (int)std::log2(capacity); // Обновляем l для хеш-функции
        
        table = new Node*[capacity];
        for(int i = 0; i < capacity; i++) table[i] = nullptr;

        for(int i = 0; i < old_capacity; i++) {
            Node* cur = old_table[i];
            while(cur) {
                Node* next_node = cur->next; // Сохраняем хвост
                
                int new_index = hash_function(cur->key);
                
                // Вставляем в начало новой цепочки
                cur->next = table[new_index];
                table[new_index] = cur;
                
                cur = next_node; // Переходим к сохраненному хвосту
            }
        }
        delete[] old_table;
    }


public:
    Hash(int  capacity): capacity(capacity){
        l = std::ceil(std::log2(capacity));
        table = new Node*[capacity];
        size = 0;
        for(int i = 0; i < capacity; i++){
            table[i] = nullptr;
        }
    }

    Hash(int capacity, int count_insert): Hash(capacity){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_key(1, 100000);

        for(int i = 0; i < count_insert; i++){
            insert(dis_key(gen), T());
            
        }
    }

    Hash(const Hash& other): l(other.l), capacity(other.capacity), size(other.size){
        table = new Node*[capacity];
        for(int i = 0; i < capacity; i++){
            table[i] = nullptr;
            Node* other_cur = other.table[i];
            while(other_cur){
                insert_or_assign(other_cur -> key, other_cur -> data);
                other_cur = other_cur -> next;
            }
        }
    }

    ~Hash(){
        clear();
        delete[] table;

    }
     
    Hash operator=(const Hash& other){
        if (this == &other) return *this;
        clear();
        l = other.l;
        capacity = other.capacity;
        size = other.size;

        table = new Node*[capacity];
        for(int i = 0; i < capacity; i++){
            table[i] = nullptr;
            Node* other_cur = other.table[i];
            while(other_cur){
                insert_or_assign(other_cur -> key, other_cur -> data);
                other_cur = other_cur -> next;
            }
        }

        return *this;
        
    }

    bool insert(int key, const T& value){ 
        if((size + 1) > capacity) rehash();

        int index = hash_function(key);
        Node* current = table[index];
        while(current){
            if(current -> key == key){
                return false;
            }
            current = current -> next;

        }
        Node* new_node = new Node(key, value);
        new_node ->next = table[index];
        table[index] = new_node;
        size++;
        return true;
    }

    void insert_or_assign(int key, T &value){
        if((size + 1) > capacity) rehash();

        int index = hash_function(key);
        Node* current = table[index];
        while(current){
            if(current -> key == key){
                current -> data = value;
                return;
            }
            current = current -> next;

        }
        Node* new_node = new Node(key, value);
        new_node ->next = table[index];
        table[index] = new_node;
        size++;

    }

    void print(){
        for(int i = 0; i < capacity; i++){
            std::cout<< "[" << i << "]: ";
            Node* cur = table[i];
            while(cur){
                std::cout << "[" << cur -> key << " | " << cur -> data << "]";
                if(cur -> next) std::cout << "->";
                cur = cur -> next;
            }
            std::cout << std::endl;
        }
    }

    bool contains(T &value){
        for (int i = 0; i < capacity; i++) {
            Node* cur = table[i];
            while (cur) {
                if (cur->data == value) return true;
                cur = cur->next;
            }
        }
        return false;
    }
    
    T* search(int key){
        int index = hash_function(key);
        Node* cur = table[index];
        while( cur){
            if(cur-> key == key) return &cur -> data;
            cur = cur -> next;
        }
        return nullptr;
    }

    bool erase(int key){
        int index = hash_function(key);
        Node* cur = table[index];
        Node* prev = nullptr;
        while(cur){
            if(cur -> key == key){
                if(prev){
                    prev -> next = cur -> next;
                }
                else{
                    table[index] = cur -> next;
                }
                size--;
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur -> next;
        }
        
        return false;
    }

    int count(int key){
        int counter = 0;
        int index = hash_function(key);
        Node* cur = table[index];
        while(cur){
            counter++;
            cur = cur -> next;
        }

        return counter;
    }

    int count_collisions(){
        int count_col = 0;
        for(int i = 0; i < capacity; i++){
            Node* cur = table[i];
            while(cur){
                if(cur->next) count_col++;
                cur = cur -> next;
            }
        }
        return count_col;
    }
};

int experement(int cap){
    int col = 0;
    for(int i = 0; i < 100; i++){
        Hash<int> h(cap, 24);
        col += h.count_collisions();
    }
    return col;
}

int main(){
    for(int i = 25; i <= 475; i +=50){
        double sum = experement(i);
        std::cout << sum / 100 << std::endl;
    }
}