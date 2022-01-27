#!/bin/bash  
# Next line shows the job name you can find when querying the job status
#SBATCH --job-name="piblock_mpi"
# Next line is the output file name of the execution log
#SBATCH --output="job_piblock1.%j.out"
# Next line shows where to ask for machine nodes
#SBATCH --partition=compute

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#Total number of MPI processes= 1*4=4
#SBATCH --export=ALL
# Next line limits the job execution time at most 3 minute.
#SBATCH -t 00:03:00

#SBATCH --account=csb175
#This job runs with 1 node, 4 core per node for a total of 4 cores using at most 1 minute CPU time.
#csb175 is the class-wide account shared among all cs140 students

#ibrun in verbose mode will give binding detail. It uses one MPI process per core.  
#it is the same as "srun --mpi=pmi2 -n 4 ../hello1_mpi"

module purge
module load slurm
module load cpu
module load gcc
module load openmpi
module load sdsc
ibrun -v ./piblock_test_mpi  
