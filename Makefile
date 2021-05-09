install:
	cd ./build && cmake ..
	cd ./build && make -j8
	cd ./build && sudo make install

test:

clean:
	rm -rf *.o
build:clean
	g++  -Wall -c *.cpp 

qrun:

publish_release: install
	git checkout release
	cp -r /usr/local/lib/libsmncpp.a ./lib
	cp -r /usr/local/include/smncpp ./include
	git add .
	git commit -A "publish_release"
	git push 
	git checkout -
