#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
  // Fill in your implementation here.

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
  //./a4soln -input scene01_plane.txt -size 200 200 -output output01.bmp -depth 8 12 depth01.bmp
  for( int argNum = 1; argNum < argc; ++argNum )
    {
      std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

  // First, parse the scene using SceneParser.
  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene.  Write the color at the intersection to that
  // pixel in your output image.

  // Sphere *s = new Sphere;
  // Hit hs = Hit();
  // Ray rs = Ray(Vector3f(2,0,0),Vector3f(-1,0,0));
  // cout << "DFDFDSFSDF" << s->intersect( rs , hs , 0);
  SceneParser *scene = new SceneParser(argv[1]);
  Camera *camera = scene->getCamera();
  Group *group = scene->getGroup();
  // //Testing for 200 px by 200 px img
  int size = 200;
  Image image(size, size);
  Vector2f pixel;
  Vector3f pixelIntersect (255,255,255);
  Vector3f pixelPass (0,0,0);
  Hit h = Hit();
  float tmin = camera->getTMin();
  bool intersect = false;
  for(int x = 0; x < size; x++){
    for(int y = 0; y < size; y++){
      pixel = Vector2f((x-size/2.0f)/(size/2.0f),(y-size/2.0f)/(size/2.0f));
      //cout << "PIXEL" << pixel.x() << ':' << pixel.y() << ':' <<'\n';
      Ray camera_ray = camera->generateRay(pixel);
      //cout << "RAYORI" << camera_ray.getOrigin().x() << ':' << camera_ray.getOrigin().y() << ':' << camera_ray.getOrigin().z() << '\n';
      //cout << "RAYDIR" << camera_ray.getDirection().x() << ':' << camera_ray.getDirection().y() << ':' << camera_ray.getDirection().z() << '\n';
      intersect = group->intersect(camera_ray, h , tmin );
      //cout << "INTERSECT" << intersect << '\n';
      if(intersect){
        image.SetPixel(x,y, pixelIntersect );
      }
      else{
        image.SetPixel(x,y, pixelPass );
      }
    }
  }
  image.SaveImage("demo200.bmp");
 
  ///TODO: below demonstrates how to use the provided Image class
  ///Should be removed when you start
  //Vector3f pixelColor (255,0,0);
  //width and height
  //Image image( 10 , 15 );
  //image.SetPixel( 5,5, pixelColor );
  //image.SaveImage("demo.bmp");
  return 0;
}

