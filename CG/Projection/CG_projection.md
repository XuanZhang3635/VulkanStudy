[TOC]
# Projections
The technique is used to constrcut a 2D image from a 3D scene is called projection.
It is defined by the intersection of a set of projection rays with a surface.

This surface is generally a plane, called **projection plane**, and a rectangular area defined on it corrs. to the screen.

## world coordinates / global coordinates
describes the objects in the 3D space

## 3D normalized screen coordinates
allow to specify the positions of points on a screen/window in a device independent way

## Properties of projection plane
1. **the projection of linear segments in the 3D scene remain straight**(this is no longer the case with projections on different surfaces: straight lines become curves)  
Since the projected segments connect the projections of their end points, it is sufficient to connect the projected vertices to recreate a 2D representation of the corresponding 3D Objects.

2. A point on the screen corrs. to an infinite nb of coordinates in the space.

## Parallel projections
All rays are parallel to the same direction.

### a projection plane: xy-plane
**near plane** : the plane with the closest z component(maximum signed, minimum absolute value)
**far plane** : the plane with the farthest z component(minimum signed, maximum absolute value)

Only the points that are contained inside the box delimited by the left, right, top, bottom screen borders, and by the near and far planes on the depth, can be visualized after the projection.

The **normalized screen coordinates** are defined such that the near plane, the left and the bottom sides of the screen are projected respectively to Zn=0,Xl=-1,Yt=-1, and the far plane, the right and the top sides to Zf=1,Xr=1,Yb=1.
- x: left,right -> l,r
- y: top, bottom -> t,b
- z: near, far -> -n,-f

![q](/CG/assets/planes.jpeg "planes")

### Orthogonal projections
projectins where the plane is either the xy, yz,or zx-plane, and the projections rays are perpendicular(垂直) to it.

It can be implemented by normalizing the x,y,z coordinates of the projection box in the (-1,1), (-1,1) and (0,1) range.

### The projection matrix Port
We define the projection matrix Port, as the matrix that multiplied with a world coordinate Pw computes the corrs. normalized screen coordinate Pn.

$$Pn = Port*Pw$$

**the projection matrix Port** is computed in 3 steps:

1. Move the center of the projection box at the near plane in the origin with a translation Tort
$$Tort = \begin{matrix}
	1&0&0&-(r+l)/2\\
	0&1&0&-(t+b)/2\\
	0&0&1&-(-n)\\
	0&0&0&1\
\end{matrix}$$
2. Normalizes the coordinates between -1 and 1(x and y-axis) or 0 and 1(z-axis) by performing a scale transformation Sort
$$Sort = \begin{matrix}
	2/(r-l)&0&0&0\\
	0&2/(t-b)&0&0\\
	0&0&1/(f-n)&0\\
	0&0&0&1\\
\end{matrix}$$
3. Corrects the direction of the axes, by inverting the y and z-axis with the mirror transformation Mort
$$Mort = \begin{matrix}
	1&0&0&0\\
	0&-1&0&0\\
	0&0&-1&0\\
	0&0&0&1\\
\end{matrix}$$

$$=>Port = Mort*Sort*Tort = \begin{matrix}
	2/(r-l)&0&0&(r+l)/(l-r)\\
	0&2/(b-t)&0&(t+b)/(t-b)\\
	0&0&1/(n-f)&n/(n-f)\\
	0&0&0&1\\
\end{matrix}$$

### aspect ratio
the proportions of the physical screen are called aspect ratio:
$$a = Dx/Dy$$
Dx and Dy are metrical units and not pixels

In order to produce images with the correct proportions l,r,t and b must be consistent with the aspect ratio a of the monitor.
In particular, we must have that: 
$$a = (r-l)/(t-b) => r-l=a*(t-b)$$

In most of the cases, the projection box is centered in the origin both horizontally and vertically: if this happen, only **the half-width of the box w, the near plane n, the far plane f, and the aspect ratio a are needed.**

$l=-w$
$r=w$
$t=w/a$
$b=-w/a$

$$Port = Mort*Sort*Tort = \begin{matrix}
	1/w&0&0&0\\
	0&-a/w&0&0\\
	0&0&1/(n-f)&n/(n-f)\\
	0&0&0&1\\
\end{matrix}$$

In the orthogonal projections, the near plane can be put at n=0(not possible for the perspective), the projection matrix can be simplified even more, becoming **a non-uniform scaling matrix of factors S(1/w,-a/w,-1/f)**

$$Port = Mort*Sort*Tort = \begin{matrix}
	1/w&0&0&0\\
	0&-a/w&0&0\\
	0&0&-1/f&0\\
	0&0&0&1\\
\end{matrix}$$

### Coordinates
World Coordinates are *Homogenous coordinates* but normalized screen coordinates are *cartesian coordinates*: the conversion prodecure from our system to the other seen in the previous lessons must be performed.

In case of parallel projection, since it is implemented using a sequence of conventional transform matrices, the last element of Pn is always one.

The normalized screen coordinates can be obtained by simply discarding the fourth element of the vector obtained after multiplying the homogeneous coordinates with the projection matrix.

![q](/CG/assets/example1.jpeg "example1")

### Axonometric Projections(轴测投影)
showing all the faces of a cube at the same time by either rotating the projection plane w.r.t the three main axis, or by making the projection plane no longer prependicular to the projection rays.

axonometric projections where rays are prependicular to the projection plane are called Orthographic projections.

**three types**
- Isometric  
the three axes are angled at 120˚ one from the other
segments  of equal lengths parallel to the main axis in the 3D space have equal lengths also in the 2D projected plane.  
isometric projections are obtained by applying **a rotation of ±45˚ around the y-axis**, followed by **a rotation of ±35.26˚ around the x-axis**, before applying the parallel projection.  

$$\begin{matrix}
	1/w&0&0&0\\
	0&-a/w&0&0\\
	0&0&1/(n-f)&n/(n-f)\\
	0&0&0&1\\
\end{matrix}*\begin{matrix}
	1&0&0&0\\
	0&cos35.26˚&sin35.26˚&0\\
	0&-sin35.26˚&cos35.26˚&0\\
	0&0&0&1\\
\end{matrix}*\begin{matrix}
	cos45˚&0&sin45˚&0\\
	0&1&0&0\\
	-sin45˚&0&cos45˚&0\\
	0&0&0&1\\
\end{matrix}$$

- Dimetric  
Dimetric projections have two different units: one for the x and z-axis and one for y-axis  
**the angle <x,y> == <y,z> != <z,x>**  
Dimetric projections are obtained by applying **a rotation of ±45˚ around the y-axis**, followed by **an arbitrary rotation a around the x-axis**, before applying the parallel projection.

$$\begin{matrix}
	1/w&0&0&0\\
	0&-a/w&0&0\\
	0&0&1/(n-f)&n/(n-f)\\
	0&0&0&1\\
\end{matrix}*\begin{matrix}
	1&0&0&0\\
	0&cosa˚&sina˚&0\\
	0&-sina˚&cosa˚&0\\
	0&0&0&1\\
\end{matrix}*\begin{matrix}
	cos45˚&0&sin45˚&0\\
	0&1&0&0\\
	-sin45˚&0&cos45˚&0\\
	0&0&0&1\\
\end{matrix}$$

- Trimetric
Trimetric projections allow a different unit for the three axes.  
Dimetric projections are obtained by applying **an arbitrary rotation b around the y-axis**, followed by **an arbitrary rotation a around the x-axis**, before applying the parallel projection.  

$$\begin{matrix}
	1/w&0&0&0\\
	0&-a/w&0&0\\
	0&0&1/(n-f)&n/(n-f)\\
	0&0&0&1\\
\end{matrix}*\begin{matrix}
	1&0&0&0\\
	0&cosa˚&sina˚&0\\
	0&-sina˚&cosa˚&0\\
	0&0&0&1\\
\end{matrix}*\begin{matrix}
	cosb˚&0&sinb˚&0\\
	0&1&0&0\\
	-sinb˚&0&cosb˚&0\\
	0&0&0&1\\
\end{matrix}$$

**property**  
one main property of axonometric projections is maintaining the up axis(e.g. y-axis in our case) parallel to the vertical direction of the screen.

### Oblique projections(倾斜投影)
In oblique projections rays are parallel, but oblique w.r.t to the projection plane.

This has the effect that two of the three axes(namely x and y) are parallel to the screen. 

The third one(z-axis) is oriented at an angle w.r.t the previous two.

The length of the z-axis can be either be equal to the one of the two other axes or halved.  
If the length is maintained, the projection is called Cavalier, otherwise it is called Cabinet.

Oblique projections can be obtained by applying a shear along the z-axis before the orthogonal projection.  
The shear factor will determine the angle of projection, and whether it will be Cavalier or Cabinet.

In particular, it can be expressed considering **the angle a of the axis, and the corrs. reduction factor p**.(Cavalier: a=45˚,p=1; Cabinet: a=45˚,p=0.5)

$$\begin{matrix}
	1/w&0&0&0\\
	0&-a/w&0&0\\
	0&0&1/(n-f)&n/(n-f)\\
	0&0&0&1\\
\end{matrix} * \begin{matrix}
	1&0&-pcosa&0\\
	0&1&-psina&0\\
	0&0&1&0\\
	0&0&0&1\\
\end{matrix}$$

## perspective projections
All rays pass through a point, called the center of projection.

Perspective projections represents an object with diff size depending on its distance from the projection plane. This make it more suitable for immmersive(沉浸的) visulaization.

The ones that are closer to the plane have a larger projection.

A point(x,y,z) in the space, is projected on the plane to a point with normalized screen coordinates(Xs,Ys,Zs), where Zs is required to sort points according to the distance from the viewer, as for parallel(就像平行投影一样).

**Y-axis**  
To simplify the computation, we put the center of projection in the origin(0,0,0).  
The projection plane is located at a **distance d** on the z-axis from the projection center.  

$$Ys = (d*y) / z(平行等比例得出)$$

Similar,
$$Xs = (d*x) / z$$

**Parameter d**

Parameter d represents the distance of the center of projection from the projection plane  
It can be used to simulate the focal length(焦长) of the lens of a camera  
In particular, changing d has the effect of performing a zoom  

- Short d corrs. to wide-lens: it emphasizes the distances of the objects from the plane. It also allows to capture a large nb of objects in the view, producing "smaller" objects in the image.  
- Large d corrs. to tele-lens: reducing the differences in size for objects at different distances. It also has the effect of reducing the nb of objects visible in the scene, producing "enlarged" views.  
- Parallel(orthographic) projections can be obtained from perspective as d tends to the infinity.  

### Matrix  

Note: world coordinate system is oriented in the opposite direction in the z-axis: the z coordinated are indeed negative.  

$$=> Ys = (d*y) / -z , Xs = (d*x) / -z$$

**Perspective Projections Matrices Ppersp**  

The projection matrix for perspective, with **center in the origin** and projection plane at distance d on the z-axis, can be defined as:

$$ Ppersp = \begin{matrix}
	d & 0 & 0 & 0\\
	0 & d & 0 & 0\\
	0 & 0 & d & 0\\
	0 & 0 & -1 & 0\\
\end{matrix}$$ 

Note that last row of Ppersp is no longer equal to | 0 0 0 1|. This also makes the product of matrix Ppersp with an homogenous coordinate resulting in a vector with component w ≠ 1.  

$$\begin{matrix}
	d & 0 & 0 & 0\\
	0 & d & 0 & 0\\
	0 & 0 & d & 0\\
	0 & 0 & -1 & 0\\
\end{matrix} * \begin{matrix}
	x\\
	y\\
	z\\
	1\\
\end{matrix} = \begin{matrix}
	d*x\\
	d*y\\
	d*z\\
	-z\\
\end{matrix}$$ 

If we divide by the w component to obtain the equivalent Cartesian coordinate, we have:

$$|d*x d*y d*z -z| = |d*x/-z d*y/-z d*z/-z 1| = |Xs Ys -d 1|$$

**Disadvantage**: the z component of the equivalent Cartesian coordinate is always -d and the info about the distance from the projection plane is completely lost.  
This does not allow th define proper 3D normalized screen coordinates with a Zs component that reflects the distance of the point form the view plane.  

Since Zs is used just for sorting the primitives, it is enough to add an element =1 in the third row of the fourth column of the matrix to obtain a Zs component that changes monotonically(单调地) with the distance of the point.

$$ \begin{matrix}
	d & 0 & 0 & 0\\
	0 & d & 0 & 0\\
	0 & 0 & d & 1\\
	0 & 0 & -1 & 0\\
\end{matrix} * \begin{matrix}
	x\\
	y\\
	z\\
	1\\
\end{matrix} = \begin{matrix}
	d*x\\
	d*y\\
	d*z+1\\
	-z\\
\end{matrix}$$

$$|d*x d*y d*z+1 -z| = |d*x/-z d*y/-z (d*z+1)/-z 1| = |Xs Ys -d-1/z 1|$$

**Add extra transforms to make sure that this range corrs. to the world area we are interested to show**  
- n: the diatance from the origin of the near plane
- f: the diatance from the origin of the far plane
- left, right, top, bottom: l,r,t,b (**computed at the near plane, the value of n corrs. to the distance of the projection plane d. Note that for perspective, it must be n>0**)

![q](/CG/perspective.jpeg "perspective")
Note, due to the perspective, the coordinates of the borders of the screen at the far plane will be diff from the ones at the near plane.  
This is due to the fact that the visible area in the case is not a box, but a frustum(截锥体).

Note, the coordinates of the borders of the screen do not need to be symmetric.

**Steps**

1. the projection matrix, replacing d with n

$$Upersp = \begin{matrix}
	n & 0 & 0 & 0\\
	0 & n & 0 & 0\\
	0 & 0 & n & 1\\
	0 & 0 & -1 & 0\\
\end{matrix}$$

2. further transformations to obtain Normalized Screen Coordinates.  
Since l,r,t,b are given at the near plane, we start computing the projections of the top-left and bottom-right corners at the near plane.

$$\begin{matrix}
	n & 0 & 0 & 0\\
	0 & n & 0 & 0\\
	0 & 0 & n & 1\\
	0 & 0 & -1 & 0\\
\end{matrix} * \begin{matrix}
	l\\
	t\\
	-n\\
	1\\
\end{matrix} = \begin{matrix}
	n*l\\
	n*t\\
	-n*n+1\\
	n\\
\end{matrix}$$

$$|n*l n*t -n*n+1 n| = |l t 1/n-n 1|$$

$$|n*r n*b -n*n+1 n| = |r b 1/n-n 1|$$

Similar, computed the projected coordinate of a point at the far plane(z=-f) to determine the proper normalization for the z-axis.

$$\begin{matrix}
	n & 0 & 0 & 0\\
	0 & n & 0 & 0\\
	0 & 0 & n & 1\\
	0 & 0 & -1 & 0\\
\end{matrix} * \begin{matrix}
	x\\
	y\\
	-f\\
	1\\
\end{matrix} = \begin{matrix}
	n*x\\
	n*y\\
	-n*f+1\\
	f\\
\end{matrix}$$

$$|n*x n*y -n*f+1 f| = |n*x/f n*y/f 1/f-n 1|$$

Apply a translation and a scaling as parallel projections to bring:

$$x=l => Xs=-1$$
$$x=r => Xs=1$$
$$y=b => Ys=-1$$
$$y=t => Ys=1$$
$$z=1/n-n => Zs=0$$
$$z=1/f-n => Zs=1$$

=====> 
$$Tpersp = \begin{matrix}
	1 & 0 & 0 & -(r+l)/2 \\
	0 & 1 & 0 & -(t+b)/2 \\
	0 & 0 & 1 & -(n-1/n) \\
	0 & 0 & 0 & 1 \\
\end{matrix}$$

$$Spersp = \begin{matrix}
	2/(r-l) & 0 & 0 & 0 \\
	0 & 2/(t-b) & 0 & 0 \\
	0 & 0 & n*f/(n-f) & 0 \\
	0 & 0 & 0 & 1 \\
\end{matrix}$$

$$Mpersp = \begin{matrix}
	1 & 0 & 0 & 0 \\
	0 & -1 & 0 & 0 \\
	0 & 0 & 1 & 0 \\
	0 & 0 & 0 & 1 \\
\end{matrix}$$

=====>

$$Ppersp = Mpersp * Spersp * Tpersp * Upersp = \begin{matrix}
	2*n/(r-l) & 0 & (r+l)/(r-l) & 0 \\
	0 & 2*n/(b-t) & (t+b)/(b-t) & 0 \\
	0 & 0 & f/(n-f) & n*f/(n-f) \\
	0 & 0 & -1 & 0 \\
\end{matrix}$$

As for parallel projection, the values l,r,t,b must be consistent with the aspect ration a of the monitor
$$r-l=a*(t-b)$$

### Camera
In parctical cases, parameters similar to the one available in a camera are used.

In this case, only the distance n and f, the angle $\Theta$ at the top of the frustum(known as "field of view" fov-y) over the y-axis, and the aspect ration a of the screen are given.

$$ t = n*tan(\Theta/2) $$
$$ b = -n*tan(\Theta/2) $$
$$ l = -a*n*tan(\Theta/2) $$
$$ r = a*n*tan(\Theta/2) $$

====> 
$$Ppersp = \begin{matrix}
	1/(a*tan(\Theta/2)) & 0 & 0 & 0 \\
	0 & -1/(tan(\Theta/2)) & 0 & 0 \\
	0 & 0 & f/(n-f) & n*f/(n-f) \\
	0 & 0 & -1 & 0 \\
\end{matrix}$$

