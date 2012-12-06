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

  RayTracer( SceneParser* scene, int max_bounces, bool shadow //more arguments as you need...
  );
  ~RayTracer();
  
  Vector3f traceRay( Ray& camera_ray, float tmin, float refractI, int bounces, Hit& h ) const;
private:
  SceneParser* m_scene;
  Group* g;
  Camera* c;
  int num_lights;
  int m_maxBounces;
  bool m_shadow;
};

#endif // RAY_TRACER_H
