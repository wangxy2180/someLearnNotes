#include <stdio.h>
#include <Kinect.h>
#include <windows.h>
#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <strstream>
#include <Eigen/Dense>
#include<Eigen/Core> 
#include <iostream>
#include <array>

using namespace cv;
using namespace std;
using namespace Eigen;


//定义hsv空间下的“绿色”的色彩范围
cv::Scalar green_lower = cv::Scalar(35, 83, 86);
cv::Scalar green_upper = cv::Scalar(77, 255, 255);

int main()
{
	//定义传感器指针
	IKinectSensor*          m_pKinectSensor = NULL;

	//定义彩色数据源指针和彩色数据读取指针
	IDepthFrameSource*      pDepthFrameSource = NULL;
	IDepthFrameReader*      m_pDepthFrameReader = NULL;


	//定义深度数据源指针和深度数据读取指针
	IColorFrameSource*      ColorFrameSource = NULL;
	IColorFrameReader*      ColorFrameReader = NULL;


	//定义图片描述指针
	IFrameDescription*      FrameDescription = NULL;

	//获取传感器
	GetDefaultKinectSensor(&m_pKinectSensor);

	//打开传感器
	m_pKinectSensor->Open();

	m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);

	m_pKinectSensor->get_ColorFrameSource(&ColorFrameSource);

	pDepthFrameSource->OpenReader(&m_pDepthFrameReader);

	ColorFrameSource->OpenReader(&ColorFrameReader);

	clock_t start_time;
	int img_count = 1;

	while (1)
	{
		cout << "frame  " << img_count++ << endl;
		start_time = clock();
		// 得到最新的一帧深度数据
		IDepthFrame*          pDepthFrame = NULL;

		// 得到最新的一帧彩色数据
		IColorFrame*          pColorFrame = NULL;

		//深度图读取
		while (pDepthFrame == NULL){
			m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);
		}
		pDepthFrame->get_FrameDescription(&FrameDescription);
		int depth_width, depth_height;

		FrameDescription->get_Width(&depth_width);
		FrameDescription->get_Height(&depth_height);

		Mat DepthImg(depth_height, depth_width, CV_16UC1);
		UINT uDepthPointNum = depth_width * depth_height;

		Mat DepthImg8(depth_height, depth_width, CV_8UC1);

		pDepthFrame->CopyFrameDataToArray(uDepthPointNum, (UINT16 *)DepthImg.data);

		//转换为8位图像,convertTo(目标矩阵）
		DepthImg.convertTo(DepthImg8, CV_8UC1, 255.0 / 4500);



		//彩色图读取
		while (pColorFrame == NULL){
			ColorFrameReader->AcquireLatestFrame(&pColorFrame);
		}
		pColorFrame->get_FrameDescription(&FrameDescription);
		int CWidth, CHeight;

		FrameDescription->get_Width(&CWidth);
		FrameDescription->get_Height(&CHeight);

		cout << CWidth << CHeight << endl;

		Mat ColorImg(CHeight, CWidth, CV_8UC4);
		UINT uColorPointNum = CWidth * CHeight;
		UINT uColorBufferSize = uColorPointNum * 4 * sizeof(unsigned char);

		pColorFrame->CopyConvertedFrameDataToArray(uColorBufferSize, (BYTE *)ColorImg.data, ColorImageFormat_Bgra);

		ICoordinateMapper* pCoordinateMapper = nullptr;
		if (m_pKinectSensor->get_CoordinateMapper(&pCoordinateMapper) != S_OK)
		{
			cerr << "get_CoordinateMapper failed" << endl;
			return -1;
		}

		UINT32 uTableSize = 0;
		PointF* aTable = nullptr;
		auto res = pCoordinateMapper->GetDepthFrameToCameraSpaceTable(&uTableSize, &aTable);
		// Enter main loop
		DepthSpacePoint* pPointArray = new DepthSpacePoint[uColorPointNum];

		pCoordinateMapper->MapColorFrameToDepthSpace(uDepthPointNum, reinterpret_cast<UINT16*>(DepthImg.data), uColorPointNum, pPointArray);

		cv::Mat blurred;
		cv::Mat hsv;
		cv::Mat mask;

		//像素尺寸
		cv::resize(ColorImg, ColorImg, Size(480, 270));

		//高斯滤波去噪
		cv::GaussianBlur(ColorImg, blurred, cv::Size(5, 5), 0);

		//转化到hsv色彩空间
		cv::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);

		//二值化，阈值内白色，其他黑色
		cv::inRange(hsv, green_lower, green_upper, mask);

		// 开运算(open)：先腐蚀后膨胀的过程,平滑较大物体的边界的   同时并不明显改变其面积。
		cv::morphologyEx(mask, mask, cv::MORPH_OPEN, NULL, cv::Point(-1, -1), 2);

		//找轮廓
		vector<vector<cv::Point>> contours;
		cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		cv::Point2f center1;
		cv::Point center1_int;
		float radius1;

		//找面积最大的边界，整个if0.001s，很神奇
		if (contours.size() > 0)
		{
			//此循环计算所有轮廓的面积
			vector<double> contours_area_vec;
			for (int i = 0; i < contours.size(); i++)
			{
				double temp_area;
				temp_area = cv::contourArea(contours[i]);
				contours_area_vec.push_back(temp_area);
			}

			//面积最大的下标
			int max_index = max_element(contours_area_vec.begin(), contours_area_vec.end()) - contours_area_vec.begin();

			//面积最大的轮廓
			vector<cv::Point> max_contour = contours[max_index];

			//面积最大轮廓最小内接圆中心点和半径
			cv::minEnclosingCircle(max_contour, center1, radius1);

			//用来存储center的int类型数值
			center1_int.x = center1.x;
			center1_int.y = center1.y;

			//面积最大边界和圆心
			cv::circle(ColorImg, center1_int, (int)radius1, cv::Scalar(0, 255, 255), 2);
			cv::circle(ColorImg, center1_int, 5, cv::Scalar(0, 0, 255), -1);

			//显示彩色图中圆心坐标
			string text1 = "(x,y) (" + to_string(center1_int.x) + "," + to_string(center1_int.y) + ")";
			cv::putText(ColorImg, text1, cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.7, cv::Scalar(0, 0, 255));
			cout << center1 << endl;
			cout <<"real xy in rgb "<<"center1.x=" << center1_int.x*4 << "   " << "center1.y=" << center1_int.y *4<< endl;
	

		}
		//显示一点深度值
		//因为kinect传入的深度图数据格式是CV_16UC1，因此获得深度数据也用原始的16数据格式

		/*	int z = DepthImg.at<unsigned short>(center1_int.x, center1_int.y);
		cout << z << endl;*/


		int a = 0;
		int b = 0;
		int z = 0;
		//开始对齐
			cout << pPointArray[center1_int.y*4* 1920 + center1_int.x*4].X << " " << pPointArray[center1_int.y*4 * 1920 + center1_int.x*4].Y << "  " << endl;

			a = (int)(pPointArray[center1_int.y *4* 1920 + center1_int.x*4].X);
			b = (int)(pPointArray[center1_int.y *4* 1920 + center1_int.x*4].Y);
			cout << a << b << endl;


			if (a >= 0 && a < DepthImg.cols && b >= 0 && b < DepthImg.rows)
			{
				z = DepthImg.at<unsigned short>(cv::Point(a, b));
				cout << z << endl;
			}
			else z = 0;

		Matrix3d k;

		//1920*1080原尺寸下的k矩阵
		// 对应rgb图尺寸缩放多少，就给内参缩放多少，（0,0）（0,2）对应x宽缩放，（1,1）（1,2）对应y高缩放
		/*k << 1054.1, 0, 960,
			0, 1053.7, 540,
			0, 0, 1;
*/
		k << 263.525, 0, 240,
			0, 263.425, 135,
			0, 0, 1;

		Eigen::MatrixXd uv(3, 1);
		uv(0, 0) = z*center1.x;
		uv(1, 0) = z*center1.y;
		uv(2, 0) = z;

		Eigen::MatrixXd coord(3, 1);
		coord = k.inverse()*uv;
		cout << "相对于深度传感器(三个小红点的中心位置）的xyz（毫米为单位）"<<endl;
		cout << coord << endl;

		//显示图像
		cv::imshow("depthImg", DepthImg8);
		cv::imshow("ColorImg", ColorImg);
		delete pPointArray;
		DepthImg.release();
		DepthImg8.release();
		ColorImg.release();

		pDepthFrame->Release();
		pColorFrame->Release();

		waitKey(1);

		cout << "整体时间为" << (double)(clock() - start_time) / CLOCKS_PER_SEC << "s" << endl;
		cout << "--------------------------------------" << endl;
	}
}