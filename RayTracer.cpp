#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"
#include <math.h>

#define EPSILON 0.001
#define REFRACTING false
//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
  Vector3f reflected = incoming - 2*Vector3f::dot(incoming,normal)*normal;
  reflected.normalize();
  return reflected;
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{

  float nr = index_n/index_nt;
  float nDi = Vector3f::dot(normal,incoming);
  float root_term =  1-nr*nr*(1-nDi*nDi);
  if (root_term < 0){
    return false;
  }
  else{
    transmitted = nr*(incoming - normal*nDi) - normal*sqrt(root_term);
    return true;
  }
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

Vector3f RayTracer::traceRay( Ray& c_ray, float tmin, float refractI, int bounces, Hit& h ) const
{

  Vector3f pixelPass = m_scene->getBackgroundColor(c_ray.getDirection());
  Vector3f pixelIntersect = m_scene->getAmbientLight();
  bool intersect = g->intersect(c_ray, h , tmin);
  if(intersect){
    float intersect_t = h.getT();
    Vector3f intersect_p = c_ray.pointAtParameter(intersect_t);

    for(int l = 0; l < num_lights; l++){
      //Light
      Light* light = m_scene->getLight(l);
      Vector3f light_dir = Vector3f();
      Vector3f light_col = Vector3f();
      float light_distance = float(0.0f);
      light->getIllumination(intersect_p, light_dir, light_col, light_distance);
      pixelIntersect = pixelIntersect*h.getMaterial()->getDiffuseColor();
      //pixelIntersect +=  h.getMaterial()->Shade(c_ray,h,light_dir,light_col);

      //Shadows
      Ray test_shade = Ray(intersect_p,light_dir);
      Hit h_shade = Hit();
      bool intersect_shade = g->intersect(test_shade, h_shade, EPSILON);
      if(h_shade.getT() == light_distance){
          pixelIntersect +=  h.getMaterial()->Shade(c_ray,h,light_dir,light_col);
      }
    }

    if(bounces > 0){
      bool refract = false;
      Vector3f norm = h.getNormal().normalized();

      float nt = 0.0f;
      if( refractI != 1.0){
        nt = h.getMaterial()->getRefractionIndex();
      }
      else{
        nt = 1.0; 
      }

      float R = 1.0;

      //Mirror reflection
      Vector3f mirrorDir = mirrorDirection(norm,c_ray.getDirection().normalized());
      Ray mirrorRay = Ray(intersect_p,mirrorDir);
      Hit mir_h = Hit();
      Vector3f reflColor = traceRay(mirrorRay, EPSILON, refractI, bounces - 1, mir_h);

      Vector3f refrDir = Vector3f();
      Vector3f refrColor = Vector3f();
      if(REFRACTING){
      //Refraction
        if(h.getMaterial()->getRefractionIndex() > 0){

          refract = transmittedDirection(norm, -1*c_ray.getDirection(), 
          refractI, nt, refrDir);

          if(refract){
            Hit refr_h = Hit();
            //cout << "refrDir:" << refrDir.x() << ":" << refrDir.y() << ":" << refrDir.z();
            Ray refrRay = Ray(intersect_p,refrDir);
            refrColor = traceRay(refrRay, EPSILON, nt, bounces - 1, refr_h);

            //Weighting
            float c = 0.0f;
            if(refractI <= nt){
              c = abs(Vector3f::dot(-1*c_ray.getDirection(),norm));
            }
            else{
              c = abs(Vector3f::dot(refrDir,norm));
            }
            float R_0 = pow(((nt - refractI)/(nt + refractI)),2);
            R = R_0 + (1-R_0)*pow((1-c),5);
          }
        }
      }

      pixelIntersect += h.getMaterial()->getSpecularColor()*(R*reflColor + (1-R)*refrColor);
    }
    return pixelIntersect;
  }
  else{
    return pixelPass;
  }
}
