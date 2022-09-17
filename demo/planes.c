#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void planes_init() {
}

void ppix(int x,int y,unsigned char c,uint32_t *pixels)

{
    if ((x>=0) && (x<192) && (y>=0) && (y<192)) {
        pixels[y*192+x]=c<<16;
    }
}

void planes_frame(uint32_t *pixels, uint32_t time)
{
int t=time/20;
for (int y=0;y<192;y++) {
    for (int x=0;x<192;x++) {
        int ax=(x-96);
        int ay=(y-96);
        int cx=abs(ax);
        int cy=abs(ay);
        float z=cy+.1;
        float z2=cx+.1;
        int q=(int)(cx*8/z) & (int)(699/z+t);
        int q2=(int)(cy*8/z2) & (int) (699/z2+t);
        if (z<6) q=0;
        if (z2<6) q2=0;
        ppix(x,y,((q*q2)&63)*4,pixels);
    }
}

}
