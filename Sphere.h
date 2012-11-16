#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
#define _USE_MATH_DEFINES
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:

	Sphere(){ 
		hasTex = false;
		m_center = Vector3f(0,0,0);
		m_radius = 1.0f;
	}

	Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
		hasTex = false;
		m_center = center;
		m_radius = radius;
		this->material =  material;
	}
	
	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		//Initialize parameters
		Vector3f r_o = r.getOrigin() - m_center;
		float b = 2*Vector3f::dot(r.getDirection().normalized(), r_o);
		float c = Vector3f::dot(r_o,r_o) - m_radius*m_radius;
		float discrim = b*b - 4*c;
		float t_0 = -1;
		//Find valid intersection at t_0
		if (discrim == 0){
			t_0 = -b/2.0f;
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
		//cout << h.getT() << '\n';
		if(t_0 > tmin and t_0 < h.getT()){
			Vector3f point = r.pointAtParameter(t_0);
			Vector3f norm = (point - m_center).normalized(); 
			h.set(t_0, this->material, norm);
			float tex_u;
			float tex_v;
			Vector3f v_n = Vector3f(0,1.0f,0);
			Vector3f v_e = Vector3f(0,0,1.0f);
			float phi = acos(-Vector3f::dot(v_n,norm));
			tex_v = phi/(M_PI);
			float theta = acos(Vector3f::dot(norm,v_e)/sin(phi))/(2*M_PI);
			tex_u = theta;
			if(Vector3f::dot(Vector3f::cross(v_e,v_n),norm) < 0)
				tex_u = 1-tex_u;
			Vector2f tex = Vector2f(tex_u,tex_v);
			h.setTexCoord(tex);
			return true;
		}
		return false;
	}
	bool hasTex;
protected:

	Vector3f m_center;
	float m_radius;

};


#endif
