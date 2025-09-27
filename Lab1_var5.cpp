#include <iostream>

template <typename T>
struct Point{
private:
	T x, y;

public:

	Point() : x(0), y(0) {}; 
	Point(T x, T y) : x(x), y(y) {}; 

};


template <typename T>
class Polyline {
private:
	Point<T>* _points;
	int  _size;
	int _capacity;
	

	void resize(int new_capacity) {
		if (new_capacity <= capacity) {
			return;
		}
		Point<T>* new_point = new Point<T>[new_capacity];
		for (int i = 0; i < size; ++i) {
			new_points[i] = _points[i];
			delete[] _points;
			
			_points = new_point;
			_capacity = new_capacity;
		}
	}


public:


};


int main() {
	Polyline<int> p1;
}