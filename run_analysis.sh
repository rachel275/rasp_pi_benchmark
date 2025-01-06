#!/bin/bash

cd src

# Clean and prepare for build
make clean

# Define matrix and tile sizes

M_SIZE=(16 32 64 100 128 150 170 200 220 256 275 300 350 400 450 500 550 600)
K_SIZE=(16 32 64 100 128 150 170 200 220 256 275 300 350 400 450 500 550 600)
N_SIZE=(16 32 64 100 128 150 170 200 220 256 275 300 350 400 450 500 550 600)
# Output CSV file
OUTPUT_FILE="perf_para_results.csv"

# Check if CSV file already exists; if not, create it with headers
if [ ! -f "$OUTPUT_FILE" ]; then
    echo "M,K,N,Command,Instructions,L1 Cache Misses,L2 Cache Misses,L3 Cache Misses,Elapsed Time" > "$OUTPUT_FILE"
fi

# List of binaries to profile
BINARIES=("blis" "pool_blis" "pthread_blis" "para_blis") #"pthread_neon_strassen")

# Loop through matrix and tile sizes
for M in "${M_SIZE[@]}"; do
    for K in "${K_SIZE[@]}"; do
	for N in "${N_SIZE[@]}"; do
            # Clean and rebuild with current matrix and tile size
            make clean
            make all M_SIZE="$M" K_SIZE="$K" N_SIZE="$N"

            # Loop through binaries for profiling
            for BINARY in "${BINARIES[@]}"; do
                echo "Profiling $BINARY for M = $M_SIZE K = $K_SIZE N= $N_SIZE..."

                # Run perf command and save output to a temporary file
                perf stat -B \
                    -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,l2d_cache_refill,l3d_cache_refill \
                    -o perf_output.txt ./"$BINARY"

                # Extract performance metrics
                INSTRUCTIONS=$(grep instructions perf_output.txt | awk '{gsub(/,/, ""); print $1}')
                #L1_CACHE_MISSES=$(grep cache-misses perf_output.txt | awk '{gsub(/,/, ""); print $1}')
                # Extract L1-dcache-load-misses
		L1_misses=$(grep "L1-dcache-load-misses" perf_output.txt | awk '{gsub(/,/, ""); print $1}')

		# Extract L2 cache misses (l2d_cache_refill)
		L2_misses=$(grep "l2d_cache_refill" perf_output.txt | awk '{gsub(/,/, ""); print $1}')

		# Extract L3 cache misses (if available)
		L3_misses=$(grep "l3d_cache_refill" perf_output.txt | awk '{gsub(/,/, ""); print $1}')

		# Check if any value was "not counted"
		#L3_misses=${L3_misses:-"N/A (not counted)"}
		ELAPSED_TIME=$(grep "seconds time elapsed" perf_output.txt | awk '{print $1}')

                # Log results to the CSV file
                echo "$M,$K,$N,$BINARY,$INSTRUCTIONS,$L1_misses,$L2_misses,$L3_misses,$ELAPSED_TIME" >> "$OUTPUT_FILE"

                # Remove temporary perf output file
                rm perf_output.txt
            done
    	done
    done
done

# Final cleanup
make clean
mv $OUTPUT_FILE ../results/
echo "Profiling completed. Results saved to $OUTPUT_FILE."

