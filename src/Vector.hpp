#pragma once
#include <cmath>
#include <memory>
#include <type_traits>

class Geometry {};

/*
 * Author: Ben Vesel
 * Date: 12/20/16
 * Classname: Vec3
 * Purpose: A generic three dimmensional vector with built in common functions.
 */
template <class T>
class Vec3 {

	public :
		T x;
		T y;
		T z;

		/* 
		 * Date: 12/20/16
		 * Function Name: Vec3 (constructor)
		 * Arguments:
		 *     T - generic first vector component
		 *     T - generic second vector component
		 *     T - generic third vector component
		 * Purpose: Constructor 
		 * Return Value: void
		 */
		Vec3(T a, T b, T c) {
			x = a;
			y = b;
			z = c;
		}

		/* 
		 * Date: 1/6/17
		 * Function Name: Vec3 (constructor)
		 * Arguments:
		 *     void
		 * Purpose: Constructor 
		 * Return Value: void
		 */
		Vec3() {
		}

		/*
		* Date: 12/28/16
		* Function Name: == (operator)
		* Arguments:
		*     const Vec3<T> - the second argument on a ==
		* Purpose: Equality operator
		* Return Value: bool
		*/
		bool operator ==(const Vec3<T> &b) {
			
			// Give some space for floating point errors when determining equality
			if ( (std::is_same<T, float>::value && std::is_same<T, float>::value) || (std::is_same<T, double>::value && std::is_same<T, double>::value) ) {

#define EPSILON 0.0001
				if (fabs(this->x - b.x) < EPSILON && fabs(this->y - b.y) < EPSILON && fabs(this->z - b.z) < EPSILON) {
					return true;
				}
				return false;
#undef EPSILON

			} else {

				if (this->x == b.x && this->y == b.y && this->z == b.z) {
					return true;
				}
				return false;
			}
		}

		/*
		* Date: 12/28/16
		* Function Name: != (operator)
		* Arguments:
		*     const Vec3<T> - the second argument on a !=
		* Purpose: Nonequality operator
		* Return Value: bool
		*/
		bool operator !=(const Vec3<T> &b) {
			if (this == b) {
				return false;
			}
			return true;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: * (operator) 
		 * Arguments:
		 *     const Vec3<T> - the second argument
		 * Purpose: Returns the dot product of the two vectors 
		 * Return Value: T
		 */
		T operator *(const Vec3<T> &b) {
			T temp = this->x * b.x + this->y * b.y + this->z * b.z;
			return temp;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: vec3 
		 * Arguments:
		 *     T - generic first vector component
		 *     T - generic second vector component
		 *     T - generic third vector component
		 * Purpose: Creates and returns a new three dimmensional vector 
		 * Return Value: Vec3<T>
		 */
		static Vec3<T> vec3(T x, T y, T z) {
			Vec3<T> vector(x, y, z);
			return vector;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: cross
		 * Arguments:
		 *     Vec3 - The first vector
		 *     Vec3 - The second vector
		 * Purpose: Computes the cross product of the two vectors 
		 * Return Value: Vec3<T>
		 */
		static Vec3<T> cross(Vec3 a, Vec3 b) {
			
			return Vec3::vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: dot
		 * Arguments:
		 *     Vec3 - The first vector
		 *     Vec3 - The second vector
		 * Purpose: Computes the dot product of the two vectors 
		 * Return Value: T
		 */
		static T dot(Vec3 a, Vec3 b) {
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: add
		 * Arguments:
		 *     Vec3 - The first vector
		 *     Vec3 - The second vector
		 * Purpose: Computes the addition of the two vectors 
		 * Return Value: Vec3<T>
		 */
		static Vec3<T> add(Vec3 a, Vec3 b) {
			return Vec3::vec3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: sub
		 * Arguments:
		 *     Vec3 - The first vector
		 *     Vec3 - The second vector
		 * Purpose: Computes the difference of the two vectors 
		 * Return Value: Vec3<T>
		 */
		static Vec3<T> sub(Vec3 a, Vec3 b) {
			return Vec3::vec3(a.x - b.x, a.y - b.y, a.z - b.z);;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: normalize
		 * Arguments:
		 *     Vec3 - The vector to normalize
		 * Purpose: Computes the normal or unit vector of a given vector
		 * Return Value: Vec3<T>
		 */
		static Vec3<T> normalize(Vec3 a) {
			double temp = Vec3::magnitude(a);
			Vec3<T> vector;
			if( temp == 0 ) {
				vector = Vec3::vec3(0, 0, 0);
			} else {
				vector = Vec3::vec3(a.x / temp, a.y / temp, a.z / temp);
			}

			return vector;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: magnitude
		 * Arguments:
		 *     Vec3 - A vector to compute the magnitude of
		 * Purpose: Computes the magnitude of a given vector
		 * Return Value: double
		 */
		static double magnitude(Vec3 a) {
			return sqrt( (double)(a.x * a.x + a.y * a.y + a.z * a.z));
		}

		/* 
		 * Date: 1/4/17
		 * Function Name: setValues
		 * Arguments:
		 *     T - first value of the vec3
		 *     T - second value of the vec3
		 *     T - third value of the vec3
		 * Purpose: Set the value of the given vector
		 * Return Value: void
		 */
		void setValues(T a, T b, T c) {
			this->x = a;
			this->y = b;
			this->z = c;
		}

};

/*
 * Author: Ben Vesel
 * Date: 12/20/16
 * Classname: Vec2
 * Purpose: A generic two dimmensional vector with built in common functions.
 */
template <class T>
class Vec2 {

	public :
		T x;
		T y;

		/* 
		 * Date: 12/20/16
		 * Function Name: Vec2 (constructor)
		 * Arguments:
		 *     T - generic first vector component
		 *     T - generic second vector component
		 * Purpose: Constructor 
		 * Return Value: void
		 */
		Vec2(T a, T b) {
			x = a;
			y = b;
		}

		/* 
		 * Date: 1/6/17
		 * Function Name: Vec2 (constructor)
		 * Arguments:
		 *     void
		 * Purpose: Constructor 
		 * Return Value: void
		 */
		Vec2() {
		}

		/*
		* Date: 12/28/16
		* Function Name: == (operator)
		* Arguments:
		*     const Vec2<T> - the second argument on a ==
		* Purpose: Equality operator
		* Return Value: bool
		*/
		bool operator ==(const Vec2<T> &b) {

			// Give some space for floating point errors when determining equality
			if ((std::is_same<T, float>::value && std::is_same<T, float>::value) || (std::is_same<T, double>::value && std::is_same<T, double>::value)) {

#define EPSILON 0.0001
				if (fabs(this->x - b.x) < EPSILON && fabs(this->y - b.y) < EPSILON) {
					return true;
				}
				return false;
#undef EPSILON

			}
			else {

				if (this->x == b.x && this->y == b.y) {
					return true;
				}
				return false;
			}
		}

		/*
		* Date: 12/28/16
		* Function Name: != (operator)
		* Arguments:
		*     const Vec2<T> - the second argument on a !=
		* Purpose: Nonequality operator
		* Return Value: bool
		*/
		bool operator !=(const Vec2<T> &b) {
			if (this == b) {
				return false;
			}
			return true;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: * (operator) 
		 * Arguments:
		 *     const Vec2<T> - the second argument
		 * Purpose: Returns the dot product of the two vectors 
		 * Return Value: T
		 */
		T operator *(const Vec2<T> &b) {
			T temp = this->x * b.x + this->y * b.y;
			return temp;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: vec2 
		 * Arguments:
		 *     T - generic first vector component
		 *     T - generic second vector component
		 *     T - generic third vector component
		 * Purpose: Creates and returns a new two dimmensional vector 
		 * Return Value: Vec2<T>
		 */
		static Vec2<T> vec2(T x, T y) {
			Vec2<T> vector(x, y);
			return vector;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: dot
		 * Arguments:
		 *     Vec2 - The first vector
		 *     Vec2 - The second vector
		 * Purpose: Computes the dot product of the two vectors 
		 * Return Value: T
		 */
		static T dot(Vec2 a, Vec2 b) {
			return a.x * b.x + a.y * b.y;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: add
		 * Arguments:
		 *     Vec2 - The first vector
		 *     Vec2 - The second vector
		 * Purpose: Computes the addition of the two vectors 
		 * Return Value: Vec2<T>
		 */
		static Vec2<T> add(Vec2 a, Vec2 b) {
			return Vec2::vec2(a.x + b.x, a.y + b.y);
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: sub
		 * Arguments:
		 *     Vec2 - The first vector
		 *     Vec2 - The second vector
		 * Purpose: Computes the difference of the two vectors 
		 * Return Value: Vec2<T>
		 */
		static Vec2<T> sub(Vec2 a, Vec2 b) {
			return Vec2::vec2(a.x - b.x, a.y - b.y);
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: normalize
		 * Arguments:
		 *     Vec2 - The vector to normalize
		 * Purpose: Computes the normal or unit vector of a given vector
		 * Return Value: Vec2<T>
		 */
		static Vec2<T> normalize(Vec2 a) {
			double temp = Vec2::magnitude(a);

			// Check for divide by zero exception
			if( temp == 0 ) {
				return Vec2::vec2(0, 0, 0);
			}
			return Vec2::vec2(a.x / temp, a.y / temp);
		}

		/* 
		 * Date: 1/4/17
		 * Function Name: magnitude
		 * Arguments:
		 *     Vec2 - A vector to compute the magnitude of
		 * Purpose: Computes the magnitude of a given vector
		 * Return Value: double
		 */
		static double magnitude(Vec2 a) {
			return sqrt( (double)(a.x * a.x + a.y * a.y));
		}


		/* 
		 * Date: 1/4/17
		 * Function Name: setValues
		 * Arguments:
		 *     T - first value of the vec2
		 *     T - second value of the vec2
		 * Purpose: Set the value of the given vector
		 * Return Value: void
		 */
		void setValues(T a, T b) {
			this->x = a;
			this->y = b;
		}

};
