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
	Vector3f m_center; 
	Vector3f m_direction;
	Vector3f m_up;
	Vector3f m_horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
		m_center = center;
		m_direction = direction;
		m_up = up;
		m_horizontal = Vector3f::cross(direction,up).normalized();
		screen_distance = 1.0f/tan(angle/2);
		m_angle = angle;

	}

	virtual Ray generateRay( const Vector2f& point){
		float x = point.x();
		float y = point.y();
		//Vector3f ray_dir = Vector3f((x*m_horizontal)+(y*m_up)+(screen_distance*m_direction)).normalized();
		Vector3f ray_dir = Vector3f(x*m_horizontal)+Vector3f(y*m_up)+Vector3f(screen_distance*m_direction);
		ray_dir = ray_dir.normalized();
		return Ray(m_center, ray_dir);
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
	float m_angle;
	float screen_distance;
};

#endif //CAMERA_H
