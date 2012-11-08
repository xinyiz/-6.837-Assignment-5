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
	int width = atoi(argv[4]);
	int height = atoi(argv[5]);
	char* output_file = argv[7];
	int depth_s = 0;
	int depth_e = 0;
	int d_range = 0;
	char* depth_output_file = argv[7];
	Image depth_image(width, height);
	if(argc > 11){
		cout << "HERE" << '\n';
		depth_s = atoi(argv[9]);
		depth_e = atoi(argv[10]);
		d_range = depth_e - depth_s;
		depth_output_file = argv[11];
	}

	Camera *camera = scene->getCamera();
	Group *group = scene->getGroup();
	// //Testing for 200 px by 200 px img
	Image image(width, height);
	Vector2f pixel;
	Vector3f pixelIntersect (255,255,255);
	Vector3f pixelPass (0,0,0);
	Hit h = Hit();
	float tmin = camera->getTMin();
	bool intersect = false;
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			pixel = Vector2f((x-width/2.0f)/(width/2.0f),(y-width/2.0f)/(width/2.0f));
			//cout << "PIXEL" << pixel.x() << ':' << pixel.y() << ':' <<'\n';
			Ray camera_ray = camera->generateRay(pixel);
			//cout << "RAYORI" << camera_ray.getOrigin().x() << ':' << camera_ray.getOrigin().y() << ':' << camera_ray.getOrigin().z() << '\n';
			//cout << "RAYDIR" << camera_ray.getDirection().x() << ':' << camera_ray.getDirection().y() << ':' << camera_ray.getDirection().z() << '\n';
			intersect = group->intersect(camera_ray, h , tmin );
			//cout << "DEPTH" << h.getT() << '\n';
			cout << "INTERSECT" << intersect << '\n';
			if(intersect){
				image.SetPixel(x,y, pixelIntersect );
				if(argc > 11){
					if(h.getT()>depth_e){
						depth_image.SetPixel(x,y, pixelPass );
					}
					else if(h.getT()<depth_s){
						depth_image.SetPixel(x,y, pixelIntersect );
					}
					else{
						float gray = (h.getT()-depth_s)/(float)d_range;
						//cout << "gray" << gray << '\n';
						Vector3f pixelDepth (1-gray,1-gray,1-gray);
						depth_image.SetPixel(x,y,pixelDepth);
					}
				}
			}
			else{
				image.SetPixel(x,y, pixelPass );
				if(argc > 11){
					depth_image.SetPixel(x,y,pixelPass);
				}
			}
		}
	}
	image.SaveImage(output_file);
	if(argc > 11){
		cout << "END" << '\n';
		depth_image.SaveImage(depth_output_file);
	}
 
	///TODO: below demonstrates how to use the provided Image class
	///Should be removed when you start
	//Vector3f pixelColor (255,0,0);
	//width and height
	//Image image( 10 , 15 );
	//image.SetPixel( 5,5, pixelColor );
	//image.SaveImage("demo.bmp");
	return 0;
}

