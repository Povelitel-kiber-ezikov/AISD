#define _USE_MATH_DEFINES

#include <iostream>
#include <stdexcept>
#include <random>
#include <cmath>
#include <complex>
#include <type_traits>
#include <iomanip>




template <typename T>
class Point{
private:
	T x, y;
	static const double EPSILON;
	
public:

	Point() : x(0), y(0) {}; 
	Point(T x, T y) : x(x), y(y) {}; 

	T get_x() const { return x; }
	T get_y() const { return y; }

	bool operator==(const Point& other) const {
		if constexpr (std::is_floating_point_v<T>) {
			return std::abs(x - other.x) < EPSILON && std::abs(y - other.y) < EPSILON;
		}
		else if constexpr (std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>>) {
			using RealType = typename T::value_type;  

			return std::abs(x.real() - other.x.real()) < EPSILON
				&& std::abs(x.imag() - other.x.imag()) < EPSILON
				&& std::abs(y.real() - other.y.real()) < EPSILON
				&& std::abs(y.imag() - other.y.imag()) < EPSILON;
		}
		else {
			return x == other.x && y == other.y;
		}
	}


	bool operator!=(const Point& other) const {
		return !(*this == other);
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Point<T>& point) {
	os << "(" << point.get_x() << ", " << point.get_y() << ")";

	return os;
}


template <typename T>
const double Point<T>::EPSILON = pow(10, -7);

template <typename T>
class Polyline {
private:
	Point<T>* _points;
	size_t  _size;
	size_t _capacity;
	

	void resize(int new_capacity) {
		if (new_capacity <= _capacity) {
			return;
		}
		Point<T>* new_points = new Point<T>[new_capacity];
		for (int i = 0; i < _size; ++i) {
			new_points[i] = _points[i];
		}
		delete[] _points;
			
		_points = new_points;
		_capacity = new_capacity;
		
	}


public:

	Polyline(const Point<T>& p) : _size(1), _capacity(1) {
		_points = new Point<T>[1];
		_points[0] = p;
	}

	Polyline(size_t count_point) : _size(count_point),  _capacity(count_point) {
		if (count_point < 1) {
			throw std::invalid_argument("count_point can not be zero or negative");
		}

		_points = new Point<T>[count_point];

		for (int i = 0; i < _capacity; ++i) {
			_points[i] = Point<T>();
		}
	}

	
	Polyline(size_t count_point, T m1, T m2)
		: _size(count_point), _capacity(count_point)
	{
		if (count_point < 1) {
			throw std::invalid_argument("count_point can not be zero or negative");
		}
		if (m1 >= m2) {
			throw std::invalid_argument("m1 must be less than m2");
		}

		_points = new Point<T>[count_point];
		std::random_device rd;
		std::mt19937 gen(rd());

		if constexpr (std::is_integral_v<T>) {
			std::uniform_int_distribution<T> dist(m1, m2);
			for (size_t i = 0; i < count_point; ++i) {
				_points[i] = Point<T>(dist(gen), dist(gen));
			}
		}
		else if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> dist(m1, m2);
			for (size_t i = 0; i < count_point; ++i) {
				_points[i] = Point<T>(dist(gen), dist(gen));
			}
		}
		else if constexpr (std::is_same_v<T, std::complex<float>> ||
			std::is_same_v<T, std::complex<double>>) {
			using RealType = typename T::value_type;
			std::uniform_real_distribution<RealType> dist_real(m1.real(), m2.real());
			std::uniform_real_distribution<RealType> dist_imag(m1.imag(), m2.imag());
			for (size_t i = 0; i < count_point; ++i) {
				T x = T(dist_real(gen), dist_imag(gen));
				T y = T(dist_real(gen), dist_imag(gen));
				_points[i] = Point<T>(x, y);
			}
		}
		else {
			throw std::invalid_argument("Unsupported type for random generation");
		}
	}


	Polyline(const Polyline& other) : _size(other._size), _capacity(other._capacity) {
		_points = new Point<T>[_capacity];
		
		for (int i = 0; i < _size; ++i) {
			_points[i] = other._points[i];
		}
	}




	Point<T>& operator[](size_t index) const {
		if (index >= _size)
			throw std::out_of_range("index out of range");

		return _points[index];
	}

	Polyline operator+(const Polyline& other) const {
		Polyline result(*this);

		result.resize(_size + other._size);


		for (int i = 0; i < other._size; ++i) {
			result._points[_size + i] = other._points[i];
		}
		
		result._size += other._size;
		return result;
	}
	
	Polyline operator+(const Point<T>& point) const {
		Polyline result(*this);

		if (result._capacity == result._size) {
			result.resize(_capacity + 1);
		}

		result._points[_size] = point;

		result._size++;
		return result;
	}

	double length() const {
		double length = 0;

		if (_size < 2) {
			return 0;
		}

		

		for (int i = 0; i < _size - 1; ++i) {

			Point<T> p1 = _points[i];
			Point<T> p2 = _points[i+1];


			if constexpr (std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>>) {
				length += std::sqrt(std::norm(p2.get_x() - p1.get_x()) + std::norm(p2.get_y() - p1.get_y()));
			}
			else {
				length += std::sqrt(std::pow(p2.get_x() - p1.get_x(), 2) + std::pow(p2.get_y() - p1.get_y(), 2));
			}
		}
		return length;
	}
	



	bool operator==(const Polyline& other) const {
		if (_size != other._size) {
			return false;
		}

		for (int i = 0; i < _size; ++i) {
			if (_points[i] != other._points[i]) {
				return false;
			}
		}

		return true;
	}

	bool operator!=(const Polyline& other) const {
		return !(*this == other);
	}


	void helper(const Point<T>& point) {
		if (_size == _capacity) {
			resize(_capacity + 1);
		}

		for (int i = 0; i < _size; ++i) {
			_points[1 + i] = _points[i];
		}
		_points[0] = point;
		_size++;
	}

	size_t get_size() const {
		return _size;
	}


};


template <typename T>
Polyline<T> operator+(const Point<T>& point, const Polyline<T>& poly) {
	Polyline<T> result(poly);
	result.helper(point);
	return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Polyline<T>& poly) {
	size_t size = poly.get_size();
	os << "size = " << size << ": ";
	for (int i = 0; i < size; i++) {
		os << poly[i];
		if (i + 1 != size) {
			os << " - ";
		}
	}

	return os;
}


template <typename T>
Polyline<T> build_n_angle(size_t n, double radius) {
	Polyline<T> poly(n);
	const double central_x = 0;
	const double central_y = 0;
	//const double radius = 5;
	const double central_angle = (2.0 * M_PI) / double(n);

	for (int i = 0; i < n; ++i) {
		double angle = i * central_angle;
		double x = central_x + radius * std::cos(angle);
		double y = central_y + radius * std::sin(angle);


		poly[i] = Point<T>(static_cast<T>(x), static_cast<T>(y));

	}
	return poly;
}


int main() {
	try {
		std::cout << std::fixed << std::setprecision(3);
		Polyline<double> p1(Point<double>(1.5, 2.0));
		Polyline<double> p2(3);
		Polyline<double> p3(3, 1.0, 100.0);
		Polyline<double> p4 = p1;
		Polyline<double> p5 = build_n_angle<double>(3, 5);

		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
		std::cout << p3 << std::endl;
		std::cout << p4 << std::endl;
		std::cout << p5 << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}