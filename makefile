# All Target
OBJ=ozomatli
.PHONY: release release-openmp install install-openmp clean

all: release

release:
	(cd Release; make)
	
release-openmp
	(cd openmp; make)

install:
	cp Release/${OBJ} /usr/bin/
	
install-openmp:
	cp openmp/${OBJ} /usr/bin/

clean:
	(cd Release; make clean)
	(cd openmp; make clean)
