/* File:
 *  mv_mult_mpi.c
 *
 * Purpose:
 *  Implement parallel matrix-vector multiplication using
 *  one-dimensional arrays to store the vectors and the
 *  matrix. Vectors use block distributions and the
 *  matrix is distributed by block rows.
 *
 * Input:
 *  Rowwise-distributed matrix A and vectors y, x, d,
 *
 * Algorithm:
 *  For k = 0 to t-1
 *    y = d + Ax
 *    x = y
 *  Endfor
 *
 * Output:
 *  Process 0 has the final copy of vector x.
 *
 * Notes:
 *    1. Number of processes should evenly divide both m and n.
 *    2. Define DEBUG for verbose output.
 *
 * IPP:      Section 3.4.9 (pp. 113 and ff.)
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define UPPER_TRIANGULAR 1

/*-------------------------------------------------------------------
 * Function:
 *  itmv_mult
 *
 * Purpose:
 *  Run t iterations of parallel computation:  {y=d+Ax; x=y}.
 *  The matrix is distributed by block rows and the column vectors
 *  d, x, y are distributed by blocks
 *
 * In args:
 *  local_A: calling process' rows of matrix A
 *  local_d: calling process' components of vector d
 *  global_x: Store the final vector x in process 0 when returning
 *  matrix_type: matrix_type=0 means A is a regular matrix.
 *               matrix_type=1 means A is an upper triangular matrix
 *  n: the global number of columns (same as the number of rows)
 *  t: the number of iterations
 *  blocksize: size of local array for x, which should be n/no_proc
 *  my_rank: local process ID
 *  no_proc: num of processes
 *  comm: communicator containing all calling processes
 *
 * In/out args:
 *  local_x: calling process' components of vector x
 *  local_y: calling process' components of vector y
 *
 * Return:
 *  1  means succesful 0  means unsuccessful
 *
 * Errors:
 *  If an error is detected (e.g. n is non-positive, n is not evenly
 *  divisible by the number of processes, malloc fails), the program returns 0.
 *
 * Global:
 *  This function should NOT access any global variable.
 */
int itmv_mult(double local_A[] /* in */, double local_x[] /* in */,
              double local_d[] /* in */, double local_y[] /* out */,
              double global_x[] /* out */, int matrix_type /* in */,
              int n /* in */, int t /* in */, int blocksize /* in */,
              int my_rank /* in */, int no_proc /* in */,
              MPI_Comm comm /* in */) {
  double *x = malloc(n*sizeof(double));
  int local_i, j, start, k;
  int succ, all_succ;

  if (n <= 0 || blocksize <= 0 || local_A == NULL || local_x == NULL ||
      local_y == NULL || no_proc <= 0 || my_rank < 0 || my_rank >= no_proc)
    return 0;
  if (n % no_proc != 0) /* n is not divisible by no_proc */
    return 0;
  if (n / no_proc != blocksize) /* wrong local array size */
    return 0;

    MPI_Allreduce(&succ, &all_succ, 1,MPI_INT, MPI_PROD, comm);
    MPI_Allgather(local_x, blocksize, MPI_DOUBLE, x, blocksize, MPI_DOUBLE, comm);

    for(int k = 0; k <t; k++){
    for(local_i = 0; local_i < blocksize; local_i ++){
      local_y[local_i] = local_d[local_i];
      if(matrix_type == UPPER_TRIANGULAR){
	start = local_i+1;
      }
      else{
	      start = 0;
      }
      for(int j = start; j <n ; j++){
        local_y[local_i] += local_A[n*local_i+ j] * x[j];
      }
 }
    MPI_Allgather(local_y, blocksize, MPI_DOUBLE, x, blocksize, MPI_DOUBLE,comm);
    }
			    
    if(my_rank == 0){
    for(int i = 0; i < n; i++){

	    global_x[i] = x[i];
    }
    }

    free(x);

		   
		    
  return 1;
}

/*-------------------------------------------------------------------
 * Function:
 *  itmv_mult_seq
 *
 * Purpose:
 *  Run t iterations of  computation:  {y=d+Ax; x=y} sequentially.
 *
 * In args:
 *  sh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQDYzEJv9YArEkSiIsulKdcZOReqxRRh7qhLSmiq7qVbwnGu/sFZ2J0DFTL+J73fc/V4TIEWJay5T93VOFnn4jKpYEYxBec2PWyo4vO0LKVDno1D20I2xw1C+KVPplFDEn2nkhvGVtP3Cze/qkirR9yXhZwLLIr/tQwMaAfbnOTSfnyuNE/Rbr13Vxx4m52pMOZmU+MmSE4yUDLQV2UMaUpQCsae1YMjN6xTeCJLVS7vlDsY21ffvViyGisbXjNqUTJ0nMrygA3hDsP/YY/azO/VViwt2+ofQhyiQ2fERBGza3iQEpEemU+E80TBOT2OSyRnGv+as+e9IntTYxI9yBC/2G5z3gRiKix20vQcerbaO5Pt1N+IKhqmj2FkOY56ME8Mp06rlmMXGk8TiDL3JrKU3vqjr7kPfU/VRP0C6VzdyrciHaaAp/SNxgcrhJN2Ky+/5yYKzrD7/fy/V/zrDzcojlip/CJR7eqULfRFRINkcoHofMf06+JrQjV5YPb8UrMxBgZpenLSf5BfWSKpbgHwKz/z4qFYYUkA+4baaojNRLO2s+2Iv0Ev9nJ5TeCCtzr17/SZHLKO6PlMssxaC6JmJuqWMIYcxaHzKkbgLRQJM25PaAlrJEFuIcU2td7QPb9BaCrZpjZOydlxdUoTbVzKMOfGFOaOxIbiGLJjFguzvw== travis_koski@ucsb.edu  : matrix A
 *  d: column vector d
 *  matrix_type: matrix_type=0 means A is a regular matrix.
 *               matrix_type=1 (UPPER_TRIANGULAR) means A is
 *               an upper triangular matrix
 *  n: the global  number of columns (same as the number of rows)
 *  t: the number of iterations
 *
 * In/out:
 *  x: column vector x
 *  y: column vector y
 *
 * Return:
 *  1  means succesful 0  means unsuccessful
 *
 * Errors:
 *  If an error is detected (e.g. n is non-positive,
 *  matrix/vector pointers are NULL.
 */
int itmv_mult_seq(double A[], double x[], double d[], double y[],
                  int matrix_type, int n, int t) {
  int i, j, start, k;

  if (n <= 0 || A == NULL || x == NULL || d == NULL || y == NULL) return 0;

  for (k = 0; k < t; k++) {
    for (i = 0; i < n; i++) {
      y[i] = d[i];
      if (matrix_type == UPPER_TRIANGULAR) {
        start = i;
      } else {
        start = 0;
      }
      for (j = start; j < n; j++) {
        y[i] += A[i * n + j] * x[j];
      }
    }
    for (i = 0; i < n; i++) {
      x[i] = y[i];
    }
  }
  return 1;
}
