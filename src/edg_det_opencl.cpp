//#include <windows.h>
//#include <wingdi.h>
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include "CImg.h"
#include <time.h>
#include <iostream>
//#include <Security/Security.h>
#include <stdlib.h>
using namespace cimg_library;

const char *source=
"__kernel void edg_det(	const int width,	\n"
"						const int height,	\n"
"					__global const float *r	\n"
"					__global const float *g	\n"
"					__global const float *b	\n"
"					__global const float *mask	\n"
"					__global	 float *r1	\n"
"					__global	 float *g1	\n"
"					__global	 float *b1)	\n"
"{											"
"int i=get_global_id(0);						"
"int j=get_global_id(1);                        "
//"if(i>0 && j>0 && i<width && j<height)          "
//"{                                              "
"r1[i*width +j]=r[(i-1)*width+(j-1)]*mask[0]+r[(i-1)*width+(j)]*mask[1]+r[(i-1)*width+(j+1)]*mask[2]+r[(i)*width+(j-1)]*mask[3]+r[(i)*width+(j)]*mask[4]+r[(i)*width+(j+1)]*mask[5]+r[(i+1)*width+(j-1)]*mask[6]+r[(i+1)*width+(j)]*mask[7]+r[(i+1)*width+(j+1)]*mask[8];"
"g1[i*width +j]=g[(i-1)*width+(j-1)]*mask[0]+g[(i-1)*width+(j)]*mask[1]+g[(i-1)*width+(j+1)]*mask[2]+g[(i)*width+(j-1)]*mask[3]+g[(i)*width+(j)]*mask[4]+g[(i)*width+(j+1)]*mask[5]+g[(i+1)*width+(j-1)]*mask[6]+g[(i+1)*width+(j)]*mask[7]+g[(i+1)*width+(j+1)]*mask[8];"
"b1[i*width +j]=b[(i-1)*width+(j-1)]*mask[0]+b[(i-1)*width+(j)]*mask[1]+b[(i-1)*width+(j+1)]*mask[2]+b[(i)*width+(j-1)]*mask[3]+b[(i)*width+(j)]*mask[4]+b[(i)*width+(j+1)]*mask[5]+b[(i+1)*width+(j-1)]*mask[6]+b[(i+1)*width+(j)]*mask[7]+b[(i+1)*width+(j+1)]*mask[8];"
//"r1[i*width +j]=r[(i)*width+(j)]"
//"g1[i*width +j]=g[(i)*width+(j)]"
//"b1[i*width +j]=b[(i)*width+(j)]"
//"}"
"}";



int main(){
    clock_t start, stop;
CImg<float> image("4000.jpg");
CImgDisplay main_disp(image,"Click a point");
//CImgDisplay main_disp(image,"Image",0);
CImg<float> darkimage(image.width(),image.height(),1,3,0);
int N=image.width()*image.height();
float *r=(float *) malloc(N*sizeof(float));
float *g=(float *) malloc(N*sizeof(float));
float *b=(float *) malloc(N*sizeof(float));
float *r1=(float *) malloc(N*sizeof(float));
float *g1=(float *) malloc(N*sizeof(float));
float *b1=(float *) malloc(N*sizeof(float));
for (int i=0;i<image.height();i++)
{
	for(int j=0;j<image.width();j++)
	{
		r[i*image.width()+j]=image(i,j,0,0);
		g[i*image.width()+j]=image(i,j,0,1);
		b[i*image.width()+j]=image(i,j,0,2);
	}
}
std :: cout << N
            << std :: endl;
std :: cout << image.height()
            << image.width() << "\n";            ;


float *mask=(float *) malloc(9*sizeof(float));
for (int i=0;i<3;i++)
for (int j=0;j<3;j++)
mask[i*3 + j]=1-i;

/* --------------------------------------------------CL Host Code -------------------------------------------------*/
//size_t work_units_per_kernel[2];
//size_t local[DIM];
start = clock ();
cl_device_id device_id;
cl_context context;
cl_command_queue commands;
cl_program program;
cl_kernel kernel;
//cl_uint nd;
int err;

cl_mem r_in, g_in, b_in, mask_in, r_out, g_out, b_out;
//initmat(Mdim, Ndim, Pdim, A, B, C);
cl_platform_id platform;
clGetPlatformIDs(1,&platform,NULL);


err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
//context = clCreateContext(0, 1, &device_id, NULL, NULL, NULL);
context = clCreateContext(NULL, 1, &device_id, NULL,
NULL, &err);

commands = clCreateCommandQueue(context, device_id, 0, &err);
//std :: cout << "Mahak" << std::endl;

r_in = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * N, NULL, NULL);
g_in = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * N, NULL, NULL);
b_in = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * N, NULL, NULL);
mask_in = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * 9, NULL, NULL);
r_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * N, NULL, NULL);
g_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * N, NULL, NULL);
b_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * N, NULL, NULL);
//std :: cout << "mac";
clEnqueueWriteBuffer(commands, r_in, CL_TRUE, 0, sizeof(float) * N, r, 0, NULL, NULL);
clEnqueueWriteBuffer(commands, g_in, CL_TRUE, 0, sizeof(float) * N, g, 0, NULL, NULL);
clEnqueueWriteBuffer(commands, b_in, CL_TRUE, 0, sizeof(float) * N, b, 0, NULL, NULL);
clEnqueueWriteBuffer(commands, mask_in, CL_TRUE, 0, sizeof(float) * 9, mask, 0, NULL, NULL);
program = clCreateProgramWithSource(context, 1, &source, NULL, NULL);
/*err = */clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
kernel = clCreateKernel(program, "edg_det", NULL);
int width=image.width();
int height=image.height();
clSetKernelArg(kernel, 0, sizeof(int), &width);
clSetKernelArg(kernel, 1, sizeof(int), &height);
clSetKernelArg(kernel, 2, sizeof(cl_mem), &r_in);
clSetKernelArg(kernel, 3, sizeof(cl_mem), &g_in);
clSetKernelArg(kernel, 4, sizeof(cl_mem), &b_in);
clSetKernelArg(kernel, 5, sizeof(cl_mem), &mask_in);
clSetKernelArg(kernel, 6, sizeof(cl_mem), &r_out);
clSetKernelArg(kernel, 7, sizeof(cl_mem), &g_out);
clSetKernelArg(kernel, 8, sizeof(cl_mem), &b_out);

size_t global[2];
global[0] = (size_t) width;
global[1] = (size_t) height;
/*err = */clEnqueueNDRangeKernel(commands, kernel, 2, NULL, global, NULL, 0, NULL, NULL);
clFinish(commands);
/*err = */clEnqueueReadBuffer(commands, r_out, CL_TRUE, 0, sizeof(float) * N, r1, 0, NULL, NULL );
clEnqueueReadBuffer(commands, g_out, CL_TRUE, 0, sizeof(float) * N, g1, 0, NULL, NULL );
clEnqueueReadBuffer(commands, b_out, CL_TRUE, 0, sizeof(float) * N, b1, 0, NULL, NULL );
//test_results(A, B, c_out);




stop = clock ();






/* -------------------------------------------------END Host Code------------------------------------------------*/


/*for (int i=1;i<image.height()-1;i++)
{
	for(int j=1;j<image.width()-1;j++)
	{
		r1[i*image.width()+j]=r[(i-1)*image.width()+(j-1)]*mask[0]+r[(i-1)*image.width()+(j)]*mask[1]+r[(i-1)*image.width()+(j+1)]*mask[2]+r[(i)*image.width()+(j-1)]*mask[3]+r[(i)*image.width()+(j)]*mask[4]+r[(i)*image.width()+(j+1)]*mask[5]+r[(i+1)*image.width()+(j-1)]*mask[6]+r[(i+1)*image.width()+(j)]*mask[7]+r[(i+1)*image.width()+(j+1)]*mask[8];
		g1[i*image.width()+j]=g[(i-1)*image.width()+(j-1)]*mask[0]+g[(i-1)*image.width()+(j)]*mask[1]+g[(i-1)*image.width()+(j+1)]*mask[2]+g[(i)*image.width()+(j-1)]*mask[3]+g[(i)*image.width()+(j)]*mask[4]+g[(i)*image.width()+(j+1)]*mask[5]+g[(i+1)*image.width()+(j-1)]*mask[6]+g[(i+1)*image.width()+(j)]*mask[7]+g[(i+1)*image.width()+(j+1)]*mask[8];
		b1[i*image.width()+j]=b[(i-1)*image.width()+(j-1)]*mask[0]+b[(i-1)*image.width()+(j)]*mask[1]+b[(i-1)*image.width()+(j+1)]*mask[2]+b[(i)*image.width()+(j-1)]*mask[3]+b[(i)*image.width()+(j)]*mask[4]+b[(i)*image.width()+(j+1)]*mask[5]+b[(i+1)*image.width()+(j-1)]*mask[6]+b[(i+1)*image.width()+(j)]*mask[7]+b[(i+1)*image.width()+(j+1)]*mask[8];
	}
}*/


for (int i=0;i<image.height();i++)
{
	for(int j=0;j<image.width();j++)
	{
		darkimage(i,j,0,0)=r1[i*image.width()+j];
		darkimage(i,j,0,1)=g1[i*image.width()+j];
		darkimage(i,j,0,2)=b1[i*image.width()+j];
	}
}

CImgDisplay dark_disp (darkimage,"Dark Image",0);
std :: cout << (stop-start)
            << std :: endl;


// -------------------------------------------------------------------------------------------
free(r);
free(g);
free(b);
free(mask);
free(r1);
free(g1);
free(b1);
clReleaseMemObject(r_in);
clReleaseMemObject(g_in);
clReleaseMemObject(b_in);
clReleaseMemObject(mask_in);
clReleaseMemObject(r_out);
clReleaseMemObject(g_out);
clReleaseMemObject(b_out);
clReleaseKernel(kernel);
clReleaseProgram(program);
clReleaseContext(context);
clReleaseCommandQueue(commands);
// -------------------------------------------------------------------------------------------------
////while (main_disp.is_closed)
 while (!main_disp.is_closed())
       {
        }

}

