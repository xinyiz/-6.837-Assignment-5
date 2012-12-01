#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.01

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
  return incoming - 2*Vector3f::dot(incoming,normal)*normal;
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces 
  //more arguments if you need...
                      ) :
  m_scene(scene)

{
  g=scene->getGroup();
  c=scene->getCamera();
  num_lights = scene->getNumLights();
  m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay( Ray& c_ray, float tmin, int bounces, Hit& h ) const
{

  Vector3f pixelPass = m_scene->getBackgroundColor(c_ray.getDirection());
  bool intersect = g->intersect(c_ray, h , tmin);

  if(intersect){
    float intersect_t = h.getT();
    Vector3f intersect_p = c_ray.pointAtParameter(intersect_t);
    Vector3f pixelIntersect = h.getMaterial()->getDiffuseColor()*m_scene->getAmbientLight();
    if(bounces == m_maxBounces){
      for(int l = 0; l < num_lights; l++){

        //Get lighting
        Light* light = m_scene->getLight(l);
        Vector3f light_dir = Vector3f();
        Vector3f light_col = Vector3f();
        float light_distance = float(0.0f);
        light->getIllumination(intersect_p, light_dir, light_col, light_distance);
        
        //Shadows
        // Ray test_shade = Ray(intersect_p,light_dir);
        // Hit h_shade = Hit();
        // bool intersect_shade = g->intersect(test_shade, h_shade, EPSILON);
        // if(h_shade.getT() == light_distance){
        //   pixelIntersect +=  h.getMaterial()->Shade(c_ray,h,light_dir,light_col);
        // }

      }
    }
    if(bounces > 0){
      //Mirror reflection
      Vector3f norm = h.getNormal().normalized();
      Vector3f mirrorDir = mirrorDirection(c_ray.getDirection(),norm);
      Ray mirrorRay = Ray(intersect_p,mirrorDir);
      Hit mir_h = Hit();
      pixelIntersect += h.getMaterial()->getSpecularColor()*traceRay(mirrorRay, EPSILON, bounces - 1, mir_h);
    }
    return pixelIntersect;
  }
  else{
    return pixelPass;
  }
    //hit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );

    //return Vector3f(0,0,0);
}
