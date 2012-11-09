#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:

	Sphere(){ 
		m_center = Vector3f(0,0,0);
		m_radius = 1.0f;
	}

	Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
		m_center = center;
		m_radius = radius;
		this->material =  material;
	}
	
	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		//Initialize parameters
		Vector3f r_o = r.getOrigin() - m_center;
		float b = 2*Vector3f::dot(r.getDirection(), r_o);
		float c = Vector3f::dot(r_o,r_o) - m_radius*m_radius;
		float discrim = sqrt(b*b - 4*c);
		float t_0 = -1;
		//Find valid intersection at t_0
		if (discrim == 0){
			t_0 = -b/2;
		}
		if (discrim > 0){
			t_0 = 0.5*(-b + sqrt(discrim));
			float t_1 = 0.5*(-b - sqrt(discrim));
			if(t_0 < 0 and t_1 < 0)
				return false;
			if(t_0 > 0 and t_1 > 0){
				t_0 = min(t_0,t_1);
			}
			else{
				if(t_1 > 0)
					t_0 = t_1;
			}
		}

		//Update hit
		cout << h.getT() << '\n';
		if(t_0 > tmin and t_0 < h.getT()){
			Vector3f point = r.pointAtParameter(t_0);
			Vector3f norm = (point - m_center).normalized(); 
			h.set(t_0, this->material, norm);
			return true;
		}
		return false;
	}

protected:

	Vector3f m_center;
	float m_radius;

};


#endif
