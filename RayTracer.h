#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <cassert>
#include <vector>
#include "SceneParser.h"
#include "Ray.h"
#include "Hit.h"

class SceneParser;


class RayTracer
{
public:
  
  RayTracer()
  {
      assert( false );
  }

  RayTracer( SceneParser* scene, int max_bounces //more arguments as you need...
  );
  ~RayTracer();
  
  Vector3f traceRay( Ray& camera_ray, float tmin, int bounces, Hit& h ) const;
private:
  SceneParser* m_scene;
  Group* g;
  Camera* c;
  int num_lights;
  int m_maxBounces;

};

#endif // RAY_TRACER_H
