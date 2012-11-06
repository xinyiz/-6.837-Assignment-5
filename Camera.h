#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 

protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
		this->center = center;
		this->direction = direction;
		this->up = up;
		this->horizontal = Vector3f::cross(direction,up);
		screen_distance = 1/tan(angle/2);
		m_angle = angle;

	}

	virtual Ray generateRay( const Vector2f& point){
		float x = point.x();
		float y = point.y();
		Vector3f ray_dir = Vector3f(x*horizontal+y*up+screen_distance*direction).normalized();
		return Ray(center, ray_dir);
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
	float m_angle;
	float screen_distance;
};

#endif //CAMERA_H
