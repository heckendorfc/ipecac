#include <stdio.h>
#include <stdlib.h>
#include <cobra.h>
#include "bench_ipecac.h"

int main(){
	BenchSet bs;

	benchset_init(&bs);

	arithmetic_suite(&bs);

	bench_run(&bs);

	bench_free(&bs);

	return EXIT_SUCCESS;
}

