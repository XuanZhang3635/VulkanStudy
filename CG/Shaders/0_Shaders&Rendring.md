[TOC]
## Shaders

![q](/CG/assets/Shaders.png "Shaders")

In Vulkan, shaders are defined by **SPIR-V** code blocks.

SPIR stands for Standard Portable Intermediate Representation, and it is a binary format for specifying instructions that a GPU can run in a device independent way.

Every Vulkan driver converts the SPIR V code into the binary instructions of their corresponding GPU.

Shaders are written in high level languages, such as:
- GLSL ( openGL Shading Language)
- HLSL (High Level Shading Language Microsoft Direct X In later

### GLSL : source code -> compiling (Compiling a shader into SPIR-V)

At development time, the shaders are compiled from their original language to SPIR V.Although the main language supported by Vulkan is GLSL, compilers to transform HLSL shaders into SPIR-V exist, allowing developer to enjoy the two most popular alternatives.

In general, depending on the shader type, the file containing the its source code has a different extension.(i.e. .vert/.tesc/...)

Compiled shaders into SPIR-V files have instead the .spv extension.

Shaders can be compiled using the *glslc* tool included in the Vulkan SDK.

The most difficult part is locating the tool in the SDK folder, and manually call it from a command line window. -> makes *glslc* directly available in the main command path

====>  Assignment 09

## Rendering

To obtain realistic images with filled 3D primitives, **light reflection** should be correctly emulated.

Rendering reproduces the effects of the illumination by defining the **light sources** of the virtual environment, and the **surface properties** of the objects populating the 3D world.

### light sources

As seen in the previous lessons, light sources emits different frequencies of the spectrum, and objects reflect part of them.

The photons emitted by the light sources bounce on the objects, and some of them reach the viewpoint (the camera). Rendering computes the **quantity and the color** of such photons.

The quantity of light reflected depends on the input direction, and can bounce in many different output directions.

### Radiance辐射

The energy (expressed in Joules J 焦耳) measures the total light emitted by a surface in all the directions during a time interval.

The power (measured in Watts W = J s 1 ) is the instantaneous light energy (emitted by a surface in all the directions in a given time instant).

The irradiance(辐照度) is the fraction of power emitted by a point of a surface (in a given time instant). It is measured in W m 2 . In the following, it will be denoted by letter E.

Radiance measures the energy emitted in a given time instant from a point of a surface in a given direction. It is measured in W /(m 2 sr) (where sr are steradians , the unit of measure for the solid angle). In the following, it will be denoted by letter L.

The use of solid angles allows the radiance measure to be independent from the distance at which the object is observed.

Readings of most light sensors (including the human eyes and the cameras) are proportional to the radiance.

Rendering determines the radiance received in each point of the projection plane i.e each pixel of the screen) according to the direction of the corresponding projection ray

### The Bidirectional Reflectance Distribution Function

Light emitted by sources is characterized by its radiance.  
The **material** that composes the surface of an object determines the direction and the intensity of reflected light.

The surface properties of one object can be encoded in a function called *the Bidirectional Reflectance Distribution Function (BRDF)*.

The function inputs are the incoming $\omega i$ and outgoing $\omega r$ directions. They are both unit vectors.

The function tells how much irradiance from the incoming angle, is reflected to an outgoing angle. It is measured in sr^(-1).

$$f_r(\theta_i,\phi_i,\theta_r,\phi_r) = f_r(\omega_i,\omega_r)$$

A good BRDF should satisfy two main properties:
- Reciprocity(互惠)
- Energy conservation

*Reciprocity* means that if the ingoing and outgoing directions are swapped, the value of the function does not change (for this reason this
is known as bidirectional.

$$f_r(\omega_i,\omega_r) = f_r(\omega_r,\omega_i)$$

*Energy conservation* means that the BRDF cannot increase the total
irradiance that leaves a point on a surface.

$$\int f_r(\omega_i,\omega_r) cos\theta_rd\omega_r ≤ 1$$
The integral can be less than one if the point absorbs energy.

BRDF db https://www.merl.com/brdf/ 

### The rendering equation 重点

The BRDF allows relating together the irradiance in all the directions for all the points of the objects composing a scene. This relation is called the rendering equation.

$$L(x,\omega_r) = L_e(x,\omega_r) + \\ \int L(y,\vec{yx})f_r(x,\vec{yx},\omega_r)G(x,y)V(x,y)dy$$

The equation determines the radiance of each point x of any object in any direction $\omega$ of the space.

- Objects can emit light: term $L_e()$ accounts for the light that the object at x emits in direction $\omega$.  
This parameter mainly characterizes light sources (i.e. a bulb or a neon).

- The $\int dy$ accounts for the light that hits the considered point x
from all the points y of the surfaces of all the objects and lights in the scene.  
The integral also includes other points of the same object to allow the computation of effects such as self-shadowing or self-reflection.

- $L(x,\vec{yx})$ -> For each object, the equation considers the radiance emitted toward point x.  
<u>Here $\vec{yx}$ represents the direction of the line that connects point y to point x.</u>

- $f_r(x,\vec{yx},\omega_r)$ -> the BRDF of the material of the object at point x.  
Since the materials of objects might change over their surface, the
BRDF depends also on position of point x.  
$\vec{yx}$ -> The input angle corresponds to the direction of the segment that connects y to x.  
$\omega_r$ (on the left hand side of the equation) -> The output angle corresponds to the direction from which the output radiance is being computed.

- $G(x,y)$ -> encodes the geometric relation between points x and y.  
  It considers both the relative orientation and the distance of the two points. It defines as:
  $$G(x,y) = (cos\theta_xcos\theta_y) / r^2_{xy}$$
  $cos()$ -> the angle relative to the respective normal vectors  
  $r^2_{xy}$ -> the squared distance of the two points

- $V(x,y)$ -> the visibility between points x and y  
  $V(x,y) == 1$ if the two points can see each other  
  $V(x,y) == 0$ if point y is hidden by some other object in between  
  $V(x,y) allows for the computation of shadows, and makes sure that in each input direction at most a single object is considered.

- $L()$ is the unknown of the equation.  
  Since it appears on both sides of the equation, the rendering equation is an integral equation of the second kind.(未知数出现在方程两边，属于第二类积分方程)

### Considering colors

The rendering equation is repeated for every wavelength $\lambda$ of the light: usually this means that the equation is repeated for the three different RGB channels.

$$L(x,\omega_r) = L_e(x,\omega_r,\lambda) + \\ \int L(y,\vec{yx},\lambda)f_r(x,\vec{yx},\omega_r,\lambda)G(x,y)V(x,y)dy$$

In particular, lights have associated RGB values that accounts for the photons emitted for each of the three main frequencies.

Objects are characterized by a BRDF with different parameters for each of the three primary colors.

Three images are produced independently, each considering either the red, green or blue components alone.  
Finally, the three colors are assembled to create the output image.

Due to the separation of the color components, combinations of light and material colors lead to results that are not straightforward.

### Extensions to the rendering equation

The proposed rendering equation is capable of accurately computing several effects like reflections, shadows, matte and glossy objects.

However, it cannot simulate other effects such as participating media
(i.e. rendering of gases and fumes) or even transparent objects like glass or water.

The BDRF and the rendering equations have been extended to account
for these other important materials.

====> consider transmitted lights:  
this is done by defining the BTDF : Bidirectional Transmittance() Distribution Function  
It has a similar definition to the BRDF , but it is used in the opposite direction.

$$f_t(\theta_i,\phi_i,\theta_r,\phi_r) = f_t(\omega_i,\omega_r)$$

Since usually the angles for the BRDF and BTDF do not overlap, they are included in a single function called BSDF: Bidirectional Scattering Distribution Function.

$$L(x,\omega_r) = L_e(x,\omega_r) + \\ \int L(y,\vec{yx})f_r(x,\vec{yx},\omega_r)G(x,y)V(x,y)dy + \\ \int L(y,\vec{yx'})f_t(x',\vec{yx'},\omega_r)G(x',y)V(x',y)dy$$


====> consider subsurface scattering(i.e. human skin, marble):
A more complex function, called BSSRDF Bidirectional surface reflectance distribution function ) must be used.

$$L(x,\omega_r) = L_e(x,\omega_r) + \\ \int\int L(y,\vec{yx'})f_r(x,\vec{yx'},,x',\omega_r)G(x',y)V(x',y)dx'dy$$

- $x'$ -> considers the point on the surface from which lights enters at angle $\omega_i$.

The rendering equation now integrates over all the points of an object to compute the quantity of lights that exits from a give position.

The rendering equations are very hard to solve, and generally require complex discretization technques.

### Lights basics

light sources can be divided into direct and indirect.

- Direct sources represent lights coming from specific positions and directions (e.g. a lamp, the sun in an outdoor scene).
- Indirect sources consider all the other types of illumination, mainly
caused by light bounces and reflections among the surfaces.

With only direct sources, images become very dark and do not seem very realistic: if a point is not hit by any light, it appears black.

Projected shadows are created by the occlusion(遮挡) of direct light sources.

Indirect lighting adds realism, by making elements in directions not directly hit by sources still visible thanks to light bounces, but it requires a lot of computation, and it is not easy to implement in real time.

In most of the cases, light contribution for single points and directions is computed off-line and stored in some image based structure, which is later used to approximate indirect lighting in real time rendering.