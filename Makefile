install:
	cd ./build && cmake ..
	cd ./build && make  && make install
test:

clean:
	rm -rf *.o
build:clean
	g++  -Wall -c *.cpp 

