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
        while(node_to_check -> next != head){
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
            else{
                node_to_check = node_to_check -> next;
            }
        }
        size_list -= count_delete;
    }


    friend std::ostream& operator<< <T>(std::ostream& os, const LinkedList<T>& list);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list){
    if(list.size_list == 0){
        os << "[]";
        return os;
    }

    os << "[-> ";
    Node<T>* current = list.head;
    for(int i = 0; i < list.size_list; ++i){
        os << current->data;
        if(i < list.size_list){
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

    
    for(size_t i = 0; i < list2.get_size(); ++i){
        temporary_res.push_tail(list2[i]);
    }

    sort_list(temporary_res);
    
    for(size_t i = 0; i < temporary_res.get_size(); ++i){
        if(i == 0 || temporary_res[i] != temporary_res[i - 1]){
            result.push_tail(temporary_res[i]);
        }
    }
    
    
    return result;

}

int main(){

    LinkedList<int> list166;
    list166.push_tail(1);
    list166.push_tail(1);
    list166.push_tail(1);
    list166.push_tail(1);
    list166.push_tail(2);
    list166.push_tail(1);
    list166.push_tail(1);
    list166.push_tail(3);
    list166.push_tail(3);
    list166.push_tail(1);
    list166.push_tail(1);
    list166.push_tail(1);

    std::cout << list166 << std::endl;
    list166.delete_node(1);
    std::cout << list166 << std::endl << list166.get_size();



    // std::cout << "=== DEMONSTRATING CONSTRUCTORS ===" << std::endl << std::endl;
    
    // // Constructor 1: empty constructor
    // std::cout << "1. Empty constructor:" << std::endl;
    // LinkedList<int> list1;
    // std::cout << "list1 = " << list1 << ", size = " << list1.get_size() << std::endl << std::endl;
    
    // // Constructor 2: with size parameter (random numbers)
    // std::cout << "2. Constructor with size (5 random elements):" << std::endl;
    // LinkedList<float> list2(5);
    // std::cout << "list2 = " << list2 << ", size = " << list2.get_size() << std::endl << std::endl;
    
    // // Constructor 3: copy constructor
    // std::cout << "3. Copy constructor:" << std::endl;
    // LinkedList<float> list3 = list2;
    // std::cout << "list3 (copy of list2) = " << list3 << ", size = " << list3.get_size() << std::endl << std::endl;


    


    // std::cout << "=== DEMONSTRATING push_tail ===" << std::endl << std::endl;
    
    // LinkedList<int> list4;
    // std::cout << "Adding elements to tail:" << std::endl;
    // list4.push_tail(5);
    // std::cout << "After push_tail(5): " << list4 << std::endl;
    // list4.push_tail(15);
    // std::cout << "After push_tail(15): " << list4 << std::endl;
    // list4.push_tail(25);
    // std::cout << "After push_tail(25): " << list4 << std::endl << std::endl;


    // std::cout << "=== DEMONSTRATING push_head ===" << std::endl << std::endl;
    
    // std::cout << "Adding elements to head:" << std::endl;
    // list4.push_head(1);
    // std::cout << "After push_head(1): " << list4 << std::endl;
    // list4.push_head(0);
    // std::cout << "After push_head(0): " << list4 << std::endl << std::endl;


    // std::cout << "=== DEMONSTRATING pop_head AND pop_tail ===" << std::endl << std::endl;
    
    // LinkedList<int> list5;
    // list5.push_tail(10);
    // list5.push_tail(20);
    // list5.push_tail(30);
    // list5.push_tail(40);
    // list5.push_tail(50);
    // std::cout << "Original list: " << list5 << std::endl;
    
    // list5.pop_head();
    // std::cout << "After pop_head(): " << list5 << std::endl;
    
    // list5.pop_tail();
    // std::cout << "After pop_tail(): " << list5 << std::endl << std::endl;


    // std::cout << "=== DEMONSTRATING push_tail(LinkedList) ===" << std::endl << std::endl;
    
    // LinkedList<int> list6, list7;
    // list6.push_tail(1);
    // list6.push_tail(2);
    // list6.push_tail(3);
    
    // list7.push_tail(4);
    // list7.push_tail(5);
    
    // std::cout << "list6 = " << list6 << std::endl;
    // std::cout << "list7 = " << list7 << std::endl;
    // list6.push_tail(list7);
    // std::cout << "After list6.push_tail(list7): " << list6 << std::endl << std::endl;


    // std::cout << "=== DEMONSTRATING push_head(LinkedList) ===" << std::endl << std::endl;
    
    // LinkedList<int> list8, list9;
    // list8.push_tail(3);
    // list8.push_tail(4);
    // list8.push_tail(5);
    
    // list9.push_tail(1);
    // list9.push_tail(2);
    
    // std::cout << "list8 = " << list8 << std::endl;
    // std::cout << "list9 = " << list9 << std::endl;
    // list8.push_head(list9);
    // std::cout << "After list8.push_head(list9): " << list8 << std::endl << std::endl;


    // std::cout << "=== DEMONSTRATING delete_node ===" << std::endl << std::endl;
    
    // LinkedList<int> list10;
    // list10.push_tail(5);
    // list10.push_tail(3);
    // list10.push_tail(5);
    // list10.push_tail(7);
    // list10.push_tail(5);
    // std::cout << "Original list: " << list10 << std::endl;
    
    // list10.delete_node(5);
    // std::cout << "After delete_node(5): " << list10 << std::endl << std::endl;


    // std::cout << "=== DEMONSTRATING check_repetitions ===" << std::endl << std::endl;
    
    // LinkedList<int> list11;
    // list11.push_tail(10);
    // list11.push_tail(20);
    // list11.push_tail(10);
    
    // std::cout << "list11 = " << list11 << std::endl;
    // std::cout << "check_repetitions(10): " << (list11.check_repetitions(10) ? "true (duplicates found)" : "false") << std::endl;
    // std::cout << "check_repetitions(20): " << (list11.check_repetitions(20) ? "true (duplicates found)" : "false") << std::endl << std::endl;


    // std::cout << "=== DEMONSTRATING sort_list ===" << std::endl << std::endl;
    
    // LinkedList<int> list12;
    // list12.push_tail(45);
    // list12.push_tail(12);
    // list12.push_tail(78);
    // list12.push_tail(3);
    // list12.push_tail(56);
    // std::cout << "Before sorting: " << list12 << std::endl;
    
    // sort_list(list12);
    // std::cout << "After sort_list(): " << list12 << std::endl << std::endl;


    // std::cout << "=== DEMONSTRATING combining_lists ===" << std::endl << std::endl;
    
    // LinkedList<int> list13, list14;
    // list13.push_tail(3);
    // list13.push_tail(1);
    // list13.push_tail(4);
    
    // list14.push_tail(1);
    // list14.push_tail(5);
    // list14.push_tail(9);
    
    // std::cout << "list13 = " << list13 << std::endl;
    // std::cout << "list14 = " << list14 << std::endl;
    
    // LinkedList<int> combined = combining_lists(list13, list14);
    // std::cout << "combining_lists(list13, list14) = " << combined << std::endl;
    // std::cout << "(merging, sorting and removing duplicates)" << std::endl << std::endl;


   


  
    
    return 0;
}
