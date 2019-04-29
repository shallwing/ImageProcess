#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;

int main()
{

	Mat image = imread("t1.jpg");
	namedWindow("1");
	imshow("1", image);

	Mat logo = imread("t2.jpg");
	namedWindow("2");
	imshow("2", logo);

	Mat srcImage4 = imread("t1.jpg");
	Mat logoImage = imread("t2.jpg");


	  
	Mat imageROI;//定义一个Mat类型并给其设定ROI区域
	//方法一  
	imageROI = srcImage4(Rect(200, 250, logoImage.cols, logoImage.rows));//列，行

	
	addWeighted(imageROI, 0.1, logoImage, 0.5, 0.0, imageROI);
	//将logo加到原图上  ，利用线性混合构建掩膜，其中logo权重是0.1，原图中的ROI区域图像是0.5

	 
	namedWindow("3");//显示结果 
	imshow("3", srcImage4);

	waitKey();
	return 0;


}