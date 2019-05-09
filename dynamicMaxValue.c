/*   动态阈值  */

#include<opencv2\opencv.hpp>
#include<vector>
#include<iostream>

using namespace std;
using namespace cv;

int row = 1;
int col = 1;

double baidianave(Mat frame, int irow, int lclo)
{
	int width_step = frame.cols / col;
	int height_step = frame.rows / row;
	int a[256] = { 0 };
	double sum = 0;
	double sum1 = 0;
	double ave;
	for (int rows = irow * height_step; rows < (irow + 1)*height_step; rows++)
	{
		for (int clos = lclo * width_step; clos < (lclo + 1)*width_step; clos++)
		{
			int d = frame.at<uchar>(rows, clos);
			a[d]++;
		}
	}
	int value = 255;
	for (int k = 255; k>0; k--)
	{
		sum1 += a[k];
		if (sum1 > width_step*height_step / 10)
			break;
		value--;
	}
	sum = 0;
	for (int i = value; i < 256; i++)
		sum += a[i] * i;
	ave = sum / sum1;
	return ave;
}
double baidianave(Mat frame, int n)
{
	int a[256] = { 0 };
	double sum = 0;
	double sum1 = 0;
	double ave;
	for (int i = 0; i < n; i++)
	{
		int d = frame.at<double>(0, i);
		a[d]++;
	}
	int value = 255;
	for (int k = 255; k>0; k--)
	{
		sum1 += a[k];
		if (sum1 >(n / 20))
			break;
		value--;
	}
	sum = 0;
	for (int i = value; i < 256; i++)
		sum += a[i] * i;
	ave = sum / sum1;
	//cout << ave << endl;
	return ave;
}
int main()
{
	Mat srcImage = imread("D:\\shantang.jpg");
	if (srcImage.empty())
	{
		printf("Load srcImage failure\n");
		return -1;
	}
	imshow("原图", srcImage);


	vector<Mat>YCrCbImage;
	Mat Yimage(srcImage.size(), CV_8UC1), Crimage(srcImage.size(), CV_8UC1), Cbimage(srcImage.size(), CV_8UC1), Image;
	cvtColor(srcImage, Image, CV_RGB2YCrCb);
	split(Image, YCrCbImage);
	Yimage = YCrCbImage[0];
	Crimage = YCrCbImage[1];
	Cbimage = YCrCbImage[2];
	//计算每个分块的高度和宽度
	int width_step = srcImage.cols / col;
	int height_step = srcImage.rows / row;
	//分别对每一个图像分块进行处理
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			int count = 0;
			//计算每部分Cb，Cr的均值和方差
			double Mb = 0, Db = 0, b = 0;
			double Mr = 0, Dr = 0, r = 0;
			//求取Cb,Cr的平均值
			for (int rows = i * height_step; rows < (i + 1)*height_step; rows++)
			{
				for (int clos = j * width_step; clos < (j + 1)*width_step; clos++)
				{
					Mb += Cbimage.at<uchar>(rows, clos);
					Mr += Crimage.at<uchar>(rows, clos);
					count++;
				}
			}
			Mb /= count;
			Mr /= count;
			for (int rows = i * height_step; rows < (i + 1)*height_step; rows++)
			{
				for (int clos = j * width_step; clos < (j + 1)*width_step; clos++)
				{
					Db += abs(Cbimage.at<uchar>(rows, clos) - Mb);
					Dr += abs(Crimage.at<uchar>(rows, clos) - Mr);
				}
			}
			Db /= count;
			Dr /= count;
			if (Mb < 0)
				b = Mb + (-1)*Db;
			else
				b = Mb + Db;
			if (Mr < 0)
				r = 1.5 * Mr + Dr * (-1);
			else
				r = 1.5 *Mr + Dr;
			double Ymax = baidianave(YCrCbImage[0], i, j);
			Mat Bbaidian(1, 600000, CV_64FC1);
			Mat Gbaidian(1, 600000, CV_64FC1);
			Mat Rbaidian(1, 600000, CV_64FC1);
			int n1 = 0;
			//进行白点选择
			for (int rows = i * height_step; rows < (i + 1)*height_step; rows++)
			{
				for (int clos = j * width_step; clos < (j + 1)*width_step; clos++)
				{
					if ((Cbimage.at<uchar>(rows, clos) - b) < 1.5*Db &&
						(Crimage.at<uchar>(rows, clos) - r) < 1.5*Dr)
					{
						double d1 = srcImage.at<Vec3b>(rows, clos)[0];
						Bbaidian.at<double>(0, n1) = d1;
						double d2 = srcImage.at<Vec3b>(rows, clos)[1];
						Gbaidian.at<double>(0, n1) = d2;
						double d3 = srcImage.at<Vec3b>(rows, clos)[2];
						Rbaidian.at<double>(0, n1) = d3;
						n1++;
						/*if (i == 0 && j == 0)
						cout << d1 << "   "<<d2<<"   "<<d3<<endl;*/
					}
				}
			}
			double Bave = baidianave(Bbaidian, n1);
			double Gave = baidianave(Gbaidian, n1);
			double Rave = baidianave(Rbaidian, n1);
			if (i == 0)
			{
				Bave -= 105;
				Gave -= 53;
			}
			cout << Bave << "  " << Gave << "   " << Rave << endl;
			double Bgain = Ymax / (Bave);
			double Ggain = Ymax / (Gave);
			double Rgain = Ymax / (Rave);

			//cout << i << endl;
			for (int rows = i * height_step; rows < (i + 1)*height_step; rows++)
			{
				for (int clos = j * width_step; clos < (j + 1)*width_step; clos++)
				{
					srcImage.at<Vec3b>(rows, clos)[0] = saturate_cast<uchar>(srcImage.at<Vec3b>(rows, clos)[0] * Bgain);
					srcImage.at<Vec3b>(rows, clos)[1] = saturate_cast<uchar>(srcImage.at<Vec3b>(rows, clos)[1] * Ggain);
					srcImage.at<Vec3b>(rows, clos)[2] = saturate_cast<uchar>(srcImage.at<Vec3b>(rows, clos)[2] * Rgain);
				}
			}
		}
	}
	//cout << srcImage.at<Vec3b>(10, 300) << endl;
	imshow("动态阈值白平衡后", srcImage);
	imwrite("动态阈值白平衡后.jpg",srcImage);
	waitKey(0);
	return 0;
}