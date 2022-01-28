/*
 * File:     treesum_mpi.c
 *
 * Purpose:  Use tree-structured communication to find the global sum
 *           of a random collection of ints.  This version doesn't
 *           require that comm_sz be a power of 2.
 *
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/*-------------------------------------------------------------------
 * Function:
 *  global_sum
 *
 * Purpose:
 *  Implement a global sum using tree-structured communication
 *
 * Notes:
 *  1.  The return value for global sum is only valid on process 0
 */
int nextPow2(int x){
int res = 1;
while(res < x){
  res *=2;
}
return res;
}
int global_sum(int my_int /* in */, int my_rank /* in */, int comm_sz /* in */,
               MPI_Comm comm /* in */) {
    int my_sum = my_int;
    int other_sum;
    int partner;
    int div = 2; //used for pairing up processes
    int finished = 0;
    int gap = 1;
    //pre checks
    int im_sending;
    while(!finished && div<=nextPow2(comm_sz)){
	    im_sending = my_rank % div;
	    if(im_sending){
	    partner = my_rank - gap;
	    MPI_Send(&my_sum,1,MPI_INT,partner,0,comm);
	    finished = 1;
	    }
	    else{
	    partner = my_rank + gap;
	    if(partner < comm_sz){
		    MPI_Recv(&other_sum,1,MPI_INT, partner, 0, comm,MPI_STATUS_IGNORE);
		    my_sum+=other_sum;
	    }
	    div*=2;
	    gap*=2;

	    }
    }
    return my_sum;
    }

  /* Your solution */ /* Global_sum */
