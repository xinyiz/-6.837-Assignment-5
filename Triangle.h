#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
///TODO: implement this class.
///Add more fields as necessary,
///but do not remove hasTex, normals or texCoords
///they are filled in by other components
class Triangle: public Object3D
{
public:
	Triangle();
        ///@param a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object3D(m){
          hasTex = false;
          m_a = a;
          m_b = b;
          m_c = c;
	}

	virtual bool intersect( const Ray& r,  Hit& h , float tmin){
		Matrix3f MA = Matrix3f(m_a.x()-m_b.x(), m_a.x()-m_c.x(), r.getDirection().x(),m_a.y()-m_b.y(), m_a.y()-m_c.y(), r.getDirection().y(),m_a.z()-m_b.z(), m_a.z()-m_c.z(), r.getDirection().z());
		Vector3f VB(m_a.x() - r.getOrigin().x(),m_a.y() - r.getOrigin().y(),m_a.z() - r.getOrigin().z());
		Vector3f params = MA.inverse()*VB;
		if(params.z() >= tmin and params.z() < h.getT()){
			if((params.x() + params.y()) <= 1 and params.x() >= 0 and params.y() >= 0){
				Vector3f norm = params.x()*normals[0] + params.y()*normals[1] + params.z()*normals[2];
				h.set(params.z(), this->material, norm);
				return true;
			}
		}
		return false;		
	}
	bool hasTex;
	Vector3f normals[3];
	Vector2f texCoords[3];
protected:
	Vector3f m_a;
	Vector3f m_b;
	Vector3f m_c;
};

#endif //TRIANGLE_H
