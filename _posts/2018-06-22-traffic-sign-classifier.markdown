# **Traffic Sign Classifier** 

Overview
---

The task is to identify which traffic sign is present in an image which has been pre-cropped, presumably by another part of a detection system.
The image data is from <a href="http://benchmark.ini.rub.de/">German Traffic Sign Detection Benchmark (GTSRB)</a> data set.

**Dataset Summary & Exploration**  
First lets load the data set and explore it:
```python
TRAIN_IMAGE_DIR = 'GTSRB\\Final_Training\\Images'

dfs = []
#open csv file
for train_file in glob.glob(os.path.join(TRAIN_IMAGE_DIR, '*/GT-*.csv')):
    folder = train_file.split('\\')[3]
    df = pd.read_csv(train_file, sep=';')
    df['Filename'] = df['Filename'].apply(lambda x: os.path.join(TRAIN_IMAGE_DIR, folder, x))
    dfs.append(df)

train_df = pd.concat(dfs, ignore_index=True)
train_df.head()
```
The train data set looks like:
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/tr_data.JPG" width="480" alt="Combined Image" />
</div>
It contains 39209 examples of 43 classes.  

Let's look at a histogramm of class distribution:
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/tr_hist.JPG" width="480" alt="Combined Image" />
</div>
It is clear that the data is unbalanced - some classes have way more examples than other.  

Now let's look at the test data set histogramm (test data set contsists of 12630 examples):
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/test_hist.JPG" width="480" alt="Combined Image" />
</div>
Compare two distributions:
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/tr_test_hist.JPG" width="480" alt="Combined Image" />
</div>
We see that the distributions are almost identical (in percentage of presence) so we may assume that unbalance of the data won't affect prediction accuracy on the test set.

Let's see at some examples from the train set. Also print corresponding labels and image size:
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/tr_data_img.JPG" width="640" alt="Combined Image" />
</div>
So the images have different resolution. I will resize them to 32x32x1 when doing preprocessing.

**Design and Test a Model Architecture**

I used a model from a <a href="http://yann.lecun.com/exdb/publis/pdf/sermanet-ijcnn-11.pdf"> Pierre Sermanet and Yann LeCun paper</a>.
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/model.JPG" width="640" alt="Combined Image" />
</div>
The input is processed in a feedforward manner through two stage of convolutions and subsampling, and finally classified with a linear classifier. The output of the 1st stage is also fed directly to the classifier as higher-resolution features. ReLU activation function was used. I used Dropout technique after first and second convolutions as regularization and <a href="https://mike9251.github.io/2017/11/07/update-rules/">Adam</a> optimizer. Also I used exponentially decayed learning rate.


```
#Stage 1
Input = [N, 32, 32, 1]
Conv1 = [5, 5, 1, 6]
ReLU
MaxPool = [2, 2]
Dropout(keep_prob=0.5)

x1 = [N, 14, 14, 6]

Conv2 = [5, 5, 6, 16]
ReLU
MaxPool = [2, 2]
Dropout(keep_prob=0.5)

x2 = [N, 5, 5, 16]

#Stage 2
Conv3 = [5, 5, 16, 400]
ReLU

x3 = [N, 1, 1, 400]

x4 = concat(flatten(x2), flatten(x3)) = [N, 800]

#Classifier
score = matmul(x4, [800, 43]) = [N, 43]
```

As it was suggested in the paper I converted the images into YUV color space and took only the luma channel (Y) to use it as input for the model. Next the data set of grayscale images is normalized so that the data has mean zero and equal variance. Also I used the histogram normalization technique to correct pixel intensity values (spread pixel intensity distribution).
```python
def preprocess(X, hist=False, normalize=False):
    X = np.array([np.expand_dims(cv2.cvtColor(rgb_img, cv2.COLOR_RGB2YUV)[:,:,0], axis=2) for rgb_img in X])
    if hist == True:
        X = np.array([np.expand_dims(cv2.equalizeHist(np.uint8(img)), axis=2) for img in X])
    if normalize == True:
        X = (X - np.mean(X)) / np.std(X)
    return X
```
A preproccessed example looks like:
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/preproccessed_example.JPG" width="640" alt="Combined Image" />
</div>

After 50 epoches of training I obtained accuracy on the training data set = 0.995 and on the validation data set = 0.983.
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/preproccessed_example.JPG" width="640" alt="Combined Image" />
</div>
Final result on the Challenge video:
<iframe width="560" height="315" src="https://www.youtube.com/embed/LT7xTa_E2bQ?ecver=1" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

First attempt without preprocessing in HSL color space and without taking into account previously detected lines:
<iframe width="560" height="315" src="https://www.youtube.com/embed/dA2_WR17HXQ?ecver=1" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

Added preprocessing in HSL CS:
<iframe width="560" height="315" src="https://www.youtube.com/embed/HTFzZqnNu0k?ecver=1" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

According to the results the preprocessing stage is very important for more accurate lane line detection.

**Code is <a href="https://github.com/mike9251/SDC/tree/master/CarND-LaneLines-P1-master">here</a>.**


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
 <img src="assets/self-driving-cars/traffic-sign-classifier/tr_data.JPG" width="480" alt="Combined Image" />
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

