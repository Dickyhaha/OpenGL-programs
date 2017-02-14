------------------------------------------------
ECS 175 2016 Fall HW Project 3
Mang Tik Chiu -- 914561017
mtchiu@ucdavis.edu
------------------------------------------------
This is the README file for HW Project 3
------------------------------------------------
------------------------------------------------

BASICS

------------------------------------------------
This project is implemented based on HW Project 2.
Most implementations remain the same except the ones which will be mentioned.
Please refer to README_hw2.txt for basic setup and implementation.
------------------------------------------------
------------------------------------------------

USAGE

------------------------------------------------
Some changes are made to the Canvas Window:

1) The program has 3 600x600 subwindows laying out horizontally.

6) GUI window is now functional and serves practical purposes, more information below.

8) The default scene consists of 2 .txt files for polygons and lights respectively. The default scene contains 1 3D model of a house, 1 ambient light source and 1 diffuse light source.

9) polygons.txt stores the information of a 3D house model, which are its .obj file, spin information and colors.

10) lights.txt stores 2 light sources, 1 being an ambient light source and the other being a diffuse and specular light source.

11) Command prompt now displays information for polygons and lights in the following format:
(Polygon: <index>/<total> Light: <index>/<total>) Command:

12) The coordination system used is as follows:

XY plane: +ve Z is further to the eye.
XZ plane: -ve Y is further to the eye.
YZ plane: -ve X is further to the eye.

Remark: Instead of having 2 lighting properties (k and I), 1 RGB vector is used instead to represent the overall numerical value. For ambient lights, the color/intensity value ranges from 0-255 while there is no limit for diffuse lights.
-----------------------------------------------
-----------------------------------------------

LIST OF NEW/MODIFIED/REMOVED COMMANDS

-----------------------------------------------
Below are the newly implemented commands and functions.
Note that all commands are in small letters.

$ polygon 

REMOVED

---

$ color <type> <r> <g> <b>

Set the currently selected polygon's <type> color to (r, g, b).
Note that the values should be integers and should range from 0 - 255.
<type> should be either 'diffuse' or 'specular'
Does nothing if there are no polygons.

---

$ info

The following information of the selected polygon will be shown:
->Center				(Center of the polygon)
->Diffuse Color			(Diffuse Color of the polygon, 0 - 255)
->Specular Color		(Specular Color of the polygon, 0 - 255)

The following information of the selected light will be shown:
->Light Model			(Ambient or Diffuse)
->Position				(Position of the light)
->Direction				(Direction to which the light faces to)
->Constant				(The C constant for light distance balancing)
->Specular degree		(The degree of power of the specular component)
->Intensity				(Ambient Color or Light Intensity)

---

$ save

Save all the information displayed by $ info to the file.
The format is as follows:

polygons.txt:

<Number of polygons>

<.obj file name>
<Spin rate>
<Spin point>
<Sping direction>
<Diffuse Color>
<Specular Color>

...

lights.txt:

<Number of light sources>

<light model (0 for ambient, 1 for diffuse/specular)>
[constant]
[specular degree]
[position]
[direction]
<color/intensity>

...

Remark: [] values are saved if the light source is a diffuse light.

---

$ load

Load the previously saved polygon and light scene to the program.

---

$ set <target> <index>

Select the <index>-th target to manipulate.
<target> should be 'polygon' or 'light'.

---

$ obj <file_name>

Load the .obj model to the selected polygon, replacing previously loaded model
List of available objects are listed in the obj_files directory.
Does nothing if no polygons are selected.

Notice that only the object name is needed, user doesn't need to type 'obj_files/' in the beginning and '.obj' at the end.

---

$ intensity <r> <g> <b>

Set the ambient color/ light intensity to (r, g, b).
Value range for ambient light sources should be [0, 1], other values are not rejected and should be tried.
Value range for diffuse light sources should be [0, 1], other values are not rejected and should be tried.

---

$ model <type>

Set the selected light cources' lighting model to ambient/diffuse.
<type> should be 'ambient' or 'diffuse'.

---

$ constant <c>

Set the constant value of the selected light source to <c>.
Setting <c> for ambient lights takes no effects as it is not used.

---

$ power <p>

Set the specular degree of the selected light source to <p>
Setting <p> for ambient lights takes no effects as it is not used.

---

$ push <target>

Push a polygon/light to the stack.
<target> should be 'polygon' or 'light'

---

$ pop <target>

Pop a polygon/light to the stack.
<target> should be 'polygon' or 'light'

---

$ position <x> <y> <z>

Set the position of the selected light to (x, y, z).
Does nothing if no lights are selected.

---

$ direction <x> <y> <z>

Set the direction of the selected light to (x, y, z).
Does nothing if no lights are selected.

---

$ swap <target> <first> <second>

Swap the <first> target with the <second> target in the stack.
<target> should be 'polygon' or 'light'.
<first> and <second> should be less than or equal to the total count.

---

$ htone <mode>

Activate half-toning with black and white as the binary colors.
<mode> should be 1 for enable and 0 for disable.

---

$ from <x> <y> <z>

Set the eye's from-vector to (x, y, z).
Default value for each projection is set to the respective projection plane.
e.g. Projection XY has the default from vector of (0, 0, -5)

---

$ lookat <fromX> <fromY> <fromZ> <atX> <atY> <atZ> <upX> <upY> <upZ> <angle> <Zmin> <Zmax>
$ lookat <option("xy", "xz", "yz")>

Specify camera viewing model parameters
Default options are XY, XZ, YZ perspective projections.

----------------------------------------------------------
----------------------------------------------------------

EXTRA FEATURE

----------------------------------------------------------

Camera Viewing Model

CVM is implemented and used by default, parameters can be changed by the user through <lookat> command

---

Back face culling

To improve the rendering speed, a simple back face culling process is implemented.
Faces with face normals pointing opposite to the projection view are not drawn at all.

---

Graphics pipeline resemblance

The overall structure of the program resembles the real graphics pipeline, where classes connect and handle the rendering pipeline: 
transformation --> lighting --> projection --> vertex render --> rasterization.

---

Complicated model rendering with animation

User can view different 3d models loaded from .obj files.
User can either load the provided models or supply custom .obj files for custom model viewing.

---

GUI enabled

User can use the GUI to tweak the diffuse colors for the selected polygon and light.
Slide bars control the R, G, B values of the selected object from top to bottom.
3 slide bars on the left are for polygon, the right ones are for the light.

---

Multiple polygon and light support

Users can push infinite number of polygons for testing.
Users can also load multiple lights with independent locations for a more realistic scene.

------------------------------------------------------
------------------------------------------------------

ALGORITHM PLACEMENT

------------------------------------------------------

Phong model
Line 71 - 87
$ ./Core/light.cpp

Gouraud shading and rasterization
Line 325 - 401
$ ./Core/pixel.cpp

Painter's algorithm
Line 113 - 154
$ ./Core/renderer.cpp

Back face culling (EXTRA)
Line 102 - 110
$ ./Core/renderer.cpp

Camera Viewing Model (EXTRA)
Line 71 - 99
$ ./Core/renderer.cpp
