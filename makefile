all:
	g++ triangulate.cpp -o triangulate

run: all
	./triangulate $(output)

render: run 
	mpost triangulation.mp

