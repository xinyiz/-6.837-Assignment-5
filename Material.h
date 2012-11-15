#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>
#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:
	
 Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0):
  diffuseColor( d_color),specularColor(s_color), shininess(s)
  {
        	
  }

  virtual ~Material()
    {

    }

  virtual Vector3f getDiffuseColor() const 
  { 
    return  diffuseColor;
  }
    

  Vector3f Shade( const Ray& ray, const Hit& hit,
                  const Vector3f& dirToLight, const Vector3f& lightColor ) {
    Vector3f ray_dir = -ray.getDirection();
    Vector3f s_normal = hit.getNormal();
    Vector3f R = -ray_dir + 2*(max(0.0f,Vector3f::dot(ray_dir,s_normal)))*s_normal;
    R.normalize();
    float c_s = 0.0f;
    if(Vector3f::dot(dirToLight,R.normalized())>0){
      c_s = pow(Vector3f::dot(dirToLight,R.normalized()),shininess);
    }
    Vector3f c_specular = Vector3f(c_s*lightColor.x()*specularColor.x(),c_s*lightColor.y()*specularColor.y(),c_s*lightColor.z()*specularColor.z());
    Vector3f color;
    if(t.valid()){
      color = t(hit.texCoord.x(),hit.texCoord.y());
    }
    else{
      color = getDiffuseColor();
    }
    Vector3f c_diffuse = max(0.0f,Vector3f::dot(dirToLight,s_normal))*Vector3f(color.x()*lightColor.x(),
      color.y()*lightColor.y(),
      color.z()*lightColor.z());

    return c_diffuse + c_specular;
		
  }

  void loadTexture(const char * filename){
    t.load(filename);
  }
 protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture t;
};



#endif // MATERIAL_H
