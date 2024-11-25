make clean
make all
perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache ./mat_mult
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-loads-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetches ./mat_mult_inter
perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache ./mat_mult_inter
perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache ./mat_mult_tiles
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-loads-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetches ./mat_mult_tiles
perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,l2d_cache,l2d_cache_refill,l3d_cache ./neon
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-loads-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetches ./mat_mult_tile_edge
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-loads-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetches ./strassen
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-loads-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetches ./openmp
#perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-loads-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetches ./neon
