---
layout: post
comments: true
title:  "Object Localization"
excerpt: "Brief notes on Object Classification + Localization problem."
date:   2017-11-25 11:00:00
mathjax: true
---

<!-- 
<svg width="800" height="200">
	<rect width="800" height="200" style="fill:rgb(98,51,20)" />
	<rect width="20" height="50" x="20" y="100" style="fill:rgb(189,106,53)" />
	<rect width="20" height="50" x="760" y="30" style="fill:rgb(77,175,75)" />
	<rect width="10" height="10" x="400" y="60" style="fill:rgb(225,229,224)" />
</svg>
 -->

**Object Classification + Localization**  
Assume that an image has only one object. Try to classify an object and find a box around it.

Pipeline:  
``Image -> ConvNet -> (Features from the last layer) -> Softmax(Number of classes)
                                                     -> Linear Regression(bx, by, bh,bw) - Bounding Box``

Upper-left Image corner = (0, 0)  
Bottom-right Image corner = (1, 1)

bx = 0.5  
by = 0.7  
bh = 0.3  
bw = 0.4

<div class="imgcap">
<img src="/assets/break/bb-example.JPG">
<div class="thecap">Bounding box example. </div>
</div>

**Representation of the target label y for training.**  
Assume we have three classes for objects + one class for background (there's no object):
1 - pedestrian; 2 - car; 3 - tree.

Target vector:  
`y = [pc, bx, by, bh, bw, c1, c2, c3]`  
where  
`pc` - probabality that there is an object, box confidence;
`bx, by, bh, bw` - bounding box;
`c1, c2, c3` - probabality which tells us what class of object is more probable.

For an image with an object, say `a car` vector `y` is:
`y = [1 bx by bh bw 0 1 0] `

If there is no object in the image then:
`y = [0 x x x x x x x]`
Box confidence `pc = 0` and the other components doen't matter.

***Loss function***

``L(y_hat, y) = (y_hat_1 - y_1)^2 + ... + (y_hat_8 - y_8)^2   if y_1 == 1 (pc == 1)
                (y_hat_1 - y_1)^2                             if y_1 == 0 (pc == 0)``

In practise `L2` loss isn't used. Instead for `pc` component `(y_1)` use Logistic Regression,
for BB components `(y_2, y_3, y_4, y_5)` use `L2 loss` and for `c1, c2, c3` components
use `Softmax loss`.


***Landmark detection***
<div class="imgcap">
<img src="/assets/break/landmarks-exampleJPG.JPG">
<div class="thecap">Example of face landmark detection.</div>
</div>

``
Image -> ConvNet -> Classification (Is it a face?)
                 -> Linear Regression (Predict coordinates of interesting points)``  
where `l1x, l1y, ..., lnx, lny` - coordinates of `1st, ..., n-th` landmark point.

One important thing is that labels `l1x, l1y` and so on should be **consistent** across all images
(*for example, (l1x, l1y) is always the left corner of the left eye, (l2x, l2y) is always the right corner of the left eye and so on*).
