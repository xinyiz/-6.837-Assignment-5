#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
///TODO implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform: public Object3D
{
public: 
	Transform(){}
	Transform( const Matrix4f& m, Object3D* obj ):o(obj){
		o = obj;
		m_transform = m;
	}
	~Transform(){
	}
	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		Vector4f new_ori = m_transform.inverse()*Vector4f(r.getOrigin(),1);
		Vector4f new_dir = m_transform.inverse()*Vector4f(r.getDirection(),0);
		Ray new_ray = Ray(new_ori.xyz(), new_dir.xyz());
		if(o->intersect( new_ray , h , tmin)){
			Matrix4f norm_trans = Matrix4f(m_transform.inverse());
			norm_trans.transpose();
			Vector4f new_hit = Vector4f(h.getNormal().normalized(),0);
			Vector4f new_norm = norm_trans*new_hit;
			h.set(h.getT(),h.getMaterial(),new_norm.xyz().normalized());
			return true;
		}
		return false;
	}

protected:
	Object3D* o; //un-transformed object
	Matrix4f m_transform;
};

#endif //TRANSFORM_H
