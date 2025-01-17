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

#ifndef hypre_EUCLID_HEADER
#define hypre_EUCLID_HEADER

#define USING_MPI
#define HYPRE_GET_ROW
#define HYPRE_MODE
#define OPTIMIZED_DH

#if defined(HYPRE_MODE)
#include "HYPRE_parcsr_mv.h"
#include "HYPRE_config.h"
#include "HYPRE_distributed_matrix_mv.h"
#include "_hypre_utilities.h"

#elif defined(PETSC_MODE)
#include "petsc_config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

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

#ifndef EUCLID_CONF_DH
#define EUCLID_CONF_DH

#define MAX_MPI_TASKS 50000

#define PETSC_EUCLID_ERR_CODE  PETSC_ERR_LIB

/* for use in printTriples functions */
#define TRIPLES_FORMAT    "%i %i %1.8e\n"
/* #define TRIPLES_FORMAT    "%i %i %1.19e\n" */

#undef PRIVATE_TIMING_DH
  /* primarily for experimental purposes; if defined, TimeLog_dh
     marks are entered in Mat_dh and Euclid_apply modules.
   */


  /* top-level error handlers. redefine to do what you want, or don't
     use it at all.  Intended usage for calling Euclid functions from
     main is:

         Euclid_dhPhoo(); ERRCHKA;
  */

#ifdef USING_MPI
#define EUCLID_EXIT hypre_MPI_Abort(comm_dh, -1)
#else
#define EUCLID_EXIT exit(-1);
#endif

#define EXIT_NOW(msg) \
      { setError_dh(msg, __FUNC__, __FILE__, __LINE__); \
        ERRCHKA; \
      } 

#define ERRCHKA   \
    if (errFlag_dh) {  \
      setError_dh("", __FUNC__, __FILE__, __LINE__); \
      if (logFile != NULL) {  \
        printErrorMsg(logFile);  \
        closeLogfile_dh();  \
      } \
      printErrorMsg(stderr);  \
      if (myid_dh == 0) { \
        Mem_dhPrint(mem_dh, stderr, false); \
      } \
      EUCLID_EXIT; \
    } 


  /* let Euclid do its thing, before handing off to PETSc;
     this is only used in main(), after a call to a function
     from the PETSc library.
   */
#define ERRCHKA_CHKERRA(ierr)   \
    if (errFlag_dh) {  \
      setError_dh("", __FUNC__, __FILE__, __LINE__); \
      if (logFile != NULL) {  \
        printErrorMsg(logFile);  \
        hypre_fprintf(logFile, "\n[%i] ierr = %i, errFlag_dh = %i\n", myid_dh, ierr, errFlag_dh); \
        closeLogfile_dh();  \
      } \
      printErrorMsg(stderr);  \
      hypre_fprintf(stderr, "\n[%i] ierr = %i, errFlag_dh = %i\n", myid_dh, ierr, errFlag_dh); \
      CHKERRA(ierr); \
    } 


#define MAX_SUBDOMAINS  20   
  /* The maximum number of subdomains into which
     the matrix may be partitioned.  Rule of thumb:
     MAX_SUBDOMAINS >= number of threads.

     Note: this is only for shared-memory.
   */


#define PIVOT_FIX_DEFAULT  1e-3

/*---------------------------------------------------------------------
 * Memory management.  These macros work with functions in Mem_dh.c;
 * Change if you want to use some memory management and reporting schemes 
 * other than that supplied with Euclid.   These depend on the global
 * object "Mem_dh mem_dh" which is defined in globalObjects.c (yuck!)
 ---------------------------------------------------------------------*/

#undef USE_PETSC_MALLOC

#ifdef USE_PETSC_MALLOC
#define MALLOC_DH(s)  PetscMalloc(s)
#define FREE_DH(p)    PetscFree(p)

#else
#define MALLOC_DH(s)  Mem_dhMalloc(mem_dh, (s))
#define FREE_DH(p)    Mem_dhFree(mem_dh, p)
#endif


  /* The actual calls used by Mem_dh objects to allocate/free memory 
   * from the heap.
   */
#define PRIVATE_MALLOC  malloc
#define PRIVATE_FREE    free

/*------------------ Memory management end -----------------------------*/

/*
Developer's note: there must be a XXX_GET_ROW defined, e.g:

  #define HYPRE_GET_ROW
  #define PETSC_GET_ROW
  #define EUCLID_GET_ROW

This is currently accomplished in the makefile system;
If you're building an interface to a solver package,
you need to write EUCLID_GET_ROW() functions: see src/getRow.c
*/

#endif
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

#ifndef MACROS_DH
#define MACROS_DH

#ifndef FMAX
#define FMAX(a,b)  ((FABS(a)) > (FABS(b)) ? (FABS(a)) : (FABS(b)))
#endif

#ifndef MAX
#define MAX(a,b)   ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef ABS
#define ABS(x) (((x)<0)?(-(x)):(x))
#endif

#ifndef FABS
#define FABS(a)    ((a) < 0 ? -(a) : a)
#endif

#ifdef HYPRE_SINGLE
#define _ATOL_ 1.0e-16   /* used to compute absolute tolerance for Euclid's internal Krylov solvers */
#define _MATLAB_ZERO_  1e-30 /* used in Mat_SEQ_PrintTriples, so matlab won't discard zeros (yuck!) */
#else // default
#define _ATOL_ 1.0e-50
#define _MATLAB_ZERO_  1e-100
#endif



/*---------------------------------------------------------------------- 
 * macros for error handling everyplace except in main.
 *---------------------------------------------------------------------- */

/* for future expansion: should check that "ptr" points to
   a valid memory address, if not null.
*/
#define ASSERT_DH(ptr) \
    { \
      if (ptr == NULL) { \
        hypre_sprintf(msgBuf_dh, "%s is NULL", ptr); \
        SET_V_ERROR(msgBuf_dh); \
      } \
    }


#if 0
#define CHECK_MPI_V_ERROR(errCode)  \
      { \
        if (errCode) { \
          HYPRE_Int len; \
          hypre_MPI_Error_string(errCode, msgBuf_dh, &len); \
          setError_dh(msgBuf_dh, __FUNC__, __FILE__, __LINE__); \
          return; \
        } \
      }

#define CHECK_MPI_ERROR(errCode)  \
      { \
        if (errCode) { \
          HYPRE_Int len; \
          hypre_MPI_Error_string(errCode, msgBuf_dh, &len); \
          setError_dh(msgBuf_dh, __FUNC__, __FILE__, __LINE__); \
          return(errCode); \
        } \
      }
#endif

#define CHECK_MPI_V_ERROR(errCode)  \
      { \
        if (errCode) { \
          setError_dh("MPI error!", __FUNC__, __FILE__, __LINE__); \
          return; \
        } \
      }

#define CHECK_MPI_ERROR(errCode)  \
      { \
        if (errCode) { \
          setError_dh("MPI error!", __FUNC__, __FILE__, __LINE__); \
          return(errCode); \
        } \
      }

#define SET_V_ERROR(msg)  \
      { setError_dh(msg, __FUNC__, __FILE__, __LINE__); \
        return; \
      }

#define SET_ERROR(retval, msg)  \
      { setError_dh(msg, __FUNC__, __FILE__, __LINE__); \
        return (retval); \
      }

#define CHECK_V_ERROR   \
          if (errFlag_dh) { \
            setError_dh("",  __FUNC__, __FILE__, __LINE__); \
            return; \
          }

#define CHECK_ERROR(retval)  \
          if (errFlag_dh) { \
            setError_dh("",  __FUNC__, __FILE__, __LINE__); \
            return (retval); \
          }

/*---------------------------------------------------------------------- 
 * informational macros
 *---------------------------------------------------------------------- */

#define SET_INFO(msg)  setInfo_dh(msg, __FUNC__, __FILE__, __LINE__);

/*---------------------------------------------------------------------- 
 * macros for tracking the function call stack
 *---------------------------------------------------------------------- */
#ifdef OPTIMIZED_DH

#define START_FUNC_DH   \
          dh_StartFunc(__FUNC__, __FILE__, __LINE__, 1);  \
          {

#define END_FUNC_DH     \
          } \
          dh_EndFunc(__FUNC__, 1);

#define END_FUNC_VAL(a) \
         dh_EndFunc(__FUNC__, 1); \
         return a ; \
         }

#define START_FUNC_DH_2 /**/
#define END_FUNC_DH_2   /**/
#define END_FUNC_VAL_2(a) return a ;

#else

#define START_FUNC_DH  \
          dh_StartFunc(__FUNC__, __FILE__, __LINE__, 1); \
          if (logFuncsToStderr || logFuncsToFile)\
            Error_dhStartFunc(__FUNC__, __FILE__, __LINE__); \
          {

#define END_FUNC_DH   \
          dh_EndFunc(__FUNC__, 1); \
          if (logFuncsToStderr || logFuncsToFile) \
            Error_dhEndFunc(__FUNC__); \
          return; \
          } \

#define START_FUNC_DH_2  \
          dh_StartFunc(__FUNC__, __FILE__, __LINE__, 2); \
          if (logFuncsToStderr || logFuncsToFile)\
            Error_dhStartFunc(__FUNC__, __FILE__, __LINE__); \
          {

#define END_FUNC_DH_2   \
          dh_EndFunc(__FUNC__, 2); \
          if (logFuncsToStderr || logFuncsToFile) \
            Error_dhEndFunc(__FUNC__); \
          return; \
          } \


#define END_FUNC_VAL(retval) \
          dh_EndFunc(__FUNC__, 1); \
          if (logFuncsToStderr || logFuncsToFile) \
            Error_dhEndFunc(__FUNC__); \
          return(retval); \
          } \

#define END_FUNC_VAL_2(retval) \
          dh_EndFunc(__FUNC__, 2); \
          if (logFuncsToStderr || logFuncsToFile) \
            Error_dhEndFunc(__FUNC__); \
          return(retval); \
          } \


#endif 

#endif  /* #ifndef MACROS_DH */
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

#ifndef COMMON_DH
#define COMMON_DH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdarg.h>

#define REAL_DH HYPRE_Real

/*-----------------------------------------------------------------------
 * compile-time dependent includes from other libraries.
 * maintainer's note: this is the only place where non-Euclid
 * files are included.
 *-----------------------------------------------------------------------*/

#if ( !defined(FAKE_MPI) && defined(USING_MPI) && \
      !defined(HYPRE_MODE) && !defined(PETSC_MODE) )
#include <mpi.h> 
#endif

#if defined(FAKE_MPI)
#include "fake_mpi.h"
#endif

#if defined(USING_OPENMP) && !defined(HYPRE_MODE)
#include <omp.h>
#endif

/*-----------------------------------------------------------------------
 * Euclid includes
 *-----------------------------------------------------------------------*/

/* #include "euclid_config.h" */

/* #include "macros_dh.h" */ /* macros for error checking, etc */

/*----------------------------------------------------------- 
 *  Euclid classes 
 *-----------------------------------------------------------*/
typedef struct _matgenfd*           MatGenFD;
typedef struct _subdomain_dh*       SubdomainGraph_dh;
typedef struct _timer_dh*           Timer_dh;
typedef struct _parser_dh*          Parser_dh;
typedef struct _timeLog_dh*         TimeLog_dh;
typedef struct _mem_dh*             Mem_dh;
typedef struct _mat_dh*             Mat_dh;
typedef struct _factor_dh*          Factor_dh;
typedef struct _vec_dh*             Vec_dh;
typedef struct _numbering_dh*       Numbering_dh;
typedef struct _hash_dh*            Hash_dh;
typedef struct _hash_i_dh*          Hash_i_dh;
typedef struct _mpi_interface_dh*   Euclid_dh;
typedef struct _sortedList_dh*      SortedList_dh;
typedef struct _extrows_dh*         ExternalRows_dh;
typedef struct _stack_dh*           Stack_dh;
typedef struct _queue_dh*           Queue_dh;
typedef struct _sortedset_dh*       SortedSet_dh;

/*
typedef struct _localPerm_dh*       LocalPerm_dh;
typedef struct _procGrid_dh*        ProcGrid_dh;
typedef struct _globalPerm_dh*      GlobalPerm_dh;
typedef struct _apply_dh*           Apply_dh;
typedef struct _externalRows_dh*    ExternalRows_dh;
*/

/*---------------------------------------------------------------------
 * misc.
 *---------------------------------------------------------------------*/


#if defined(__cplusplus)
#else
typedef HYPRE_Int bool;
#define true   1
#define false  0
#endif

/* ------------------------------------------------------------------
 * Globally scoped variables, error handling functions, etc.
 * These are all defined in /src/globalObjects.c 
 * ------------------------------------------------------------------*/
HYPRE_THREAD_LOCAL_EP  extern Parser_dh   parser_dh;  /* for setting/getting runtime options */
extern HYPRE_THREAD_LOCAL_EP TimeLog_dh  tlog_dh;    /* internal timing  functionality */
extern HYPRE_THREAD_LOCAL_EP Mem_dh      mem_dh;     /* memory management */
extern HYPRE_THREAD_LOCAL_EP FILE        *logFile;
extern HYPRE_THREAD_LOCAL_EP HYPRE_Int         np_dh;     /* number of processors and subdomains */
extern HYPRE_THREAD_LOCAL_EP HYPRE_Int         myid_dh;   /* rank of this processor (and subdomain) */
extern HYPRE_THREAD_LOCAL_EP MPI_Comm    comm_dh; 


extern HYPRE_THREAD_LOCAL_EP bool ignoreMe;    /* used to stop compiler complaints */
extern HYPRE_THREAD_LOCAL_EP HYPRE_Int  ref_counter; /* for internal use only!  Reference counter
                            to ensure that global objects are not
                            destroyed when Euclid's destructor is called,
                            and more than one instance of Euclid has been
                            instantiated.
                          */


/* Error and message handling.  These are accessed through
 * macros defined in "macros_dh.h"
 */
extern HYPRE_THREAD_LOCAL_EP bool  errFlag_dh;
extern void  setInfo_dh(const char *msg,const char *function,const char *file, HYPRE_Int line);
extern void  setError_dh(const char *msg,const char *function,const char *file, HYPRE_Int line);
extern void  printErrorMsg(FILE *fp);

#ifndef hypre_MPI_MAX_ERROR_STRING
#define hypre_MPI_MAX_ERROR_STRING 256
#endif

#define MSG_BUF_SIZE_DH MAX(1024, hypre_MPI_MAX_ERROR_STRING)
#if defined(HYPRE_USING_RAJA) || defined(HYPRE_USING_KOKKOS) || defined(HYPRE_USING_CUDA)
static HYPRE_THREAD_LOCAL_EP char  msgBuf_dh[MSG_BUF_SIZE_DH];
#else
extern HYPRE_THREAD_LOCAL_EP char  msgBuf_dh[MSG_BUF_SIZE_DH];
#endif

/* Each processor (may) open a logfile.
 * The bools are switches for controlling the amount of informational 
 * output, and where it gets written to.  Function trace logging is only 
 * enabled when compiled with the debugging (-g) option.
 */
extern void openLogfile_dh(HYPRE_Int argc, char *argv[]);
extern void closeLogfile_dh();
extern HYPRE_THREAD_LOCAL_EP bool logInfoToStderr;
extern HYPRE_THREAD_LOCAL_EP bool logInfoToFile;
extern HYPRE_THREAD_LOCAL_EP bool logFuncsToStderr;
extern HYPRE_THREAD_LOCAL_EP bool logFuncsToFile;
extern void Error_dhStartFunc(char *function, char *file, HYPRE_Int line);
extern void Error_dhEndFunc(char *function);
extern void dh_StartFunc(const char *function,const char *file, HYPRE_Int line, HYPRE_Int priority);
extern void dh_EndFunc(const char *function, HYPRE_Int priority);
extern void printFunctionStack(FILE *fp);

extern void EuclidInitialize(HYPRE_Int argc, char *argv[], char *help); /* instantiates global objects */
extern void EuclidFinalize();    /* deletes global objects */
extern bool EuclidIsInitialized(); 
extern void printf_dh(const char *fmt, ...);
extern void fprintf_dh(FILE *fp,const char *fmt, ...);

  /* echo command line invocation to stdout.
     The "prefix" string is for grepping; it may be NULL.
  */
extern void echoInvocation_dh(MPI_Comm comm, char *prefix, HYPRE_Int argc, char *argv[]);


#endif
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

/* for internal use */

#ifndef EXTERNAL_ROWS_DH_H
#define EXTERNAL_ROWS_DH_H

/* #include "euclid_common.h" */

extern void ExternalRows_dhCreate(ExternalRows_dh *er);
extern void ExternalRows_dhDestroy(ExternalRows_dh er);
extern void ExternalRows_dhInit(ExternalRows_dh er, Euclid_dh ctx);
extern void ExternalRows_dhRecvRows(ExternalRows_dh extRows);
extern void ExternalRows_dhSendRows(ExternalRows_dh extRows);
extern void ExternalRows_dhGetRow(ExternalRows_dh er, HYPRE_Int globalRow,
                        HYPRE_Int *len, HYPRE_Int **cval, HYPRE_Int **fill, REAL_DH **aval);

struct _extrows_dh {
    SubdomainGraph_dh sg;  /* not owned! */
    Factor_dh F;           /* not owned! */

    hypre_MPI_Status status[MAX_MPI_TASKS];
    hypre_MPI_Request req1[MAX_MPI_TASKS]; 
    hypre_MPI_Request req2[MAX_MPI_TASKS];
    hypre_MPI_Request req3[MAX_MPI_TASKS]; 
    hypre_MPI_Request req4[MAX_MPI_TASKS];
    hypre_MPI_Request cval_req[MAX_MPI_TASKS];
    hypre_MPI_Request fill_req[MAX_MPI_TASKS];
    hypre_MPI_Request aval_req[MAX_MPI_TASKS];

    /*------------------------------------------------------------------------
     *  data structures for receiving, storing, and accessing external rows 
     *  from lower-ordered nabors
     *------------------------------------------------------------------------*/
    /* for reception of row counts, row numbers, and row lengths: */
    HYPRE_Int rcv_row_counts[MAX_MPI_TASKS]; /* P_i will send rcv_row_counts[i] rows */
    HYPRE_Int rcv_nz_counts[MAX_MPI_TASKS];  /* P_i's rows contain rcv_nz_counts[i] nonzeros */
    HYPRE_Int *rcv_row_lengths[MAX_MPI_TASKS];  /* rcv_row_lengths[i][] lists the length of each row */
    HYPRE_Int *rcv_row_numbers[MAX_MPI_TASKS];  /* rcv_row_lengths[i][] lists the length of each row */

    /* for reception of the actual rows: */
    HYPRE_Int      *cvalExt;
    HYPRE_Int      *fillExt;
    REAL_DH  *avalExt;

    /* table for accessing the rows */
    Hash_dh rowLookup;

    /*--------------------------------------------------------------------------
     *  data structures for sending boundary rows to higher-ordered nabors
     *--------------------------------------------------------------------------*/
    /* for sending row counts, numbers, and lengths: */
    HYPRE_Int *my_row_counts;     /* my_row_counts[i] = nzcount in upper tri portion o */
    HYPRE_Int *my_row_numbers;    /* my_row_numbers[i] = global row number of local ro */

    /* for sending the actual rows: */
    HYPRE_Int     nzSend;      /* total entries in upper tri portions of bdry rows */
    HYPRE_Int     *cvalSend;
    HYPRE_Int     *fillSend;
    REAL_DH  *avalSend;

    bool debug;
};

#endif
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

#ifndef FACTOR_DH
#define FACTOR_DH

/* #include "euclid_common.h" */

struct _factor_dh {
  /* dimensions of local rectangular submatrix; global matrix is n*n */
  HYPRE_Int m, n;    

  HYPRE_Int id;          /* this subdomain's id after reordering */
  HYPRE_Int beg_row;     /* global number of 1st locally owned row */
  HYPRE_Int first_bdry;  /* local number of first boundary row */
  HYPRE_Int bdry_count;  /* m - first_boundary */

  /* if true, factorization was block jacobi, in which case all
     column indices are zero-based; else, they are global.
  */
  bool blockJacobi;

  /* sparse row-oriented storage for locally owned submatrix */
  HYPRE_Int *rp;       
  HYPRE_Int *cval;
  REAL_DH *aval;
  HYPRE_Int *fill;
  HYPRE_Int *diag;
  HYPRE_Int alloc; /* currently allocated length of cval, aval, and fill arrays */

  /* used for PILU solves (Apply) */
  HYPRE_Int          num_recvLo, num_recvHi;
  HYPRE_Int          num_sendLo, num_sendHi;  /* used in destructor */
  HYPRE_Real   *work_y_lo;  /* recv values from lower nabors; also used as
                               work vector when solving Ly=b for y.
                            */
  HYPRE_Real   *work_x_hi;  /* recv values from higher nabors; also used as
                               work vector when solving Ux=y for x.
                            */
  HYPRE_Real   *sendbufLo, *sendbufHi;
  HYPRE_Int          *sendindLo, *sendindHi;
  HYPRE_Int          sendlenLo, sendlenHi;
  bool         solveIsSetup;
  Numbering_dh numbSolve;

  hypre_MPI_Request  recv_reqLo[MAX_MPI_TASKS], recv_reqHi[MAX_MPI_TASKS]; /* used for persistent comms */
  hypre_MPI_Request  send_reqLo[MAX_MPI_TASKS], send_reqHi[MAX_MPI_TASKS]; /* used for persistent comms */
  hypre_MPI_Request  requests[MAX_MPI_TASKS];
  hypre_MPI_Status   status[MAX_MPI_TASKS];  

  bool debug;
};

extern void Factor_dhCreate(Factor_dh *mat);
extern void Factor_dhDestroy(Factor_dh mat);

extern void Factor_dhTranspose(Factor_dh matIN, Factor_dh *matOUT);

extern void Factor_dhInit(void *A, bool fillFlag, bool avalFlag,
                          HYPRE_Real rho, HYPRE_Int id, HYPRE_Int beg_rowP, Factor_dh *F);

extern void Factor_dhReallocate(Factor_dh F, HYPRE_Int used, HYPRE_Int additional);
  /* ensures fill, cval, and aval arrays can accomodate
     at least "c" additional entrie
   */

  /* adopted from ParaSails, by Edmond Chow */
extern void Factor_dhSolveSetup(Factor_dh mat, SubdomainGraph_dh sg);


extern void Factor_dhSolve(HYPRE_Real *rhs, HYPRE_Real *lhs, Euclid_dh ctx);
extern void Factor_dhSolveSeq(HYPRE_Real *rhs, HYPRE_Real *lhs, Euclid_dh ctx);

  /* functions for monitoring stability */
extern HYPRE_Real Factor_dhCondEst(Factor_dh mat, Euclid_dh ctx);
extern HYPRE_Real Factor_dhMaxValue(Factor_dh mat);
extern HYPRE_Real Factor_dhMaxPivotInverse(Factor_dh mat);

extern HYPRE_Int Factor_dhReadNz(Factor_dh mat);
extern void Factor_dhPrintTriples(Factor_dh mat, char *filename);

extern void Factor_dhPrintGraph(Factor_dh mat, char *filename);
 /* seq only */


extern void Factor_dhPrintDiags(Factor_dh mat, FILE *fp);
extern void Factor_dhPrintRows(Factor_dh mat, FILE *fp);
  /* prints local matrix to logfile, if open */

#endif
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

#ifndef VEC_DH_H
#define VEC_DH_H

/* #include "euclid_common.h" */

struct _vec_dh {
  HYPRE_Int n;
  HYPRE_Real *vals;
};

extern void Vec_dhCreate(Vec_dh *v);
extern void Vec_dhDestroy(Vec_dh v);
extern void Vec_dhInit(Vec_dh v, HYPRE_Int size);
        /* allocates storage, but does not initialize values */

extern void Vec_dhDuplicate(Vec_dh v, Vec_dh *out);
        /* creates vec and allocates storage, but neither
         * initializes nor copies values 
         */

extern void Vec_dhCopy(Vec_dh x, Vec_dh y);
        /* copies values from x to y;
         * y must have proper storage allocated,
         * e.g, through previous call to Vec_dhDuplicate,
         * or Vec_dhCreate and Vec_dhInit.
         */

extern void Vec_dhSet(Vec_dh v, HYPRE_Real value);
extern void Vec_dhSetRand(Vec_dh v);

extern void Vec_dhRead(Vec_dh *v, HYPRE_Int ignore, char *filename);
extern void Vec_dhReadBIN(Vec_dh *v, char *filename);
extern void Vec_dhPrint(Vec_dh v, SubdomainGraph_dh sg, char *filename);
extern void Vec_dhPrintBIN(Vec_dh v, SubdomainGraph_dh sg, char *filename); 
#endif
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

#ifndef MATGENFD_DH_DH
#define MATGENFD_DH_DH

/*=====================================================================
option summary:
---------------
processor topology
     -px <HYPRE_Int> -py <HYPRE_Int> -pz <HYPRE_Int>
     defaults:  -px 1 -py 1 -pz 0

grid topology
  -m <HYPRE_Int>
  if pz=0, each processor has a square grid of dimension m*m,
  hence there are m*m*px*py unknowns.
  if pz > 0, each local grid is of dimension m*m*m, hence
  there are m*m*m*px*py*pz unknowns.


diffusion coefficients (default is 1.0):
    -dx <HYPRE_Real> -dy <HYPRE_Real> -dz <HYPRE_Real>

convection coefficients (default is 0.0)
    -cx <HYPRE_Real> -cy <HYPRE_Real> -cz <HYPRE_Real>

grid dimension; if more than one mpi process, this is
the local size for each processor:
     -m <HYPRE_Int>

boundary conditions:
  This is very primitive; boundary conditions can only be generated for
  2D grids; the condition along each side is either dirichlet (constant),
  if bcXX >= 0, or neuman, if bcXX < 0.

   -bcx1 <HYPRE_Real>
   -bcx2 <HYPRE_Real>
   -bcy1 <HYPRE_Real>
   -bcy2 <HYPRE_Real>

Misc.
     -debug_matgen
     -striped (may not work?)
=====================================================================*/

/* #include "euclid_common.h" */

struct _matgenfd {
  bool allocateMem; 
        /* If true, memory is allocated when run() is called, in which case
         * the caller is responsible for calling FREE_DH for the rp, cval,
         * aval, and rhs arrays.  If false, caller is assumed to have
         * allocated memory when run is called.  
         * Default is "true"
         */
  HYPRE_Int px, py, pz;  /* Processor graph dimensions */
  bool threeD;  
  HYPRE_Int m;           /* number of matrix rows in local matrix */
  HYPRE_Int cc;          /* Dimension of each processor's subgrid */
  HYPRE_Real hh;       /* Grid spacing; this is constant,  equal to 1.0/(px*cc-1) */
  HYPRE_Int id;          /* the processor whose submatrix is to be generated */
  HYPRE_Int np;          /* number of subdomains (processors, mpi tasks) */
  HYPRE_Real stencil[8];


  /* derivative coefficients; a,b,c are 2nd derivatives, 
   * c,d,e are 1st derivatives; f,g,h not currently used.
   */
  HYPRE_Real a, b, c, d, e, f, g, h;

  HYPRE_Int first; /* global number of first locally owned row */
  bool debug;

  /* boundary conditions; if value is < 0, neumen; else, dirichelet */
  HYPRE_Real bcX1, bcX2;
  HYPRE_Real bcY1, bcY2;
  HYPRE_Real bcZ1, bcZ2;
                
  /* The following return coefficients; default is konstant() */
  HYPRE_Real (*A)(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
  HYPRE_Real (*B)(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
  HYPRE_Real (*C)(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
  HYPRE_Real (*D)(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
  HYPRE_Real (*E)(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
  HYPRE_Real (*F)(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
  HYPRE_Real (*G)(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
  HYPRE_Real (*H)(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
};

extern void MatGenFD_Create(MatGenFD *mg);
extern void MatGenFD_Destroy(MatGenFD mg);
extern void MatGenFD_Run(MatGenFD mg, HYPRE_Int id, HYPRE_Int np, Mat_dh *A, Vec_dh *rhs);

 /* =========== coefficient functions ============== */
extern HYPRE_Real konstant(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
extern HYPRE_Real e2_xy(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);



/* 3 boxes nested inside the unit square domain.
   diffusivity constants are: -dd1, -dd2, -dd3.
*/
/* box placement */
#define BOX1_X1 0.1
#define BOX1_X2 0.4
#define BOX1_Y1 0.1
#define BOX1_Y2 0.4

#define BOX2_X1 0.6
#define BOX2_X2 0.9
#define BOX2_Y1 0.1
#define BOX2_Y2 0.4

#define BOX3_X1 0.2
#define BOX3_X2 0.8
#define BOX3_Y1 0.6
#define BOX3_Y2 0.8

/* default diffusivity */
#define BOX1_DD  10
#define BOX2_DD  100
#define BOX3_DD  50

extern HYPRE_Real box_1(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);
  /* -bd2 is diffusion coeff outside box;
     -bd1 is diffusion coeff inside box.
  */
     


extern HYPRE_Real box_2(HYPRE_Real coeff, HYPRE_Real x, HYPRE_Real y, HYPRE_Real z);

#endif
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

#ifndef MAT_DH_DH
#define MAT_DH_DH

/* #include "euclid_common.h" */

  /* this stuff for experimental internal timing */
#define MAT_DH_BINS      10
#define MATVEC_TIME       0  /* time to actually perform matvec */
#define MATVEC_MPI_TIME   1  /* time for comms + vector copying needed */
#define MATVEC_MPI_TIME2  5  /* time for comms, + vector copying needed */
#define MATVEC_TOTAL_TIME 2  /* MATVEC_TIME+MATVEC_MPI_TIME */
#define MATVEC_RATIO      3  /* computation/communication ratio */
#define MATVEC_WORDS      4  /* total words sent to other procs. */

struct _mat_dh {
  HYPRE_Int m, n;    /* dimensions of local rectangular submatrix;
                * the global matrix is n by n.
                */
  HYPRE_Int beg_row;   /* global number of 1st locally owned row */
  HYPRE_Int bs;        /* block size */

  /* sparse row-oriented storage for locally owned submatrix */
  HYPRE_Int *rp;       
  HYPRE_Int *len;   /* length of each row; only used for MPI triangular solves */
  HYPRE_Int *cval;
  HYPRE_Int *fill;
  HYPRE_Int *diag;
  HYPRE_Real *aval;
  bool owner;  /* for MPI triangular solves */

  /* working space for getRow */
  HYPRE_Int len_private;
  HYPRE_Int rowCheckedOut;
  HYPRE_Int *cval_private;
  HYPRE_Real *aval_private;

  /* row permutations to increase positive definiteness */
  HYPRE_Int *row_perm;

  /* for timing matvecs in experimental studies */
  HYPRE_Real time[MAT_DH_BINS];
  HYPRE_Real time_max[MAT_DH_BINS];
  HYPRE_Real time_min[MAT_DH_BINS];
  bool matvec_timing;

  /* used for MatVecs */
  HYPRE_Int          num_recv; 
  HYPRE_Int          num_send;   /* used in destructor */
  hypre_MPI_Request  *recv_req;
  hypre_MPI_Request  *send_req; 
  HYPRE_Real   *recvbuf, *sendbuf;  
  HYPRE_Int          *sendind;
  HYPRE_Int          sendlen;               
  HYPRE_Int          recvlen;               
  bool         matvecIsSetup;
  Numbering_dh numb;
  hypre_MPI_Status   *status;  

  bool debug;
};

extern void Mat_dhCreate(Mat_dh *mat);
extern void Mat_dhDestroy(Mat_dh mat);

extern void Mat_dhTranspose(Mat_dh matIN, Mat_dh *matOUT);
extern void Mat_dhMakeStructurallySymmetric(Mat_dh A);

  /* adopted from ParaSails, by Edmond Chow */
extern void Mat_dhMatVecSetup(Mat_dh mat);
extern void Mat_dhMatVecSetdown(Mat_dh mat);

/*========================================================================*/
/* notes: if not compiled with OpenMP, Mat_dhMatVec() and Mat_dhMatVec_omp()
          perform identically; similarly for Mat_dhMatVec_uni()
          and Mat_dhMatVec_uni_omp()
*/

extern void Mat_dhMatVec(Mat_dh mat, HYPRE_Real *lhs, HYPRE_Real *rhs);
  /* unthreaded MPI version */

extern void Mat_dhMatVec_omp(Mat_dh mat, HYPRE_Real *lhs, HYPRE_Real *rhs);
  /* OpenMP/MPI version */

extern void Mat_dhMatVec_uni(Mat_dh mat, HYPRE_Real *lhs, HYPRE_Real *rhs);
  /* unthreaded, single-task version */

extern void Mat_dhMatVec_uni_omp(Mat_dh mat, HYPRE_Real *lhs, HYPRE_Real *rhs);
  /* OpenMP/single primary task version */


extern HYPRE_Int Mat_dhReadNz(Mat_dh mat);

  /* for next five, SubdomainGraph_dh() may be NULL; if not null,
     caller must ensure it has been properly initialized;
     if not null, matrix is permuted before printing.

     note: use "-matlab" when calling Mat_dhPrintTriples, to
           insert small value in place of 0.

     Mat_dhPrintCSR only implemented for single cpu, no reordering.
   */
extern void Mat_dhPrintGraph(Mat_dh mat, SubdomainGraph_dh sg, FILE *fp);
extern void Mat_dhPrintRows(Mat_dh mat, SubdomainGraph_dh sg, FILE *fp);

extern void Mat_dhPrintCSR(Mat_dh mat, SubdomainGraph_dh sg, char *filename);
extern void Mat_dhPrintTriples(Mat_dh mat, SubdomainGraph_dh sg, char *filename);
extern void Mat_dhPrintBIN(Mat_dh mat, SubdomainGraph_dh sg, char *filename);

extern void Mat_dhReadCSR(Mat_dh *mat, char *filename);
extern void Mat_dhReadTriples(Mat_dh *mat, HYPRE_Int ignore, char *filename);
extern void Mat_dhReadBIN(Mat_dh *mat, char *filename);


extern void Mat_dhPermute(Mat_dh Ain, HYPRE_Int *pIN, Mat_dh *Bout);
  /* for single cpu only! */

extern void Mat_dhFixDiags(Mat_dh A);
  /* inserts diagonal if not explicitly present;
     sets diagonal value in row i to sum of absolute
     values of all elts in row i.
  */

extern void Mat_dhPrintDiags(Mat_dh A, FILE *fp);

extern void Mat_dhGetRow(Mat_dh B, HYPRE_Int globalRow, HYPRE_Int *len, HYPRE_Int **ind, HYPRE_Real **val);
extern void Mat_dhRestoreRow(Mat_dh B, HYPRE_Int row, HYPRE_Int *len, HYPRE_Int **ind, HYPRE_Real **val);

  /* partition matrix into "k" blocks.  User must free storage. */
extern void Mat_dhPartition(Mat_dh mat, HYPRE_Int k, HYPRE_Int **beg_rowOUT, 
                            HYPRE_Int **row_countOUT, HYPRE_Int **n2oOUT, HYPRE_Int **o2nOUT);




extern void Mat_dhZeroTiming(Mat_dh mat);
extern void Mat_dhReduceTiming(Mat_dh mat);


extern void Mat_dhRowPermute(Mat_dh);

extern void dldperm(HYPRE_Int job, HYPRE_Int n, HYPRE_Int nnz, HYPRE_Int colptr[], HYPRE_Int adjncy[],
                HYPRE_Real nzval[], HYPRE_Int *perm, HYPRE_Real u[], HYPRE_Real v[]);


#endif
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

#ifndef SUBDOMAIN_GRAPH_DH
#define SUBDOMAIN_GRAPH_DH

/* #include "euclid_common.h" */

#define MAX_SUBDOMAIN_COLOR 100
  /* could be done better: if we can't color the subdomain graph
     with this many colors, an error is thrown in SubdomainGraph_dhColor().
   */

/* for internal timing */
#define TIMING_BINS_SG 10
enum{ TOTAL_SGT,  /* total Init (setup) time */
      FIND_NABORS_SGT,
      ORDER_BDRY_SGT,
      FORM_GRAPH_SGT,
      EXCHANGE_PERMS_SGT
    };

struct _subdomain_dh {
  HYPRE_Int blocks;          /* number of subdomains */
  HYPRE_Int *ptrs, *adj;     /* csr structure for representing subdomain graph */
  HYPRE_Int *o2n_sub;        /* subdomain graph permutation; */
  HYPRE_Int *n2o_sub;        /* inverse permutation; */
  HYPRE_Int colors;    /* number of colors used  for coloring the subdomain graph */
  bool doNotColor; /* if true, subdomain graph is not colored and reordered */
  HYPRE_Int *colorVec;  /* if colorVec[i] = x, then subdomain i was colored "x".
                     this array is probably only useful for debugging.
                   */

  HYPRE_Int *beg_row;   /* global ordering of first local row owned by P_i */
  HYPRE_Int *beg_rowP;  /* global ordering of first local row owned by P_i after
                     subdomain reordering 
                   */
  HYPRE_Int *row_count; /* P_i owns row_count[i] local rows */
  HYPRE_Int *bdry_count; /* bdry_count[i] of P_i's rows are boundary rows */

  /* Nearest neighbors in subdomain graph, before reordering;
     "self" is not included.  Not used for sequential case.
   */
  HYPRE_Int *loNabors, loCount;
  HYPRE_Int *hiNabors, hiCount;
  HYPRE_Int *allNabors, allCount;


  /* permutation information for global unknowns (matrix rows) */
  HYPRE_Int m;               /* length of n2o_row and o2n_col */
  HYPRE_Int *n2o_row;        /* permutation for locally owned matrix rows */
  HYPRE_Int *o2n_col;        /* permutation for locally owned matrix columns */

  Hash_i_dh o2n_ext;   /* permutation for external columns */
  Hash_i_dh n2o_ext;   /* inverse permutation for external columns */

  HYPRE_Real timing[TIMING_BINS_SG];
  bool debug;
};

extern void SubdomainGraph_dhCreate(SubdomainGraph_dh *s);
extern void SubdomainGraph_dhDestroy(SubdomainGraph_dh s);

extern void SubdomainGraph_dhInit(SubdomainGraph_dh s, HYPRE_Int blocks, bool bj, void *A);
  /* Partitions matrix A into the specified number of blocks,
     if there is a single MPI task; for mpi use, "blocks" must be the same 
     as the number of mpi tasks; for sequential, it may vary.
     On completion, the subdomain graph will be fully formed,
     (all fields valid); o2n_row[] and n2o_col[] will be permutations
     for the locally owned portion of A such that A's interior nodes are
     ordered first.
     This function may call a partitioner, such as METIS (currently, only for sequential).
     On completion, "o2n" contains a natural ordering, beg_row is identical to
     beg_rowP, and_rowP is identical to end_rowP.

     if "bj" is true, the following setup steps are NOT performed:
     form subdomain graph; find neighbors; order boundary nodes
  */

extern void SubdomainGraph_dhColor(SubdomainGraph_dh s);
  /*
     Colors and orders subdomain graph; on completion, o2n[], beg_rowP[], and
     end_rowP[] may be altered.
  */

extern HYPRE_Int SubdomainGraph_dhFindOwner(SubdomainGraph_dh s, HYPRE_Int idx, bool permuted);
  /* Returns the subdomain block to which row idx belongs, or throws an error.
     If "permuted" is true, it's assumed the graph has been permuted (i.e.,
     'globally reordering phase' in PILU algorithm).
  */

extern void SubdomainGraph_dhExchangePerms(SubdomainGraph_dh s);
  /*
     exchange permutation information for external columns with nearest neighbors;
     caller must ensure SubdomainGraph_dhInit() has completed before calling.
  */

extern void SubdomainGraph_dhPrintSubdomainGraph(SubdomainGraph_dh s, FILE *fp);

extern void SubdomainGraph_dhPrintStatsLong(SubdomainGraph_dh s, FILE *fp);
  /* similar to Short, but prints complete list of interior/bdry node ratios;
     also prints subdomain permutation
   */

extern void SubdomainGraph_dhDump(SubdomainGraph_dh s, char *filename);
  /* for testing */

extern void SubdomainGraph_dhPrintRatios(SubdomainGraph_dh s, FILE *fp);
  /* prints ratios of interior/boundary node for all subdomains */


extern void SubdomainGraph_dhPrintStats(SubdomainGraph_dh sg, FILE *fp);

#endif
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

/*
    Euclid employs a global object: 

        TimeLog_dh timlog_dh;

    for recording timing information.  
*/

#ifndef TIMELOG_DH_DH
#define TIMELOG_DH_DH

/* #include "euclid_common.h" */

extern void TimeLog_dhCreate(TimeLog_dh *t);
extern void TimeLog_dhDestroy(TimeLog_dh t);
extern void TimeLog_dhStart(TimeLog_dh t);
extern void TimeLog_dhStop(TimeLog_dh t);
extern void TimeLog_dhReset(TimeLog_dh t);
extern void TimeLog_dhMark(TimeLog_dh t, const char *description);
extern void TimeLog_dhPrint(TimeLog_dh t, FILE *fp, bool allPrint);

#endif
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

#ifndef SORTED_SET_DH
#define SORTED_SET_DH

/* #include "euclid_common.h" */

struct _sortedset_dh {
  HYPRE_Int n;   /* max items that can be stored */
  HYPRE_Int *list;  /* list of inserted elements */
  HYPRE_Int count;  /* the number of elements in the list */
};

extern void SortedSet_dhCreate(SortedSet_dh *ss, HYPRE_Int initialSize);
extern void SortedSet_dhDestroy(SortedSet_dh ss);
extern void SortedSet_dhInsert(SortedSet_dh ss, HYPRE_Int idx);
extern void SortedSet_dhGetList(SortedSet_dh ss, HYPRE_Int **list, HYPRE_Int *count);


#endif
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

#ifndef MEM_DH_DH
#define MEM_DH_DH

/* #include "euclid_common.h" */

extern void  Mem_dhCreate(Mem_dh *m);
extern void  Mem_dhDestroy(Mem_dh m);

extern void *Mem_dhMalloc(Mem_dh m, size_t size);
extern void  Mem_dhFree(Mem_dh m, void *ptr);
  /* preceeding two are called via the macros
     MALLOC_DH and FREE_DH; see "euclid_config.h"
   */

extern void  Mem_dhPrint(Mem_dh m, FILE* fp, bool allPrint);
  /* prints memory usage statistics;  "allPrint" is only
     meaningful when running in MPI mode.
   */

#endif
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

#ifndef SUPPORT_DH
#define SUPPORT_DH

/* #include "euclid_common.h" */

extern void shellSort_int(const HYPRE_Int n, HYPRE_Int *x);
extern void shellSort_float(HYPRE_Int n, HYPRE_Real *v);

/*
extern void shellSort_int_int(const HYPRE_Int n, HYPRE_Int *x, HYPRE_Int *y);
extern void shellSort_int_float(HYPRE_Int n, HYPRE_Int *x, HYPRE_Real *v);
extern void shellSort_int_int_float(HYPRE_Int n, HYPRE_Int *x, HYPRE_Int *y, HYPRE_Real *v);
*/

#endif
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

#ifndef NUMBERING_DH_H
#define NUMBERING_DH_H

/* code and algorithms in this class adopted from Edmond Chow's
   ParaSails
*/

/* #include "euclid_common.h" */

struct _numbering_dh {
  HYPRE_Int   size;    /* max number of indices that can be stored;
                    (length of idx_ext[]) 
                  */
  HYPRE_Int   first;   /* global number of 1st local index (row) */
  HYPRE_Int   m;       /* number of local indices (number of local rows in mat) */
  HYPRE_Int   *idx_ext;   /* sorted list of external indices */
  HYPRE_Int   *idx_extLo; /* sorted list of external indices that are < first */
  HYPRE_Int   *idx_extHi; /* sorted list of external indices that are >= first+m */
  HYPRE_Int   num_ext; /* number of external (non-local) indices = num_extLo+num_extHi */
  HYPRE_Int   num_extLo; /* number of external indices < first */
  HYPRE_Int   num_extHi; /* number of external indices >= first+num_loc */
  Hash_i_dh global_to_local;

  bool debug;
};

extern void Numbering_dhCreate(Numbering_dh *numb);
extern void Numbering_dhDestroy(Numbering_dh numb);

  /* must be called before calling Numbering_dhGlobalToLocal() or
     Numbering_dhLocalToGlobal().
   */
extern void Numbering_dhSetup(Numbering_dh numb, Mat_dh mat);


  /* input: global_in[len], which contains global row numbers.
     output: local_out[len], containing corresponding local numbers.
     note: global_in[] and local_out[] may be identical.
   */
extern void Numbering_dhGlobalToLocal(Numbering_dh numb, HYPRE_Int len, 
                                      HYPRE_Int *global_in, HYPRE_Int *local_out);

#endif
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

/* This is similar to the Hash_i_dh class (woe, for a lack
   of templates); this this class is for hashing data
   consisting of single, non-negative integers.
*/

#ifndef HASH_I_DH
#define HASH_I_DH

/* #include "euclid_common.h" */
                                 
/*
    class methods 
    note: all parameters are inputs; the only output 
          is the "HYPRE_Int" returned by Hash_i_dhLookup.
*/
extern void Hash_i_dhCreate(Hash_i_dh *h, HYPRE_Int size);
  /* For proper operation, "size," which is the minimal
     size of the hash table, must be a power of 2.
     Or, pass "-1" to use the default.
   */


extern void Hash_i_dhDestroy(Hash_i_dh h);
extern void Hash_i_dhReset(Hash_i_dh h);

extern void Hash_i_dhInsert(Hash_i_dh h, HYPRE_Int key, HYPRE_Int data);
  /* throws error if <data, data> is already inserted;
     grows hash table if out of space.
   */

extern HYPRE_Int  Hash_i_dhLookup(Hash_i_dh h, HYPRE_Int key);
    /* returns "data" associated with "key,"
       or -1 if "key" is not found.
     */

#endif
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

#ifndef TIMER_DH_H
#define TIMER_DH_H

/* #include "euclid_common.h" */

/*--------------------------------------------------------------*/
/* Stuph in this block isn't really needed for multi-processor
 * runs, since recording CPU time probably isn't useful.
 * if EUCLID_TIMING is defined in $PCPACK_DIR/bmake_XXX/common,
 * the times() function is used;
 * then hypre_MPI_Wtime() is used in preference to times().
 *
 * You may need to fiddle with some of these includes, depending
 * on your system.  Make sure and check the logFile to ensure
 * that CLK_TCK was properly defined.  See Timer_dhCreate()
 * for additional details. 
 *
 * if "JUNK_TIMING" is defined during compilation, timing functions
 * either do nothing, or return -1.0; this is primarily for debugging.
 */

#include "HYPRE_config.h"

#ifdef EUCLID_TIMING
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>

#elif !defined(JUNK_TIMING)
#include <time.h>
#ifndef WIN32
#include <unistd.h>  /* needed for sysconf(_SC_CLK_TCK) */
#endif
#endif


/* 
   ??? may be needed for some compilers/platforms?
#include <limits.h>
#include <time.h>
#include <sys/resource.h>
*/

/*--------------------------------------------------------------*/


struct _timer_dh {
  bool isRunning;
  hypre_longint sc_clk_tck;
  HYPRE_Real begin_wall; 
  HYPRE_Real end_wall;

#ifdef EUCLID_TIMING
  struct tms  begin_cpu;
  struct tms  end_cpu;
#endif
 
};

extern void Timer_dhCreate(Timer_dh *t);
extern void Timer_dhDestroy(Timer_dh t);
extern void Timer_dhStart(Timer_dh t);
extern void Timer_dhStop(Timer_dh t);
extern HYPRE_Real Timer_dhReadCPU(Timer_dh t);
extern HYPRE_Real Timer_dhReadWall(Timer_dh t);
extern HYPRE_Real Timer_dhReadUsage(Timer_dh t);

/* notes:
    (1)  unless compiled with EUCLID_TIMING defined, readCPU 
         and readUseage return -1.0.
    (2)  whenever start() is called, the timer is reset; you
         don't need to call stop() first.
    (3)  if stop() HAS been called, the readXX functions return
         timings between start() and stop(); , if start()
         was called but not stop(), they sample the time then
         return; if start() was never called, they return junk.
*/


#endif
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

#ifndef PARSER_DH_DH
#define PARSER_DH_DH

/* #include "euclid_common.h" */

extern void Parser_dhCreate(Parser_dh *p);
extern void Parser_dhDestroy(Parser_dh p);

extern bool Parser_dhHasSwitch(Parser_dh p,const char *in);
extern bool Parser_dhReadString(Parser_dh p,const char *in, char **out);
extern bool Parser_dhReadInt(Parser_dh p,const char *in, HYPRE_Int *out);
extern bool Parser_dhReadDouble(Parser_dh p,const char *in, HYPRE_Real *out);
  /* if the flag (char *in) is found, these four return 
     true and set "out" accordingly.  If not found, they return 
     false, and "out" is unaltered.
   */

extern void Parser_dhPrint(Parser_dh p, FILE *fp, bool allPrint);
  /* Outputs all <flag,value> pairs.  "bool allPrint" is
   * only meaningful when Euclid is compiled in MPI mode
   */

extern void Parser_dhInsert(Parser_dh p,const char *name,const char *value);
  /* For inserting a new <flag,value> pair, or altering
   * the value of an existing pair from within user apps.
   */

extern void Parser_dhUpdateFromFile(Parser_dh p,const char *name);

extern void Parser_dhInit(Parser_dh p, HYPRE_Int argc, char *argv[]);
  /* Init enters <flag,value> pairs in its internal database in
     the following order:

       (1)   $PCPACK_DIR/options_database  
       (2)   "database" in local directory, if the file exists
       (3)   "pathname/foo" if argv[] contains a pair of entries:
               -db_filename pathname/foo
       (4)   flag,value pairs from the command line (ie, argv)

      If a flag already exists, its value is updated if it is
      encountered a second time.  

      WARNING! to enter a negative value, you must use two dashes, e.g:
                      -myvalue  --0.1
               otherwise, if you code "-myvalue -0.1" you will have entered
               the pair of entries <-myvalue, 1>, <-0.1, 1>.  Yuck!@#
               But this works, since Euclid doesn't use negative numbers much.

      If the 2nd entry is missing, a value of "1" is assumed (this only
      works on the command line; for files, you must explicitly code a
      value.  See $PCPACK_DIR/options_database for samples.

      The following will cause Parser_dhHasSwitch(p, "-phoo") to return false:
          -phoo 0
          -phoo false
          -phoo False
          -phoo FALSE
      any other value, including something silly like -phoo 0.0
      will return true.
   */

#endif

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

#ifndef SORTEDLIST_DH_H
#define SORTEDLIST_DH_H

/* for private use by mpi factorization algorithms */

/* #include "euclid_common.h" */

typedef struct _srecord {
    HYPRE_Int    col;
    HYPRE_Int    level;
    HYPRE_Real val;
    HYPRE_Int next;
} SRecord;


extern void SortedList_dhCreate(SortedList_dh *sList);
extern void SortedList_dhDestroy(SortedList_dh sList);
extern void SortedList_dhInit(SortedList_dh sList, SubdomainGraph_dh sg);
extern void SortedList_dhEnforceConstraint(SortedList_dh sList, SubdomainGraph_dh sg);

extern void SortedList_dhReset(SortedList_dh sList, HYPRE_Int row);

extern HYPRE_Int SortedList_dhReadCount(SortedList_dh sList);
  /* returns number of records inserted since last reset */

extern void SortedList_dhResetGetSmallest(SortedList_dh sList);
  /* resets index used for SortedList_dhGetSmallestLowerTri().
   */

extern SRecord * SortedList_dhGetSmallest(SortedList_dh sList);
  /* returns record with smallest column value that hasn't been
     retrieved via this method since last call to SortedList_dhReset()
     or SortedList_dhResetGetSmallest().
     If all records have been retrieved, returns NULL.
   */

extern SRecord * SortedList_dhGetSmallestLowerTri(SortedList_dh sList);
  /* returns record with smallest column value that hasn't been
     retrieved via this method since last call to reset.  
     Only returns records where SRecord sr.col < row (per Init).
     If all records have been retrieved, returns NULL.
   */

extern void SortedList_dhInsert(SortedList_dh sList, SRecord *sr);
  /* unilateral insert (does not check to see if item is already
     in list); does not permute sr->col; used in numeric
     factorization routines.
   */

extern void SortedList_dhInsertOrUpdateVal(SortedList_dh sList, SRecord *sr);
  /* unilateral insert: does not check to see if already
     inserted; does not permute sr->col; used in numeric 
     factorization routines.
   */

extern bool SortedList_dhPermuteAndInsert(SortedList_dh sList, SRecord *sr, HYPRE_Real thresh);
  /* permutes sr->col, and inserts record in sorted list.
     Note: the contents of the passed variable "sr" may be changed.
     Note: this performs sparsification 
  */


extern void SortedList_dhInsertOrUpdate(SortedList_dh sList, SRecord *sr);
  /* if a record with identical sr->col was inserted, updates sr->level
     to smallest of the two values; otherwise, inserts the record.
     Unlike SortedList_dhPermuteAndInsert, does not permute sr->col.
     Note: the contents of the passed variable "sr" may be changed.
     Warning: do not call SortedList_dhGetSmallestLowerTri() again
     until reset is called.
  */

extern SRecord * SortedList_dhFind(SortedList_dh sList, SRecord *sr);
  /* returns NULL if no record is found containing sr->col 
   */

extern void SortedList_dhUpdateVal(SortedList_dh sList, SRecord *sr);

#endif
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

#ifndef HASH_D_DH
#define HASH_D_DH

/* todo: rehash should be implemented in Hash_dhInsert();
         as of now, an error is set if the table overflows.
*/

/* #include "euclid_common.h" */

/* This should be done with templates, if this were in C++;
   for now, a record contains every type of entry we might
   need; this is a waste of memory, when one is only intersted
   in hashing <key, HYPRE_Int> pairs!
*/
typedef struct _hash_node {
  HYPRE_Int     iData;      /* integer */
  HYPRE_Real  fData;      /* float */
  HYPRE_Int     *iDataPtr;  /* pointer to integer */
  HYPRE_Int     *iDataPtr2; /* pointer to integer */
  HYPRE_Real  *fDataPtr;  /* pointer to float */
} HashData;


typedef struct _hash_node_private HashRecord;

/* data structure for the hash table; do not directly access */
struct _hash_dh {
  HYPRE_Int         size;   /* total slots in table */
  HYPRE_Int         count;  /* number of insertions in table */
  HYPRE_Int         curMark;
  HashRecord  *data;
};


extern void Hash_dhCreate(Hash_dh *h, HYPRE_Int size);
extern void Hash_dhDestroy(Hash_dh h);
extern void Hash_dhInsert(Hash_dh h, HYPRE_Int key, HashData *data);
extern HashData *Hash_dhLookup(Hash_dh h, HYPRE_Int key);
         /* returns NULL if record isn't found. */

extern void Hash_dhReset(Hash_dh h);
extern void Hash_dhPrint(Hash_dh h, FILE *fp);


#define HASH_1(k,size,idxOut)    \
         {  *idxOut = k % size;  }

#define HASH_2(k,size,idxOut)      \
          {  \
            HYPRE_Int r = k % (size-13); \
            r = (r % 2) ? r : r+1; \
            *idxOut = r;           \
          }

#endif
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

#ifndef MAT_DH_PRIVATE
#define MAT_DH_PRIVATE

/* Functions called by Mat_dh, Factor_dh, and possibly others.
   Also, a few handy functions for dealing with permutations,
   etc.
 
 */

/* #include "euclid_common.h" */

extern HYPRE_Int mat_find_owner(HYPRE_Int *beg_rows, HYPRE_Int *end_rows, HYPRE_Int index);

extern void mat_dh_transpose_private(HYPRE_Int m, HYPRE_Int *rpIN, HYPRE_Int **rpOUT,
                                     HYPRE_Int *cvalIN, HYPRE_Int **cvalOUT,
                                     HYPRE_Real *avalIN, HYPRE_Real **avalOUT);

  /* same as above, but memory for output was already allocated */
extern void mat_dh_transpose_reuse_private(HYPRE_Int m, 
                                     HYPRE_Int *rpIN, HYPRE_Int *cvalIN, HYPRE_Real *avalIN,
                                     HYPRE_Int *rpOUT, HYPRE_Int *cvalOUT, HYPRE_Real *avalOUT);

/*-------------------------------------------------------------------------
 * utility functions for reading and writing matrices in various formats.
 * currently recognized filetypes (formats) are:
 *    trip
 *    csr
 *    petsc
 * the "ignore" parameter is only used for the matrix "trip" format,
 * and the vector "csr" and "trip" formats (which are misnamed, and identical);
 * the intention is to skip over the first "ignore" lines of the file;
 * this is a hack to enable reading of Matrix Market, etc, formats. 
 *-------------------------------------------------------------------------*/
extern void readMat(Mat_dh *Aout, char *fileType, char *fileName, HYPRE_Int ignore);
extern void readVec(Vec_dh *bout, char *fileType, char *fileName, HYPRE_Int ignore);
extern void writeMat(Mat_dh Ain, char *fileType, char *fileName);
extern void writeVec(Vec_dh b, char *fileType, char *fileName);

/* Next function is primarily (?) for testing/development/debugging.
   P_0 reads and partitions the matrix, then distributes 
   amongst the other processors.
*/
extern void readMat_par(Mat_dh *Aout, char *fileType, char *fileName, HYPRE_Int ignore);

extern void profileMat(Mat_dh A);
  /* writes structural and numerical symmetry and other info to stdout;
     for a single mpi task only.
  */



/*-------------------------------------------------------------------------*
 * functions called by public Mat_dh class methods.
 *
 *   (following notes need to be updated!)
 *
 *         m is number of local rows;
 *         beg_row is global number of 1st locally owned row;
 *         m, beg_row, rp, cval may not be null (caller's responsiblity);
 *         if n2o is NULL, it's assumed that o2n is NULL;
 *         if 
 *
 *         error thrown:
 *         if a nonlocal column (a column index that is less than beg_row,
 *         or >= beg_row+m), and can't be located in hash table.
 *
 *         print_triples_private() and print_mat_private() are 1-based.
 *
 *-------------------------------------------------------------------------*/

/* seq or mpi */
extern void mat_dh_print_graph_private(HYPRE_Int m, HYPRE_Int beg_row, HYPRE_Int *rp, HYPRE_Int *cval, 
                   HYPRE_Real *aval, HYPRE_Int *n2o, HYPRE_Int *o2n, Hash_i_dh hash, FILE* fp);


/* seq; reordering not implemented */
/* see io_dh.h
                                HYPRE_Int *rp, HYPRE_Int *cval, HYPRE_Real *aval, 
                           HYPRE_Int *n2o, HYPRE_Int *o2n, Hash_i_dh hash, char *filename);
*/

/* seq only */
extern void mat_dh_print_csr_private(HYPRE_Int m, HYPRE_Int *rp, HYPRE_Int *cval, HYPRE_Real *aval,
                                                                    FILE* fp); 


/* seq only */
extern void mat_dh_read_csr_private(HYPRE_Int *m, HYPRE_Int **rp, HYPRE_Int **cval, HYPRE_Real **aval,
                                                                    FILE* fp); 

/* seq only */
extern void mat_dh_read_triples_private(HYPRE_Int ignore, HYPRE_Int *m, HYPRE_Int **rp, 
                                         HYPRE_Int **cval, HYPRE_Real **aval, FILE* fp); 

/* seq or mpi */ 
/* see io_dh.h
                                     HYPRE_Real **aval, char *filename);
*/

/*-------------------------------------------------------------------------*/

extern void create_nat_ordering_private(HYPRE_Int m, HYPRE_Int **p);
extern void destroy_nat_ordering_private(HYPRE_Int *p);
extern void invert_perm(HYPRE_Int m, HYPRE_Int *pIN, HYPRE_Int *pOUT);


extern void make_full_private(HYPRE_Int m, HYPRE_Int **rp, HYPRE_Int **cval, HYPRE_Real **aval);
  /* converts upper or lower triangular to full;
     may bomb if input is not triangular!
   */

extern void make_symmetric_private(HYPRE_Int m, HYPRE_Int **rp, HYPRE_Int **cval, HYPRE_Real **aval);
  /* pads with zeros to make structurally symmetric. */

extern void make_symmetric_private(HYPRE_Int m, HYPRE_Int **rp, HYPRE_Int **cval, HYPRE_Real **aval);

#endif
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

#ifndef GET_ROW_DH
#define GET_ROW_DH

/* #include "euclid_common.h" */

/* "row" refers to global row number */

extern void EuclidGetDimensions(void *A, HYPRE_Int *beg_row, HYPRE_Int *rowsLocal, HYPRE_Int *rowsGlobal);
extern void EuclidGetRow(void *A, HYPRE_Int row, HYPRE_Int *len, HYPRE_Int **ind, HYPRE_Real **val);
extern void EuclidRestoreRow(void *A, HYPRE_Int row, HYPRE_Int *len, HYPRE_Int **ind, HYPRE_Real **val);

extern HYPRE_Int EuclidReadLocalNz(void *A);

extern void PrintMatUsingGetRow(void* A, HYPRE_Int beg_row, HYPRE_Int m,
                          HYPRE_Int *n2o_row, HYPRE_Int *n2o_col, char *filename);


#endif

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

#ifndef ILU_MPI_DH
#define ILU_MPI_DH

/* #include "euclid_common.h" */

void reallocate_private(HYPRE_Int row, HYPRE_Int newEntries, HYPRE_Int *nzHave,
                HYPRE_Int **rp, HYPRE_Int **cval, float **aval, HYPRE_Real **avalD, HYPRE_Int **fill);

extern void ilu_mpi_pilu(Euclid_dh ctx);
  /* driver for comms intermingled with factorization */


extern void iluk_mpi_pilu(Euclid_dh ctx);
  /* the factorization algorithm */

extern void compute_scaling_private(HYPRE_Int row, HYPRE_Int len, HYPRE_Real *AVAL, Euclid_dh ctx);

extern void iluk_mpi_bj(Euclid_dh ctx);

extern void iluk_seq(Euclid_dh ctx);
extern void iluk_seq_block(Euclid_dh ctx);
  /* for sequential or parallel block jacobi.  If used
     for block jacobi, column indices are referenced to 0
     on return; make sure and add beg_row to these values
     before printing the matrix!

     1st version is for single precision, 2nd is for HYPRE_Real.
   */

extern void ilut_seq(Euclid_dh ctx);


#endif

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

#ifndef EUCLID_MPI_INTERFACE_DH
#define EUCLID_MPI_INTERFACE_DH

#define DEFAULT_DROP_TOL 0.01

/* #include "euclid_common.h" */

/*======================================================================
 * Naming convention: functions ending in _mpi are located in
 * src/Euclid_mpi.c; those ending in _seq are in src/Euclid_seq.c;
 * most others should be in Euclid_all.c.
 *
 * Exceptions: all Apply() (triangular solves) are in src/Euclid_apply.c;
 *             except for the Apply for MPI PILU, which is called
 *             Mat_dhSolve, and is in src/Mat_dh.c
 *
 * Users should only need to call functions with names of the form
 * Euclid_dhXXX (public functions). 
 *
 * Some of the functions whose names are of the form XXX_private_XXX,
 * as could easily be static functions; similarly, the enums and
 * structs do need to be public.  They are, primarily, for ease in
 * debugging and ready reference.
 *
 * Exceptions: the apply_private functions aren't listed here --- they're
 * all static in src/Euclid_apply.c
 *======================================================================*/

extern void Euclid_dhCreate(Euclid_dh *ctxOUT);
extern void Euclid_dhDestroy(Euclid_dh ctx);
extern void Euclid_dhSetup(Euclid_dh ctx);
extern void Euclid_dhSolve(Euclid_dh ctx, Vec_dh lhs, Vec_dh rhs, HYPRE_Int *its);
extern void Euclid_dhApply(Euclid_dh ctx, HYPRE_Real *lhs, HYPRE_Real *rhs);

extern void Euclid_dhPrintTestData(Euclid_dh ctx, FILE *fp);
extern void Euclid_dhPrintScaling(Euclid_dh ctx, FILE *fp);

extern void Euclid_dhPrintStatsShort(Euclid_dh ctx, HYPRE_Real setup, HYPRE_Real solve, FILE *fp);


extern void Euclid_dhPrintStatsShorter(Euclid_dh ctx, FILE *fp);
  /* on-line reporting, for making quick tables */

extern void Euclid_dhPrintHypreReport(Euclid_dh ctx, FILE *fp);

extern void Euclid_dhPrintStats(Euclid_dh ctx, FILE *fp);
  /* prints same info as Euclid_dhPrintParams(), but also
     prints timing information, number of iterations, etc;
     may be called after solve is completed.
   */

#ifdef HYPRE_MODE
/* is this still needed? */
extern void Euclid_dhInputHypreMat(Euclid_dh ctx, HYPRE_ParCSRMatrix A);
#endif

/*----------------------------------------------------------------------
 * Private data structures
 *----------------------------------------------------------------------*/

#define MAX_OPT_LEN 20

/* for internal timing */
#define TIMING_BINS 10
enum{ SOLVE_START_T,
      TRI_SOLVE_T,  /* triangular solves */
      SETUP_T,      /* total setup */
      SUB_GRAPH_T,  /* setup SubdomainGraph_dh */
      FACTOR_T,     /* factorization */
      SOLVE_SETUP_T, /* setup for solves */
      COMPUTE_RHO_T,
      /* note: SETUP_T - (FACTOR_T + SUB_GRAPH_T) should be small! */
      TOTAL_SOLVE_TEMP_T,
      TOTAL_SOLVE_T
    };

/* for statistical reporting */
#define STATS_BINS 10
enum{ NZA_STATS,       /* cumulative nonzeros for all systems solved */
      NZF_STATS,       /* cumulative nonzeros for all systems solved */
      NZA_USED_STATS,  /* cumulative nonzeros NOT dropped by sparseA */ 
      NZA_RATIO_STATS  /* NZA_USED_STATS/NZA_STATS, over all processors */
    };


/* primary data structure: this is monstrously long; but it works. 
   Users must ensure the following fields are initialized prior
   to calling Euclid_dhSetup(): m, n, beg_row, A
*/
struct _mpi_interface_dh {
  bool isSetup;

  HYPRE_Real rho_init;  
  HYPRE_Real rho_final;  
    /* Memory allocation for factor; will initially allocate space for 
       rho_init*nzA nonzeros; rho_final is computed after factorization,
       and is the minimum that rho_init whoulc have been to avoid
       memory reallocation; rho_final is a maximum across all processors.
    */

  HYPRE_Int m;         /* local rows in matrix */
  HYPRE_Int n;         /* global rows in matrix */
  HYPRE_Real *rhs;   /* used for debugging; this vector is not owned! */
  void *A;       /*  PETSc, HYPRE, Euclid, or other matrix object. */
  Factor_dh F;   /* data structure for the factor, F = L+U-I */
  SubdomainGraph_dh sg; 

  REAL_DH *scale;      /* row scaling vector */
  bool    isScaled;    /* set at runtime, turns scaling on or off */

  /* workspace for factorization and triangular solves */
  HYPRE_Real *work;
  HYPRE_Real *work2;
  HYPRE_Int from, to;  /* which local rows to factor or solve */

  /* runtime parameters (mostly) */
  char algo_par[MAX_OPT_LEN]; /* parallelization strategy */
  char algo_ilu[MAX_OPT_LEN]; /* ILU factorization method */
  HYPRE_Int level;      /* for ILU(k) */
  HYPRE_Real droptol;     /* for ILUT */
  HYPRE_Real sparseTolA;  /* for sparsifying A */
  HYPRE_Real sparseTolF;  /* for sparsifying the factors */
  HYPRE_Real pivotMin;    /* if pivots are <= to this value, fix 'em */
  HYPRE_Real pivotFix;    /* multiplier for adjusting small pivots */
  HYPRE_Real maxVal;      /* largest abs. value in matrix */

  /* data structures for parallel ilu (pilu) */
  SortedList_dh   slist;
  ExternalRows_dh extRows;

  /* for use with Euclid's internal krylov solvers; */
  char    krylovMethod[MAX_OPT_LEN];
  HYPRE_Int     maxIts;
  HYPRE_Real  rtol;
  HYPRE_Real  atol;
  HYPRE_Int     its; /* number of times preconditioner was applied since last call to Setup */
  HYPRE_Int     itsTotal; /* cululative number of times preconditioner was applied */

  /* internal statistics */
  HYPRE_Int setupCount;
  HYPRE_Int logging;    /* added in support of Hypre */
  HYPRE_Real timing[TIMING_BINS];
  HYPRE_Real stats[STATS_BINS];
  bool timingsWereReduced;
  bool   printStats; /* if true, on 2nd and subsequent calls to Setup,
                        calls Euclid_dhPrintStatsShorter().  Intent is to
                        print out stats for each setup phase when 
                        using Euclid, e.g, for nonlinear solves.
                     */
}; 

#endif /*  #ifndef EUCLID_MPI_INTERFACE_DH */
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

#ifndef THREADED_KRYLOV_H
#define THREADED_KRYLOV_H

/* #include "blas_dh.h" */

extern void bicgstab_euclid(Mat_dh A, Euclid_dh ctx, HYPRE_Real *x, HYPRE_Real *b, 
                                                              HYPRE_Int *itsOUT);

extern void cg_euclid(Mat_dh A, Euclid_dh ctx, HYPRE_Real *x, HYPRE_Real *b, 
                                                              HYPRE_Int *itsOUT);

#endif
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

/*
   Note: this module contains functionality for reading/writing 
         Euclid's binary io format, and opening and closing files.
         Additional io can be found in in mat_dh_private, which contains
         private functions for reading/writing various matrix and
         vector formats; functions in that module are called by
         public class methods of the Mat_dh and Vec_dh classes.
*/

#ifndef IO_DH
#define IO_DH

/* #include "euclid_common.h" */

/*--------------------------------------------------------------------------
 * open and close files, with error checking
 *--------------------------------------------------------------------------*/
extern FILE * openFile_dh(const char *filenameIN, const char *modeIN);
extern void closeFile_dh(FILE *fpIN);

/*---------------------------------------------------------------------------
 * binary io; these are called by functions in mat_dh_private
 *---------------------------------------------------------------------------*/

bool isSmallEndian();

/* seq only ?? */
extern void io_dh_print_ebin_mat_private(HYPRE_Int m, HYPRE_Int beg_row,
                                HYPRE_Int *rp, HYPRE_Int *cval, HYPRE_Real *aval, 
                           HYPRE_Int *n2o, HYPRE_Int *o2n, Hash_i_dh hash, char *filename);

/* seq only ?? */
extern void io_dh_read_ebin_mat_private(HYPRE_Int *m, HYPRE_Int **rp, HYPRE_Int **cval,
                                     HYPRE_Real **aval, char *filename);

/* seq only */
extern void io_dh_print_ebin_vec_private(HYPRE_Int n, HYPRE_Int beg_row, HYPRE_Real *vals,
                           HYPRE_Int *n2o, HYPRE_Int *o2n, Hash_i_dh hash, char *filename);
/* seq only */
extern void io_dh_read_ebin_vec_private(HYPRE_Int *n, HYPRE_Real **vals, char *filename);


#endif

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

#ifndef THREADED_BLAS_DH
#define THREADED_BLAS_DH

/* notes: 1. all calls are threaded with OpenMP.
          2. for mpi MatVec, see "Mat_dhMatvec()" in Mat_dh.h
          3. MPI calls use hypre_MPI_COMM_WORLD for the communicator,
             where applicable.
*/

/* #include "euclid_common.h" */

#ifdef SEQUENTIAL_MODE
#define MatVec       matvec_euclid_seq
#endif

extern void matvec_euclid_seq(HYPRE_Int n, HYPRE_Int *rp, HYPRE_Int *cval, HYPRE_Real *aval, HYPRE_Real *x, HYPRE_Real *y);
extern HYPRE_Real InnerProd(HYPRE_Int local_n, HYPRE_Real *x, HYPRE_Real *y);
extern HYPRE_Real Norm2(HYPRE_Int local_n, HYPRE_Real *x);
extern void Axpy(HYPRE_Int n, HYPRE_Real alpha, HYPRE_Real *x, HYPRE_Real *y);
extern HYPRE_Real Norm2(HYPRE_Int n, HYPRE_Real *x);
extern void CopyVec(HYPRE_Int n, HYPRE_Real *xIN, HYPRE_Real *yOUT);
extern void ScaleVec(HYPRE_Int n, HYPRE_Real alpha, HYPRE_Real *x);

#endif

#ifdef __cplusplus
}
#endif

#endif

