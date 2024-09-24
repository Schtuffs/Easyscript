# Welcome to Easyscript!
 To use, simply create an object with the Easyscript class and start using some functions! 
 On Windows, to compile with g++ you will need to add the '-lgdi32' tag after. 
 Ex: 'g++ -o main *.cpp -lgdi32'.

## Newest Features

### Improved keyboard input
 The old way the function worked seemed to only work for things 
 such as text documents. Now, it seems to work with games as well. 
 Meaning that this will work for its intended use of trying to script 
 some menial tasks, both for games, and for other programs that didn't like 
 the virtual keys.

## Plans for additions.

### Image file reading
The plan is that you would be able to open and read an image file on your computer 
and then find/compare that image with another. This would work well with the snip 
function, allowing for easy comparisons between the images hopefully. 

### Basic image recognition
I'm hoping to add some basic image recognition that will be able to figure out 
where an image is and be able to give back its (x, y) coordinates. Scaling and 
maybe rotation will be attempted to be added later.

### Other OS's
Currently know it works for windows, will check functionality on linux. Very unlikely 
that I will figure out if it works on Mac.

### A way to sense key inputs
This is less likely to be added next, but a way to sense if/when a key is pressed 
and allow it to do something, such as temporarily taking over and performing some 
other function call before returning to the execution of the previous part of the 
program. 

## Plans for fixes

### None at the moment

## General maintenance

### Code improvements
 Reformatted with include and src directories. Code moved out of 
 .hpp and into .cpp files.

### Updates and main maintenance
New features will be added as I learn more. Maintenance for bugs will be fixed 
as they are found before new features are added.

## Thank you!
