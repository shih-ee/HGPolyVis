/*

Matrix and Vector header.

---------------------------------------------------------------

Copyright (c) 1994 The Board of Trustees of The Leland Stanford
Junior University.  All rights reserved.   
  
Permission to use, copy, modify and distribute this software and its   
documentation for any purpose is hereby granted without fee, provided   
that the above copyright notice and this permission notice appear in   
all copies of this software and that you do not sell the software.   
  
THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND,   
EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY   
WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.   

*/

#ifndef __MATRIX_H__
#define __MATRIX_H__
#ifndef __GL_GL_H__
typedef float Matrixt[4][4];
#endif

typedef float Vectort[3];
typedef float Vector4t[4];
typedef float Plane[4];
typedef float Quaterniont[4];


float vnorm(Vectort);
float vdot(Vectort,Vectort);
void vadd(Vectort,Vectort,Vectort);
void vsub(Vectort,Vectort,Vectort);
float vlen(Vectort);
void vset(float *, float, float, float);
void vec_scale(float *, float);
void vapply(Matrixt, Vectort, Vectort);
void vcopy(Vectort, Vectort);
void push();
void pop();
void mat_ident(Matrixt);
void mat_mult (Matrixt prod, Matrixt a, Matrixt b);
void translate (float x, float y, float z);
void mat_copy (Matrixt dest, Matrixt source);
void mat_transpose (Matrixt m);
void mat_translate (Matrixt m, float x, float y, float z);
void mat_rotate (Matrixt mat, float angle, char axis);
void mat_apply (Matrixt m, Vectort v);
void mat_apply_normal (Matrixt m, Vectort v);
void mat_print (Matrixt m);
void mat_apply_plane (Matrixt m, Plane p);

void quat_to_mat(Quaterniont q, Matrixt mat);
void mat_to_quat(Matrixt mat, Quaterniont q);

void mat_invert (Matrixt mat);
float determinant(Matrixt mat);

#endif /* __MATRIX_H__ */

