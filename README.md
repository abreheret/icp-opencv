icp-opencv 
==========

A simple example of icp (Iterative Closest Point) with [opencv](http://opencv.org/) and [kdtree](https://code.google.com/p/kdtree/).


[](image.png)

https://www.youtube.com/watch?v=tfckXoa-wRQ

Install 
=========

Generate the project with [cmake](http://www.cmake.org/) :

### Windows (Visualstudio)  

Example for visual 2008 in console in directory project :
 
      > mkdir visual2009
      > cd visual2009
      > cmake .. -G"Visual Studio 9 2008"

Or launch the bat file `create_project_studio2010.bat` for visual studio 2010

### MacOS (Xcode)


launch the shell file `create_project_xcode.sh`, verify that the file is executable else : `> chmod 755 create_project_xcode.sh`.

### Linux (Makefiles)

      > mkdir makeproject
      > cd makeproject
      > cmake .. -G"Unix Makefiles"
      > make

License
========

### MIT license 

Copyright (c) 2014 Amaury BREHERET

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

![](http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png)
