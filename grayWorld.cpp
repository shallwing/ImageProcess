#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("D:\\shantang.jpg");
	if(!image.data)
	{
		cout<<"There is an exception in imread\n";
		system("pause");
		return -1;
	}

	imshow("Original", image);
	vector<Mat> imageRGB;

	split(image, imageRGB);

	double R,B,G;
	B = mean(imageRGB[0])[0];
	R = mean(imageRGB[1])[0];
	G = mean(imageRGB[2])[0];

	double kr, kg, kb;
	kb = (R+B+G)/(3*B);
	kg = (R+B+G)/(3*G);
	kr = (R+B+G)/(3*R);

	imageRGB[0] = imageRGB[0]*kb;
	imageRGB[1] = imageRGB[1]*kg;
	imageRGB[2] = imageRGB[2]*kr;

	merge(imageRGB, image);
	imshow("After_balance", image);
	waitKey(10000000);
	
	return 0;
}