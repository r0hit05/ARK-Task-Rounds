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

void flip(Mat img, int rows, int cols)
{
  for(int i=0;i<rows ; i++)
    for(int j=0;j<cols/2 ; j++)
    {
      int temp= img.at<Vec3b>(i,j)[0];
      img.at<Vec3b>(i,j)[0]=img.at<Vec3b>(i,cols-j-1)[0];
      img.at<Vec3b>(i,cols-j-1)[0]=temp;
      temp= img.at<Vec3b>(i,j)[1];
      img.at<Vec3b>(i,j)[1]=img.at<Vec3b>(i,cols-j-1)[1];
      img.at<Vec3b>(i,cols-j-1)[1]=temp;
      temp= img.at<Vec3b>(i,j)[2];
      img.at<Vec3b>(i,j)[2]=img.at<Vec3b>(i,cols-j-1)[2];
      img.at<Vec3b>(i,cols-j-1)[2]=temp;
  }
}



int main()
{
  Mat img;
  int rows,cols;
  VideoCapture cap(0);
  vector <Rect> faces;
  CascadeClassifier face;
  face.load("/home/r0hit/Documents/Image_Processing/haarcascade_frontalface_alt.xml");
  int c=0,k=0,speedvert=4,speedhor=2,flag=0;
	while(1)
	{
    cap >> img;
    rows=img.rows;
    cols=img.cols;
    flip(img,rows,cols);
    Mat obs(rows,cols,CV_8UC3,Scalar(0,0,0));
    Mat img2(rows,cols,CV_8UC1,Scalar(0));
    // Mat imgcanny(rows,cols,CV_8UC1,Scalar(0));
    cvtColor(img,img2,CV_BGR2GRAY);
    equalizeHist(img2,img2);
    //Canny(img2,imgcanny,50,200,3);
    face.detectMultiScale(img2,faces,1.1, 3, 0, Size(30,30),Size(300,300));
    for(int i=0;i<faces.size();i++)
    {
      Point p=midpoint(Point(faces[i].x,faces[i].y),Point((faces[i].x+faces[i].width),(faces[i].y+faces[i].height)));
      circle(obs,Point(p.x,p.y),15,Scalar(0,0,255),-1);
      circle(img,Point(p.x,p.y),15,Scalar(0,0,255),-1);
      if(obs.at<Vec3b>(p.x,p.y)[1]==255)
      {
        flag = 1;
        break;
      }
    }
    if(flag==1)
    {
      cout<<"Game Over"<<endl;
      break;
    }
    // vector < vector <Point>> contours;
    // vector <Vec4i> hierarchy;
    // findContours(img2,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
    //
    // vector<Moments> mu(contours.size());
    // vector<Point> mc(contours.size());
    // for(int i=0;i<contours.size();i++)
    //   mu[i]=moments(contours[i]);
    // for(int i=0;i<contours.size();i++)
    //   {
    //     mc[i].x = mu[i].m10/mu[i].m00;
    //     mc[i].y=mu[i].m01/mu[i].m00;
    //     circle(img,Point(mc[i].x,mc[i].y),2,Scalar(0,0,255),-1);
    //   }
    //  rectangle()
    rectangle(obs,Rect(0,c,cols/2-k-50,50),Scalar(0,255,0),-1);
    rectangle(obs,Rect((cols/2)+50-k,c,(cols/2)+k-20,50),Scalar(0,255,0),-1);
    rectangle(obs,Rect((cols/2)+50-k,c,70,50),Scalar(0,0,0),-1);

    c+=speedvert;
    k+=speedhor;
    if(c>rows-50 || c<0)
      speedvert=-speedvert;
    if(k== cols/2 || k == -(cols/2))
      {
        speedhor = -speedhor;
      }
    // imshow()
    imshow("Game",obs);
    imshow("Face",img);
    //obs = tempobs.clone();
      char k = waitKey(1);
      if(k == 'q')
      break;
  }
	waitKey(0);
}
