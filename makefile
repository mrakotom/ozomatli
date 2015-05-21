# All Target
OBJ=ozomatli
.PHONY: all release openmp install install-openmp clean 

all: release 

release:
	(cd Release; make)
	
openmp:
	(cd openmp; make)

install:
	cp Release/${OBJ} /usr/bin/
	
install-openmp:
	cp openmp/${OBJ}-openmp /usr/bin/

clean:
	(cd Release; make clean)
	(cd openmp; make clean)
