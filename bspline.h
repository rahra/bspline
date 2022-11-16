#ifndef BSPLINE_H
#define BSPLINE_H

#include <cairo.h>


#define COL_BG(x) cairo_set_source_rgb(x, 1, 1, 1)
#define COL_CURVE(x) cairo_set_source_rgb(x, 0, 0, 1)
#define COL_LINE(x) cairo_set_source_rgb(ctx, 0, 0, 0)
#define COL_C1(x) cairo_set_source_rgb(ctx, 1, 0.2, 0.2)
#define COL_C2(x) cairo_set_source_rgb(ctx, 0.2, 0.8, 0.2)
#define COL_TLINE(x) cairo_set_source_rgb(ctx, 1, 0.2, 0.2)
#define COL_HLINE(x) cairo_set_source_rgb(ctx, 0, 0, 0)

#define W_CURVE 5
#define W_LINE 2
#define F_SIZE 10
#define R 3

typedef struct point
{
   double x, y;
} point_t;

typedef struct line
{
   point_t A, B;
} line_t;


#ifdef CONSTRUCTION
void draw_lines(cairo_t *, const point_t *, int );
void draw_construction(cairo_t *, const point_t *, int , int );
#endif

double angle(const line_t *);
void control_points(const line_t *, const line_t *, point_t *, point_t *);
void draw_curve(cairo_t *, const point_t *, int , int );

#endif

