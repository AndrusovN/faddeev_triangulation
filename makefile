all:
	g++ triangulate.cpp -o triangulate

run: all
	./triangulate

render: run
	mpost triangulation11.mp

