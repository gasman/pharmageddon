#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

void linework_init() {
}

void linework_frame(uint32_t *pixels, uint32_t time)
{
int t=time/20;
int part=t>>8;
int tt=t;
float zoom=sin(t/24)*4+5;
for (int y=0;y<192;y++) {
    for (int x=0;x<192;x++) {
        int sx=abs(x-96);
        int sy=abs(y-96);
        int q=0;
        for (int l=1;l<5;l++) {
            int qn=(int)(sx*zoom-tt) & (int)(sy * zoom-tt);
            if (part>1) qn=(int)(sx*zoom-tt) ^ (int)(sy * zoom-tt);

            q=q|qn;
        }
        int color = (q&192);// << 16;
        gfx_putpixel(pixels,x,y,color<<16);
    }
}
}
