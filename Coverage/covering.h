#pragma once
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "cycle.h"

typedef struct measurement
{
	ticks mpi_send_cost,operation_cost;
	int rank;
}  MEASUREMENT;

MEASUREMENT coverage(int rank, int size, uint64_t NB_BYTES, uint64_t NB_OPERATION);