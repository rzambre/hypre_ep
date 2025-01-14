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

#include "_hypre_utilities.h"

#if defined(HYPRE_USING_KOKKOS)
#include <Kokkos_Core.hpp>
#endif

/******************************************************************************
 *
 * hypre initialization
 *
 *****************************************************************************/
thread_local int hypre_min_workload=1;

void
HYPRE_Init( hypre_int argc, char *argv[] )
{
#ifdef HYPRE_USING_OPENMP
	const char * hypre_min_workload_str = getenv("HYPRE_MIN_WORKLOAD");
	if(hypre_min_workload_str){
		hypre_min_workload = atoi(hypre_min_workload_str);
		if(hypre_min_workload < 1)
			hypre_min_workload = 1;
	}
//	else
//		hypre_min_workload = 64;

	extern __thread int g_rank;
	if(g_rank==0)
		printf("hypre_min_workload per thread %d\n", hypre_min_workload);

#endif


   /*
   HYPRE_Int  num_procs, myid;
   
   hypre_MPI_Comm_size(hypre_MPI_COMM_WORLD, &num_procs);
   hypre_MPI_Comm_rank(hypre_MPI_COMM_WORLD, &myid);
   */
#if defined(HYPRE_USING_KOKKOS)
   /*
   Kokkos::InitArguments args;
   args.num_threads = 10;
   Kokkos::initialize (args);
   */
   Kokkos::initialize (argc, argv);
#endif

#if !defined(HYPRE_USING_RAJA) && !defined(HYPRE_USING_KOKKOS) && defined(HYPRE_USING_CUDA)
   if (!cuda_reduce_buffer)
   {
      cuda_reduce_buffer = hypre_TAlloc(HYPRE_double6, 1024, HYPRE_MEMORY_DEVICE);
   }
#endif

#if defined(HYPRE_USING_UNIFIED_MEMORY)
   hypre_GPUInit(-1);
#endif

   /* hypre_InitMemoryDebug(myid); */

#if defined(HYPRE_USING_DEVICE_OPENMP)
   /*
   hypre__offload_device_num = omp_get_initial_device();
   hypre__offload_host_num   = omp_get_initial_device();
   */
   HYPRE_OMPOffloadOn();
#endif
}

/******************************************************************************
 *
 * hypre finalization
 *
 *****************************************************************************/

/* declared in "struct_communication.c" */
extern HYPRE_THREAD_LOCAL_EP HYPRE_Complex *global_recv_buffer, *global_send_buffer;
extern HYPRE_THREAD_LOCAL_EP HYPRE_Int      global_recv_size, global_send_size;

void
HYPRE_Finalize()
{
#if defined(HYPRE_USING_UNIFIED_MEMORY)
   hypre_GPUFinalize();
#endif

#if defined(HYPRE_USING_KOKKOS)
   Kokkos::finalize ();
#endif

#if !defined(HYPRE_USING_RAJA) && !defined(HYPRE_USING_KOKKOS) && defined(HYPRE_USING_CUDA)
   hypre_TFree(cuda_reduce_buffer, HYPRE_MEMORY_DEVICE);
#endif

   if (global_send_buffer)
   {
     hypre_TFree(global_send_buffer, HYPRE_MEMORY_DEVICE);
   }
   if (global_recv_buffer)
   {
     hypre_TFree(global_recv_buffer, HYPRE_MEMORY_DEVICE);
   }
   
#ifdef HYPRE_USING_MPI_EP
   hypre_destroy_thread();
#endif
}

