#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <cmath>

namespace fe {
	namespace math {
		/*********** Consts */
		const float PI = 3.14159265359f;

		/*********** Math */
		template<typename T>
		T abs(T _val) {
			return std::abs(_val);
		}
		
		template <typename T> int sign(T val) {
			return (T(0) < val) - (val < T(0));
		}

		template<typename T>
		T max(T _val1, T _val2) {
			return (_val1 > _val2 ? _val1 : _val2);
		}

		template<typename T>
		T min(T _val1, T _val2) {
			return (_val1 < _val2 ? _val1 : _val2);
		}

		template<typename T>
		T sqrt(T _value) {
			return std::sqrt(_value);
		}

		template<typename T>
		T pow(T _value, int _pow) {
			return std::pow(_value, _pow);
		}

		float randomClamped(); // returns random float within [-1.0, 1.0]

		/*********** Trigonometry */
		template<typename T>
		T sin(T _radians) {
			return std::sin(_radians);
		}

		template<typename T>
		T cos(T _radians) {
			return std::cos(_radians);
		}

		template<typename T>
		T atan2(T _y, T _x) { // return in radians
			return std::atan2(_y, _x);
		}

		template<typename T>
		T degToRad(T _deg) { 
			return (_deg / (T)180.0) * (PI);
		}

		template<typename T>
		T radToDeg(T _rad) { 
			return (_rad / PI) * (T)180.0;
		}

		/*********** Vector2 */
		template <typename T>
		sf::Vector2<T> perp2D(sf::Vector2<T> _vect1) { 
			return sf::Vector2<T>(-(_vect1.y),  _vect1.x);
		}
		
		template <typename T>
		float dotProduct(sf::Vector2<T> _vect1, sf::Vector2<T> _vect2) {
			return (_vect1.x * _vect2.x + _vect1.y * _vect2.y);
		}

		template <typename T>
		float length(sf::Vector2<T> _vect) {
			return std::sqrt(dotProduct(_vect, _vect));
		}

		template <typename T>
		float lengthSquare(sf::Vector2<T> _vect) {
			return dotProduct(_vect, _vect);
		}

		template <typename T>
		sf::Vector2<T> proj(sf::Vector2<T> _vect1, sf::Vector2<T> _vect2) {
			return (dotProduct(_vect1, _vect2) / lengthSquare(_vect2)) * _vect2;
		}

		template <typename T>
		sf::Vector2<T> perp(sf::Vector2<T> _vect1, sf::Vector2<T> _vect2) {
			return (_vect1 - proj(_vect1, _vect2));
		}

		template <typename T>
		sf::Vector2<T> normalize(sf::Vector2<T> _vect1) {
			return (_vect1 / length(_vect1));
		}

		template <typename T> 
		sf::Vector2<T> truncate(sf::Vector2<T> _vect, T _length) { // truncate vector to given length
			if (pow(_length, 2) < lengthSquare(_vect)) {
				return (normalize(_vect) * _length);
			}
			else {
				return _vect;
			}
		}
	}
}