#!/bin/bash

# Clean and prepare for build
make clean

# Define matrix and tile sizes
MAT_SIZES=(64 128 256 512 1024 2048 4096)
TILE_SIZES=(4 8 16 32 64 128 256 512 1024 2048)

# Output CSV file
OUTPUT_FILE="perf_results.csv"

# Check if CSV file already exists; if not, create it with headers
if [ ! -f "$OUTPUT_FILE" ]; then
    echo "Matrix Size,Tile Size,Command,Instructions,Cache Misses,Elapsed Time" > "$OUTPUT_FILE"
fi

# List of binaries to profile
BINARIES=("mat_mult" "mat_mult_inter" "mat_mult_tiles" "mat_mult_tile_edge" "openmp" "neon" "blis")

# Loop through matrix and tile sizes
for MAT_SIZE in "${MAT_SIZES[@]}"; do
    for TILE_SIZE in "${TILE_SIZES[@]}"; do
        # Ensure tile size is less than or equal to matrix size
        if [ "$TILE_SIZE" -le "$MAT_SIZE" ]; then
            # Clean and rebuild with current matrix and tile size
            make clean
            make all MAT_SIZE="$MAT_SIZE" TILE_SIZE="$TILE_SIZE"

            # Loop through binaries for profiling
            for BINARY in "${BINARIES[@]}"; do
                echo "Profiling $BINARY for Matrix Size $MAT_SIZE and Tile Size $TILE_SIZE..."

                # Run perf command and save output to a temporary file
                perf stat -B \
                    -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses \
                    -o perf_output.txt ./"$BINARY"

                # Extract performance metrics
                INSTRUCTIONS=$(grep instructions perf_output.txt | awk '{gsub(/,/, ""); print $1}')
                CACHE_MISSES=$(grep cache-misses perf_output.txt | awk '{gsub(/,/, ""); print $1}')
                ELAPSED_TIME=$(grep "seconds time elapsed" perf_output.txt | awk '{print $1}')

                # Log results to the CSV file
                echo "$MAT_SIZE,$TILE_SIZE,$BINARY,$INSTRUCTIONS,$CACHE_MISSES,$ELAPSED_TIME" >> "$OUTPUT_FILE"

                # Remove temporary perf output file
                rm perf_output.txt
            done
        fi
    done
done

# Final cleanup
make clean

echo "Profiling completed. Results saved to $OUTPUT_FILE."

