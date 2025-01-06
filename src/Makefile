all: simple_mult blis para_blis pthread_blis opengl 

clean: 
	rm -f mat_mult blis para_blis atlas pool_blis pthread_blis opengl

M_SIZE ?= 2000
K_SIZE ?= 2000
N_SIZE ?= 2000
MAT_SIZE ?= 2000
TILE_SIZE ?= 128

BLIS_PREFIX = /usr/local
BLIS_INC    = $(BLIS_PREFIX)/include/blis
BLIS_LIB    = $(BLIS_PREFIX)/lib/libblis.a

CFLAGS = -D MAT_SIZE=$(MAT_SIZE) -D TILE_SIZE=$(TILE_SIZE)

OTHERFLAGS = -D M_SIZE=$(M_SIZE) -D K_SIZE=$(K_SIZE) -D N_SIZE=$(N_SIZE)
PARAFLAGS = -D M_SIZE=2*$(M_SIZE) -D K_SIZE=2*$(K_SIZE) -D N_SIZE=2*$(N_SIZE)

simple_mult:
	gcc $(CFLAGS) -o mat_mult simple_mm.c

blis:
	gcc $(OTHERFLAGS) -o blis blis_mm.c -lblis -lm -O3

para_blis:
	gcc $(PARAFLAGS) -lblis -o para_blis para_blis_mm.c $(BLIS_LIB) -L/usr/local/lib -fopenmp -lm -O3

pool_blis:
	gcc $(OTHERFLAGS) -pthread -o pool_blis para_pool_mm.c -lblis -fopenmp -lm -O3

pthread_blis:
	gcc $(OTHERFLAGS) -pthread -o pthread_blis para_threads_mm.c -lblis -fopenmp -lm -O3

#atlas:
#	gcc $(OTHERFLAGS) -o atlas atlas_mm.c -I "/home/rachel/ATLAS/include" -L "/home/rachel/atlas-build/lib" -lcblas -latlas -lm

opengl:
	g++ opengl.cpp -o opengl -lGL -lGLEW -lglfw


