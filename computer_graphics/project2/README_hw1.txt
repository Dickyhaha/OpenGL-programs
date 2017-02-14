------------------------------------------------
ECS 175 2016 Fall HW Project 1
Mang Tik Chiu -- 914561017
mtchiu@ucdavis.edu
------------------------------------------------
This is the README file for HW Project 1
------------------------------------------------
------------------------------------------------

COMPILATION

------------------------------------------------
This project contains multiple source and header files. 
All the files are categorized and placed under the respective directories. 
To make sure that the program compiles without error, please do not move the files to another location.

A Makefile is written for convenient compilation, simple type <make> in the terminal.
------------------------------------------------
------------------------------------------------

START UP

------------------------------------------------
To start the program, type

$ ./world [Width] [Height]

in the terminal. If no dimension is supplied, the default window size of [600x600] will be used.

After starting the program, 2 windows, tittled "ECS 175 Canvas" and "ECS 175 GUI" respectively, will show up. 
The Canvas window is the main drawing window while the GUI window is used for display and control.
The Terminal window will also be used as the main control interface.

At start up, 8 individual drawn lines will be displayed for easy validation.
Initially, the canvas is set to be non-refresh mode, which means the drawn pixels will not be cleared on each display call.
To change to refresh mode, type <erase>.
------------------------------------------------
------------------------------------------------

USAGE

------------------------------------------------
Before entering commands, the following line will show up:

$ (Count: 0, Selected:  -1) Enter Command: 

<Count> indicates the total number of polygons.
<Selected> indicates the current selected polygon index.
When no polygons are selected, -1 is displayed.

A list of available commands can be found in the section below.
-------------------------------------------------
-------------------------------------------------

LIST OF COMMANDS

-------------------------------------------------
Below are the available commands in Terminal.
Note that all commands are in small letters.

$ bresenham <x0> <y0> <x1> <y1>

Draw a straight line from (x0, y0) to (x1, y1) using Bresenham algorithm.
No specific order is required.
Only works in non-refresh mode since no actual line objects are created.

---

$ dda <x0> <y0> <x1> <y1>

Draw a straight line from (x0, y0) to (x1, y1) using Bresenham algorithm.
No specific order is required.
Only works in non-refresh mode since no actual line objects are created.

---

$ circle <x> <y> <radius>

Draw a circle using Bresenham's circle drawing algorithm.

---

$ set <index>

Select the index-th polygon.
Does nothing if index is invalid or there are no polygons.

---

$ polygon <num> <x1> <y1> ...

Set the currently selected polygon's vertices to (x1, y1), ...
If there are no polygons, a new polygon is created.

---

$ push

Push a new polygon to the stack.
There is theoretically no upper limit.

---

$ pop

Pop the last polygon from the stack.
Does nothing if there are no polygons.

---

$ translate <x> <y>

Translate the selected polygon by the coordinate (x, y).
Does nothing if there are no polygons.

---

$ rotate <angle>

Rotate the selected polygon by <angle> degree counter-clockwise.
Does nothing if there are no polygons.

---

$ scale <x> <y>

Scale the selected polygon by <x> along the x-axis and <y> along the y-axis.
Does nothing if there are no polygons.

---

$ rasterize

Rasterize the selected polygon.
Does nothing if there are no polygons.

---

$ unrasterize

Un-rasterize the selected polygon.
Does nothing if there are no polygons.

---

$ cut <x0> <y0> <x1> <y1>

Cut (Clip) the selected polygon with the bounding box (x0, y0) (x1, y1).
The coordinates must be relative to the entire canvas, i.e. absolute coodinates.
This action is destructive, the polygon cut will have its vertices modified.

---

$ info

Display the coordinates and total number of vertices of the selected polygon.
Does nothing if there are no polygons.

---

$ cls

Clear the screen for better tidiness.

---

$ swap <Index1> <Index2>

Swap the Index1-th polygon with the Index2-th polygon.
Does nothing if there are no such polygons.

---

$ spin <rate>

Spin the selected polygon for rate degree every 15ms (Glut refresh time).
A rate of 1 is recommended for detailed testing.
Does nothing if there are no polygons.

---

$ stop

Stop the selected polygon from spinning.
Does nothing if there are no polygons.

---

$ firework <x> <y>

Draw 8 line segments from (x, y) to the 8 octants with length > 1000.
Bresenham's line drawing algorithm is used.
Used for Clipping test and line drawing test.

---

$ clip <x0> <y0> <x1> <y1>

Apply Cohen-Sutherland algorihtm to the canvas with bounding box (x0, y0) (x1, y1).
Only lines that are drawn after applying clipping will be clipped.

---

$ unclip

Disable clipping for canvas.

---

$ save

Save the current scene of polygons into a text file.
No lines will be saved to the text file.
The format is as follows:

<Number of polygons>

<Polygon Index> <Rasterize State>
<x1> <y1>
<x2> <y2>
...

---

$ load

Load the previously saved polygon scene to the program.
All polygons previously pushed will be discarded after calling this command.
Canvas will be set to refresh mode after calling this command.

---

$ erase

Change canvas to refresh mode.
Polygons will get updated on every refresh call.
Lines will be erased immediately after drawing.
Not recommended when testing lines.

---

$ keep

Change canvas to non-refresh mode.
Lines will be kept and continue displaying.
Polygon updates will be kept and will not be erased.
Not recommended when testing polygons.

---

$ flush

Clear all polygons in the polygon list.
No polygons will be selected after calling this command.

---

$ clear

Erase the canvas once.

-------------------------------------------------
-------------------------------------------------

EXTRA FEATURE

------------------------------------------------

GUI

Simply for entertainment but mainly in the hope of a few extra credits, a simple GUI is designed to facilitate polygon testing.
A second window, named "ECS 175 GUI" is displayed, which contains to components.

1) Textbox
The textbox displays the currently selected polygon index.
If no polygon is selected, "99" is displayed.

2) Slider
The slider controls the spinning rate of the currently selected polygon.
The range of the slider is [0, 100].

---

Index Display

The index of the polygon is display under the polygon for clearer reference.

---

Commands for convenient testing

Multiple commands are implemented to make testing of the program easier.
Such as: <firework>, <spin>, <info>

---

Concave/Complex polygon rasterization

A sample data file containing multiple different polygons are provided.
Concave polygons, complex polygons are proved to rasterize successfully, even when spinning.

---

Circle drawing algorithm

An extra algorithm function is implemented

---

Well organized project

The entire project is well organized and categorized.
This makes code inspection much easier.

---

Stunning UI

Let's face it, the UI (Terminal and custom-made GUI) is pretty amazing.

---

Stunning README

It is so detailed and clear.

-------------------------------------------------------
-------------------------------------------------------

ALGORITHM PLACEMENT

-------------------------------------------------------

Bresenham algorithm
Line 159 - 193
$ ./Core/pixel.cpp

---

DDA algorithm
Line 132 - 156
$ ./Core/pixel.cpp

---

Scan line rasterization
Line 282 - 370
$ ./Core/pixel.cpp

---

Cohen-Sutherland line clipping algorihtm
Line 415 - 450
$ ./Core/pixel.cpp

---

Polygon translation
Line 25 - 35
$ ./Object/polygon.cpp

---

Polygon rotation
Line 38 - 56
$ ./Object/polygon.cpp

---

Polygon scaling
Line 59 - 73
$ ./Object/polygon.cpp

---

Sutherland-Hodgeman algorithm
Line 89 - 118
$ ./Object/polygon.cpp

---

Pixel buffer modification
Line 71 - 89
$ ./Core/pixel.cpp

---

Raw glDrawPixel function
Line 483
$ ./Core/pixel.cpp

-----------------------------------------------------

P.S. All functions and classes are categorized in different directories and source files.
	 Searching for functions should be convenient.
