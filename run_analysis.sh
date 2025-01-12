#!/bin/bash

cd src

# Clean and prepare for build
make clean

# Define matrix and tile sizes

#M_SIZE=(2100 2200 2300 2400 2500 2600 2700 2800 2900 3000 3100 3200 3300 3400 3500 3600 3700 3800 3900 4000)
#K_SIZE=(16 32 64 100 128 150 170 200 220 256 275 300 350 400 450 500 550 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000 2100 2200 2300 2400 2500 2600 2700 2800 2900 3000 3100 3200 3300 3400 3500 3600 3700 3800 3900 4000)
#N_SIZE=(100 200 300 400 500 600 700 800 900 1000)
# Output CSV file
OUTPUT_FILE="openblas_results.csv"

# Check if CSV file already exists; if not, create it with headers
if [ ! -f "$OUTPUT_FILE" ]; then
    echo "M,K,N,Command,Elapsed Time" > "$OUTPUT_FILE"
fi

# List of binaries to profile
BINARIES=("openblas") #"blis" "para_blis" "atlas" "pthread_blis") #"pthread_neon_strassen")

M=100

# Loop through matrix and tile sizes
while [ $M -le 10000 ]; do
            # Clean and rebuild with current matrix and tile size
	make clean
        make all M_SIZE="$M" K_SIZE="$M" N_SIZE="$M"

        # Loop through binaries for profiling
        for BINARY in "${BINARIES[@]}"; do
        	echo "Profiling $BINARY for M = $M_SIZE K = $K_SIZE N= $N_SIZE..."

                # Run perf command and save output to a temporary file
		./"$BINARY" > perf_output.txt

                # Extract performance metrics
         	#INSTRUCTIONS=$(grep instructions perf_output.txt | awk '{gsub(/,/, ""); print $1}')
                #L1_CACHE_MISSES=$(grep cache-misses perf_output.txt | awk '{gsub(/,/, ""); print $1}')
                # Extract L1-dcache-load-misses
		#L1_misses=$(grep "L1-dcache-load-misses" perf_output.txt | awk '{gsub(/,/, ""); print $1}')

		# Extract L2 cache misses (l2d_cache_refill)
		#L2_misses=$(grep "l2d_cache_refill" perf_output.txt | awk '{gsub(/,/, ""); print $1}')

		# Extract L3 cache misses (if available)
		#L3_misses=$(grep "l3d_cache_refill" perf_output.txt | awk '{gsub(/,/, ""); print $1}')

		# Check if any value was "not counted"
		#L3_misses=${L3_misses:-"N/A (not counted)"}
		#ELAPSED_TIME=$(grep "Elapsed Time (us): " perf_output.txt | awk '{print $1}')

                #ELAPSED_TIME=$(grep "Elapsed Time (us): " perf_output.txt | awk -F": " '{print $2}')
		ELAPSED_TIME=$(grep "Elapsed time (us): " perf_output.txt | awk -F": " '{sum += $2} END {print sum}')

        	echo "$M,$M,$M,$BINARY,$ELAPSED_TIME" >> "$OUTPUT_FILE"
                # Remove temporary perf output filei
        	rm perf_output.txt
	done
	((M += 100))
done

# Final cleanup
make clean
mv $OUTPUT_FILE ../results/
echo "Profiling completed. Results saved to $OUTPUT_FILE."

