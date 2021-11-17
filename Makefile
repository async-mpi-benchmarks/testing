CC = mpicc
CFLAGS = -Wall -Wextra -g
OFLAFS = -march=native -mtune=native -O1

build: sync async

sync: sync/p2p sync/ping_pong sync/ring

async: async/p2p async/ping_pong async/ring

hello_world: src/mpi_hello_world.c
	@mkdir -p target/
	$(CC) $(CFLAGS) $(OFLAFS) $< -o target/$@
	
sync/p2p: src/sync/mpi_p2p.c
	@mkdir -p target/sync/
	$(CC) $(CFLAGS) $(OFLAFS) $< -o target/$@

sync/ping_pong: src/sync/mpi_ping_pong.c
	@mkdir -p target/sync/
	$(CC) $(CFLAGS) $(OFLAFS) $< -o target/$@

sync/ring: src/sync/mpi_ring.c
	@mkdir -p target/sync/
	$(CC) $(CFLAGS) $(OFLAFS) $< -o target/$@

async/p2p: src/async/mpi_p2p.c
	@mkdir -p target/async/
	$(CC) $(CFLAGS) $(OFLAFS) $< -o target/$@

async/ping_pong: src/async/mpi_ping_pong.c
	@mkdir -p target/async/
	$(CC) $(CFLAGS) $(OFLAFS) $< -o target/$@

async/ring: src/async/mpi_ring.c
	@mkdir -p target/async/
	$(CC) $(CFLAGS) $(OFLAFS) $< -o target/$@

clean:
	rm -rf target/
