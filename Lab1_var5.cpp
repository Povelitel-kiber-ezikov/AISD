#define _USE_MATH_DEFINES

#include <iostream>
#include <stdexcept>
#include <random>
#include <cmath>
#include <complex>
#include <type_traits>
#include <iomanip>

template <typename T>
class Point {
private:
	T _x, _y;
	static const double EPSILON;

public:
	Point() : _x(0), _y(0) {};
	Point(T x, T y) : _x(x), _y(y) {};

	T get_x() const { return _x; }
	T get_y() const { return _y; }

	bool operator==(const Point& other) const {
		if constexpr (std::is_floating_point_v<T>) {
			return std::abs(_x - other._x) < EPSILON && std::abs(_y - other._y) < EPSILON;
		}
		else if constexpr (std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>>) {
			using RealType = typename T::value_type;
			return std::abs(_x.real() - other._x.real()) < EPSILON
				&& std::abs(_x.imag() - other._x.imag()) < EPSILON
				&& std::abs(_y.real() - other._y.real()) < EPSILON
				&& std::abs(_y.imag() - other._y.imag()) < EPSILON;
		}
		else {
			return _x == other._x && _y == other._y;
		}
	}

	bool operator!=(const Point& other) const {
		return !(*this == other);
	}
};timyrlox


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

		// Проверка m1 >= m2 только для типов, поддерживающих сравнение (check m1 >= m2 only for types supporting comparison)
		if constexpr (!std::is_same_v<T, std::complex<float>> &&
			!std::is_same_v<T, std::complex<double>>) {
			if (m1 >= m2) {
				throw std::invalid_argument("m1 must be less than m2");
			}
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

	~Polyline(){
		delete[] _points;
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
Polyline<T> build_n_gon(size_t n, double radius) {
	Polyline<T> poly(n);
	const double center_x = 0;
	const double center_y = 0;
	const double central_angle = (2.0 * M_PI) / double(n);

	for (int i = 0; i < n; ++i) {
		double angle = i * central_angle;
		double x = center_x + radius * std::cos(angle);
		double y = center_y + radius * std::sin(angle);

		poly[i] = Point<T>(static_cast<T>(x), static_cast<T>(y));
	}
	return poly;
}

int main() {
	try {
		std::cout << std::fixed << std::setprecision(3);

		// Генератор случайных чисел 
		std::random_device rd;
		std::mt19937 gen(rd());

		std::cout << "=== Polyline Operations Demonstration ===" << std::endl;

		// 1. Случайная генерация параметров для ломаной 
		std::uniform_int_distribution<size_t> size_dist(3, 8);  // от 3 до 8 точек 
		std::uniform_int_distribution<int> coord_dist(-20, 20); // координаты от -20 до 20 

		size_t random_size = size_dist(gen);
		int min_coord = coord_dist(gen);
		int max_coord = min_coord + std::uniform_int_distribution<int>(5, 15)(gen);

		std::cout << "\n1. Randomly generated polyline:" << std::endl;
		std::cout << "Number of points: " << random_size << std::endl;
		std::cout << "Coordinate range: [" << min_coord << ", " << max_coord << "]" << std::endl;

		Polyline<int> random_poly(random_size, min_coord, max_coord);
		std::cout << "Random polyline: " << random_poly << std::endl;
		std::cout << "Length: " << random_poly.length() << std::endl;

		// 2. Создание случайной точки
		std::uniform_int_distribution<int> point_dist(-50, 50);
		Point<int> random_point(point_dist(gen), point_dist(gen));
		std::cout << "\n2. Randomly generated point: " << random_point << std::endl;

		// 3. Конструкторы с случайными параметрами
		std::cout << "\n3. Constructors:" << std::endl;
		Polyline<int> poly_from_point(random_point);
		std::cout << "From random point: " << poly_from_point << std::endl;

		size_t empty_size = std::uniform_int_distribution<size_t>(2, 5)(gen);
		Polyline<int> empty_poly(empty_size);
		std::cout << "Empty polyline (" << empty_size << " points): " << empty_poly << std::endl;

		Polyline<int> copy_poly(random_poly);
		std::cout << "Copy of random: " << copy_poly << std::endl;

		// 4. Оператор [] с случайными индексами
		std::cout << "\n4. Element access (operator[]):" << std::endl;
		std::uniform_int_distribution<size_t> index_dist(0, random_poly.get_size() - 1);
		size_t rand_index1 = index_dist(gen);

		std::cout << "Point at index " << rand_index1 << ": " << random_poly[rand_index1] << std::endl;
		
		// 5. Операторы сложения
		std::cout << "\n5. Addition operators:" << std::endl;
		Polyline<int> poly_plus_point = random_poly + random_point;
		std::cout << "Random polyline + random point: " << poly_plus_point << std::endl;

		Polyline<int> point_plus_poly = random_point + random_poly;
		std::cout << "Random point + random polyline: " << point_plus_poly << std::endl;

		Polyline<int> two_polys = random_poly + poly_from_point;
		std::cout << "Two random polylines: " << two_polys << std::endl;

		// 6. Операторы сравнения
		std::cout << "\n6. Comparison operators:" << std::endl;
		std::cout << "random_poly == copy_poly: " << (random_poly == copy_poly) << std::endl;
		std::cout << "random_poly != poly_from_point: " << (random_poly != poly_from_point) << std::endl;

		// 7. Геттер размер
		std::cout << "\n7. Polyline sizes:" << std::endl;
		std::cout << "Random polyline size: " << random_poly.get_size() << std::endl;
		std::cout << "Combined polyline size: " << two_polys.get_size() << std::endl;

		// 8. Правильные n-угольники со случайными параметрами 
		std::cout << "\n8. Regular polygons (random parameters):" << std::endl;
		size_t random_n = std::uniform_int_distribution<size_t>(3, 8)(gen);
		double random_radius = std::uniform_real_distribution<double>(2.0, 10.0)(gen);

		std::cout << "N = " << random_n << ", radius = " << random_radius << std::endl;
		Polyline<double> random_ngon = build_n_gon<double>(random_n, random_radius);
		std::cout << "Random " << random_n << "-gon: " << random_ngon << std::endl;
		std::cout << "Length: " << random_ngon.length() << std::endl;

		// 9. Случайная ломаная с вещественными числами 
		std::cout << "\n9. Random polyline with floating-point numbers:" << std::endl;
		std::uniform_real_distribution<double> double_dist(-5.0, 5.0);
		double min_double = double_dist(gen);
		double max_double = min_double + std::uniform_real_distribution<double>(1.0, 10.0)(gen);
		size_t double_size = std::uniform_int_distribution<size_t>(2, 6)(gen);

		Polyline<double> double_poly(double_size, min_double, max_double);
		std::cout << "Size: " << double_size << ", range: [" << min_double << ", " << max_double << "]" << std::endl;
		std::cout << "Random double polyline: " << double_poly << std::endl;
		std::cout << "Length: " << double_poly.length() << std::endl;

		// 10. Случайная комплексная ломаная 
		std::cout << "\n10. Random polyline with complex numbers:" << std::endl;
		std::uniform_real_distribution<double> complex_dist(-3.0, 3.0);
		std::complex<double> c_min(complex_dist(gen), complex_dist(gen));
		std::complex<double> c_max(c_min.real() + 2.0, c_min.imag() + 2.0);

		Polyline<std::complex<double>> complex_poly(2, c_min, c_max);
		std::cout << "Range: [" << c_min << ", " << c_max << "]" << std::endl;
		std::cout << "Complex polyline: " << complex_poly << std::endl;
		std::cout << "Length: " << complex_poly.length() << std::endl;

		
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
