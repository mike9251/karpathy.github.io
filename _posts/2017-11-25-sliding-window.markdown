---
layout: post
comments: true
title:  "Sliding Window Object Detection"
excerpt: "Some key moments of Sliding Window Detector."
date:   2017-11-25 20:00:00
---
**Sliding Window Object Detector**  
Assume we have a training set `X` which consists of two types of images: with cars and without.
Giving target label vector is: `y = [1 1 1 0 0]`, where 1 indicates car object in the image.
<div class="imgcap">
<img src="/assets/break/cars.JPG">
<div class="thecap">
 Training dataset example.
</div>
</div>
*Training process:*  
`X -> ConvNet -> y_hat (Is there a car?)`  
is a classification problem.

In test time we can have a bigger image, so we slide across the image with a box (Sliding Window), take a crop, resize it to the size the ConvNet is expected and feed it to the ConvNet. Then we can change the size of the sliding window and repeat. 
A huge disadvantage of this method is computational cost. In order to get hight accuracy we need to slide across images with small stride (s = 1). Using bigger stride reduces the number of candidate crops but it increases possibility that the object will be partly missed (resulting Bounding Box will be shifted, not accurately placed around an object).

If `FC nets` are used then to classify all crops we need `#crops` forward passes. Not optimal because there are lots of overlapped computations.
There is a better way - turn each `FC layer` into `Conv layer`. It will allow to classify all crops in one forward pass.

Example:  
`X {14x14x3} -> Conv {5x5x3x16} -> {10x10x16} -> MaxPool {2x2} -> {5x5x16} -> FC {400} -> FC {400} -> Softmax {4}`  

``X {14x14x3} -> Conv {5x5x3x16} -> {10x10x16} -> MaxPool {2x2} -> {5x5x16} -> Conv {5x5x16x400} -> {1x1x400} -> Conv {1x1x400x400} -> {1x1x400} -> Conv {1x1x400x4}``

`OverFeat` architecture uses convolutional implementation of sliding windows.

Assume that we use a sliding window of size `14x14` which is sliding with a `stride = 2`. To evaluate our detector we pass an image 16x16x3. Then in one forward pass we get predictions for all 4 crops:  
<div class="imgcap">
<img src="/assets/break/fc-to-conv.JPG">
<div class="thecap">
 Evaluating four crops in one pass.
</div>
</div>
For bigger image we get:
<div class="imgcap">
<img src="/assets/break/sliding-window-net.JPG">
<div class="thecap">
 Evaluating eight crops in one pass.
</div>
</div>
The output of the network is kind of 2-d array where each cell represents a particular crop of the input image with predicted probability (box confidence).

**Intersection over Union (IoU)**  
Metric which is used to evaluate an object detection algorithm (map localization to accuracy). IoU is a measure of the overlap between two BBs.
<div class="imgcap">
<img src="/assets/break/IoU.JPG">
<div class="thecap">
 red – ground truth BB, purple – predicted BB, Union – yellow region, Intersection – green region.
</div>
</div>
`IoU(graund truth box, predicted box) = area of Union / area of Intersection`  
Set threshold value, usually 0.5 (or higher). So if IoU >= 0.5 then we consider the detection is "correct".
<div class="imgcap">
<img src="/assets/break/IoU-example.JPG">
</div>

**Non-maximum Suppression**  
The algorithm is used to get rid of redundant BBs.  
1) Discard boxes with pc < 0.6  
2) Pick a box with the highest pc  
3) Discard remaining boxes for which the IoU >= 0.5 with one from the step 2.
