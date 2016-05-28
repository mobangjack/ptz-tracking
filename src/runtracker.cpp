#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "kcftracker.hpp"
#include "time.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "cbus.h"

using namespace cv;
using namespace std;

Rect box;
bool getbox = false;
bool drawbox = false;

void mouseHandler(int event, int x, int y, int flags, void *param);

int  init()
{
	int fd;
	fd = open("/dev/ttyTHS0", O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd==-1)
	{
		perror("open error");
		return -1;
	}
    struct termios newio;
    //设置字符大小
	bzero(&newio,sizeof(newio));
	//newio.c_cflag |=CLOCAL|CREAD;
	newio.c_cflag &=~CSIZE;
	//设置停止位 CS7
	newio.c_cflag|=CS8;
	//设置奇偶校验位(奇)
	newio.c_cflag&=~PARENB;
	//newio.c_cflag|=~PARODD;//(奇数，偶数前面加~)
	newio.c_iflag&=~INPCK;
	//波特率
	cfsetispeed(&newio,B115200);
	cfsetospeed(&newio,B115200);
	//停止位1位
	newio.c_cflag&=~CSTOPB;
	//设置最少字符等待时间
	newio.c_cc[VTIME]=0;
	newio.c_cc[VMIN]=0;
	tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newio))!=0)
	{
		perror("com set error");
		return -1;
	}
	printf("set done\n");
	return fd;
}

void pack(CBus* cbus, uint8_t* cbuf, uint8_t x, uint8_t y, uint8_t z, uint8_t f)
{
	cbus->x = x;
	cbus->y = y;
	cbus->z = z;
	cbus->f = f;
	CBus_PackUp(cbus, cbuf);
}

#define scale(num,min1,max1,min2,max2) ((num-min1)*(max2-min2)/(max1-min1)+min2)
int main(int argc, char* argv[]){

	if (argc > 6) return -1;
	
	int fd,nwrite,nread;
	uint8_t x=0,y=0,z=0,f=0;
	CBus cbus;
	uint8_t cbuf[CBUS_BUF_LEN] = {0};
	
	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;
	bool CAM = false;
	string videoname;

	for(int i = 0; i < argc; i++){
		if ( strcmp (argv[i], "hog") == 0 )
			HOG = true;
		else if (strcmp(argv[i], "fixed_window") == 0)
			FIXEDWINDOW = true;
		else if ( strcmp (argv[i], "singlescale") == 0 )
			MULTISCALE = false;
		else if ( strcmp (argv[i], "show") == 0 )
			SILENT = false;
		else if ( strcmp (argv[i], "lab") == 0 ){
			LAB = true;
			HOG = true;
		}
		else if (strcmp(argv[i], "gray") == 0)
			HOG = false;
		else if (strcmp(argv[i], "camera") == 0)
			CAM = true;
		else videoname = string(argv[i]);
	}
	
	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// Tracker results
	Rect result;

	// Write Results
	ofstream resultsFile;
	string resultsPath = "output.txt";
	resultsFile.open(resultsPath);

	// Frame counter
	unsigned int nFrames = 0;
	
	VideoCapture cap;
	if (CAM)
		cap.open(0);
	else 
		cap.open(videoname);
	
	if (!cap.isOpened())
	{
		printf("fail to open video capture device\n");
		return -1;
	}
	else
	printf("success\n");

	fd=init();

	Mat frame,frametemp;
	cap >> frame;
	cvNamedWindow("KCF", CV_WINDOW_AUTOSIZE);
	imshow("KCF", frame);
	cvSetMouseCallback("KCF", mouseHandler, NULL);


	while (!getbox)
	{
		if (CAM)
			cap >> frame;
		frame.copyTo(frametemp);
		rectangle(frametemp, box, Scalar(0, 255, 255));
		imshow("KCF", frametemp);
		cvWaitKey(50);
	}
	 
	unsigned long start = 0, end = 0;
	double dt = 0;
	CvPoint center;
	while (1)
	{
	
		// First frame, give the groundtruth to the tracker
		if (nFrames == 0) {
			tracker.init(box, frame);
			rectangle(frame, box, Scalar(0, 255, 255), 1, 8);
			//cout << box.x << "," << box.y << "," << box.width << "," << box.height << endl;
			center.x=box.x+box.width/2;center.y=box.y+box.height/2;
			cout<<center.x<<","<<center.y<<"      "<<center.x-320<<' '<<center.y-240<<endl;
			
		}
		// Update
		else{
			start = getTickCount();
			result = tracker.update(frame);
			rectangle(frame, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 255, 255), 1, 8);
			//cout << result.x << "," << result.y << "," << result.width << "," << result.height << endl;
			end = getTickCount();
			dt = ((double)end - start)/getTickFrequency()*1000;
			center.x=result.x+result.width/2;center.y=result.y+result.height/2;
			cout << dt << "," << center.x << "," << center.y << "      " << center.x-320 << ' ' << center.y-240 << endl;
			x = scale((float)center.x,0.0f,640.0f,0.0f,254.0f);
			y = scale((float)center.y,0.0f,480.0f,0.0f,254.0f);
			z = 127.0f*0.5*result.width*result.height/box.width/box.height;
			pack(&cbus,cbuf,x,y,z,0);
			nwrite=write(fd,cbuf,CBUS_BUF_LEN);
			
		}

		nFrames++;

		if (!SILENT){
			imshow("KCF", frame);
			waitKey(1);
		}
		cap >> frame;
		
	}


	close(fd);
	resultsFile.close();

}

void mouseHandler(int event, int x, int y, int flags, void *param)
{
	switch (event)
	{

	case CV_EVENT_MOUSEMOVE:
		if (drawbox){
			box.width = x - box.x;
			box.height = y - box.y;
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		drawbox = true;
		box = Rect(x, y, 0, 0);
		break;
	case CV_EVENT_LBUTTONUP:
		drawbox = false;
		if (box.width < 0){
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height < 0){
			box.y += box.height;
			box.height *= -1;
		}
		getbox = true;
		break;
	}
}
