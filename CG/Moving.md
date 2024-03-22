[TOC]

#### Moving aasets

- Ground motion (similar to the **Walk camera navigation** model).
- World coordinates motion.
- Local coordinates motion (similar to the **Fly camera navigation** model).

#### Moving objects on a ground based scene

经常用在third-person applications，移动和camera目标对应的物体。大多数情况下，只需要yaw。

#### Local and Global coordinates motion model

Global/World和Local coordinates用于模拟Blender等应用程序中用于定位对象的经典工具。（就是建模工具中常用到的两种坐标）

Global和Ground不同，因为Global运动方向不受物体方向的影响，而是始终与场景主轴对齐。

#### Camera position in third person applications

在一个third person application中，必须确认camera和its target的位置

#### Two cases

- flying a space ship using local coordinates motion model
- controlling a character using ground based model

#### Damping 阻尼

就是让移动看起来更顺滑

$p=p_{old}·e^{-\lambda·dt}+P_{new}·(1-e^{-\lambda·dt})$
$P_{old}=P$

Where $\lambda$ is the damping speed. Usually a factor of $\lambda$ = 10, produce reasonable results.