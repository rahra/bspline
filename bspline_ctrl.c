/*! \file bspline_ctrl.c
 * This file contains the functions to calculate control points from a list of
 * points for drawing bezier curves.
 *
 * License: This is free software. Take it, use it, modify it!
 *
 *  @author Bernhard R. Fischer
 *  @version 2015/11/30
 *
 */

#include <cairo.h>
#include <math.h>

#include "bspline.h"

// This factor defines the "curviness". Play with it!
#define CURVE_F 0.25
// This defines the method of using isosceles triangles. Undefine it to see the
// method of equal distances.
#define ISOSCELES_TRIANGLE


/*! This function calculates the angle of line in respect to the coordinate
 * system.
 * @param g Pointer to a line.
 * @return Returns the angle in radians.
 */
double angle(const line_t *g)
{
   return atan2(g->B.y - g->A.y, g->B.x - g->A.x);
}


/*! This function calculates the control points. It takes two lines g and l as
 * arguments but it takes three lines into account for calculation. This is
 * line g (P0/P1), line h (P1/P2), and line l (P2/P3). The control points being
 * calculated are actually those for the middle line h, this is from P1 to P2.
 * Line g is the predecessor and line l the successor of line h.
 * @param g Pointer to first line.
 * @param l Pointer to third line (the second line is connecting g and l).
 * @param p1 Pointer to memory of first control point. This will receive the
 * coordinates.
 * @param p2 Pointer to memory of second control point.
 */ 
void control_points(const line_t *g, const line_t *l, point_t *p1, point_t *p2)
{
   line_t h;
   double f = CURVE_F;
   double lgt, a;

   // calculate length of line (P1/P2)
   lgt = sqrt(pow(g->B.x - l->A.x, 2) + pow(g->B.y - l->A.y, 2));

#ifdef ISOSCELES_TRIANGLE
   // end point of 1st tangent
   h.B = l->A;
   // start point of tangent at same distance as end point along 'g'
   h.A.x = g->B.x - lgt * cos(angle(g));
   h.A.y = g->B.y - lgt * sin(angle(g));
#else
   h.A = g->A;
   h.B = l->A;
#endif

   // angle of tangent
   a = angle(&h);
   // 1st control point on tangent at distance 'lgt * f'
   p1->x = g->B.x + lgt * cos(a) * f;
   p1->y = g->B.y + lgt * sin(a) * f;

#ifdef ISOSCELES_TRIANGLE
   // start point of 2nd tangent
   h.A = g->B;
   // end point of tangent at same distance as start point along 'l'
   h.B.x = l->A.x + lgt * cos(angle(l));
   h.B.y = l->A.y + lgt * sin(angle(l));
#else
   h.A = g->B;
   h.B = l->B;
#endif

   // angle of tangent
   a = angle(&h);
   // 2nd control point on tangent at distance 'lgt * f'
   p2->x = l->A.x - lgt * cos(a) * f;
   p2->y = l->A.y - lgt * sin(a) * f;
}


void draw_curve(cairo_t *ctx, const point_t *pt, int cnt, int start)
{
   // Helping variables for lines.
   line_t g, l;
   // Variables for control points.
   point_t c1, c2;
 
#ifdef CONSTRUCTION
   // Draw direct lines between points.
   draw_lines(ctx, pt, cnt);
#endif
 
   // Draw bezier curve through all points.
   COL_CURVE(ctx);
   cairo_set_line_width(ctx, W_CURVE);
   cairo_move_to(ctx, pt[(start - 1 + cnt) % cnt].x, pt[(start - 1 + cnt) % cnt].y);
   for (int i = start; i < cnt; i++)
   {
      g.A = pt[(i + cnt - 2) % cnt];
      g.B = pt[(i + cnt - 1) % cnt];
      l.A = pt[(i + cnt + 0) % cnt];
      l.B = pt[(i + cnt + 1) % cnt];

      // Calculate controls points for points pt[i-1] and pt[i].
      control_points(&g, &l, &c1, &c2);

      // Handle special case if points are not connected in a loop.
      if (start && i == 1) c1 = g.B;
      if (start && i == cnt - 1) c2 = l.A;

      // Create Cairo curve path.
      cairo_curve_to(ctx, c1.x, c1.y, c2.x, c2.y, pt[i].x, pt[i].y);
   }
   // Actually draw curve.
   cairo_stroke(ctx);

#ifdef CONSTRUCTION
   // Draw construction lines.
   draw_construction(ctx, pt, cnt, start);
#endif
}

