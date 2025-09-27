#include <iostream>
#include <stdexcept>
#include <random>


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
	size_t  _size;
	size_t _capacity;
	

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

	Polyline(const Point<T>& p) : _size(1), _capacity(1) {
		_points = new Point<T>[1];
		_points[0] = p;
	}

	Polyline(size_t count_point) : _size(count_point),  _capacity(count_point) {
		if (count_point < 1) {
			throw std::invalid_argument("count_point can not be zero or negative")
		}

		_points = new Point<T>[count_point];

		for (int i = 0; i < _capacity; ++i) {
			_points[i] = Point<T>();
		}
	}

	Polyline(size_t count_point, T m1, T m2 ): _size(count_point), _capacity(count_point){
		if(count_point < 1) {
			throw std::invalid_argument("count_point can not be zero or negative");
		}
		if (m1 >= m2) {
			throw std::invalid_argument("m1 can not be bigger m2");
		}

		_points = new Point<T>[count_point];
		std::random_device rd;
		std::mt19937 gen(rd());

		if constexpr (std::is_integral_v<T>) {
			std::uniform_int_distribution<T> dist(m1, m2);
			for (size_t i = 0; i < count_point; ++i) {
				_points[i].x = dist(gen);
				_points[i].y = dist(gen);
			}
		}
		else if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> dist(m1, m2);
			for (size_t i = 0; i < count_point; ++i) {
				_points[i].x = dist(gen);
				_points[i].y = dist(gen);
			}
		}
		else if constexpr (std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>>) {
			using RealType = typename T::value_type;
			std::uniform_real_distribution<RealType> dist_real(m1.real(), m2.real());
			std::uniform_real_distribution<RealType> dist_imag(m1.imag(), m2.imag());
			for (size_t i = 0; i < count_point; ++i) {
				_points[i].x = T(dist_real(gen), dist_imag(gen));
				_points[i].y = T(dist_real(gen), dist_imag(gen));
			}
		}
		else {
			throw std::invalid_argument("Unsupported type for random generation");
		}

	}

	Polyline(const Polyline& other) : _size(other._size), _capacity(other._capacity) {
		_points = new Point<T>[_capacity];
		
		for (int i = 0; i < _size; ++i) {
			_points[i] = other._points[i]
		}
	}


	Point<T>& operator[](size_t index) {
		if (index < 0 or index >= _size) {
			throw std::out_of_range("index out of range");
		}

		return _points[index];
	}

	Polyline operator+(const Polyline& other) const {
		Polyline result(*this);

		result.resize(_size + other._size);


		for (int i = 0; i < other._size; ++i) {
			_points[_size + i] = other._points[i];
		}
		
		result._size += other._size;
	}
	
	Polyline operator+(const Point<T>& point) const {
		Polyline result(*this);

		if (result._capacity = result._size) {
			result.resize(_capacity + 1);
		}

		result[_size] = point;

		result._size ++;
	}

	friend Polyline<T> operator+(const Point<T>& point, const Polyline<T>& poly);


};


template <typename T>
Polyline<T> operator+(const Point<T>& point, const Polyline<T>& poly) {
	Polyline result(poly);

	if (result._capacity = result._size) {
		result.resize(_capacity + 1);
	}

	result[0] = point;
	for (int i = 0; i < result._size; ++i) {
		result._points[o + i] = poly._points[i];
	}

	result._size++;
}

int main() {
	Polyline<int> p1(5);
}