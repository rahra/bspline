#ifdef CONSTRUCTION

#include <stdio.h>
#include <cairo.h>
#include <math.h>

#include "bspline.h"


static void dp(cairo_t *ctx, double x, double y)
{
   cairo_rectangle(ctx, x - R, y - R, R * 2, R * 2);
   cairo_fill(ctx);
}


static void show_angle(cairo_t *ctx, const line_t *l)
{
   char buf[20];
   double a;

   a = angle(l);
   snprintf(buf, sizeof(buf), "%.1f°", a * 180 / M_PI);
   cairo_save(ctx);
   cairo_move_to(ctx, (l->A.x + l->B.x) / 2, (l->A.y + l->B.y) / 2 - 5);
   if (a < -M_PI_2 || a > M_PI_2)
      a += M_PI;
   cairo_rotate(ctx, a);
   cairo_show_text(ctx, buf);
   cairo_restore(ctx);
}


void draw_lines(cairo_t *ctx, const point_t *pt, int cnt) 
{
   char buf[20];
   int i;

   cairo_save(ctx);
   cairo_set_line_width(ctx, W_LINE);
   COL_LINE(ctx);
   cairo_new_path(ctx);
   for (i = 0; i < cnt; i++)
      cairo_line_to(ctx, pt[i].x, pt[i].y);
   cairo_stroke(ctx);

   cairo_select_font_face(ctx, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size(ctx, F_SIZE);
   for (i = 0; i < cnt; i++)
   {
      snprintf(buf, sizeof(buf), "P%d", i);
      cairo_move_to(ctx, pt[i].x - 5, pt[i].y + 14);
      cairo_show_text(ctx, buf);
      if (i < cnt - 1)
         show_angle(ctx, &((line_t) {pt[i], pt[(i + 1) % cnt]}));
   }
   cairo_restore(ctx);
}


void draw_construction(cairo_t *ctx, const point_t *pt, int cnt, int start)
{
   point_t c1, c2, prev_p;
   line_t g, l;
   double dash[] = {10, 5};
   char buf[20];
   int i;

   cairo_save(ctx);
   cairo_select_font_face(ctx, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size(ctx, F_SIZE);
   for (i = start; i < cnt; i++)
   {
      g.A = pt[(i + cnt - 2) % cnt];
      g.B = pt[(i + cnt - 1) % cnt];
      l.A = pt[(i + cnt + 0) % cnt];
      l.B = pt[(i + cnt + 1) % cnt];

      control_points(&g, &l, &c1, &c2);
      if (start && i == 1) c1 = g.B;
      if (start && i == cnt - 1) c2 = l.A;

      COL_C1(ctx);
      dp(ctx, c1.x, c1.y);
      snprintf(buf, sizeof(buf), "C1[%d] (%d/%d)", i, (int) c1.x, (int) c1.y);
      cairo_move_to(ctx, c1.x + 5, c1.y);
      cairo_show_text(ctx, buf);

      COL_C2(ctx);
      dp(ctx, c2.x, c2.y);
      snprintf(buf, sizeof(buf), "C2[%d] (%d/%d)", i, (int) c2.x, (int) c2.y);
      cairo_move_to(ctx, c2.x + 5, c2.y);
      cairo_show_text(ctx, buf);

      if (i > start)
      {
         cairo_save(ctx);
         COL_TLINE(ctx);
         cairo_set_line_width(ctx, 1);
         cairo_set_dash(ctx, dash, 2, 0);
         cairo_move_to(ctx, prev_p.x, prev_p.y);
         cairo_line_to(ctx, c1.x, c1.y);
         cairo_stroke(ctx);
         cairo_restore(ctx);
         }
      prev_p = c2;
   }
   cairo_restore(ctx);
}

#endif

