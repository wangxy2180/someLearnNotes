//#pragma comment(lib,"opencv_world411d.lib")
#pragma comment(lib,"opencv_world411.lib")

#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<deque>
#include<math.h>
#include<ctime>
//定义轨迹长度为32
#define PATH_LENGTH 32

using namespace std;

//定义hsv空间下的“绿色”的色彩范围
cv::Scalar green_lower = cv::Scalar(29, 86, 6);
cv::Scalar green_upper = cv::Scalar(64, 255, 255);

//似乎没用上
typedef struct{
	int x;
	int y;
}posXY;

clock_t start_time, end_time, whole_time;

int main()
{
	int path_counter = 0;
	int dX = 0;
	int dY = 0;
	string direction = "";
	//cout << green_higher[1] << endl;
	cv::VideoCapture capture(0);

	if (!capture.isOpened())
		return -1;

	deque<cv::Point> center_path;

	//定义在外面，虽然几乎没有效果
	//分别是做高斯模糊，色彩空间转换，腐蚀膨胀操作的?可不可以改成开操作？
	cv::Mat blurred;
	cv::Mat hsv;
	cv::Mat mask;
		
	int boom_count = 0;
	while (1)
	{
		cout << "boom  " << boom_count++ << endl;
		start_time = clock();
		whole_time = clock();
		//读取与预处理
		clock_t read_time = clock();
		cv::Mat frame;
		capture >> frame;//从相机读取新一帧
		//从1280*720resize为640*360，能显著提升性能，但是会
		clock_t resize_time = clock();
		cv::resize(frame, frame, cv::Size(640, 360));
		//cv::resize(frame, frame, cv::Size(320, 180));
		cout << "frame的尺寸是" << frame.rows << "*" << frame.cols << "*" << frame.channels() << endl;
		cout << "*读取和resize时间为" << (double)(clock() - read_time) / CLOCKS_PER_SEC << "s" << endl;
		//cv::Mat blurred;
		clock_t pre_proc = clock();
		//把高斯核尺寸从11改为5，能节省0.045s
		cv::GaussianBlur(frame, blurred, cv::Size(11, 11),0);
		//cv::GaussianBlur(frame, blurred, cv::Size(5, 5),0);
		cout << "*resize+blur time" << (double)(clock() - resize_time) / CLOCKS_PER_SEC << "s" << endl;
		cout << "*blur time" << (double)(clock() - pre_proc) / CLOCKS_PER_SEC << "s" << endl;
		cout << "**单纯的模糊时间为" << (double)(clock() - pre_proc) / CLOCKS_PER_SEC << "s" << endl;

		cv::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);
		cout << "*模糊与hsv时间为" << (double)(clock() - pre_proc) / CLOCKS_PER_SEC << "s" << endl;

		clock_t mask_time = clock();
		cv::inRange(hsv, green_lower, green_upper, mask);
		//经过试验，二者时间差不多
		//cv::erode(mask, mask, NULL, cv::Point(-1, -1), 2);
		//cv::dilate(mask, mask, NULL, cv::Point(-1, -1), 2);
		cv::morphologyEx(mask, mask, cv::MORPH_OPEN, NULL, cv::Point(-1, -1), 2);
		end_time = clock();
		cout << "*mask阶段的时间是" << (double)(end_time - mask_time) / CLOCKS_PER_SEC << "s" << endl;
		cout << "读取与预处理阶段的时间是" << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
		
		start_time = clock();
		//找轮廓
		vector<vector<cv::Point>> contours;
		cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		end_time = clock();
		cout << "找轮廓时间为" << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;

		cv::Point center_int;

		start_time = clock();
		//找面积最大的边界
		if (contours.size()>0)
		{
			//找面积最大的边界，好怀念Python
			//计算所有轮廓的面积
			clock_t max_time = clock();
			vector<double> contours_area_vec;
			for (int i = 0; i < contours.size(); i++)
			{
				double temp_area;
				temp_area = cv::contourArea(contours[i]);
				contours_area_vec.push_back(temp_area);
			}
			//面积最大的下标
			int max_index = max_element(contours_area_vec.begin(), contours_area_vec.end())-contours_area_vec.begin();
			//面积最大的轮廓
			vector<cv::Point> max_contour = contours[max_index];
			cout << "*max_time "<< (double)(clock() - max_time) / CLOCKS_PER_SEC<<"s"<<endl;
			cv::Point2f center;
			float radius;
			cv::minEnclosingCircle(max_contour, center, radius);
			
			center_int.x = center.x;
			center_int.y = center.y;
			if (radius > 10)
			{
				//边界和圆心
				cv::circle(frame, center_int, (int)radius,cv::Scalar(0,255,255),2);
				cv::circle(frame, center_int, 5,cv::Scalar(0,0,255),-1);
				string text = "(x,y) (" + to_string(center_int.x) + "," + to_string(center_int.y)+")";
				cv::putText(frame, text, cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.7, cv::Scalar(0, 0, 255));
			}
		}
		end_time = clock();
		cout << "找面积最大边界时间为" << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;

		center_path.push_front(center_int);
		if (center_path.size() > PATH_LENGTH)center_path.pop_back();

		start_time = clock();
		//画轨迹
		for (int i = 1;i<center_path.size();i++)
		{
			if (center_path.empty())continue;

			//cout << "111" << endl;
			int tickness = sqrt(PATH_LENGTH / (i + 1.0)*2.5);
			//cout << tickness << endl;
			cv::line(frame, center_path[i - 1], center_path[i], cv::Scalar(0, 255, 255), tickness);
		}

		cv::imshow("tennisIsGreen 任意键退出", frame);
		end_time = clock();
		cout << "画轨迹时间为" << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
		
		start_time = clock();
		if (cv::waitKey(1)>=0)break;
		end_time = clock();
		cout << "wait时间为" << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
		cout << "整体时间为" << (double)(end_time - whole_time) / CLOCKS_PER_SEC << "s" << endl;
		cout << "--------------------------------------" << endl;

	}
}
