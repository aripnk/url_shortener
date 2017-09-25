CC=gcc
all :
	cd src; make clean; make; cp shortensvc ../;cd ..

clean :
	cd src; make clean
	rm -rf shortensvc
