install:
	cd ./build && cmake ..
	cd ./build && make -j8
	cd ./build && sudo make install

test:

clean:
	rm -rf *.o
build:clean
	g++  -Wall -c *.cpp 

