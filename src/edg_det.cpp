//#include <windows.h>
//#include <wingdi.h>
#include "CImg.h"
#include <time.h>
#include <iostream>
#include <stdlib.h>
using namespace cimg_library;
int main(){
    clock_t start, stop;
CImg<unsigned char> image("lena.bmp");
CImgDisplay main_disp(image,"Click a point");
//CImgDisplay main_disp(image,"Image",0);
CImg<unsigned char> darkimage(image.width(),image.height(),1,3,0);
int N=image.width()*image.height();
unsigned int *r=(unsigned int *) malloc(N*sizeof(unsigned int));
unsigned int *g=(unsigned int *) malloc(N*sizeof(unsigned int));
unsigned int *b=(unsigned int *) malloc(N*sizeof(unsigned int));
unsigned int *r1=(unsigned int *) malloc(N*sizeof(unsigned int));
unsigned int *g1=(unsigned int *) malloc(N*sizeof(unsigned int));
unsigned int *b1=(unsigned int *) malloc(N*sizeof(unsigned int));
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
            << image.width()
            << std :: endl;

unsigned int *mask=(unsigned int *) malloc(9*sizeof(unsigned int));
for (int i=0;i<3;i++)
for (int j=0;j<3;j++)
mask[i*3 + j]=1-i;
start = clock ();
for (int i=1;i<image.height()-1;i++)
{
	for(int j=1;j<image.width()-1;j++)
	{
		r1[i*image.width()+j]=r[(i-1)*image.width()+(j-1)]*mask[0]+r[(i-1)*image.width()+(j)]*mask[1]+r[(i-1)*image.width()+(j+1)]*mask[2]+r[(i)*image.width()+(j-1)]*mask[3]+r[(i)*image.width()+(j)]*mask[4]+r[(i)*image.width()+(j+1)]*mask[5]+r[(i+1)*image.width()+(j-1)]*mask[6]+r[(i+1)*image.width()+(j)]*mask[7]+r[(i+1)*image.width()+(j+1)]*mask[8];
		g1[i*image.width()+j]=g[(i-1)*image.width()+(j-1)]*mask[0]+g[(i-1)*image.width()+(j)]*mask[1]+g[(i-1)*image.width()+(j+1)]*mask[2]+g[(i)*image.width()+(j-1)]*mask[3]+g[(i)*image.width()+(j)]*mask[4]+g[(i)*image.width()+(j+1)]*mask[5]+g[(i+1)*image.width()+(j-1)]*mask[6]+g[(i+1)*image.width()+(j)]*mask[7]+g[(i+1)*image.width()+(j+1)]*mask[8];
		b1[i*image.width()+j]=b[(i-1)*image.width()+(j-1)]*mask[0]+b[(i-1)*image.width()+(j)]*mask[1]+b[(i-1)*image.width()+(j+1)]*mask[2]+b[(i)*image.width()+(j-1)]*mask[3]+b[(i)*image.width()+(j)]*mask[4]+b[(i)*image.width()+(j+1)]*mask[5]+b[(i+1)*image.width()+(j-1)]*mask[6]+b[(i+1)*image.width()+(j)]*mask[7]+b[(i+1)*image.width()+(j+1)]*mask[8];
	}
}

stop = clock ();
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
////while (main_disp.is_closed)
 while (!main_disp.is_closed())
       {
        }

}

