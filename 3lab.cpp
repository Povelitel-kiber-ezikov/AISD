#include <iostream>
#include <random>
#include <vector>

//односвязанный линейный список
template <typename T>
struct Node
{
    T data;
    Node* next;


    Node(T value): data(value), next(nullptr){}
};

template <typename T>
class LinkedList;

template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list);

template <typename T>
class LinkedList{
private:
    Node<T>* head;


public:
    LinkedList(): head(nullptr){}
    
    LinkedList(size_t count){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(1, 100);

        head = new Node<T>(dis(gen));
        Node<T>* cur = head;
        for(size_t i = 1; i < count; ++i){
            Node<T>* new_node = new Node<T>(dis(gen));
            cur->next = new_node;
            cur = cur->next;
        }
    }

    LinkedList(size_t count, bool flag){
        head = new Node<T>(1);
        Node<T>* cur = head;
        for(size_t i = 2; i <= count; ++i) {
            Node<T>* new_node = new Node<T>(i);
            cur->next = new_node;
            cur = cur->next;
        }
    }

    LinkedList(size_t count, int reverse_flag) {

    head = new Node<T>(count);
    Node<T>* cur = head;
    for(size_t i = count - 1; i > 0; --i) {
        Node<T>* new_node = new Node<T>(i);
        cur->next = new_node;
        cur = cur->next;
    }
}



    Node<T>*& get_head(){
        return head;
    }

    ~LinkedList(){
        Node<T>* cur = head;
        while(cur != nullptr){
            Node<T>* next = cur->next;
            delete cur;
            cur = next;
        }
    }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const LinkedList<U>& list);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list)
{
    Node<T>* cur = list.head;
    
    while(cur != nullptr) {
        os << cur->data;
        if(cur->next != nullptr) {
            os << " -> ";
        }
        cur = cur->next;
    }
    
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vect)
{
    for(size_t i = 0; i < vect.size(); ++i) {
        os << vect[i];
        if(i < vect.size() - 1) {
            os << " -> ";
        }
    }
    return os;
}

//сортировки
struct stats{
    size_t comparison_count = 0; //сравнения
    size_t copy_count = 0; 
};

template <typename T>
std::vector<T> list_for_vec(Node<T>*& head){
    std::vector<T> result;
    Node<T>* cur = head;

    while(cur != nullptr){
        result.push_back(cur->data);
        cur = cur->next;
    }
    return result;
}


template <typename T>
stats bubble_sort(Node<T>*& head){
    stats stat;

    if(head == nullptr || head -> next == nullptr){
        return stat;    
    }
    bool swap;
    Node<T>* end = nullptr;
    do{
        swap = false;
        Node<T>* cur = head;
        while(cur->next != end){
            stat.comparison_count++; 
            if(cur -> data  > cur ->next -> data){
                T temp = cur -> data;
                cur -> data = cur -> next -> data;
                cur -> next -> data = temp;
                swap = true;
                stat.copy_count += 3; 
            }
            cur = cur -> next;
        }  
        end = cur;
    }while(swap);

    return stat;
}


template <typename T>
stats comb_sort(std::vector<T>& vect){
    stats stat;
    size_t size = vect.size();
    int step =  size;
    bool swap = true;
    while(step > 1 || swap){
        if(step > 1){
            step = step/ 1.25;
        }

        swap = false;
        for(int i = 0; i+ step < size; ++i){
            stat.comparison_count++;
            if(vect[i] > vect[i + step]){
                T temp = vect[i];
                vect[i] = vect[i + step];
                vect[i + step] = temp;
                swap = true;
                stat.copy_count += 3;
            }
        }
    }
    return stat;
}


template <typename T>
stats heap_sort(std::vector<T>& vect){
    int size = vect.size();
    stats stat;
    
    for(int i = size/2 - 1; i >= 0; i--){
        int pos = i;
        while (true)
        {
            int largest = pos;
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;

            if(left < size){
                stat.comparison_count++;
                if(vect[left] > vect[largest]){
                    stat.comparison_count++;
                    largest = left;
                }
            }

            if(right < size){
                stat.comparison_count++;
                if(vect[right] > vect[largest]){
                    stat.comparison_count++;
                    largest = right;
                }
            }
            
            if(largest != pos){
                T temp = vect[pos];
                stat.copy_count += 3;
                vect[pos] = vect[largest];
                vect[largest] = temp;
                pos = largest;
            }else{
                break;
            }
        }
    }
    
    for(int i = size - 1; i >= 1; --i){
        T temp = vect[0];
        stat.copy_count += 3;
        vect[0] = vect[i];
        vect[i] = temp;

        int pos = 0;
        while(true){
            int largest = pos;
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;

            if(left < i){
                stat.comparison_count++;
                if(vect[left] > vect[largest]){
                    stat.comparison_count++;
                    largest = left;
                }
            }
            if(right < i){
                stat.comparison_count++;
                if(vect[right] > vect[largest]){
                    stat.comparison_count++;
                    largest = right;
                }
            }
            
            if(largest != pos){
                T temp = vect[pos];
                stat.copy_count += 3;
                vect[pos] = vect[largest];
                vect[largest] = temp;
                pos = largest;
            }else{
                break;
            }
        }
    }
    
    return stat;
}

template <typename T>
void print_bubble_stat(Node<T>*& head){

    stats stat = bubble_sort(head);
    std::cout << "Bubble sort stats:" << std::endl;
    std::cout << "Count comparison: "<< stat.comparison_count << std::endl;
    std::cout << "Count copy: "<< stat.copy_count << std::endl << std::endl;
}

template <typename T>
void print_comb_stat(std::vector<T>& vect){

    stats stat = comb_sort(vect);
    std::cout << "Comb sort stats:" << std::endl;
    std::cout << "Count comparison: "<< stat.comparison_count << std::endl;
    std::cout << "Count copy: "<< stat.copy_count << std::endl << std::endl;
}

template <typename T>
void print_heap_stat(std::vector<T>& vect){

    stats stat = heap_sort(vect);
    std::cout << "Heap sort stats:" << std::endl;
    std::cout << "Count comparison: "<< stat.comparison_count << std::endl;
    std::cout << "Count copy: "<< stat.copy_count << std::endl << std::endl;
}


int main(){

    stats average_bubble;
    stats average_comb;
    stats average_heap;

    std::vector<int> sizes = {1000, 2000, 3000, 5000, 10000, 25000, 50000, 100000};
    size_t iterat = 100;
    //2.а
    for(int size: sizes){
        std::cout << "Size: " << size << std::endl; 
        for(size_t j = 0; j<iterat; ++j){
            std::cout << "Iter: " << j << std::endl;
            LinkedList<int> list(size);
            std::vector<int> vect1 = list_for_vec(list.get_head());
            std::vector<int> vect2 = list_for_vec(list.get_head());

            stats bubble = bubble_sort(list.get_head());
            stats comb = comb_sort(vect1);
            stats heap = heap_sort(vect2);

            average_bubble.comparison_count += bubble.comparison_count;
            average_bubble.copy_count += bubble.copy_count;
            average_comb.comparison_count += comb.comparison_count;
            average_comb.copy_count += comb.copy_count;
            average_heap.comparison_count += heap.comparison_count;
            average_heap.copy_count += heap.copy_count;
        }
        std::cout << "Bubble:" << std::endl << average_bubble.comparison_count/iterat << " " << average_bubble.copy_count/iterat << std::endl;
        std::cout << "Comb:" << std::endl << average_comb.comparison_count/iterat << " " << average_comb.copy_count/iterat << std::endl;
        std::cout << "Heap:" << std::endl << average_heap.comparison_count/iterat << " " << average_heap.copy_count/iterat << std::endl;
    }

    //2.б
    for(int size: sizes){
        std::cout << size << std::endl;
        LinkedList<int> list(size, true);
        std::vector<int> vect1 = list_for_vec(list.get_head());
        std::vector<int> vect2 = list_for_vec(list.get_head());
        
        print_bubble_stat(list.get_head());
        print_comb_stat(vect1);
        print_heap_stat(vect2);
    }
    //2.в
    for(int size: sizes){
        std::cout << size << std::endl;
        LinkedList<int> list(size, -1);
        std::vector<int> vect1 = list_for_vec(list.get_head());
        std::vector<int> vect2 = list_for_vec(list.get_head());
        
        print_bubble_stat(list.get_head());
        print_comb_stat(vect1);
        print_heap_stat(vect2);
    }





    return 0;
}

