#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include "RayTracer.h"
#include <string.h>
#define BOUNCES 4
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
	for( int argNum = 1; argNum < argc; ++argNum ){
		std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;

	}
	cout << "ARGC" << argc << '\n';
	// First, parse the scene using SceneParser.
	// Then loop over each pixel in the image, shooting a ray
	// through that pixel and finding its intersection with
	// the scene.  Write the color at the intersection to that
	// pixel in your output image.

	// Parse args;
	SceneParser *scene = new SceneParser(argv[2]);
	RayTracer *r_trace = new RayTracer(scene,BOUNCES);
	int width = atoi(argv[4]);
	int height = atoi(argv[5]);
	// General args
	Camera *camera = scene->getCamera();
	Image image(width, height);
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			Vector2f pixel = Vector2f((x-width/2.0f)/(width/2.0f),(y-height/2.0f)/(height/2.0f));
			Hit h = Hit();
			Ray camera_ray = camera->generateRay(pixel);
			Vector3f color = r_trace->traceRay( camera_ray, camera->getTMin(), 1.000293, BOUNCES, h);
			image.SetPixel(x,y, color);
		}
	}
	image.SaveImage(argv[7]);
	return 0;
}