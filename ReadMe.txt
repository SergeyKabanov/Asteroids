The task:

You’re part of a mission to explore a group of asteroids.
These asteroids are well behaved in that they all move in the same plane and are static to one another.
A space craft, equipped with measuring devices, will land on one of the asteroids to perform all sorts
of experiments.
It’s your task to choose the ideal asteroid to land on such that from that vantage point the system can
monitor the whole asteroid constellation within the smallest viewing angle.

Input:

A text file. The first line contains a single integer, N the number of asteroids.
The following N lines contain integer X and Y coordinates of the asteroids separated by spaces.

Output:

On the console.
Two integers, X and Y, separated by a space,
that are the coordinates of the ideal asteroid to land on.

Example:

Input:

3
0 0
1 1
10 0

Output:

10 0

C++ solution:

1). For each point we shift the coordinate system in the way that the point should be in the center(i.e.x = 0, y = 0).

2). Calculate the angles for the points around the new coordinate center.

3). Arrange the points around the central point by angle(sorting).

4). Calculate the angle between the first and the last points around the central point.

5). Do that cyclically, shifting the first and the last point around the central point,
and calculate the angle span between the first and the last point
(considering the crossing of X axis), and find the minimal angle span.

6). Repeat 1) ... 5) for each point and find the minimal angle span for it.
Then we find the very minimal of minimal angle spans for each points, and the corresponding point will be the point we are looking for.

