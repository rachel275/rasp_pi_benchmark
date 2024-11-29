all: simple_mult loop_interchange_mm loop_tiling_mm open_mp neon_mm blis

clean: 
	rm -f mat_mult mat_mult_inter mat_mult_loop mat_mult_tiles mat_mult_tile_edge openmp neon blis

MAT_SIZE ?= 1024
TILE_SIZE ?= 128

CFLAGS = -D MAT_SIZE=$(MAT_SIZE) -D TILE_SIZE=$(TILE_SIZE)

simple_mult:
	gcc $(CFLAGS) -o mat_mult simple_mm.c

loop_interchange_mm:
	gcc $(CFLAGS) -o mat_mult_inter loop_interchange_mm.c

loop_tiling_mm:
	gcc $(CFLAGS) -o mat_mult_tiles loop_tiling_mm.c

loop_tiling_edge_mm:
	gcc $(CFLAGS) -o mat_mult_tile_edge loop_tiling_edge_mm.c

open_mp:
	gcc $(CFLAGS) -o openmp -fopenmp openmp.c

neon_mm:
	gcc $(CFLAGS) -o neon neon.c

blis:
	gcc $(CFLAGS) -o blis blis_mm.c -lblis -lm -O3
