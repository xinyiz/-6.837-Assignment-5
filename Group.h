
#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group:public Object3D
{
public:
	Group(){
		m_num_objects = 0;
	}

	Group( int num_objects ){
		m_num_objects = num_objects;
	}

	~Group(){

	}

	virtual bool intersect( const Ray& r , Hit& h , float tmin ){
		cerr << "DSFOIDSFSDOIFJS" << '\n';
		bool result = false;
		for(unsigned i = 0; i < m_objects.size(); i++){
			result = m_objects[i]->intersect(r, h, tmin);
		}
		return result;
	}

	void addObject( int index , Object3D* obj ){
		m_objects.push_back(obj);
		assert((int)m_objects.size() <= m_num_objects);
	}

	int getGroupSize(){ 
		return m_objects.size();
	}

private:
	int m_num_objects;
	vector<Object3D*> m_objects;
};

#endif

