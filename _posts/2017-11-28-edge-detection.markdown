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

The gradient of an image:  \\(\nabla_{x,y}{f} = [G_{x}, G_{y}]\\) where \\(G_{x} = \franc{\partial{f}{\partial{x}}, G_{y} = \frac{\partial{f}}{\partial{y}}\\) - partial derivitives of `f` wrt `x` and `y`.

The gradient direction is given by: \\(\theta = \tan^{-1} \left(\frac{G_{y}}{G_{x}}\right)\\).

The amount of change is given by the gradient magnitude (how rapidly the fanction is changing):  
\\(| \nabla_{x,y}{f} | = \sqrt {(G^2_{x} + G^2_{y})}\\)

\\( R\_t = \sum\_{k=0}^{\infty} \gamma^k r\_{t+k} \\)
For discrete data the dirivitive is (finite defference): \\(\frac{\partial{f(x,y)}\partial{x} ~ \frac{f(x+1, y) - f(x,y)}{1} = f(x+1, y) - f(x,y)}\\)

Magnitude of the gradient looks like edge!

Sobel operator

1/8 * [-1 0 1]
      [-2 0 2]
      [-1 0 1]

1/8 * [1 2 1]
      [0 0 0]
      [-1 -2 -1]
nabla I = [Gx, Gy]^T

Real images have some noise and computing gradient won't help finding eges.
So first of all we need to use a low-pass filter h to get rid of the noise and after that calculate the gradient.
h * f - filtered image
d(h * f)/dx = (dh/dx) * f
We can precompute the derivitive of h and apply it to the original image (with noise). This save us one operation.

With bigger sigma more small variations are smoothed away. So small value of sigma allows to detect fine features, big value suitable for detecting large scale edges.

Canny Edge Operator

Primary edge detection steps:
1. Smooth derivatives to suppress noise and compute gradient (take a 2nd derivatives of low-pass filter g and convolve the result with the image).
2. Threshold the result to fing regions of "significant" gradient.
3. "Thin" to get localized edge pixel (NMS) - look at a thick edge and pick only one pixel with max intensity along the gradient direction.
4. Linking
  4.1 Define two thresholds: low and high
  4.2 Use the high threshold to start edge curves and then low threshold to continue them


Laplasian
Based on the 2nd derivatives, more sensitive to noise.

Hough Transform / Line fitting

Use `Voting`, a general technique where we let the features vote for all models that are compatible with it.

1. Cycle through features (`edge points)`, each casting votes for model parameters.
2. Look for model parameters that receive a lot of votes.

Noise also will vote, but its votes typically are inconsistent with majority of `good` features.

Even if some parts of a line are missing we still be able to fit the line

**Hough Transform**
Left - Image Space Right - Hough Space
The line y = m0 * x + b0 is parameterized by m0, b0.
`A line  in the image corresponds to a point in Hough space (m0, b0).`

Suppose we have a point (x0, y0) in the image. Then a line that goes through this point is: y0 = m * x0 + b, m,b - some parameters.
Then in Hough space this point will correspond to a line: b = -x0 * m + y0
`A point in the image corresponds to a line in Hough space.`

Then a line that fits both points will be parameterized by m and b where corresponding lines in Hough space intersect.

Hough Algorithm

Devide Hough space in bins (d and Q will take values in some range, say d = [-100, 100], Q = [0, 180])
For each edge point get a line in Hough space, their intersections will vote for line parameters m, b.
Accumulate votes in discrete set of bins; parameters with the most votes indicate line in image space.

There is an issue with vertical lines - it's slope (m) is inf. Use Polar representation of lines.
A line in polar coordinates is defined by two parameters: d and Q.
Point in image space is a sinusoid segment in Hough space.

So use the polar parameterization and a Hough accumulator array (H(Q,d)). We can choose size of bins (1grad then number of bins = 180 of 10grad then number of bins = 18).

Basic Alg:
1. Initialize H[d; Q] = 0
2. For each edge point E(x, y) in the image
    for Q = 0 to 180 // here we can set Step
        d = xcosQ - ysinQ
        H[d, Q] += 1
3. Find the values of (d, Q) where H[d, Q] is maximum
4. The detected line in the image is given by d=xcosQ - ysinQ

d is index corresponding to some bin.

Space complexity
k^n - number of bins ,where n - dimension (or number of parameters, in case of line it's 2) k - bins each.
For lines k^2

Time complexity (in terms of number of voting elements): number of edge points

Noise in real images affect result in Houph space - the peack is not so precise, and if very fine bins are used, the we may even lost it. In this case one approach can be used: blur Hough space, so we can find a pick (maybe a litle shifted).

Extensions
1. Use the gradient for Q - reduce the number of bins Q, just one value
2. Give more votes for stronger edges
3. Change the sampling of(d, Q) to give more/less resolution. Big bins make the alg fast, but similar lines can land in the same bin. Too fine bins make the alg to be easy affected by noise.
  3.1 First, start with corse bins
  3.2 Find peaks
  3.3 Use more fine bins for some area around the peak to improve the model
