#include <bits/stdc++.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;



Point midpoint(Point p1, Point p2)
{
  return (p1+p2)/2;
}

int main()
{
  Mat obs = imread("MTI3NTgyMjIwOTYwNjM1MzU4.jpg",1);
  Mat tempobs = obs.clone();
  VideoCapture cap(0);
  CascadeClassifier face;
  //CascadeClassifier eyes;
  face.load("/home/r0hit/Documents/Image_Processing/haarcascade_frontalface_alt.xml");
  //eyes.load("/home/r0hit/Documents/Image_Processing/haarcascade_eye.xml");
	while(1)
	{
		Mat img,img2;
		cap >> img;
    int rows=img.rows;
    int cols=img.cols;
    for(int i=0;i<rows ; i++)
    for(int j=0;j<cols/2 ; j++)
    {
    int temp= img.at<Vec3b>(i,j)[0];
    img.at<Vec3b>(i,j)[0]=img.at<Vec3b>(i,cols-j-1)[0];
    img.at<Vec3b>(i,cols-j-1)[0]=temp;
    int temp2= img.at<Vec3b>(i,j)[1];
    img.at<Vec3b>(i,j)[1]=img.at<Vec3b>(i,cols-j-1)[1];
    img.at<Vec3b>(i,cols-j-1)[1]=temp2;
    int temp3= img.at<Vec3b>(i,j)[2];
    img.at<Vec3b>(i,j)[2]=img.at<Vec3b>(i,cols-j-1)[2];
    img.at<Vec3b>(i,cols-j-1)[2]=temp3;
    }
    cvtColor(img,img2,CV_BGR2GRAY);
    equalizeHist(img2,img2);
    vector <Rect> faces;
    face.detectMultiScale(img2,faces,1.1, 2, 0, Size(30,30));
    for(int i=0;i<faces.size();i++)
    {
      //rectangle(img,faces[i],Scalar(0,255,0),1,CV_AA);
      Point p=midpoint(Point(faces[i].x,faces[i].y),Point((faces[i].x+faces[i].width),(faces[i].y+faces[i].height)));
      if(obs.at<Vec3b>(p.x,p.y)[1] <=255 && obs.at<Vec3b>(p.x,p.y)[1] >=180)
      {circle(obs,p,10,Scalar(0,0,255),-1);}
      circle(obs,p, 10, Scalar(137,184,22),-1);
      imshow("Output",obs);
      imshow("Output1",img2);
      waitKey(1);
      obs = tempobs.clone();

    }

	}

	waitKey(0);
}
