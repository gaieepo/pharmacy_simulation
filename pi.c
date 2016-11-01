/* C Example */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int monteCarlo(int n, int seed);

int main(int argc,char** argv)
{
    int i, total, myResult, seed, myId, dest, tag;
    int np, nTrial;
    double pi;
    MPI_Status status;

    if (argc < 2 ){
        printf("%s <Number of trials>\n", argv[0]);
        exit(1);
    }

    dest = 0;        //the root processor 
    tag = 123;       //abitrary message id

    MPI_Init(&argc,&argv);              /* starts MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);  /* get current process id */
    MPI_Comm_size(MPI_COMM_WORLD, &np);     /* get number of processes */

    nTrial = atoi(argv[1]);
    seed = time(NULL) / (myId*7+3); //simple way to have different random seed

    myResult = monteCarlo( nTrial, seed );

    printf("Process %d counted %d out of %d\n", myId, myResult, nTrial);

    if(myId == 0) {
        total = myResult;
        for (i=1; i<np; i++) {
            //TODO: Write the correct receive operation
            MPI_Recv(&myResult, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
            total += myResult;
        }
        pi = 4.0 * total / (np * nTrial);
    
        printf("The total =%.12lf\n", pi);
    } else {
        //TODO: Write the correct send operation
        MPI_Send(&myResult, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);      
    }
    MPI_Finalize();                  

    return 0;
}

int monteCarlo( int n, int seed )
{
    int result = 0, i;
    double x, y;

    srand48( seed );
    for (i = 0; i < n; i++){
        x = drand48();        
        y = drand48();        
        if (x*x + y*y <= 1.0 ){
            result++;
        }
    }

    return result;
}
