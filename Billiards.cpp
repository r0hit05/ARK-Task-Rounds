#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <bits/stdc++.h>

using namespace std;
using namespace cv;

Mat imgcolor = imread("1.png",1);
Mat imggray(imgcolor.rows,imgcolor.cols,CV_8UC3,Scalar(0));
Mat imgline(imgcolor.rows,imgcolor.cols,CV_8UC3,Scalar(0));
// int coll=0;
float distance(Point a, Point b)
{
  return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

struct myClass
{
  bool operator()(Point pt1,Point pt2){return(pt1.x<pt2.x);}
}obj;
int main()
{
  cvtColor(imgcolor,imggray,CV_BGR2GRAY);
  threshold(imggray,imgline,50,255,CV_THRESH_BINARY);
  threshold(imggray,imggray,127,255,CV_THRESH_BINARY);
  Canny(imgline,imgline,50,200,3);
  Canny(imggray,imggray,50,200,3);
  vector<Vec4i> lines;
  //vector<Vec4i> rt;
  HoughLinesP(imgline,lines,1,CV_PI/180,30,30,10);
  //HoughLines(imgline,rt,1,CV_PI/180,30);
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( imggray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
  int n = contours.size();
  vector<Moments> mu(n);
  for( int i = 0; i<n; i++ )
      mu[i] = moments( contours[i], false );
int x1=0,y1=0,x2=0,y2=0;
   for(int i=0;i<lines.size();i++)
   {
     Vec4i l = lines[i];
     x1+=l[0];
     y1+=l[1];
     x2+=l[2];
     y2+=l[3];
   }

   x1/=lines.size();
   x2/=lines.size();
   y1/=lines.size();
   y2/=lines.size();
   //cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
   //LineIterator it(i)
  vector<Point2f> mc(n);
  for( int i = 0; i<n; i++)
    mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
  float slope = (y2-y1)/(x2-x1);
  float theta = atan(slope);
  theta/=CV_PI/180;
  //cout<<minimum<<endl;
  // for(int i=0;i<rt.size();i++)
  // {
  //   cout<<rt[i][0]<<" "<<rt[i][1]<<endl;
  // }
  vector <float> rad(n);
  vector <Point2f> cen(n);
  for(int i=0;i<n;i++)
    minEnclosingCircle(contours[i],cen[i],rad[i]);
  Mat Drawing = imggray.clone();
  sort(rad.begin(), rad.end());
  float radius = rad[n-1];
  int c=1;
  vector <Point2f> mcc;
  Point2f curr,prev;
  for(int i=0;i<n;i++)
  {
    if(i==0)
      {
        curr= mc[i];
        mcc.push_back(curr);
      }
    else
    {
      prev = curr;
      curr = mc[i];
      float d = distance(prev,curr);
      if(d>radius+1)
        mcc.push_back(curr);
    }
  }
  // for(int i=0;i<mcc.size();i++)
  // cout<<mcc[i]<<" ";
  // cout<<endl<<mcc.size()<<endl;
  // for(int i=0;i<n;i++)
  // cout<<mc[i]<<" ";
  sort(mcc.begin(),mcc.end(),obj);
  // for(int i=0;i<mcc.size();i++)
  // cout<<mcc[i]<<" ";
  for(int i=0;i<200;i++)
  {
    float x = x2 + i*cos(theta);
    float y = y2 + i*sin(theta);
    Point p(x,y);
    float d = distance(p,mcc[c]);

    if(d<=radius*2 +1 && d >= radius*2-1)
      {
        circle(Drawing,p,radius-1,Scalar(255,255,255),1,CV_AA);
        x2 = x;
        y2 = y;
        slope = (mcc[c].y-y2)/(mcc[c].x-x2);
        theta = atan(slope);
        c++;
        //coll=1;
      }
    line(Drawing,Point(x2,y2),Point(x,y),Scalar(255,255,255),1,CV_AA);
    if(x <= radius || y <= radius || y>= Drawing.rows-radius || x>=Drawing.cols - radius)
    {
      circle(Drawing,p,radius-1,Scalar(255,255,255),1,CV_AA);
      x2 = x;
      y2 = y;
      if(y<=radius || y>= Drawing.rows-radius)
      theta = -theta;
      else
      theta = (CV_PI)-theta;
      //coll=0;

    }
    namedWindow("Output",0);
    imshow("Output",Drawing);
    waitKey(10);
  }
  /*for(int i=0 ;i<n;i++)
  {
    circle(Drawing,mc[i],4,Scalar(255,255,255),1,CV_AA);
    cout<<mc[i].x<<" "<<mc[i].y<<endl;
  }*/
  //float radi = distance(Point(x2,y2),mc[1]);
  //cout<<radi<<endl;
  //line(Drawing,Point(x1,y1),Point(x2,y2),Scalar(255,255,255),1,CV_AA);
  namedWindow("Output",0);
  imshow("Output",Drawing);
  waitKey(0);
  //cout<<radius<<endl;
  return 0;

}
