/*Please copy the file 'shantang.jpg' to D disk before running the program*/
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("D:\\shantang.jpg");
	if(!img.data)
	{
		cout<<"Fail to read the image file 'img'"<<endl;
		system("pause");
		return -1;
	}
	namedWindow("Shantang");
	imshow("Shantang",img);
	waitKey(1000000);
	return 0;
}