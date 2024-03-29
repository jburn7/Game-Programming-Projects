#pragma once
#include "SFML\System\Vector2.hpp"

/* Vector2D is a generally usable 2D vector.  Most of the operator overloading code is patterned after the notes from 
	California Institue of Technology site: http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html .  
	Exact author unknown.
	May be missing some important functions but this should be enough to do most things.

	by Dean Lawson
	Champlain College
	2011
*/

#include "Trackable.h"
#include <cfloat>


class Vector2D:public Trackable
{
public:
	Vector2D( float x = 0.0f, float y = 0.0f );//constructor
	Vector2D( const Vector2D& rhs );//copy constructor
	Vector2D(const sf::Vector2i &rhs);
	Vector2D(const sf::Vector2f &rhs);
	~Vector2D();//destructor

	//math operators
	Vector2D& operator*(float mult);
	Vector2D& operator/(float mult);
	Vector2D& operator += ( const Vector2D& rhs );
	Vector2D& operator -= ( const Vector2D& rhs );
	Vector2D& operator *= ( float mult );
	Vector2D& operator /= ( float div );
	Vector2D& operator = ( const Vector2D& rhs );
	
	bool operator == ( const Vector2D& rhs );
	bool operator != ( const Vector2D& rhs );

	const Vector2D Vector2D::operator+(const Vector2D &other) const;
	const Vector2D Vector2D::operator-(const Vector2D &other) const;
	const Vector2D Vector2D::operator*(float mult) const;

	//getters and setters
	inline float getX() const { return mX; };
	inline float getY() const { return mY; };
	inline void setX( float x ) { mX = x; };
	inline void setY( float y ) { mY = y; };
	
	//length functions
	bool hasNonZeroLength() const;
	float getLength() const;
	float getLengthSquared() const;//more effecient than get length - use when comparing distances and actual distance is not needed

	void normalize();//makes vector a unit vector (length of 1)

private:
	float mX;
	float mY;
};

extern Vector2D gZeroVector2D;//useful when we need to return a pointer to a default vector from a function
