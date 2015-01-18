**Introduction**


This code implements a 3D unit cube with perspective using 2D line segments. You can manipulate this cube with various geometric transformations such as rotation, translation and scaling. Stuff like field of view, near plane, far plane and clipping is implemented as well. The clipping algorithm is based on the paper "A trip down the Graphics pipeline: Line Clipping" by Jim Blinn.

**To make this code:**
Type make in source directory.

**How to use this program:**

This program has three modes in the menu bar.

**Model mode (applying transformations to the cube)**

hold down 'r' + mouse button 1 for rotation along x axis
hold down 'r' + mouse button 2 for rotation along y axis
hold down 'r' + mouse button 3 for rotation along z axis

hold down 't' + mouse button 1 for translation along x axis
hold down 't' + mouse button 2 for translation along y axis
hold down 't' + mouse button 3 for translation along z axis

hold down 's' + mouse button 1 for scaling along x axis
hold down 's' + mouse button 2 for scaling along y axis
hold down 's' + mouse button 3 for scaling along z axis

**View mode (applying transformations to the camera)**

hold down 'o' + mouse button 1 for rotation along x axis
hold down 'o' + mouse button 2 for rotation along y axis
hold down 'o' + mouse button 3 for rotation along z axis

hold down 'n' + mouse button 1 for translation along x axis
hold down 'n' + mouse button 2 for translation along y axis
hold down 'n' + mouse button 3 for translation along z axis

hold down 'p' + mouse button 1 for field of view adjustment
hold down 'p' + mouse button 2 for near plane adjustment
hold down 'p' + mouse button 3 for far plane adjustment

**Viewport mode**

this mode allows the user to recreate viewport of different sizes by clicking one corner of the viewport then holding the button until the second corner of the viewport.

![Alt text](https://github.com/markqian/3D-cube-with-perspective/blob/master/screenshots/screenshot1.png)

![Alt text](https://github.com/markqian/3D-cube-with-perspective/blob/master/screenshots/screenshot2.png)
