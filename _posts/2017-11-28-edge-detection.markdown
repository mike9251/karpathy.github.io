---
layout: post
comments: true
title:  "Edge Detection"
excerpt: "Overview of image gradients and key edge detectors."
date:   2017-11-28 22:54:00
mathjax: true
---

An edge is a place of rapid change in the image intensity `f` function.  
The gradient points in the direction of most rapid increase in intensity `f`.

The gradient of an image:  \\(\nabla_{x,y}{f} = [G_{x}, G_{y}]\\) where \\(G_{x} = \frac{\partial{f}}{\partial{x}}, G_{y} = \frac{\partial{f}}{\partial{y}}\\) - partial derivitives of `f` wrt `x` and `y`.
<div class="imgcap">
<img src="/assets/cv/image-gradient.JPG">
</div>
<div class="imgcap">
<img src="/assets/cv/imageJPG.JPG"><img src="/assets/cv/intensity.JPG"><img src="/assets/cv/1st-derivitive-peaks.JPG">
<div class="thecap">Image, intensity, 1st derivative of intensity.</div>
</div>

The gradient direction is given by: \\(\theta = \tan^{-1} \left(\frac{G_{y}}{G_{x}}\right)\\).

The amount of change is given by the gradient magnitude (how rapidly the fanction is changing): 
\\(\mid \nabla_{x,y}{f} \mid = \sqrt {(G\_{x}^{2} + G\_{y}^{2})}\\)

For discrete data the dirivative is (finite defference): \\(\frac{\partial{f(x,y)}}{\partial{x}} ~ \frac{f(x+1, y) - f(x,y)}{1} = f(x+1, y) - f(x,y)\\)

Magnitude of the gradient looks like edge!

**Sobel Operator**

$$G_{x} = \frac{1}{8}
    \begin{bmatrix}
    -1 & 0 & 1 \\
    -2 & 0 & 2 \\
    -1 & 0 & 1 \\
    \end{bmatrix}
$$ 
$$G_{y} = \frac{1}{8}
    \begin{bmatrix}
    1 & 2 & 1 \\
    0 & 0 & 0 \\
    -1 & -2 & -1 \\
    \end{bmatrix}
$$ and 
\\(\nabla_{x,y}{I} = [G_{x}, G_{y}]\\)

Real images have some noise and computing gradient won't help finding eges.
So first of all we need to use a low-pass filter `h` to get rid of the noise and after that calculate the gradient.
h * f - filtered image
d(h * f)/dx = (dh/dx) * f
We can precompute the derivitive of `h` and convole it with the original image (with noise). This save us one operation.

**Canny Edge Operator**

Primary edge detection steps:  
1. Smooth derivatives to suppress noise and compute gradient (take a 2nd derivatives of low-pass filter g and convolve the result with the image).  
2. Threshold the result to fing regions of "significant" gradient.  
3. "Thin" to get localized edge pixel (`NMS`) - look at a thick edge and pick only one pixel with max intensity along the gradient direction.  



With bigger sigma more small variations are smoothed away. So small value of sigma allows to detect fine features, big value suitable for detecting large scale edges.


**Laplasian**
Based on the 2nd derivatives, more sensitive to noise.
