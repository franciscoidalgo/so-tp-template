all:
	make -C modulo1
	make -C modulo2
	make -C moduloConsola

release:
	make release -C modulo1
	make release -C modulo2
	make release -C moduloConsola

clean:
	make clean -C modulo1
	make clean -C modulo2
	make clean -C moduloConsola