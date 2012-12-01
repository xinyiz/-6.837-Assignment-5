#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
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

Vector3f RayTracer::traceRay( Ray& camera_ray, float tmin, int bounces, Hit& h ) const
{

  Vector3f pixelPass = m_scene->getBackgroundColor(camera_ray.getDirection());
  bool intersect = g->intersect(camera_ray, h , tmin);
  if(intersect){
    float intersect_t = h.getT();
    Vector3f intersect_p = camera_ray.pointAtParameter(intersect_t);
    Vector3f pixelIntersect = h.getMaterial()->getDiffuseColor()*m_scene->getAmbientLight();
    for(int l = 0; l < num_lights; l++){
      Light* light = m_scene->getLight(l);
      Vector3f light_dir = Vector3f();
      Vector3f light_col = Vector3f();
      float light_distance = float(0.0f);
      light->getIllumination(intersect_p, light_dir, light_col, light_distance);
      //Shadows
      Ray test_shade = Ray(intersect_p,light_dir);
      Hit h_shade = Hit();
      bool intersect_shade = g->intersect(test_shade, h_shade, 0.001);
      if(h_shade.getT() == light_distance){
        pixelIntersect +=  h.getMaterial()->Shade(camera_ray,h,light_dir,light_col);
      }
    }
    pixelIntersect = pixelIntersect;
    return pixelIntersect;
  }
  else{
    return pixelPass;
  }
    //hit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );

    //return Vector3f(0,0,0);
}
