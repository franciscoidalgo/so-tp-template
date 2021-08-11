all:
	make -C modulo1
	make -C modulo2

clean:
	make clean -C modulo1
	make clean -C modulo2