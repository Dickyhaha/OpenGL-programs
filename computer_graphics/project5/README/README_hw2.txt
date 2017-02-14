------------------------------------------------
ECS 175 2016 Fall HW Project 2
Mang Tik Chiu -- 914561017
mtchiu@ucdavis.edu
------------------------------------------------
This is the README file for HW Project 2
------------------------------------------------
------------------------------------------------

BASICS

------------------------------------------------
This project is implemented based on HW Project 1.
Most implementations remain the same except the ones which will be mentioned.
Please refer to README_hw1.txt for basic setup and implementation.
------------------------------------------------
------------------------------------------------

USAGE

------------------------------------------------
Some changes are made to the Canvas Window:

1) The program now has 3 subwindows, each displaying the scene in different projections.
   The respective projection planes are shown on the bottom left corner of each window.
   Plane XY is at the 	TOP 	LEFT 	corner.
   Plane XZ is at the 	BOTTOM 	LEFT 	corner.
   Plane YZ is at the 	TOP 	RIGHT	corner.
   The BOTTOM RIGHT corner is left empty and will not be used.

2) Axis mapping to window coordinates are as follows:
   Plane XY: X -> Sx, Y -> Sy.
   Plane XZ: X -> Sx, Z -> Sy.
   Plane YZ: Z -> Sx, Y -> Sy.
   (X, Y, Z) are world coordinates.
   (Sx, Sy) are screen coordinates with (0, 0) at the bottom left corner.

3) The Window size is fixed at (1001, 1001).
   This is due to the subwindows' size being (500, 500).
   2 one pixel wide gap is setup for clearer boundary.

4) The scene is displayed using GENERALIZED NDC coordinate.
   Everything in the world will be scaled and drawn to the Canvas.

5) All Canvas are set to refresh mode by default, any line drawn will be erased immediately.

6) GUI slider's functionality is disabled due to the complicated action in rotation.

7) Index display is also disabled due to the complication of 3D world projection.

8) To easily examine most of the newly implemented functionalities, simply load the scene.

9) polygon.txt 			stores 2 spinning polygons with 3 triangular surfaces as plane reference.
   The 3 triangular surfaces have indices 2-4, while the 2 polygons have indices 0-1.
   polygon_regular.txt 	stores 1 cube and 1 regular tetrahedron, all coordinates are exactly places for intial manipulation.
   To load polygon_regular.txt, rename it to polygon.txt and load it in the program, make sure to backup the original file.
-----------------------------------------------
-----------------------------------------------

LIST OF NEW/MODIFIED COMMANDS

-----------------------------------------------
Below are the newly implemented commands and functions.
Note that all commands are in small letters.

$ polygon <num_vertex> <x1> <v1> ... <num_edge> <e11> <e12> <e21> <e22> ...

Set the currently selected polygon's vertices to (x1, x2), ...
Set the currently selected polygon's edges to (e11, e12), (e21, e22), ...
If there are no polygons, a new polygon is created.

Note: Edge indices should start from 0, i.e. the index of the Vertex array.

---

$ color <r> <g> <b>

Set the currently selected polygon's color to (r, g, b).
Note that the values should be integers and should range from 0 - 255.
Does nothing if there are no polygons.

---

$ translate <x> <y> <z>

Translate the selected polygon by the 3D coordinate (x, y, z).
Does nothing if there are no polygons.

---

$ rotate <x1> <y1> <z1> <x2> <y2> <z2> <angle>

Set up the axis formed by the line segment (x1, y1, z1), (x2, y2, z2).
Rotate the selected polygon around the axis by <angle> degree.
The rotation axis is shown in RED.
Does nothing if there are no polygons.

Note: Only the line segment instead of the axis will be drawn.
	  To see the axis clearly, a large coordinate difference of the 2 points is recommended.

---

$ scale <x> <y> <z>

Scale the selected polygon by <x> along x-axis, <y> along y-axis, <z> along z-axis.
Does nothing if there are no polygons.

---

$ info

The following information of the polygon will be shown:
->Spinning rate			(rate of angle of rotation)
->Spinning point 		(1st point along the axis)
->Spinning direction 	(2nd point along the axis)
->Color					(Color of the polygon)
->Center				(Center of the polygon)
->Vertex count			(Number of vertices)
->Vertices				(Coordinates of the vertices)
->Edge count			(Number of connected edges)
->Edges					(Indices of vertices of the connected edges)

---

$ spin <x1> <y1> <z1> <x2> <y2> <z2> <rate>

Set up the axis formed by the line segment (x1, y1, z1), (x2, y2, z2).
Spin the selected polygon around the axis by <rate> degree every 30ms (Glut refresh time).
The rotation axis is shown in RED.
Does nothing if there are no polygons.

Note: Only the line segment instead of the axis will be drawn.
	  To see the axis clearly, a large coordinate difference of the 2 points is recommended.
	  A spin rate of 0.0005 is recommended for best effect.

---

$ save

Save all the information displayed by $ info to the file.
The format is as follows:

<Number of polygons>

<Rasterization state>
<Spin rate>
<Spin point>
<Sping direction>
<Color>
<Number of vertices>
<x1> <y1>
<x2> <y2>
...
<Number of edges>
<e11> <e12>
<e21> <e22>
...

<Rasterization state of polygon 2>
...

---

$ load

Load the previously saved polygon scene to the program.
All polygons previously pushed will be discarded after calling this command.
Polygons with spinning date will begin to spin immediately.

----------------------------------------------------------
----------------------------------------------------------

EXTRA FEATURE

----------------------------------------------------------

Environment setup for convenient testing

Upon loading the scene, 2 spinning 3D polygons will be displayed in 3 different projections.
This helps in showcasing most of the required implementaiton of functions.
Axis planes are also shown with 3 triangular surfaces to aid visualization.

---

Infinite polygon support

Instead of 3 polygons, users can push theoretically infinite number of polygons for testing.
With <push>, <pop>, <set>, <swap> commands, users can easily manipulate polygons.

---

Animation

Users can define spinning animations for each individual polygon.
This aids in testing rotation and projection functions.

---

Generalized NDC

A complete NDC coordinate system is implemented.
Everything in the world will be drawn to scale and fit in the canvas.

---

Coloring of polygons

Users can now set the color of the polygons.
This aids object and world visualization.

---

Fully integrated UI

Terminal input supports all customized functions and reuired functions for testing.

------------------------------------------------------
------------------------------------------------------

ALGORITHM PLACEMENT

------------------------------------------------------

3D transformation
Line 65 - 124
$ ./Object/polygon.cpp

Orthogonal projection to XY, XZ, YZ planes
Line 230 - 312
$ ./Object/polygon.cpp

Generalized NDC coordinate system
Line 269 - 281
$ ./Core/pixel.cpp
