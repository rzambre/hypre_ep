/*BHEADER**********************************************************************
 * Copyright (c) 2008,  Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 * This file is part of HYPRE.  See file COPYRIGHT for details.
 *
 * HYPRE is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License (as published by the Free
 * Software Foundation) version 2.1 dated February 1999.
 *
 * $Revision$
 ***********************************************************************EHEADER*/

/******************************************************************************
 *
 * Structured axpy routine
 *
 *****************************************************************************/

#include "_hypre_struct_mv.h"

/*--------------------------------------------------------------------------
 * hypre_StructAxpy
 *--------------------------------------------------------------------------*/

HYPRE_Int
hypre_StructAxpy( HYPRE_Complex       alpha,
                  hypre_StructVector *x,
                  hypre_StructVector *y     )
{
   hypre_Box        *x_data_box;
   hypre_Box        *y_data_box;
                    
   HYPRE_Complex    *xp;
   HYPRE_Complex    *yp;
                    
   hypre_BoxArray   *boxes;
   hypre_Box        *box;
   hypre_Index       loop_size;
   hypre_IndexRef    start;
   hypre_Index       unit_stride;
                    
   HYPRE_Int         i;

   hypre_SetIndex(unit_stride, 1);

   boxes = hypre_StructGridBoxes(hypre_StructVectorGrid(y));
   hypre_ForBoxI(i, boxes)
   {
      box   = hypre_BoxArrayBox(boxes, i);
      start = hypre_BoxIMin(box);

      x_data_box = hypre_BoxArrayBox(hypre_StructVectorDataSpace(x), i);
      y_data_box = hypre_BoxArrayBox(hypre_StructVectorDataSpace(y), i);

      xp = hypre_StructVectorBoxData(x, i);
      yp = hypre_StructVectorBoxData(y, i);

      hypre_BoxGetSize(box, loop_size);
	
#define DEVICE_VAR is_device_ptr(yp,xp)
      hypre_BoxLoop2BeginSimd(hypre_StructVectorNDim(x), loop_size,
			  x_data_box, start, unit_stride, xi,
			  y_data_box, start, unit_stride, yi);
      {
         yp[yi] += alpha * xp[xi];
      }
      hypre_BoxLoop2EndSimd(xi, yi);
#undef DEVICE_VAR
   }

   return hypre_error_flag;
}
