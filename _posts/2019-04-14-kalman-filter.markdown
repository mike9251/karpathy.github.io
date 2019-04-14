---
layout: post
comments: true
title:  "Udacity Self-Driving Car Project #6-7: Extended / Unscented Kalman Filters"
excerpt: "State estimation of a moving object of interest with noisy lidar and radar measurements. Notes on underlying theory."
date:   2019-04-14 13:00:00
mathjax: true
---

`Bayes Filter` is a recursive algorithm for calculating the belief distribution from measurements and control data (a belief reflects the robot’s internal knowledge about the state of the environment).

Bayes Filter consists of two steps:
1. Prediction step. Calculates the beliefe distribution over the current state given previous state and executed control command.
2. Update step. Calculated corrected beliefe distribution over the current state \\(x\_{t}\\) by taking into account the sesnor's data. 

<div class="imgcap">
<img src="/assets/self-driving-cars/kf/bf.png">
</div>

\\(\mid \nabla_{x,y}{f} \mid = \sqrt {(G\_{x}^{2} + G\_{y}^{2})}\\)
 \\(\frac{\partial{f(x,y)}}{\partial{x}} ~ \frac{f(x+1, y) - f(x,y)}{1} = f(x+1, y) - f(x,y)\\)

`Kalman Filter` is an implementation of the Bayes Filter. It works under restrictions:  
1. The state transition probability \\(p(x\_{t} | u\_{t}, x\_{t−1})\\) must be a linear function in its arguments with added Gaussian noise
<div class="imgcap">
<img src="/assets/self-driving-cars/kf/state_prediction.png">
</div>

The posterior state probability:
<div class="imgcap">
<img src="/assets/self-driving-cars/kf/kf_posterior_state.png">
</div>

2. The measurement probability \\(p(z\_{t} | x\_{t})\\) must also be linear in its arguments, with added Gaussian noise
<div class="imgcap">
<img src="/assets/self-driving-cars/kf/measurements_updata.png">
</div>

The measurement probability:
<div class="imgcap">
<img src="/assets/self-driving-cars/kf/kf_measurement_probability.png">
</div>

3. The initial belief \\(bel(x\_{0})\\) must be normally distributed.
<div class="imgcap">
<img src="/assets/self-driving-cars/kf/kf_initial_belief.png">
</div>

These three assumptions are sufficient to ensure that the posterior \\(bel(x\_{t})\\) is always a Gaussian, for any point in time \\(t\\) (a linear transformation of Gaussian distribution is still Gaussian).

**The Kalman filter algorithm:**
<div class="imgcap">
<img src="/assets/self-driving-cars/kf/kf.png">
</div>

Kalman filters represent the belief \\(bel(x\_{t})\\) at time \\(t\\) by the mean \\(μ\_{t})\\) and the covariance \\(Σ\_{t}\\). The input of the Kalman filter is the belief at time t − 1, represented by μ t−1 and Σ t−1 . To update these parameters, Kalman filters require the control u t and the measurement z t . The output is the belief at time t, represented by μ t and Σ t . In lines 2 and 3, the predicted belief μ̄ and Σ̄ is calculated representing
the belief bel(x t ) one time step later, but before incorporating the measure-
ment z t . This belief is obtained by incorporating the control u t . The mean
is updated using the deterministic version of the state transition function, with the mean μ t−1 substituted for the state x t−1 . The update of the co-
variance considers the fact that states depend on previous states through the
linear matrix A t . This matrix is multiplied twice into the covariance, since
the covariance is a quadratic matrix.
The belief bel(x t ) is subsequently transformed into the desired belief
bel(x t ) in lines 4 through 6, by incorporating the measurement z t . The vari-
able K t , computed in line 4 is called Kalman gain. It specifies the degree
to which the measurement is incorporated into the new state estimate. Line 5 manipulates the mean,
by adjusting it in proportion to the Kalman gain K t and the deviation of
the actual measurement, z t , and the measurement predicted according to the
measurement probability. The key concept here is the innovation, which
is the difference between the actual measurement z t and the expected mea-
surement C t μ̄ t in line 5. Finally, the new covariance of the posterior belief
is calculated in line 6, adjusting for the information gain resulting from the
measurement.
The Kalman filter is computationally quite efficient. For today’s best algo-
rithms, the complexity of matrix inversion is approximately O(d 2.4 ) for a ma-
trix of size d × d. Each iteration of the Kalman filter algorithm, as stated here,
is lower bounded by (approximately) O(k 2.4 ), where k is the dimension of the
measurement vector z t . This (approximate) cubic complexity stems from the
matrix inversion in line 4. Even for certain sparse updates discussed in fu-
ture chapters, it is also at least in O(n 2 ), where n is the dimension of the state
space, due to the multiplication in line 6 (the matrix K t C t may be sparse).
In many applications—such as the robot mapping applications discussed in
later chapters—-the measurement space is much lower dimensional than the
state space, and the update is dominated by the O(n 2 ) operations.



The extended Kalman filter

The assumptions that observations are linear functions of the state and that
the next state is a linear function of the previous state are crucial for the cor-
rectness of the Kalman filter. The observation that any linear transformation
of a Gaussian random variable results in another Gaussian random variable
played an important role in the derivation of the Kalman filter algorithm.
The efficiency of the Kalman filter is then due to the fact that the parameters
of the resulting Gaussian can be computed in closed form.

Unfortunately, state transitions and measurements are rarely linear in
practice. For example, a robot that moves with constant translational and
rotational velocity typically moves on a circular trajectory, which cannot be
described by linear state transitions.
img ekf_linear_case, ekf_nonlinear_case

The extended Kalman filter, or EKF, relaxes one of these assumptions: the
linearity assumption. Here the assumption is that the state transition proba-
bility and the measurement probabilities are governed by nonlinear functions
g and h, respectively
img ekf_state_prediction, ekf_state_prediction_1

This model strictly generalizes the linear Gaussian model underlying
Kalman filters. The function g
replaces the matrices A t and B t and h replaces the matrix C t. Unfortunately, with arbitrary functions g and h, the belief is no longer
a Gaussian. In fact, performing the belief update exactly is usually impossi-
ble for nonlinear functions g and h, and the Bayes filter does not possess a
closed-form solution.

Linearization Via Taylor Expansion
The key idea underlying the EKF approximation is called linearization. Linearization approximates the nonlin-
ear function g by a linear function that is tangent to g at the mean of the
Gaussian (dashed line in the upper right graph). Projecting the Gaussian
through this linear approximation results in a Gaussian density, as indicated
by the dashed line in the upper left graph. The solid line in the upper left
graph represents the mean and covariance of the Monte-Carlo approxima-
tion. The mismatch between these two Gaussians indicates the error caused
by the linear approximation of g.

img ekf_nonlinear_case_result

The key advantage of the linearization, however, lies in its efficiency.
The Monte-Carlo estimate of the Gaussian was achieved by passing 500,000
points through g followed by the computation of their mean and covariance.
The linearization applied by the EKF, on the other hand, only requires deter-
mination of the linear approximation followed by the closed-form computa-
tion of the resulting Gaussian. In fact, once g is linearized, the mechanics of
the EKF’s belief propagation are equivalent to those of the Kalman filter.
This technique also is applied to the multiplication of Gaussians when a
measurement function h is involved. Again, the EKF approximates h by a
linear function tangent to h, thereby retaining the Gaussian nature of the
posterior belief.
There exist many techniques for linearizing nonlinear functions. EKFs
utilize a method called (first order) Taylor expansion. Taylor expansion constructs a linear approximation to a function g from g’s value and slope. The
slope is given by the partial derivative

img ekf_derivative

g is approximated by its
value at μ t−1 (and at u t ), and the linear extrapolation is achieved by a term
proportional to the gradient of g at μ t−1 and u t :
img ekf_g

n as Gaussian, the state transition probability is approximated as fol-
lows:

img ekf_state_transition_prob

Notice that G t is a matrix of size n × n, with n denoting the dimension of
the state. This matrix is often called the Jacobian. The value of the Jacobian
depends on u t and μ t−1 , hence it differs for different points in time.
EKFs implement the exact same linearization for the measurement func-
tion h. Here the Taylor expansion is developed around μ̄ t , the state deemed
most likely by the robot at the time it linearizes h:

img ekf_h

Measurement probability:
img ekf_measurement_prob


That is, the linear predictions in Kalman filters are replaced by their nonlin-
ear generalizations in EKFs. Moreover, EKFs use Jacobians G t and H t instead
of the corresponding linear system matrices A t , B t , and C t in Kalman filters.
The Jacobian G t corresponds to the matrices A t and B t , and the Jacobian H t
corresponds to C t

img ekf_algorithm

An important limitation of the EKF arises from the fact that it approxi-
mates state transitions and measurements using linear Taylor expansions. In
most robotics problems, state transitions and measurements are nonlinear.
The goodness of the linear approximation applied by the EKF depends on
two main factors: The degree of uncertainty and the degree of local non-
linearity of the functions that are being approximated.

img ekf_dependency_on_uncertanty

A comparison to the Gaussians resulting from the Monte-Carlo
approximations illustrates the fact that higher uncertainty typically results in
less accurate estimates of the mean and covariance of the resulting random
variable.

The second factor for the quality of the linear Gaussian approximation is
the local nonlinearity of the function g,

img ekf_dependency_on_linearization_point

The mismatch between
the accurate Monte-Carlo estimate of the Gaussian (solid line, upper left)
and the Gaussian resulting from linear approximation (dashed line) shows
that higher nonlinearities result in larger approximation errors. The EKF
Gaussian clearly underestimates the spread of the resulting density

To summarize, if the nonlinear functions are approximately linear at the
mean of the estimate, then the EKF approximation may generally be a good
one, and EKFs may approximate the posterior belief with sufficient accuracy.
Furthermore, the less certain the robot, the wider its Gaussian belief, and the
more it is affected by nonlinearities in the state transition and measurement
functions. In practice, when applying EKFs it is therefore important to keep
the uncertainty of the state estimate small.


The Unscented Kalman Filter

Another linearization method is applied by the unscented Kalman filter, or UKF,
which performs a stochastic linearization through the use of a weighted sta-
tistical linear regression process.

Linearization Via the Unscented Transform

the UKF deterministically extracts so-called sigma points from the
Gaussian and passes these through g. 
img ukf_linearization_in_ut

In the general case, these sigma points
are located at the mean and symmetrically along the main axes of the covari-
ance (two per dimension). For an n-dimensional Gaussian with mean μ and
covariance Σ, the resulting 2n + 1 sigma points X [i] are chosen according to
the following rule:
img ukf_sigma_points

Here λ = α 2 (n+κ)−n, with α and κ being scaling parameters that determine
how far the sigma points are spread from the mean. Each sigma point X [i]
[i]
has two weights associated with it. One weight, w m , is used when comput-
[i]
ing the mean, the other weight, w c , is used when recovering the covariance
of the Gaussian.

img ukf_weights, 2

The parameter β can be chosen to encode additional (higher order) know-
ledge about the distribution underlying the Gaussian representation. If the
distribution is an exact Gaussian, then β = 2 is the optimal choice.
The sigma points are then passed through the function g, thereby probing
how g changes the shape of the Gaussian.

img ukf_transformed_sp

The parameters (μ  Σ  ) of the resulting Gaussian are extracted from the mapped sigma points Y [i] according to

img ukf_mean_cov_of_transformed_sp

the dependency of the unscented transform on the
uncertainty of the original Gaussian.

img ukf_dependency_on_uncertanty

here in dependency of the local nonlinearity of the function g. As
can be seen, the unscented transform is more accurate than the first order
Taylor series expansion applied by the EKF. In fact, it can be shown that the
unscented transform is accurate in the first two terms of the Taylor expan-
sion, while the EKF captures only the first order term. (It should be noted,
however, that both the EKF and the UKF can be modified to capture higher
order terms.)

img ukf_dependency_on_nonlinearity_1/2

The UKF Algorithm
The UKF algorithm utilizing the unscented transform is presented in Ta-
ble 3.4. The input and output are identical to the EKF algorithm. Line 2
determines the sigma
points of the previous belief,

with γ short for √(n + λ). These points are propagated through the noise-free
state prediction in line 3. The predicted mean and variance are then com-
puted from the resulting sigma points (lines 4 and 5). R t in line 5 is added to
the sigma point covariance in order to model the additional prediction noise
uncertainty (compare line 3 of the EKF algorithm in Table 3.3). The predic-
tion noise R t is assumed to be additive.
A new set of sigma points is extracted from the predicted Gaussian in line
6. This sigma point set X̄ t now captures the overall uncertainty after the pre-
diction step. In line 7, a predicted observation is computed for each sigma
point. The resulting observation sigma points Z̄ t are used to compute the
predicted observation ẑ t and its uncertainty, S t . The matrix Q t is the co-
variance matrix of the additive measurement noise. Note that S t represents
the same uncertainty as H t Σ̄ t H t T + Q t in line 4 of the EKF algorithm.
Line 10 determines the cross-covariance between state and obser-
vation, which is then used in line 11 to compute the Kalman gain K t . The cross-covariance Σ̄ x,z
t corresponds to the term Σ̄ t H t T in line 4 of the EKF algorithm. With this in mind it is straightforward to show that the estima-
tion update performed in lines 12 and 13 is of equivalent form to the update
performed by the EKF algorithm.

The asymptotic complexity of the UKF algorithm is the same as for the
EKF. In practice, the EKF is often slightly faster than the UKF. The UKF is still
highly efficient, even with this slowdown by a constant factor. Furthermore,
the UKF inherits the benefits of the unscented transform for linearization.
For purely linear systems, it can be shown that the estimates generated by
the UKF are identical to those generated by the Kalman filter. For nonlinear systems the UKF produces equal or better results than the EKF, where
the improvement over the EKF depends on the nonlinearities and spread
of the prior state uncertainty. In many practical applications, the difference
between EKF and UKF is negligible.
Another advantage of the UKF is the fact that it does not require the com-
putation of Jacobians, which are difficult to determine in some domains. The
UKF is thus often referred to as a derivative-free filter.







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
