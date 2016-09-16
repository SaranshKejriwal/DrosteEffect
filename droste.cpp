#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<stdio.h>
using namespace cv;
using namespace std;
Mat draw_droste(Mat img,Rect ROI);//declaration
int main()
{

	Mat img,imgt;
	Mat img2;
	VideoCapture cap(1);
	namedWindow("Color");
	namedWindow("Droste");
	moveWindow("Color",20,0);
	moveWindow("Droste",20,0);
	static int bl=0,rl=0,gl=50,bh=50,gh=106,rh=50;
	vector < vector < Point > >contours;
	for(;;)
	{
		cap>>img;
		flip(img,img,1);
		createTrackbar("BL","Color",&bl,255);
		createTrackbar("GL","Color",&gl,255);
		createTrackbar("RL","Color",&rl,255);
		
		
		createTrackbar("BH","Color",&bh,255);
		createTrackbar("GH","Color",&gh,255);
		createTrackbar("RH","Color",&rh,255);
		
		inRange(img,Scalar(bl,gl,rl),Scalar(bh,gh,rh),imgt);
		findContours (imgt, contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);//cause of unhandled exceptions
		cvtColor(imgt,imgt,CV_GRAY2BGR);


		for(int i=0;i<contours.size();i++)
					{
		 				if(contourArea(contours[i])>30)//to eliminate noise contours
			 				{
								drawContours (imgt, contours, -1, Scalar(0,200,200), 2);
								//drawContours (mask, contours, -1, color, 2);
							 }
						 
		 
					}
		int max=90,maxi=-10;
		vector<Rect> boundRect( contours.size() );
		for(int i=0;i<contours.size();i++)
					{
						 boundRect[i] = boundingRect( contours[i] );
					}
		for(int i=0;i<boundRect.size();i++)
					{
						if(boundRect[i].width*boundRect[i].height>max)
						 {
							max=boundRect[i].width*boundRect[i].height;
							maxi=i;
						 }
						
			 
					}
		
	
		imshow("Droste",img);
		char c=waitKey(10);
		if(c=='b')
			break;
	}
	return 1;
			
	}
Mat draw_droste(Mat img,Rect ROI)
{
	if(img.rows<10)
	{
		Mat droste_end(10,10,CV_8UC3,Scalar(120,120,120));
		return droste_end;//stop when the sub-img is too small; prevent infinite recursion	
	}
	Mat img2;
	resize(img,img2,Size(ROI.width,ROI.height));
	int x_new=(ROI.x*img2.cols)/img.cols;
	int y_new=(ROI.y*img2.rows)/img.rows;
	int h_new=(ROI.height*img2.rows)/img.rows;
	int w_new=(ROI.width*img2.cols)/img.cols;
	Rect ROI2=Rect(x_new,y_new,w_new,h_new);//for next call
	try
	{
		addWeighted(img(ROI),0,draw_droste(img2,ROI2),1,0,img(ROI));//recursive call here
		/*The try-catch block is the only thing that keeps the code running, as the droste_end 
		may not be the same size as the actual smallest image*/
	
	}
	catch(exception e)
	{
		cout<<"some Exception\n";
	}	
	return img;

}

/*Note: The return type of a recursive function should be the same as that of the primary object it is modifying*/
