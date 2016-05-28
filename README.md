<<<<<<< HEAD
# C++ KCF Tracker
This package includes a C++ class with several tracking methods based on the Kernelized Correlation Filter (KCF) [1, 2].   
It also includes an executable to interface with the VOT benchmark.

[1] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
"High-Speed Tracking with Kernelized Correlation Filters", TPAMI 2015.

[2] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
"Exploiting the Circulant Structure of Tracking-by-detection with Kernels", ECCV 2012.


Authors: Joao Faro, Christian Bailer, Joao F. Henriques   
Contacts: joaopfaro@gmail.com, Christian.Bailer@dfki.de, henriques@isr.uc.pt   
Institute of Systems and Robotics - University of Coimbra / Department of Augmented Vision DFKI   

### Algorithms (in this folder) ###

"KCFC++", command: ./KCF   
Description: KCF on HOG features, ported to C++ OpenCV. The original Matlab tracker placed 3rd in VOT 2014.

"KCFLabC++", command: ./KCF lab   
Description: KCF on HOG and Lab features, ported to C++ OpenCV. The Lab features are computed by quantizing CIE-Lab colors into 15 centroids, obtained from natural images by k-means.   

The CSK tracker [2] is also implemented as a bonus, simply by using raw grayscale as features (the filter becomes single-channel).   

### Compilation instructions ###
There are no external dependencies other than OpenCV 3.0.0. Tested on a freshly installed Ubuntu 14.04.   

1) cmake CMakeLists.txt   
2) make   

### Running instructions ###

The runtracker.cpp is prepared to be used with the VOT toolkit. The executable "KCF" should be called as:   

./KCF [OPTION_1] [OPTION_2] [...]

Options available:   

gray - Use raw gray level features as in [1].   
hog - Use HOG features as in [2].   
lab - Use Lab colorspace features. This option will also enable HOG features by default.   
singlescale - Performs single-scale detection, using a variable-size window.   
fixed_window - Keep the window size fixed when in single-scale mode (multi-scale always used a fixed window).   
show - Show the results in a window.   

To include it in your project, without the VOT toolkit you just need to:
	
	// Create the KCFTracker object with one of the available options
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// Give the first frame and the position of the object to the tracker
	tracker.init( Rect(xMin, yMin, width, height), frame );

	// Get the position of the object for the new frame
	result = tracker.update(frame);
=======
#Anti-UAV System Based on PTZ Camera


## 1. Introductions ##

1. Recognize UAV (Unmanned Aerial Vehicle) by using the combinition of optical image and IR (Infrared) thermal image.

2. Track it by CF (corelation filter).

3. Locate it by LRF (Laser Range Finder) and pan-tilt bias (based on the geodetic coordinate system).

4. Hit it with laser gun.

or to simplify the problem:

1. Recognize UAV (Unmanned Aerial Vehicle) using optical image.

2. Track it by CF (Correlation Filter).

3. Hit it with laser gun.

## 2. Components ##

1. Tank. ![Tank](http://git.oschina.net/uploads/images/2016/0524/012136_7222a4f7_359863.jpeg "Tank")
2. USB Camera. ![USB Camera](http://git.oschina.net/uploads/images/2016/0524/001414_560b5489_359863.jpeg "USB Camera")
3. Laser. ![Laser](http://git.oschina.net/uploads/images/2016/0524/012505_5139f970_359863.jpeg "Laser")
3. Board. ![Board](http://git.oschina.net/uploads/images/2016/0524/001543_1f1ea73e_359863.jpeg "Board")
4. UAV. ![UAV](http://git.oschina.net/uploads/images/2016/0524/001641_25834374_359863.jpeg "UAV")

## 3. Assembling ##
1. The USB camera is bound onto the pan-tilt of the tank,with its optical axis paralleled with the gun.
2. Laser is bound onto the gun, paralleled with the optical axis of the camera; that is, gun, camera and laser should be in a line.
3. Board is put on the chassis of the tank.

## 4. Execution ##
1. Bangjie Mo : Pan-tilt Controller (May take PID algorithm, stable and sensitive required)
2. Junwei Liu : UAV Recognition (Indoor condition, fast & rubost required)
3. WanJun He : UAV Tracking (Indoor condition, fast & rubost required)

## 5. Schedule ##
1. 2016/5/27 -> Tank can be fully controlled by MinnowBoard, including chassis, pan-tilt, laser and gun.
2. 2016/5/28 -> Tank can response to the movement of the target (UAV) and aim at it using laser in real time.
3. 2016/6/04 -> Tank can follow the movement of the UAV, getting closer if the UAV flys father, getting farther if the UAV flys closer.
4. 2016/6/05 -> Tank can still aim at the UAV using laser when both of the two sides are moving.
5. 2016/6/11 -> Final test for the function requirements above.
6. 2016/6/12 -> Apply Guidance module to the tank, give it the ability to avoid obstacles.

- improvment:

7. 2016/6/18 -> Apply the same algorithm to the UAV.
8. 2016/6/19 -> Start the war between the tank and UAV.

>>>>>>> 58d0730c7bb368d855d52981ec2d1ba9a988f81b
