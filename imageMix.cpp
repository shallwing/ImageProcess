#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;
using namespace std;

int main()
{
	Mat back = imread("D:\\t1.jpg");
	if(!back.data)
	{
		cout<<"There is an exception in background\n";
		return -1;
	}
	Mat logo = imread("D:\\t2.jpg");
	if(!logo.data)
	{
		cout<<"There is an exception in logo\n";
		return -1;
	}

	Mat ROI = back( Rect(200, 250, logo.cols, logo.rows) );
	Mat mask = imread("D:\\t2.jpg", 0);

	logo.copyTo(ROI, mask);

	namedWindow("This is a test example");
	imshow("This is a test example", back);

	waitKey(1000000000);
	return 0;
}