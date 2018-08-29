---
layout: post
comments: true
title:  "Udacity Self-Driving Car Project #1: Finding Lane Lines on the Road"
excerpt: "Traffic Signs Classification"
date:   2018-05-26 14:00:00
mathjax: true
--- 

Overview
---

When we drive, we use our eyes to decide where to go.  The lines on the road that show us where the lanes are act as our constant reference for where to steer the vehicle.  Naturally, one of the first things we would like to do in developing a self-driving car is to automatically detect lane lines using an algorithm.

In this project I will detect lane lines in images using Python and OpenCV.  OpenCV means "Open-Source Computer Vision", which is a package that has many useful tools for analyzing images.

Final result on the Challenge video:
<iframe width="560" height="315" src="https://www.youtube.com/embed/LT7xTa_E2bQ?ecver=1" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

First attempt without preprocessing in HSL color space and without taking into account previously detected lines:
<iframe width="560" height="315" src="https://www.youtube.com/embed/dA2_WR17HXQ?ecver=1" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

Added preprocessing in HSL CS:
<iframe width="560" height="315" src="https://www.youtube.com/embed/HTFzZqnNu0k?ecver=1" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

According to the results the preprocessing stage is very important for more accurate lane line detection.

**Code is <a href="https://github.com/mike9251/SDC/tree/master/CarND-LaneLines-P1">here</a>.**


Writeup
---
The first task of the Self-Driving Car course is to create a simple lane line detector. The implemented script works with video only

*1. Pipeline*

 - load an image
 
 <div class="imgcap">
 <img src="/assets/self-driving-cars/input_img.JPG" width="480" alt="Combined Image" />
 </div>
 
 - preprocessing in HSL color space - IMPORTANT - really helps in the 'challenge' video case
 <div class="imgcap">
 <img src="/assets/self-driving-cars/input_img_hls.JPG" width="480" alt="Combined Image" />
 <div class="thecap">Input image in HSL color space</div>
 </div>
 
 Extract white and yellow lines with corresponding masks:
 <div class="imgcap">
 <img src="/assets/self-driving-cars/white_mask.JPG" width="480" alt="Combined Image" />
 <div class="thecap">Mask for white color</div>
 </div>
 <div class="imgcap">
 <img src="/assets/self-driving-cars/yellow_mask.JPG" width="480" alt="Combined Image" />
 <div class="thecap">Mask for yellow color</div>
 </div>
 
 Combine extracted images:
 <div class="imgcap">
 <img src="/assets/self-driving-cars/y_w_mask.JPG" width="480" alt="Combined Image" />
 <div class="thecap">Yellow + White mask</div>
 </div>
 
 Apply this mask to the input image to extract parts of it:
 <div class="imgcap">
 <img src="/assets/self-driving-cars/extract_y_w.JPG" width="480" alt="Combined Image" />
 </div>
 
 - convert the result of the previous stage into a gray scale
 <div class="imgcap">
 <img src="/assets/self-driving-cars/gray.JPG" width="480" alt="Combined Image" />
 </div>
 
 - perform filtering with the Gaussian filter
 <div class="imgcap">
 <img src="/assets/self-driving-cars/blured.JPG" width="480" alt="Combined Image" />
 </div>
 
 - find edges with the Canny edge detector
 <div class="imgcap">
 <img src="/assets/self-driving-cars/edges.JPG" width="480" alt="Combined Image" />
 </div>
 
 - define a region of interest (a region where we'll look for lane lines)
 <div class="imgcap">
 <img src="/assets/self-driving-cars/roi.JPG" width="480" alt="Combined Image" />
 </div>
 
 - find lines from the edge points with the Hough algorithm
 
 
 Hough algorithm returns an array of points: `x1 y1 x2 y2`. In my case I get 9 lines:
 
 ```
 [[[227 513 424 359]]

 [[213 512 374 391]]

 [[550 347 838 513]]

 [[529 338 821 513]]

 [[379 389 421 357]]

 [[292 461 407 371]]

 [[373 393 405 368]]

 [[585 368 747 465]]

 [[529 340 746 465]]]
 ```
 - separate found lines on left and right lines, calculate an average left/right lines
 
 
 To do so calculate slope of every line, if a slope is negative - it is a left line, if positive - right.
 Average left/right lines:
 
 ```left line:  ((186, 540), (466, 324)) right_line:  ((874, 540), (506, 324))```
 
 - draw lane lines
 <div class="imgcap">
 <img src="/assets/self-driving-cars/lane_line.JPG" width="480" alt="Combined Image" />
 </div>
 
 *Usage:* python pipeline.py test_videos\solidYellowLeft.mp4

*2. Identify any shortcomings*

Current implementation suffers to accurately identify curved lane lines.

*3. Suggest possible improvements*

- One possible way to improve accuracy of lane line detection is to apply extraction masks (white, yellow) to the input image - Done.
This method really helps, detector more accurately finds lines.

- To increase accuracy of curved line detection try to apply some different methods for outlier lane line rejection.

