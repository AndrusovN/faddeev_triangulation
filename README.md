# Faddeev curve Newton triangle triangulation algorithm
In chapter 5 of my article (here will be the link to arxiv soon) there is a method of triangulation described. 
This is a small app for visualization of this triangulation method. It helped to determine the difference between this method and the one of A-Hilbert scheme studied in the work of 
[Alastair Craw](https://researchportal.bath.ac.uk/en/publications/an-explicit-construction-of-the-mckay-correspondence-for-ema-emhi/).

## Usage
Firstly clone or download the files to your computer.

In the file `triangulate.cpp` in function `int main()` specify the coordinates of the vertices of the triangle you want to triangulate. Note that it should be a prime triangle. Example:
```
Triangle special = Triangle(Vector2(0, 0), Vector2(-5, 11), Vector2(-1, 0));
```
Here the coordinates are `(0,0), (-5, 11)` and `(-1, 0)`

If you are using Windows or MacOS, you should check out how to compile C++ code on your machine and compile the `triangulate.cpp` file then run the executable file.
If you are using linux, just use command `make run` in console from the root directory of the project. After running the program will write its output to `triangulation.mp`.

If you have MetaPost installed on your computer (usually goes with standard LaTeX package), you can produce the image with it: `mpost triangulation.mp` or you can use websites like [this one](http://www.tlhiv.org/mppreview/) 
and paste the content from `triangulation.mp` there to get the image. If you are using Linux you can use command `make render` instead of `make run` to immediately get the picture.

When using `make run` or `make render` you can specify `output=<FORMAT>` to get the output in the specified format. Example:
```
make render output=svg
```

## Repository structure
- `.gitignore` and `CITATION.cff` are auxiliary files;
- `auxiliary,h` contains basic functions;
- `geometry.h` provides vector operations;
- `faddeev_triangle.h` contains utils for work with triangles. Also it contains legacy code which checked if the main theorem from chapter 5 holds true on examples;
- `triangulate.cpp` is the main file;
- `makefile` provides shortcuts for Linux users;
- `triangulation11.mp` is an example of the output with MetaPost code;
- `README.md` is this file with documentation

## Contribution
Basically this repository does not need contribution, but if you notice any bug or want to improve anything feel free to create a pull-request or write directly to me in Telegram: [@n_andrusov](https://t.me/n_andrusov)
