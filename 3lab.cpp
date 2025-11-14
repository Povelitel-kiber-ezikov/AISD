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

    Node<T>*& get_head(){
        return head;
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

int main(){

    LinkedList<int> list1(5);
    std::vector<int> vect1 = list_for_vec(list1.get_head());

    std::cout<< "Before Bubble: " << list1 << std::endl;
    stats res1 = bubble_sort(list1.get_head());
    std::cout<< "After Bubble: " << list1 << std::endl << std::endl;
    std::cout << "Stats Bubble: " << std::endl << "comparison_count: " << res1.comparison_count << std::endl << "copy_count: " << res1.copy_count  << std::endl << std::endl;
    
    std::cout<< "Before Comb: "  << vect1 << std::endl;
    stats res2 = heap_sort(vect1);
    std::cout<< "After Comb: " << vect1 << std::endl << std::endl;
    std::cout << "Stats Comb: " << std::endl << "comparison_count: " << res2.comparison_count << std::endl << "copy_count: " << res2.copy_count  << std::endl << std::endl;

    return 0;
}