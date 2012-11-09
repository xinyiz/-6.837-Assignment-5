#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
using namespace std;
///TODO: Implement Plane representing an infinite plane
///choose your representation , add more fields and fill in the functions
class Plane: public Object3D
{
public:
	Plane(){
		m_normal = Vector3f(0,0,1.0f);
		m_d = 0.0f;
		m_material = new Material(Vector3f(0,0,0),Vector3f(0,0,0),0.0f);
	}
	Plane( const Vector3f& normal , float d , Material* m):Object3D(m){
		m_normal = normal;
		m_d = d;
		m_material = m;
	}
	~Plane(){}
	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		float projection = Vector3f::dot(m_normal,r.getDirection());
		if(projection != 0.0){
			float t_0 = -(-m_d + Vector3f::dot(m_normal,r.getOrigin()))/projection;
			if(t_0 > tmin and t_0 < h.getT()){
				h.set(t_0, this->material, m_normal);
				return true;
			}
		}
		return false;
	}

protected:
	Vector3f m_normal;
	float m_d;
	Material* m_material;

};
#endif //PLANE_H
		

