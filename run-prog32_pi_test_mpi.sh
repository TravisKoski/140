#!/bin/bash  
# Next line shows the job name you can find when querying the job status
#SBATCH --job-name="prog32pi_mpi"
# Next line is the output file name of the execution log
#SBATCH --output="job_prog32_pi4.%j.out"
# Next line shows where to ask for machine nodes
#SBATCH --partition=compute
#SBATCH --account=csb175

#Total number of MPI processes= 1*4=4
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --export=ALL
# Next line limits the job execution time at most 3 minute.
#SBATCH -t 00:03:00


#ibrun in verbose mode will give binding detail. It uses one MPI process per core.
#it is the same as "srun --mpi=pmi2 -n 4 ../hello1_mpi"

module purge
module load slurm
module load cpu
module load gcc
module load openmpi
module load sdsc

./pi 1000000000
ibrun -v ./prog32_pi_test_mpi  
./pi 100000000
