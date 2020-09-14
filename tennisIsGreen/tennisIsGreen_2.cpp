//#pragma comment(lib,"opencv_world411d.lib")
#pragma comment(lib,"opencv_world411.lib")

#include<iostream>
#include<string>
#include<deque>
#include<math.h>
#include<ctime>
#include<opencv2/opencv.hpp>
//定义轨迹长度为32
#define PATH_LENGTH 32

using namespace std;

//定义hsv空间下的“绿色”的色彩范围
cv::Scalar green_lower = cv::Scalar(29, 86, 6);
cv::Scalar green_upper = cv::Scalar(64, 255, 255);

int main()
{
	cv::VideoCapture capture(0);

	if (!capture.isOpened())
		return -1;

	//中心点的路径队列
	deque<cv::Point> center_path;

	//定义在外面，虽然几乎没有效果
	//分别是做高斯模糊，色彩空间转换，腐蚀膨胀操作的?可不可以改成开操作？
	cv::Mat blurred;
	cv::Mat hsv;
	cv::Mat mask;

	clock_t start_time;
	int frame_count = 1;
	while(1)
	{
		cout << "frame  " << frame_count++ << endl;
		start_time = clock();
		//读取与预处理
		cv::Mat frame;
		capture >> frame;//从相机读取新一帧
		
		//从1280*720resize为640*360，能显著提升性能,再到320*180，提升不明显。
		//但是这样需要在坐标转换时多做一步
		cv::resize(frame, frame, cv::Size(640, 360));
		//cv::resize(frame, frame, cv::Size(320, 180));

		//把高斯核尺寸从11改为5，大概能节省0.004s，反正也能用。
		cv::GaussianBlur(frame, blurred, cv::Size(11, 11), 0);
		//cv::GaussianBlur(frame, blurred, cv::Size(5, 5),0);

		cv::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);

		//二值化，阈值内白色，其他黑色
		cv::inRange(hsv, green_lower, green_upper, mask);

		//经过试验，二者时间差不多，0.001s左右
		//事实上，不做这一步，结果好像也没啥差别，原文用的None，不知道为啥这样做
		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
		//cv::erode(mask, mask, element, cv::Point(-1, -1), 2);
		//cv::dilate(mask, mask, element, cv::Point(-1, -1), 2);
		cv::morphologyEx(mask, mask, cv::MORPH_OPEN, element, cv::Point(-1, -1), 2);

		//找轮廓
		vector<vector<cv::Point>> contours;
		cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

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

			//最小内接圆中心点和半径
			cv::Point2f center;
			float radius;
			cv::minEnclosingCircle(max_contour, center, radius);

			//用来存储center的int类型数值
			cv::Point center_int;
			center_int.x = center.x;
			center_int.y = center.y;

			//这个判断要不要删掉：
			if (radius > 10)
			{
				//边界和圆心
				cv::circle(frame, center_int, (int)radius, cv::Scalar(0, 255, 255), 2);
				cv::circle(frame, center_int, 5, cv::Scalar(0, 0, 255), -1);
			}
			string text = "(x,y) (" + to_string(center_int.x) + "," + to_string(center_int.y) + ")";
			cv::putText(frame, text, cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.7, cv::Scalar(0, 0, 255));

			//把圆心加入队列，用来画图
			center_path.push_front(center_int);
			if (center_path.size() > PATH_LENGTH)center_path.pop_back();

		}//end if (contours.size() > 0)

		//画轨迹
		for (int i = 1; i < center_path.size(); i++)
		{
			if (center_path.empty())continue;

			//cout << "111" << endl;
			int tickness = sqrt(PATH_LENGTH / (i + 1.0)*2.5);
			//cout << tickness << endl;
			cv::line(frame, center_path[i - 1], center_path[i], cv::Scalar(0, 255, 255), tickness);
		}

		cv::imshow("tennisIsGreen 任意键退出", frame);
		if (cv::waitKey(1) >= 0)break;

		cout << "整体时间为" << (double)(clock() - start_time) / CLOCKS_PER_SEC << "s" << endl;
		cout << "--------------------------------------" << endl;
	}
}
