# Welcome to Easyscript!

## Newest Features

### Snipping tool
This allows you to enter a start (x, y) and a width and height to return a 1d 
array of pixels. If you also enter a filename it will save an image in the 
.exe filepath, requires proper filetype (ex .png, .jpeg, .bmp, etc.).

### Keyboard input
There are 2 functions that allow you to enter keyboard input. The first takes a 
string as a parameter and then retypes it out. The second is for special keys 
and allows you to hit keys such as arrows or 'enter' key. These functions currently 
do not work in non-text applications since they do not send the same ascii values 
that these programs are used to.

### Scroll
This lets you simulate mouse scrolls. You can scroll up or down depending on the 
parameters you send to the function.

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
Currently know it works for windows, will check functionality on linux. MacOS 
is unknown right now since I do not have any Mac systems. Most likely will not 
do anything with Darwin since I believe its unsupported now.

### A way to sense key inputs
This is less likely to be added next, but a way to sense if/when a key is pressed 
and allow it to do something, such as temporarily taking over and performing some 
other function call before returning to the execution of the previous part of the 
program. 

## Plans for fixes

### None at the moment

## General maintenance

### Code improvements
Will be refactoring code as I learn better (more efficient or faster) methods.

### Updates and main maintenance
New features will be added as I learn more. Maintenance for bugs will be fixed 
as they are found before new features are added.

## Thank you!
