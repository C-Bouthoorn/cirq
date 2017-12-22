#include "benchmark.h"

#include "defines.h"

#include <stdlib.h>


void BENCH_ADD(const char func[], double effective_time, double actual_time) {
	char *bench_file_name = "/tmp/bench";
	char *temp_file_name = "/tmp/bench.tmp";

	FILE *in = fopen(bench_file_name, "r");
	FILE *out = fopen(temp_file_name, "a");


	bool new = true;

	#define FMT_STR "%lf = %lf / %i \t%lf \t%s\n"

	double frac = 0.0;
	double eff = 0.0;
	int i = 0;
	double act = 0.0;

	char *f = malloc(BUF_SIZE);

	while (true) {
		// Get old data
		int z = fscanf(in, FMT_STR, &frac, &eff, &i, &act, f);

		// EOF? break
		if (z == EOF) break;

		// No data? continue
		if (z == 0) continue;

		// Check if current line contains data about current function
		if (strcmp(f, func) == 0) {
			// Calculate new fraction
			frac = (eff + effective_time) / (i + 1);

			// Print new data
			fprintf(out, FMT_STR, frac, eff + effective_time, i + 1, act + actual_time, func);

			// Don't print a clean line
			new = false;
		}

		else {
			// Copy old line
			fprintf(out, FMT_STR, frac, eff, i, act, f);
		}
	}

	free(f);


	if (new) {
		// Print a clean line (no old data exists)
		// NOTE: frac = effective_time, since you'd devide it by 1
		fprintf(out, FMT_STR, effective_time, effective_time, 1, actual_time, func);
	}


	// Close files
	fclose(in);
	fclose(out);

	// Replace temp file to regular file
	rename(temp_file_name, bench_file_name);
}
