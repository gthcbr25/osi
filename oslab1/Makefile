run:
	mkdir build
	g++ -o build/parent parent.cpp
	g++ -o build/child1 child.cpp
	g++ -o build/child2 child.cpp

	./build/parent

clean:
	rm -r ./build

.PHONY: run clean