all:
	g++ triangulate.cpp -o triangulate

run: all
	./triangulate

