# Classify Traffic Signs On Roads 

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
 <img src="/assets/self-driving-cars/traffic-sign-classifier/preproccessed_example.JPG" width="240" alt="Combined Image" />
</div>

After 50 epoches of training I obtained accuracy on the training data set = 0.995 and on the validation data set = 0.983. The model achieves 0.929 accuracy on the test data set.
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/accuracy_tr_val.JPG" width="640" alt="Combined Image" />
</div>

**Test a Model on New Images**

Now I'm going to use the model to predict classes of the images from the Internet. Preprocess them and put labels:
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/new_imgs.JPG" width="480" alt="Combined Image" />
</div>
Print 5 best predictions for each image:  
<img src="/assets/self-driving-cars/traffic-sign-classifier/pred_1.JPG" width="480" alt="Combined Image" />  
```
Ahead only  =  99.9324202538 %
Dangerous curve to the left  =  0.0289083487587 %
Go straight or right  =  0.0158331880812 %
Turn right ahead  =  0.0116099086881 %
Right-of-way at the next intersection  =  0.00920008678804 %
```
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/pred_2.JPG" width="240" alt="Combined Image" />
</div>
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/pred_3.JPG" width="240" alt="Combined Image" />
</div>
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/pred_4.JPG" width="240" alt="Combined Image" />
</div>
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/pred_5.JPG" width="240" alt="Combined Image" />
</div>
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/pred_6.JPG" width="240" alt="Combined Image" />
</div>
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/pred_7.JPG" width="240" alt="Combined Image" />
</div>
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/pred_8.JPG" width="240" alt="Combined Image" />
</div>
<div class="imgcap">
 <img src="/assets/self-driving-cars/traffic-sign-classifier/pred_9.JPG" width="240" alt="Combined Image" />
</div>

**Code is <a href="https://github.com/mike9251/SDC/blob/master/CarND-Traffic-Sign-Classifier-Project-master/Traffic_Sign_Classifier.ipynb">here</a>.**



*2. Identify any shortcomings*

Current implementation suffers to accurately identify curved lane lines.

*3. Suggest possible improvements*

- One possible way to improve accuracy of lane line detection is to apply extraction masks (white, yellow) to the input image - Done.
This method really helps, detector more accurately finds lines.

- To increase accuracy of curved line detection try to apply some different methods for outlier lane line rejection.

