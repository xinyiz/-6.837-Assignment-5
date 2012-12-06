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
using namespace std;


float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int cap(int num, int c){
	if (c == 0){
		if (num < 0){
			return 0;
		}
	}
	else{
		if (num > c){
			return c;
		}
	}
	return num;
}
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
	bool SHADOW = false;
	bool JITTER = false;
	bool FILTER = false;
	int BOUNCES = 4;
	cout << "argc" << argc << '\n';
	if(std::strcmp(argv[8],"-shadows")==0){
		cout << "AHERE" << '\n';;
		SHADOW = true;
		BOUNCES = atoi(argv[10]);
		if(argc == 11){

		}
		else if(argc == 13){
			JITTER = true;
			FILTER = true;
		}
		else if(std::strcmp(argv[11],"-jitter")==0){
			JITTER = true;
		}
		else if(std::strcmp(argv[11],"-filter")==0){
			FILTER = true;
		}
	}
	else{
		cout << "BHERE" << '\n';
		BOUNCES = atoi(argv[9]);
		if(argc == 10){

		}
		else if(argc == 12){
			JITTER = true;
			FILTER = true;
		}
		else if(std::strcmp(argv[10],"-jitter")==0){
			JITTER = true;
		}
		else if(std::strcmp(argv[10],"-filter")==0){
			FILTER = true;
		}
	}

	// First, parse the scene using SceneParser.
	// Then loop over each pixel in the image, shooting a ray
	// through that pixel and finding its intersection with
	// the scene.  Write the color at the intersection to that
	// pixel in your output image.

	// Parse args;
	SceneParser *scene = new SceneParser(argv[2]);
	RayTracer *r_trace = new RayTracer(scene,BOUNCES,SHADOW);
	int width = atoi(argv[4]);
	int height = atoi(argv[5]);
	// General args
	Camera *camera = scene->getCamera();
	int i_width = width;
	int i_height = height;
	if(JITTER){
		i_width = width*3;
		i_height = height*3;
	}
	Image image(i_width,i_height);
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			if(JITTER){
				for(int i = 0; i < 3; i++){
					for(int j = 0; j < 3; j++){
						float r = (rand()/float(RAND_MAX))-0.5f;
						float new_x = 3*x + i;
						float new_y = 3*y + j;
						Vector2f pixel = Vector2f((new_x+r-i_width/2.0f)/(i_width/2.0f),(new_y+r-i_height/2.0f)/(i_height/2.0f));
						Hit h = Hit();
						Ray camera_ray = camera->generateRay(pixel);
						Vector3f color = r_trace->traceRay( camera_ray, camera->getTMin(), 1.0, BOUNCES, h);
						image.SetPixel(new_x,new_y, color);
					}
				}
			}
			else{
				Vector2f pixel = Vector2f((x-width/2.0f)/(width/2.0f),(y-height/2.0f)/(height/2.0f));
				Hit h = Hit();
				Ray camera_ray = camera->generateRay(pixel);
				Vector3f color = r_trace->traceRay( camera_ray, camera->getTMin(), 1.0, BOUNCES, h);
				image.SetPixel(x,y, color);
			}
		}
	}
	if(FILTER){
		Image blur_image_h(i_width,i_height);
		Image blur_image_w(i_width,i_height);
		Image image_final(i_width/3,i_height/3);
		for(int x = 0; x < width; x++){
			for(int y = 0; y < height; y++){
				if(JITTER){
					for(int i = 0; i < 3; i++){
						for(int j = 0; j < 3; j++){
							float new_x = 3*x + i;
							float new_y = 3*y + j;
							Vector3f blurred_h = image.GetPixel(new_x,cap(new_y-2,0))*0.1201 + image.GetPixel(new_x,cap(new_y-1,0))*0.2339 + image.GetPixel(new_x,new_y)*0.2931 + image.GetPixel(new_x,cap(new_y+1,i_height-1))*0.2339 + 
							image.GetPixel(new_x,cap(new_y+2,i_height-1))*0.1201;
							//cout << "blurred_h:" << blurred_h.x() << ':' << blurred_h.y() << ':' << blurred_h.z() << '\n';
							blur_image_h.SetPixel(new_x,new_y,blurred_h);
						}
					}
				}
				else{
					Vector3f blurred_h = image.GetPixel(x,cap(y-2,0))*0.1201 + image.GetPixel(x,cap(y-1,0))*0.2339 + image.GetPixel(x,y)*0.2931 + image.GetPixel(x,cap(y+1,i_height-1))*0.2339 + 
					image.GetPixel(x,cap(y+2,i_height-1))*0.1201;
					//cout << "blurred_h:" << blurred_h.x() << ':' << blurred_h.y() << ':' << blurred_h.z() << '\n';
					//cout << "blurred_h:" << 0.1201 << ':' << blurred_h.y() << ':' << blurred_h.z() << '\n';
					blur_image_h.SetPixel(x,y,blurred_h);
				}
			}
		}
		cout << "F_HERE" << '\n';
		for(int x = 0; x < width; x++){
			for(int y = 0; y < height; y++){
				if(JITTER){
					for(int i = 0; i < 3; i++){
						for(int j = 0; j < 3; j++){
							float new_x = 3*x + i;
							float new_y = 3*y + j;
							Vector3f blurred_w = blur_image_h.GetPixel(cap(new_x-2,0),new_y)*0.1201 + blur_image_h.GetPixel(cap(new_x-1,0),new_y)*0.2339 + blur_image_h.GetPixel(new_x,new_y)*0.2931 + blur_image_h.GetPixel(cap(new_x+1,i_width-1),new_y)*0.2339 + 
							blur_image_h.GetPixel(cap(new_x+2,i_width-1),new_y)*0.1201;
							blur_image_w.SetPixel(new_x,new_y,blurred_w);
						}
					}
				}
				else{
					Vector3f blurred_w = blur_image_h.GetPixel(cap(x-2,0),y)*0.1201 + blur_image_h.GetPixel(cap(x-1,0),y)*0.2339 + blur_image_h.GetPixel(x,y)*0.2931 + blur_image_h.GetPixel(cap(x+1,i_width-1),y)*0.2339 + 
					blur_image_h.GetPixel(cap(x+2,i_width-1),y)*0.1201;
					blur_image_w.SetPixel(x,y,blurred_w);
				}
			}
		}
		cout << "F_HERE2" << '\n';
		for(int x = 0; x < i_width/3; x++){
			for(int y = 0; y < i_height/3; y++){

				int a_x = 3*x+1;
				int a_y = 3*y+1;
				Vector3f sum = Vector3f();
				for(int i = -1; i < 2; i++){
					for(int j = -1; j < 2; j++){
						sum += blur_image_w.GetPixel(a_x+i,a_y+j);
					}
				}
				sum = sum/9.0;
				image_final.SetPixel(x,y,sum);

			}
		}
		image_final.SaveImage(argv[7]);
	}
	else{
		image.SaveImage(argv[7]);
	}

	return 0;
}

