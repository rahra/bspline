/*! \file bspline.c
 * This file contains the functions to calculate control points from a list of
 * points for drawing bezier curves.
 *
 * License: This is free software. Take it, use it, modify it!
 *
 *  @author Bernhard R. Fischer
 *  @version 2015/11/30
 *
 */

#include <stdio.h>
#include <cairo.h>
#include <math.h>

#include "bspline.h"


int main(int argc, char **argv)
{
   // Cairo surface and context.
   cairo_surface_t *dst;
   cairo_t *ctx;
   // Loop variable and variable for number of Points.
   int cnt;
   // Array of points.
   point_t pt[] = {{50, 400}, {500, 400}, {650, 220}, {480,90}};
   // Alternative array of points.
   //point_t pt[] = {{160, 190}, {100, 400}, {470, 600}, {300, 200}, {400, 150},
   //   {470, 250}, {575, 400}, {630, 390}, {650, 300}, {670, 210}};

  /* This parameter defines if the points shall be connected in a loop (start
    * = 0) or as open line (start = 1). */
   int start = 1;

   // Number of points in array.
   cnt = sizeof(pt) / sizeof(*pt);

//#define WITH_RAND
#ifdef WITH_RAND
   srand(time(NULL));
   for (int i = 0; i < cnt; i++)
      pt[i].x = rand() % 800, pt[i].y = rand() % 800;
#endif

   // Create and init Cairo drawing context.
   dst = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 800);
   ctx = cairo_create(dst);
   COL_BG(ctx);
   cairo_paint(ctx);

   // Draw curve between the points pt.
   draw_curve(ctx, pt, cnt, start);

   // Save image and destroy Cairo context.
   cairo_destroy(ctx);
   cairo_surface_write_to_png(dst, "bspline.png");
   cairo_surface_destroy(dst);
   
   return 0;
}

