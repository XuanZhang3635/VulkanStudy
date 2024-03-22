[TOC]
### Scan-line rendering

一组物体和一组光源。没有投影阴影和间接照明产生。  

渲染方程只有发射项，变量有位置和方向。  

物体只反射光，即使发射光，也不能照亮物体。

$L(x,\omega_{r}) = L_{e}(x,\omega_{r}) + \sum_{l}L_e(l,\vec{lx})f_{r'}(x,\vec{lx},\omega_r)$

$L_{e}(x,\omega_{r})$: 发射光
$\sum$: 对场景中所有的灯光求和
$f_{r'}(x,\vec{lx},\omega_r)$: BRDF,跟材质有关
x: 每一个三角形的像素

Vulkan支持Scan-line rendering的pipeline称为，**The graphics pipeline**

### The graphics pipeline
可编程过程多达五种Shaders。通常只需要开始和结束阶段： Vertex Sharder和Fragment Shader.

1. Draw Command: Vulkan结合所有参数创建顶点。
2. Vertex Shader: 顶点坐标转换，或者计算与顶点相关的颜色和其他值，之后使用
3. Tessellation（镶嵌）: 比如根据距离调整分辨率，近看物体应该分得更细，远看则分得更粗糙。
4. Geometry shader: remove or add primitives to the stream， more complex code and slower performance.
5. **？？Rasterization**（光栅化）: 决定每个primitive占用的像素数。对应三角形的每一行是一个fragment。
6. Fragment shader: 决定每个fragment的颜色
7. color blending: 替换或混合相同位置的原有颜色

Note:
3D应用程序通常需要几个管道来组成最终图像，每个管道都有自己的参数和着色器。创建一个管道也是3D应用程序中最复杂的部分.

### Ray casting
计算所有三角形点和场景光对的可见性函数（the visibility function）。
可见性函数是通过投射一条光线来计算的，这条光线将被考虑的点与每个光源连接起来:如果光线与物体相交，则光线被遮挡，其效果不被考虑在渲染方程中。

$$L(x,\omega_{r}) = L_{e}(x,\omega_{r}) + \sum_{l}L_e(l,\vec{lx})f_{r'}(x,\vec{lx},\omega_r)V(x,l)$$

测试光线是否被实时遮挡的典型技术之一是使用阴影贴图(Shadow Maps):从光源位置渲染的图像，其中每个像素表示点到光源的距离。着色器计算到光源的距离，并将其与Shadow Maps中的距离进行比较:如果距离更大，则不考虑光线。

通常用Graphics Pipeline实现。

### Ray tracing
还要考虑镜面反射（mirror reflection）和折射（refraction）。
$$L(x,\omega_{r}) = L_{e}(x,\omega_{r}) + \sum_{l}L_e(l,\vec{lx})f_{r'}(x,\vec{lx},\omega_r)V(x,l)$$ $$ + L(r,\vec{rx})f_{r'}(x,\vec{rx},\omega_r)V(x,r) + L(t,\vec{tx'})f_{t'}(x',\vec{tx'},\omega_r)V(x,t)$$

- 对于反射来说，就是入射角等于反射角。
- 对于折射，通过考虑材料的折射率来模拟物体的物理性质，以确定投射折射光线的角度。

**Ray depth**: 递归重复到给定的弹跳次数.

### the Ray Tracing pipeline
- RayGen shader
- Intersection shader
- Closet Hit shader
- Any Hit shader
- Miss shader

### Radiosity
简化的渲染方程，BRDF为常数，即，$f_r(x,\omega_i,\omega_r)=\rho_x$

这样反射不依赖于从它被看到的方向，渲染方程被简化为每个点的一个变量$\rho_x$,称为物体的辐射度（Radiosity）。
$$L(x)=L_e(x)+\rho_x\int L(y)G(x,y)V(x,y)$$

然后，物体的表面被分成小块，每个小块都有一个未知数。
光源就是发出辐射的小块。渲染方程变成一个可以用迭代求解的线性方程组。
$$L(x_i)=L_e(x_i)+\rho_{x_i}\sum_{y_i}L(y_i)G(x_i,y_i)V(x_i,y_i)$$
让矩阵$R$表示，$R(x_i,y_i)=\rho_{x_i}G(x_i,y_i)V(x_i,y_i)$
渲染方程变成，$$L=L_e+R·L$$
?? 然后将方程的解内插以产生场景的视图。

辐射度能够捕获间接照明效果。但是由于块（patch）的数量大小，阴影通常非常不接近。镜面反射和折射不能直接考虑，因为它们在很大程度上取决于观察物体的方向。

Radiosity通常是一种离线技术，几乎从未实现过实时。
由于这个原因，Vulkan和其他低级图形引擎没有特定的管道来支持它。

### Montecarlo techniques

通过对从方程中选择的几个随机点和方向取平均值来计算积分。

许多技术扩展了光线追踪:不是在镜像方向上发送单个光线，而是考虑最可能输出方向的采样。
因此，对每个选定的方向跟踪光线，并使用所考虑的材料的BRDF计算辐射度。

相反，**Photon mapping** 模拟了场景中光子的运动，考虑了反弹、聚焦和其他高级现象，如焦散。

由于驱动技术的随机性，基于蒙特卡罗的渲染算法倾向于产生嘈杂的图像，其效果只能通过增加光线数量来降低(因此，渲染时间会增加)。

### The Mesh Shader pipeline 网格着色器

The Mesh Shader pipeline 是 Graphics pipeline 的泛化，其中不考虑管道的初始固定部分，网格生成可以完全由着色器处理。

网格着色器计算索引三角形列表，作为一组顶点和每个三角形的三个索引组返回。顶点计算归一化屏幕坐标，以简化光栅化。

网格着色器可以生成的顶点和三角形的数量是有限的。由于这个原因，每个对象被划分为所谓的meshlet:small patches of a mesh。

1. Task Shader: 将较大的网格细分为较小的meshlet，并控制相应的网格着色器以生成所有所需的patches

### Compute pipeline

不是为了渲染图形，是用来提交GPU的计算性能。