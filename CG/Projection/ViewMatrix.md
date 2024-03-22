[TOC]
### Axis and Orientation

Set the origin in the middle of the 3D world, and use a compass to define directions:  
the negative z-axis -> the North direction  
the positive x-axis -> the East

### the View Matrix

Assumes that the projection plane is parallel to the xy-plane.
- For parallel projections, assuming that the projection rays are oriented along the negative z-axis 对于平行投影，假设投影射线沿负z轴方向
- For perspective, we consider the center of projection in the origin.

In both cases, it corrs. to a camera looking North.

The projection plane can be seen as the view of a camera that looks at the scene from the center of projection.(这句啥意思)投影平面可以看作是从投影中心观看场景的摄像机所看到的。

<u>The camera is characterized by</u>
- its position
- the direction where it is aiming
- its angle around this direction

Initially, a camera positioned in the origin, and aiming along the negative z-axis.cam初始在原点，朝向-z

**the Camera Matrix**: A transformation matrix Mc that moves this camera object to its target position that orient the negative z-axis along the desired direction. **Mc是把cam移到初始点的矩阵**

If we apply the inverse of Mc to all the objects in the scene, we obtain a new 3D world where the projection plane is placed exactly as required by the projection transformations seen in the previous lessons.

**the View Matrix: matrix Mc^(-1), we denote it as Mv**

### the View-Projection Matrix

$$Mvp = Mprj * Mv$$

It transforms a point from world coordinates to a 3D normalized screen coordinates as seen by a given camera characterized by matrix **Mprj**, in a given location and direction in space accordng to matrix **Mv**.

## Creating a View Matrix 

### Look-in-direction model -> first person applications

the user directly controls the camera position and the view direction.

In the Look-in-direction model, the position (Cx,Cy,Cz) of the camerais given in world coordinates.

The direction where the camera is looking is specified with three angles:
- the "compass" direction (angle $\alpha$)
- the elevation (angle $\beta$) 
- the roll over the viewing direction(angle $\rho$), this one is rarely used 

In particular,
- $\alpha$ = 0˚, the camera looks North
- $\alpha$ = 90˚, the camera looks West
- $\alpha$ = 180˚, the camera looks South
- $\alpha$ = -90˚/270˚, the camera looks East
- $\beta$ > 0˚, the camera looks up
- $\rho$ > 0˚ turns the camera counterclockwise

Considering the camera object, 
- roll corrs. to a rotation around the z-axis
- elevation(also known as the pitch) along the x-axis
- direction(alos known as yaw) around the y-axis

Rotation must be performed in a specific order:  
roll -> elevation -> the direction. Translation is performed after the rotations. The Camera Matrix is then composed in this way:

$$Mc = T(Cx,Cy,Cz) * Ry(\alpha)*Rx(\beta)*Rz(\gamma)$$

The View Matrix is the inverse of the Camera Matrix:

$$Mv = (Mc)^-1 = Rz(-\gamma)*Rx(-\beta)*Ry(-\alpha)*T(-Cx,-Cy,-Cz) $$

### Look-at model -> third person applications

In this case, the camera tracks a point (or an object) aiming at it.

In the look-at model, the center of the camera is positioned at c=(Cx,Cy,Cz), and the target is a point of coordinates a=(ax,ay,az).

The tech also requires the up vector: the direction u=(Ux,Uy,Uz) perpendicular to "the ground" of the scene.

In y-up coordinate systems, it is usually set to u=(0,1,0).  
In this way, the camera oriented with the y-axis perpendicular to the horizon.

the View Matrix is computed by:

- <u>determine the direction of its axis in World coordinates, and then using the coresponding info to build the Camera Matrix.</u> 两步：一是计算世界坐标轴，二是计算camera matrix。Mv就是camera matrix的逆。

#### determine the direction of its axis in World coordinates

first determine the transformed(negative) z-axis as the normalized vector that ends into the camera center and that starts from the point that it is looking.

Normalization(unit size) is obtained by dividing for the length of the resulting vector.

$$Vz= (c-a) / |c-a|$$

The new x-axis must be perpendicular to both the new z-axis, and the up-vector. it can be computed via the normalized cross product of the two.

the corss product of the two 3 components vectors is defined as:

$ => |Ux Uy Uz|$ X $|Vx Vy Vz| = |UyVz-UzVy UzVx-UxVz UxVy-UyVx|$

The corss porduct = 0 if these two ventors are aligned.

$$Vx = (u'Vz) / |u'Vz|$$

Problem occurs when the viewer is perfectly vertical, and thus it is impossible to align the camera with the ground: the simplest solution is to use the previously computed matrix, or select a random orientation for the x-axis.

Finally, the new y-axis should be perpendiculat to both the new z-axis and new x-axis. This could be computed via cross product of the two vectors just obtained.

Since both the new z-axis and the new x-axis are by construction perpendicular and unit vectors, normalization is not required.

$$Vy = Vz'Vx$$

#### the Camera Matrix
====> the Camera Matrix
$$Mc = \begin{matrix}
	Vx & Vy & Vz & c\\
	0 & 0 & 0 & 1\\
\end{matrix} = \begin{matrix}
	Rc & c\\
	0 & 1\\
\end{matrix}$$

====> the View Matrix
$$Mv = [Mc]^(-1) = \begin{matrix}
	(Rc)^T & -(Rc)^T*c\\
	0 & 1\\
\end{matrix}$$

## Camera navigation model

The motion of the camera is characterized by six axes:

- three motion axes
- three rotation axes

Two main navigation model: 

- Walk: has a ref "ground" and some gravity that anchors the user over it
- Fly: the viewer can fly in an open space withour specific ref points(i.e. controlling a starship in orbit)

Note: the Camera navigation is independent from the first-third person view

老师的课件中讲的是look-in-direction camera model。作业七中要写的是lookAt。

### Controls: get input form the user

All controls - keyboards, joysticks(操纵杆), gamepads and mouse - should be wrapped to return values in the -1 and +1 range for each axis.

- "+1" -> the direction aling the considered axis is selected
- "0" -> this axis is not being changed
- "-1" -> the opposite direction along the target axis is selected

Discrete sources such as keyboards, buttons, hat-switches or DPads return boolean values, that can be mapped exactly one of these three value per axis.

Continuous sources such as joysticks, thumbsticks, triggers or mouse pointer return instead  a floating oint value in the range, depending on the intensity of the pressure / motion.

A navigation model update procedure receives then up to six floating point values in the [-1,1] range:

- mx : control along the horizontal axis for the movement
- my : control along the vertical axis for the movement
- mz : control along the depth axis for the movement
- rx : rotation control around the horizontal axis
- ry : rotation control around the vertical axis
- rz : rotation control around the depth axis

### Walk navigation model

rotations around the three axis are used directly to update **the $\alpha$, $\beta$, $\rho$ parameters of the camera**.

For the motion, three vectirs Ux,Uy,Uz that represents the unitary movement in each axis, are computed.

$Ux = [Ry(\alpha)*|1 0 0 1|].xyz$
$Uy = |0 1 0|$
$Uz = [Ry(\alpha)*|0 0 $-$1 1|].xyz$

Here the [...].xyz denotes the cartesian coordinate corrs. the homogeneous one.

Motion is the performed updating the position of the center of the camera c, adding or subtracting one of the three vectors Ux, Uy and Uz.

### Speed

In order to properly animate the navigation, a linear and an angular
speed must be set:

- <u> $\mu$ is the linear soeed</u>, expressed in world units per second. it is used to update the positions.
- <u> $\omega$ is the angular speed</u>, defined in radians per second. It is used to update the rotations.

More over, since updates occurs every time a frame is shown on
screen, <u>the fraction of time passed since last update dt</u>, must be
known. dt is measured in seconds.

### the Fly navigation model

In the fly navigation models, displacements(位移) and rotations are along the axis of the camera space.

Since the view matrix brings the three axes of the camera along the x, y and z-axes, and its center to the origin, things are much simpler. We store only the view matrix and we update it directly.

It is enough to either translate or rotate the view matrix in the opposite direction to the one of the movement. The opposite is required since the view matrix is the inverse of the camera matrix。

### GLFW

to interact with the Host O.S.