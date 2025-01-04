cd src
make clean
make all MAT_SIZE=340 TILE_SIZE=64
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./mat_mult
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./mat_mult_inter
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./mat_mult_tiles
perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache,l3d_cache_refill ./neon
perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache,l3d_cache_refill ./blis
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./strassen
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./neon_strassen
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./mpi_strassen
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./pthread_neon_strassen
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./para_blis
perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill ./atlas
make clean
cd ..
