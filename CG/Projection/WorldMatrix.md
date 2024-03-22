View Matrix -> allow the user arbitrarily change her point of view  

World Matrix -> move the objects in the 3D space

# Local coordinates and World Matrix

One of the main features of 3D computer graphics is the ability of placing and moving objects in the virtual space.

Movement of the objects is usually performed with a transformation matrix: **the World Matrix** $M_w$.  

Every object is characterized by a set of local coordinates: the positions of the object's points in the space where it was created. 

When a scene is composed, the object points are moved from the positions they were modeled, to the one where they are shown.当一个场景被合成时，对象点从它们被建模的位置移动到它们被显示的位置。

The coordinates reached after this transformation are the object's global (or world) coordinates previously introduced.

The World Matrix Mw transforms the local coordinates into the corresponding global ones.  
The world matrix applies a series of translations, rotations (and possibly scaling or shears) to the local coordinates.  
As we have seen, the order of transformations is important since matrix product is not commutative.  正如我们所看到的，变换的顺序很重要，因为矩阵乘积是不可交换的。

# Creating a World Matrix

Given an object described in local coordinates the user generally wants to:
- Position the object
- Rotate the object
- Change the scale / Mirror the object

Shear and more advanced transformations, like scaling or rotations along arbitrary axes, are generally not considered since they cannot be easily generalized: should these transforms be necessary, custom procedures must be developed.

**# Position the object** 

When positioning an object, the user wants to specify the coordinates where it should be placed in the 3D space.

These coordinates should be independent of the size and on the orientation of the object.

Positioning at *p=(px,py,pz)* must then be performed by applying translation *T(px,py,pz)* **as the last transform** (otherwise the location would be changed during rotation or scaling).

**# Scaling**

If the object should be scaled not proportionally (or mirrored) of *sx,sy,sz*, rotations must be applied after, otherwise scaling direction should be oriented differently.

Scaling of *sx,sy,sz* must then be performed by a transformation *S(sx,sy,sz)* that is applied first.

The parameters of the scaling convert the local units to the global units.  
In particular, a scaling factor sx means that a unitary segment in local coordinates (along the x-axis), becomes *sx* units in global coordinates. The same applies also to factors *sy* and *sz*.

**# Rotation**

Rotation should then be performed in between scaling and translation.

To define an orientation in a 3D space, the basic rotations along the three axes x, y and z, should be combined.

**Euler angles**

One of the goals of creating the rotation component of the world matrix is to simplify the specification of the parameters required by the application to define the direction of an object.

The *Euler Angles* are three parameters that define the orientation of an object in a 3D space, and they are usually addressed as:
- Roll -> x-axis 的旋转角度
- Pitch -> y
- Yaw -> z

Euler angles were introduced for z-up coordinates systems, and are much easier to remember in that case.  
We will initially explain them in a z-up coordinate system: in this case the World's North will be aligned with the positive y-axis.

ENU means:
- East on the x
- North on the y
- Up on the z

Objects must be modeled such that they are facing along the positive x-axis, their side is aligned with the y-axis, and their vertical line is oriented along the z-axis.

![q](/CG/assets/objectModeledInWorld.png "objectModeledInWorld")

The facing axis can be for example where the car has its front view, or where an airplane or a ship is directed.

The Roll $\phi$ identifies the rotation of the object along its facing axis (the x-axis).  
A positive roll, turns the object clockwise in the direction it is facing.

The Pitch $\theta$ defines the elevation of the object and corresponds to a rotation around its y-axis (the side axis).  
A positive pitch turns the head of the object facing down.

The Yaw $\psi$ defines the direction of the object, and corresponds to a rotation along the z-axis (the vertical axis).
It defines the direction where the object is heading: $\psi=0˚$ is East, $\psi=90˚$ corresponds to North.

With this convention the rotations are performed in the alphabetic order: x-axis, y-axis and finally z-axis.

For different conventions, the axes must always be rotated in the roll, pitch, yaw order (RPW).  
If the coordinates system is y-up (as we have considered until now), and the object faces the positive z-axis, and rotations should be performed in the z, x and y order.
In this case, a yaw $\psi=0˚$ makes the object face South, and $\psi=90˚$ East.

Since different conventions can be used, 3D tools (such as Blender) might support several rotation orders and allow the user to choose the one that better suits her needs.

For example, the *"Look-in-direction"* camera model introduced earlier, uses also a y-up Euler angle orientation system.  
The camera however is oriented along the negative z-axis (and not the positive one as introduce here).  
For this reason, Roll $\phi$ and Pitch $\theta$ works in the opposite way with respect to $\rho$ and $\beta$, and direction $\alpha=0˚$ corresponds to the camera looking North instead of South for yaw $\psi=0˚$.  
Rotations are however performed in the same order.

**# final result**

With this convention, an object can be positioned in the space using nine parameters: three positions, three Euler angles, and three scaling factors.

All the parameters have an intuitive "physical" meaning.

The World Matrix $M_w$ is then be computed by factorizing the five transformations in the described order:

$$M_w = T(p_x,p_y,p_z)*R_y(y)*R_x(q)*R_z(j)*S(s_x,s_y,s_z)$$

**# Conventions**

In a real application, the modelers and the developers defines which convention should be used to create proper assets that can be integrated in the project.

An adaptation transform matrix $M_A$, can be chained before the world matrix to adapt the convention followed by third party assets, to the one required by the application.

$$M_w = T(p_x,p_y,p_z)*R_y(y)*R_x(q)*R_z(j)*S(s_x,s_y,s_z)*M_A$$

For example, this starship has been modelled off-centered (2,3,2), using the z-up convention, and two times bigger than required.

It can be corrected with the following adaptation matrix:
$$M_A = S(0.5,0.5,0.5)*R_y(90)*R_x(-90)*T(-2,-3,-2)$$

The way in which the adaptation matrix can be defined is very application dependent, and cannot be generalized. It must be custom defined in a different way for each case.

# Quatenrions

**# Gimbal(万向节) Lock**

A rotation defined by the Euler Angles, is perfect for "planar" movements, like the one available in a driving simulation or in a FPS.

However they are not the proper solution for applications such as flight or space simulators since they can suffer a problem known as Gimbal Lock.

A gimbal is a ring that can spin around its diameter.  
A physical system that allows freely orienting an object in the space has at least three gimbals connected to each other.

During rotations, the pitch also moves the roll axis, and the yaw moves both the pitch and the roll axes.  
If the pitch rotates exactly 90o degrees, the roll and the yaw become superposed(叠加的).  

When a Gimbal Lock occurs, some movements are no longer possible in a natural way.

Such movements must be accomplished by complex combinations of the three basic rotations.

A common solution is to express the rotation of an object with a mathematical device called a quaternion(四元数).

Euler Angles are however used very commonly, since Gimbal Lock does not occur in most common VR applications, and quaternions increase the mathematical complexity of the procedure.v 

**# Quatenrions**

Quaternions are an extension of complex numbers that have three imaginary components:

complex number -> $a+ib$
Quatenrions -> $a+ib+jc+kd$

The three imaginary components, that are called the vector part, are subject to the following relations:

$$i^2 = j^2 = k^2 = ijk = -1$$

From the previous specification, a complete algebra can be defined, where some of the operations are as follows:

- Sum: $(a_1 + ib_1 + jc_1 + kd_1) + (a_2 + ib_2 + jc_2 + kd_2) = (a_1 + a_2) +i(b_1 + b_2) + j(c_1 + c_2) + k(d1 + d_2)$

- Product with scalar: $\alpha*(a_1 + ib_1 + jc_1 + kd_1) = \alpha a+ i*\alpha b + j*\alpha c + k*\alpha d$

- Product with Quaternions: $(a_1 + ib_1 + jc_1 + kd_1)(a_2 + ib_2 + jc_2 + kd_2) = \\(a_1a_2 - b_1b_2 - c_1c_2 - d_1d_2) + \\ i(a_1b_2 + b_1a_2 + c_1d_2 - d_1c_2) +\\ j(a_1c_2 + c_1a_2 + d_1b_2 - b_1d_2) +\\ k(a_1d_2 + d_1a_2 + b_1c_2 - c_1b_2)$

- Norm/length: $||a+ib+jc+kd|| = \sqrt{a^2+b^2+c^2+d^2}$  
  $q = arccos(a/\sqrt{a^2+b^2+c^2+d^2})$

- Power: $(a+ib+jc+kd)^{\alpha} = ||a+ib+jc+kd||^{\alpha}[cos(\alpha q)+\frac{ib+jc+kd}{\sqrt{b^2+c^2+d^2}}sin(\alpha q)]$

**# Quaternion rotation**

A unitary quaternion（统一的四元数） q has it norm ||q||=1.

Let us consider a rotation of an angle $\theta$ around an axis oriented along a unitary vector $v = (x,y,z)$. This rotation can be represented by the following quaternion:

$$q = cos\frac{\theta}{2} + sin\frac{\theta}{2}(ix+jy+kz)$$

Since v is unitary, also q is unitary.
- v : 旋转轴向量
- $\theta$ : 绕v旋转的角度

Example 1:

Consider a rotation of 90˚ about an arbitrary axis that lies on the xy-plane and it is angled 30˚ with respect to the x-axis. Write the quaternion that encodes such rotation.

The vector that defines the direction of the axis has the following form:
$$v = (cos30˚,sin30˚,0) = (0.866,0.5,0)$$

Rotation is thus encoded by the following Quaternion:  
$0.707+0.612i+0.354j$ ($v$ 代入 $q = cos\frac{q}{2} + sin\frac{q}{2}(ix+jy+kz)$)

Example 2:

An arbitrary axis v lies on the diagonal of a box, from point A (3, 3, 0) to point B (0, 3, 0). A quaternion that encodes a rotation of 30˚ around it can be computed in the following way:

- We start computing vector v as the normalized difference of the two points:

$$v = \frac{(0,3,0)-(3,3,0)}{|(0,3,0)-(3,3,0)|} = \frac{(-3,0,0)}{||(-3,0,0)||} = (-1,0,0)$$

- We then apply the formula previously given:
$$q = cos15˚ + sin15˚(-1,0,0) = 0.966 - 0.259i$$

Quaternions can be directly converted to rotation transform matrices ($q = a+ib+jc+kd$):

$$R(q) = \begin{matrix}
	1-2c^2-2d^2 & 2bc+2ad & 2bd-2ac & 0\\
	2bc-2ad & 1-2b^2-2d^2 & 2cd+2ad & 0\\
	2bd+2ac & 2cd-2ad & 1-2b^2-2c^2 & 0\\
	0 & 0 & 0 & 1\\
\end{matrix}$$

for the matrix on the right notation, the transform matrix would be transposed(转置).

If q1 and q2 are two unitary quaternions that encode two different rotations, their products encodes the composed transform:
$$M_1<=>q_1, M_2<=>q_2$$ $$=> M_1*M_2<=>q_1*q_2$$

Note that also the quaternions product is not commutative(互换的).  
Since the rotation order matters, the order in which quaternions are multiplied should be identical to the one of the corresponding matrices.

It is possible to extract the Euler angles from a rotation matrix:

...一些看起来很复杂的运算...

To extract Euler angles from a quaternion, first its rotation matrix is computed, and then angles are extracted from the matrix.

**# Quaternion usage**
$M_w$ 可以用 q 来计算
$$M_w = T(p_x,p_y,p_z)*R(q)*S(s_x,s_y,s_z)$$


$\delta q = cos\frac{\theta}{2} + sin\frac{\theta}{2}(ix+jy+kz)$
- rotation first: $q = \delta q · q$ -> world space
- rotation last: $q = q · \delta q$ -> local space

没懂这里有什么区别?
