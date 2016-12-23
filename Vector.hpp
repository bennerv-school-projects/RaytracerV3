#pragma once
#include <math.h>
/* 
 * Notes: Must free all memory allocated using the delete keyword on every vec3 or vec2 function call.
 */


enum Delete { DELETE_FIRST, DELETE_SECOND, DELETE_ALL, DELETE_NONE };

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
		 * Return Value: 
		 */
		Vec3(T a, T b, T c) {
			x = a;
			y = b;
			z = c;
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
		static Vec3<T> * vec3(T x, T y, T z) {
			Vec3<T> * vector = new Vec3<T>(x, y, z);
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
		static Vec3<T> * cross(Vec3 * a, Vec3 * b, Delete del = DELETE_NONE) {
			Vec3<T> * vector =  Vec3::vec3(a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x);
			switch(del) {
				case DELETE_FIRST :
					delete a;
					break;
				case DELETE_SECOND :
					delete b;
					break;
				case DELETE_ALL :
					delete a;
					delete b;
					break;
				case DELETE_NONE :
				default :
					break;
			}
			return vector;
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
		static T dot(Vec3 * a, Vec3 * b, Delete del = DELETE_NONE) {
			T temp =  a->x * b->x + a->y * b->y + a->z * b->z;
			
			switch(del) {
				case DELETE_FIRST :
					delete a;
					break;
				case DELETE_SECOND :
					delete b;
					break;
				case DELETE_ALL :
					delete a;
					delete b;
					break;
				case DELETE_NONE :
				default :
					break;
			}

			return temp;
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
		static Vec3<T> * add(Vec3 * a, Vec3 * b, Delete del = DELETE_NONE) {
			Vec3<T> *  vector =  Vec3::vec3(a->x + b->x, a->y + b->y, a->z + b->z);
			
			switch(del) {
				case DELETE_FIRST :
					delete a;
					break;
				case DELETE_SECOND :
					delete b;
					break;
				case DELETE_ALL :
					delete a;
					delete b;
					break;
				case DELETE_NONE :
				default :
					break;
			}

			return vector;
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
		static Vec3<T> * sub(Vec3 * a, Vec3 * b, Delete del = DELETE_NONE) {
			Vec3<T> * vector =  Vec3::vec3(a->x - b->x, a->y - b->y, a->z - b->z);
				
			switch(del) {
				case DELETE_FIRST :
					delete a;
					break;
				case DELETE_SECOND :
					delete b;
					break;
				case DELETE_ALL :
					delete a;
					delete b;
					break;
				case DELETE_NONE :
				default :
					break;
			}

			return vector;
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: normalize
		 * Arguments:
		 *     Vec3 - The vector to normalize
		 * Purpose: Computes the normal or unit vector of a given vector
		 * Return Value: Vec3<T>
		 */
		static Vec3<T> * normalize(Vec3 * a, Delete del = DELETE_NONE) {
			double temp = Vec3::magnitude(a);
			Vec3<T> * vector =  Vec3::vec3(a->x / temp, a->y / temp, a->z / temp);
			
			switch(del) {
				case DELETE_FIRST :
					delete a;
					break;
					break;
				case DELETE_ALL :
					delete a;
					break;
				case DELETE_SECOND :
				case DELETE_NONE :
				default :
					break;
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
		static double magnitude(Vec3 * a) {
			return sqrt( (double)(a->x * a->x + a->y * a->y + a->z * a->z));
		}

};

/*
 * Author: Ben Vesel
 * Date: 12/20/16
 * Classname: Vec3
 * Purpose: A generic two dimmensional vector with built in common functions.
 */
template <class T>
class Vec2 {

	public :
		T x;
		T y;

		Vec2(T a, T b) {
			x = a;
			y = b;
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
		static Vec2<T> * vec2(T x, T y) {
			Vec2<T> * vector = new Vec2<T>(x, y);
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
		static T dot(Vec2 * a, Vec2 * b) {
			return a->x * b->x + a->y * b->y;
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
		static Vec2<T> * add(Vec2 * a, Vec2 * b) {
			return Vec2::vec2(a->x + b->x, a->y + b->y);
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
		static Vec2<T> * sub(Vec2 * a, Vec2 * b) {
			return Vec2::vec2(a->x - b->x, a->y - b->y);
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: normalize
		 * Arguments:
		 *     Vec2 - The vector to normalize
		 * Purpose: Computes the normal or unit vector of a given vector
		 * Return Value: Vec2<T>
		 */
		static Vec2<T> * normalize(Vec2 * a) {
			double temp = Vec2::magnitude(a);
			return Vec2::vec2(a->x / temp, a->y / temp);
		}

		/* 
		 * Date: 12/20/16
		 * Function Name: magnitude
		 * Arguments:
		 *     Vec2 - A vector to compute the magnitude of
		 * Purpose: Computes the magnitude of a given vector
		 * Return Value: double
		 */
		static double magnitude(Vec2 * a) {
			return sqrt( (double)(a->x * a->x + a->y * a->y));
		}

};
