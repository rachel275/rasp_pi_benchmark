cd src
make clean
make all M_SIZE=5000 K_SIZE=5000 N_SIZE=5000
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./mat_mult
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./mat_mult_inter
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./mat_mult_tiles
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache,l3d_cache_refill ./neon
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache,l3d_cache_refill ./blis
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./strassen
printf "BLIS: "
./blis
printf "\n"
printf "ATLAS: "
./atlas

printf "\n"
printf "OPENBLAS: "
./openblas
printf "\n"
printf "PARA_BLIS: "
./para_blis

printf "\n"
printf "PTHREAD_BLIS: "
./pthread_blis
#perf stat -B -e cache-references,cache-misses,cycles,instructions ./atlas
#perf stat -B -e cache-references,cache-misses,cycles,instructions ./para_blis
#perf stat -B -e cache-references,cache-misses,cycles,instructions ./pthread_blis
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./neon_strassen
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./mpi_strassen
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./pthread_neon_strassen
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache,l3d_cache_refill ./para_blis
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./atlas
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache,l3d_cache_refill ./pool_blis
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache,l3d_cache_refill ./para_blis
make clean
cd ..
