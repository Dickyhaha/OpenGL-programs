------------------------------------------------
ECS 175 2016 Fall HW Project 4
Mang Tik Chiu -- 914561017
mtchiu@ucdavis.edu
------------------------------------------------
This is the README file for HW Project 4
------------------------------------------------
------------------------------------------------

BASICS

------------------------------------------------
This project is implemented based on HW Project 3.
Most implementations remain the same except the ones which will be mentioned.
Please refer to README_hw3.txt for basic setup and implementation.
------------------------------------------------
------------------------------------------------

USAGE

------------------------------------------------
Some changes are made to the Canvas Window:

1) The program has 1 main window only

6) GUI window is disabled.

8) The default scene consists of only curves, though polygons and lights can still be manipulated, for the purpose of this project it is left empty.

9) curves.txt stores the information of curves, precise format will be given below.

10) Bezier curves are by default CYAN while b-spline curves are by default GREEN, colors can be customized.

11) Interactive editor mode is available for direct control point manipulation using the mouse, details will be given below.

-----------------------------------------------
-----------------------------------------------

LIST OF NEW/MODIFIED/REMOVED COMMANDS

-----------------------------------------------
Below are the newly implemented commands and functions.
Note that all commands are in small letters.

$ color <type> <r> <g> <b>

Set the selected curve to color (r, g, b)
The range is [0, 255]
For the purpose of this project <type> should be 'curve'

---

$ info

For the purpose of this project, only curve information will be displayed.

---

$ save

Save all the information displayed by $ info to the file.
The format is as follows:

curves.txt

<curve count>

<type>			(0 for bezier, 1 for b-spline)
[degree]		(Degree of the curve, only present for b-spline curves)
<segment>		(Number of segments)
<r> <g> <b>		(Color of the curve)
<point count>	(Number of control points)
<x1> <y1>		(Coordinates of the 1st control point)
<x2> <y2>
.
.
.

...

---

$ load

Load the previously saved curves scene to the program.

---

$ set <target> <index>

Select the <index>-th target to manipulate.
For the purpose of this project <target> should be 'curve'.

---

$ push <target>

Push a curve to the stack.
For the purpose of thie project <target> should be 'curve'

---

$ pop <target>

Pop a curve from the stack.
For the purpose of thie project <target> should be 'curve'
---

$ swap <target> <first> <second>

Swap the <first> target with the <second> target in the stack.
<target> should be 'curve'.
<first> and <second> should be less than or equal to the total count.

---

$ curve <type> [degree] <segment> <point count> <x1> <y1> <x2> <y2> ...

Define the attributes of the selected curve
<type> 			should be 'bezier' or 'bspline'
[degree] 		is needed only when type is 'bspline'
<segment> 		number of segments to draw lines, aka resolution
<point count>	total number of control points to be defined
<x1> <y1> ..	coordinates of the control points

---

$ knot <degree> <u1> <u2> ...

Set the knot vector of the selected curve.
Only takes effect when the select curve is of type BSPLINE
The number of knots to be defined equals Degree + Numer of control points

---

$ degree <degree>

Set the degree of the b-spline curve to <degree>
The knot vector will reset to default values when called.

---

$ segment <segment>

Set the number of segments(resolution) to <segment>

---

$ move <index> <x> <y>

Move the index-th control point to (x, y)

---

$ remove <index>

Erase the index-th control point

---

$ add <index> <x> <y>

Insert a new control point (x, y) at index-th position.
New control point becomes the index-th control point

---

$ translate/rotate/scale <type> <...>

Carry out transformation on curves
<type> should be 'curve'
Other parameters shold follow the polygon transformation in 2D

---

$ spin <type> <rate>

Spin the selected curve in rate degrees per update
For the purpose of this project <type> should be 'curve'
For a normal spinning speed, set rate to 2

----------------------------------------------------------
----------------------------------------------------------

Interactive Editor Mode

User can manipulated the selected curve interactively through the canvas window.
By focusing on the window and pressing <space bar>, control points and the corresponding indices will be displayed.

The following mouse operations are supported:
Push   point --- Right click on a space where no control points are present
Delete point --- Right click on a control point
Modify point --- Left click and drag a control point to change position

Remark: NDC is supported in this program.
		Dragging control points to the edge is allowed and will change the scaling.
		Interactive Editor Mode is support while animation is ongoing, however the center will be recalculated on every update

----------------------------------------------------------
----------------------------------------------------------

EXTRA FEATURE

----------------------------------------------------------

1) Normalized device coordinates

2) Interactive editor mode

3) Comprehensive UI and CMD

4) Animation

------------------------------------------------------
------------------------------------------------------

ALGORITHM PLACEMENT

------------------------------------------------------

DeCasteljau's/DeBoor's algorithms
Line 240 - 290
$ ./Core/renderer.cpp

