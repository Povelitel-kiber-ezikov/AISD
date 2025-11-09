#include <iostream>
#include <stdexcept>
#include <random>

template <typename T>
struct Node{
    T data;
    Node* next;

    Node(const T& value): data(value), next(nullptr){}
};

template <typename T>
class LinkedList;

template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list);



template <typename T>
class LinkedList{
private:
    Node<T>* head;
    Node<T>* tail; 

    size_t size_list;

public:
    //конструкторы
    LinkedList(): head(nullptr), tail(nullptr), size_list(0) {}

    LinkedList(const LinkedList& other){
       if(other.size_list == 0)
       {
            head = nullptr;
            tail = nullptr;
            size_list = 0;
            return;
       }

       size_list = other.size_list;
        
       head = new Node<T>(other.head -> data);
       tail = head;

       Node<T>* current_ptr = other.head->next;

       for(int i = 1; i < size_list; ++i){
            Node<T>* new_node = new Node<T>(current_ptr -> data);
            tail -> next = new_node;
            tail = new_node;
            current_ptr = current_ptr -> next;
       }

       tail -> next = head;
    }

    LinkedList(const size_t size){
        if(size == 0){
            head = nullptr;
            tail = nullptr;
            size_list = 0;
            return;
        }


        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(1, 100);

        head = new Node<T>(dis(gen));
        tail = head;
        size_list = 1;

        for (int i = 1; i < size; ++i) {
            Node<T>* new_node = new Node<T>(dis(gen));
            tail->next = new_node;
            tail = new_node;
            size_list++;
        }

        tail->next = head;
    }


    ~LinkedList(){
        Node<T>* node = head;
        for(int i = 0; i < size_list; ++i){
            Node<T>* next_node = node->next;
            delete node;
            node = next_node;
        }
    }


    size_t get_size() const{
        return size_list;
    }

    LinkedList& operator=(const LinkedList& other){
        if(this == &other){
            return *this;
        }

        Node<T>* node = head;
        for(int i = 0; i < size_list; ++i){
            Node<T>* next_node = node->next;
            delete node;
            node = next_node;
        }

        size_list = other.size_list;
        
        head = new Node<T>(other.head -> data);
        tail = head;

        Node<T>* current_ptr = other.head->next;

        for(int i = 1; i < size_list; ++i){
            Node<T>* new_node = new Node<T>(current_ptr -> data);
            tail -> next = new_node;
            tail = new_node;
            current_ptr = current_ptr -> next;
        }

        tail -> next = head;

        return *this;
    }
    
    //чтение
    T operator[](const size_t index) const{
        if(index >= size_list){
            throw std::out_of_range("Index out of range");
        }
        else{
            Node<T>* node = head;
            for(int i = 0; i < index; ++i){
                node = node -> next;
            }
            return node ->data;
        }
        
    }
    //запись
    T& operator[](const size_t index){
        if(index >= size_list){
            throw std::out_of_range("Index out of range");
        }
        else{
            Node<T>* node = head;
            for(int i = 0; i < index; ++i){
                node = node -> next;
            }
            return node ->data;
        }
        
    }
   
    //добавление одного нода в хвост
    void push_tail(const T& value){
        Node<T>* new_node = new Node<T>(value);
    
        if(size_list == 0){
            head = new_node;
            tail = new_node;
            tail->next = head;
        }
        else{
            tail->next = new_node;
            tail = new_node;
            tail->next = head;
        }
        size_list++;
    }

    //добавление списка в хвост
    void push_tail(const LinkedList<T>& other){
        if(other.size_list == 0){ //добавляемый список пуст
            return;
        }
        if(size_list == 0){ //оригинальный пуст
            size_list = other.size_list;
        
            head = new Node<T>(other.head -> data);
            tail = head;

            Node<T>* current_ptr = other.head->next;

            for(int i = 1; i < size_list; ++i){
                Node<T>* new_node = new Node<T>(current_ptr -> data);
                tail -> next = new_node;
                tail = new_node;
                current_ptr = current_ptr -> next;
            }

            tail -> next = head;
        }
        else{
            Node<T>* node = other.head;
            for(int i = 0; i < other.size_list; ++i){
                Node<T>* new_node = new Node<T>(node->data);
                tail -> next = new_node;
                tail = new_node;
                node = node-> next;
            }
            tail->next = head;
            size_list += other.size_list;
        }
        
        
    }

    //добавление нода в голову
    void push_head(const T& value){
        Node<T>* new_node = new Node<T>(value);
    
        if(size_list == 0){
            head = new_node;
            tail = new_node;
            new_node->next = head;
        }
        else{
            new_node->next = head;
            head = new_node;
            tail->next = head;
        }
        size_list++;
    }

    //добавление списка в голову
    void push_head(const LinkedList<T>& other){
        if(other.size_list == 0){
            return;
        }
        if(size_list == 0){
             size_list = other.size_list;
        
            head = new Node<T>(other.head -> data);
            tail = head;

            Node<T>* current_ptr = other.head->next;

            for(int i = 1; i < size_list; ++i){
                Node<T>* new_node = new Node<T>(current_ptr -> data);
                tail -> next = new_node;
                tail = new_node;
                current_ptr = current_ptr -> next;
            }

            tail -> next = head;
        }
        else{
            Node<T>* new_head = new Node<T>(other.head -> data);
            Node<T>* new_tail = new_head;

            Node<T>* node = other.head->next ;
            for(int i = 1; i < other.size_list; ++i){
                Node<T>* new_node = new Node<T>(node -> data);
                new_tail -> next = new_node;
                new_tail = new_node;
                node = node -> next;
            }
            new_tail->next = head;
            head = new_head;
            tail->next = head;
            size_list += other.size_list;
        }

    }

    void pop_head(){
        if(size_list == 0) return;
        
        if(size_list == 1){
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        else{
            Node<T>* new_head = head->next;
            delete head;
            head = new_head;
            tail -> next = head;

        }
        size_list--;
    }

    void pop_tail(){
        if(size_list == 0) return;
        if(size_list == 1){
            delete tail;
            head = nullptr;
            tail = nullptr;
        }
        else{
            Node<T>* new_tail = head;
            while(new_tail->next != tail){
                new_tail = new_tail -> next;
            }
            delete tail;
            tail = new_tail;
            new_tail -> next = head;
        }
        size_list--;
    }


    void delete_node(const T& value){
        if(size_list == 0) return;
    
        size_t count_delete = 0;
    
        // проверка головы
        while(head->data == value && size_list > 0){
            if(size_list == 1){
                delete head;
                head = nullptr;
                tail = nullptr;
                size_list = 0;
                return;
            }
            Node<T>* new_head = head->next;
            delete head;
            head = new_head;
            tail->next = head;
            count_delete++;
        }
    
        // проверка тела
        Node<T>* node_to_check = head;
        for(int i = 0; i < size_list - 1; ++i){
            if(node_to_check -> next -> data == value){
                Node<T>* delete_node = node_to_check -> next;

                //проверка на хвост
                if(delete_node == tail){
                    tail = node_to_check;
                    tail -> next = head;
                    node_to_check -> next = head;
                    delete delete_node;
                    count_delete++;
                }
                else{
                    node_to_check -> next = node_to_check -> next -> next;
                    delete delete_node;
                    count_delete++;
                }
            }
        }
        size_list -= count_delete;
    }


    bool check_repetitions(const T value){
        Node<T>* node = head;
        int count = 0;
        for(int i = 0; i < size_list; ++i){
            if(node -> data == value){
                count++;
            }
            node = node -> next;
        }
        if(count > 1){
            return true;
        }
        return false;
    }


    friend std::ostream& operator<< <T>(std::ostream& os, const LinkedList<T>& list);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list){
    if(list.size_list == 0){
        os << "[]";
        return os;
    }

    os << "[";
    Node<T>* current = list.head;
    for(int i = 0; i < list.size_list; ++i){
        os << current->data;
        if(i < list.size_list - 1){
            os << " -> ";
        }
        current = current->next;
    }
    os << "]";
    return os;
}

template <typename T>
void sort_list(LinkedList<T>& list){
    if (list.get_size() == 0){
        return;
    }
    for(int i = 0; i < list.get_size() - 1; i++){
        for(int j = 0; j < list.get_size() - i  - 1; ++j){
            if (list[j] > list[j + 1]) {
                T temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }                                                                                         

                                                                                            
}

template <typename T>
LinkedList<T> combining_lists(const LinkedList<T>& list1,const LinkedList<T>& list2 ){
    LinkedList<T> result;
    LinkedList<T> temporary_res;
    
    for(size_t i = 0; i < list1.get_size(); ++i){
        temporary_res.push_tail(list1[i]);
    }

    //добавление второго списка
    for(size_t i = 0; i < list2.get_size(); ++i){
        temporary_res.push_tail(list2[i]);
    }

    sort_list(temporary_res);
    //удаление дубликатов
    for(size_t i = 0; i < temporary_res.get_size(); ++i){
        if(i == 0 || temporary_res[i] != temporary_res[i - 1]){
            result.push_tail(temporary_res[i]);
        }
    }
    
    
    return result;

}

int main(){
    
}