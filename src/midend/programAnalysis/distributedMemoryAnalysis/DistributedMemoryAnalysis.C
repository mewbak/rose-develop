// Non-template functions of the distributed memory traversal framework.
// Author: Gergo Barany
// $Id: DistributedMemoryAnalysis.C,v 1.1 2008/01/08 02:55:52 dquinlan Exp $

#include <sage3basic.h>
#include <mpi.h>
#include "DistributedMemoryAnalysis.h"

void initializeDistributedMemoryProcessing(int *argc, char ***argv)
{
    MPI_Init(argc, argv);
}
                                                                                                                                                                                                     
void finalizeDistributedMemoryProcessing()
{
    MPI_Finalize();
}
