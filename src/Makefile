all: simple_mult loop_interchange_mm loop_tiling_mm open_mp neon_mm blis strassen mpi_strassen neon_strassen pthread_neon_strassen para_blis atlas opengl

clean: 
	rm -f mat_mult mat_mult_inter mat_mult_loop mat_mult_tiles mat_mult_tile_edge openmp neon blis strassen mpi_strassen neon_strassen pthread_neon_strassen para_blis atlas opengl

MAT_SIZE ?= 2048
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

strassen:
	gcc $(CFLAGS) -o strassen Strassen.c

mpi_strassen:
	mpicc $(CFLAGS) -o mpi_strassen mpi_strassen.c

neon_strassen:
	gcc $(CFLAGS) -o neon_strassen neon_strassen.c

pthread_neon_strassen:
	gcc $(CFLAGS) -lpthread -o pthread_neon_strassen pthread_neon_strassen.c

blis:
	gcc $(CFLAGS) -o blis blis_mm.c -lblis -lm -O3

para_blis:
	gcc $(CFLAGS) -o para_blis para_blis_mm.c -lblis -lm -O3

open_blas:

atlas:
	gcc $(CFLAGS) -o atlas atlas_mm.c -latlas -lcblas -llapack

opengl:
	g++ opengl.cpp -o opengl -lGL -lGLEW -lglfw
