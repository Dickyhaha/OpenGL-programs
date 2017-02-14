------------------------------------------------
ECS 175 2016 Fall HW Project 5
Mang Tik Chiu -- 914561017
mtchiu@ucdavis.edu
------------------------------------------------
This is the README file for HW Project 5
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

1) The program is incomplete. Ray tracing result cannot be seen due to the flawed ray vector calculation.

2) Some components of ray tracining is implemented for partial credit, while 2 CVM models are present for regular Phong model lighting and ray tracing.

-----------------------------------------------
-----------------------------------------------

LIST OF NEW/MODIFIED/REMOVED COMMANDS

-----------------------------------------------

$ trace <mode>

Turn on/off ray tracing for CVM result inspection
<mode> should be 1/0

----------------------------------------------------------
----------------------------------------------------------

ALGORITHM PLACEMENT

------------------------------------------------------

Camera Viewing Model 
Line 68 - 114
$ ./Core/renderer.cpp

Ray tracing
Line 196 - 238
$ ./Core/renderer.cpp

Ray tracing rendering
Line 302 - 332
$ ./Core/renderer.cpp
