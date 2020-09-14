//#pragma comment(lib,"opencv_world411d.lib")
////#pragma comment(lib,"opencv_world411.lib")

#include<iostream>
#include<string>
#include<deque>
#include<math.h>
#include<ctime>
#include"find_obj_by_color.h"

using namespace std;

//定义hsv空间下的“tennis”的色彩范围
cv::Scalar tennis_lower = cv::Scalar(29, 86, 6);
cv::Scalar tennis_upper = cv::Scalar(64, 255, 255);
//这个绿网球就不行了，但是纸巾和积木可以
cv::Scalar green_lower = cv::Scalar(35, 43, 46);
cv::Scalar green_upper = cv::Scalar(77, 255, 255);
////积木红 需修改
//cv::Scalar red_lower = cv::Scalar(35, 43, 46);
//cv::Scalar red_upper = cv::Scalar(77, 255, 255);
//积木蓝
cv::Scalar blue_lower = cv::Scalar(100, 43, 46);
cv::Scalar blue_upper = cv::Scalar(124, 255, 255);



int main()
{
	cv::VideoCapture capture(0);
	if (!capture.isOpened())
		return -1;

	//中心点的路径队列
	deque<cv::Point> center_path;

	clock_t start_time;
	int frame_count = 1;
	while(1)
	{
		cout << "frame  " << frame_count++ << endl;
		start_time = clock();

		cv::Mat frame;
		capture >> frame;//从相机读取新一帧

		cv::Mat mask;
		mask= pre_proc(&frame, green_lower, green_upper);
		//找轮廓
		vector<vector<cv::Point>> contours;
		cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		if (!contours.empty())
		{
			//计算所有轮廓的面积
			vector<double> contours_area_vec = calc_area_by_contours(contours);

			//计算面积最大轮廓的圆心和半径
			cv::Point center;
			int radius = 0;
			find_max_area(&contours, &contours_area_vec, &center, &radius);

			//再计算一个
			cv::Point center_2;
			int radius_2 = 0;
			find_max_area(&contours, &contours_area_vec, &center_2, &radius_2);

			cout << "obj1 " << center.x << " " << center.y << " " << radius << endl
				<< "obj2 " << center_2.x << " " << center_2.y << " " << radius_2 << endl;

			if (radius > 1)
			{
				//边界和圆心
				cv::circle(frame, center, (int)radius, cv::Scalar(0, 255, 255), 2);
				cv::circle(frame, center, 5, cv::Scalar(0, 0, 255), -1);
			}
			//第二个
			if (radius_2 > 1)
			{
				cv::circle(frame, center_2, (int)radius_2, cv::Scalar(0, 255, 255), 2);
				cv::circle(frame, center_2, 5, cv::Scalar(0, 0, 255), -1);
			}

			string text1 = "(x1,y1) (" + to_string(center.x) + "," + to_string(center.y) + ")";
			cv::putText(frame, text1, cv::Point(10, 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.7, cv::Scalar(0, 0, 255));
			string text2 = "(x2,y2) (" + to_string(center_2.x) + "," + to_string(center_2.y) + ")";
			cv::putText(frame, text2, cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.7, cv::Scalar(0, 0, 255));

			//把圆心加入队列，用来画图
			center_path.push_front(center);
			if (center_path.size() > PATH_LENGTH)center_path.pop_back();
		}//end if (!contours.empty())
		
		frame = draw_path(frame, center_path);

		cv::imshow("tennisIsGreen 任意键退出", frame);
		if (cv::waitKey(1) >= 0)break;

		cout << "整体时间为" << (double)(clock() - start_time) / CLOCKS_PER_SEC << "s" << endl;
		cout << "--------------------------------------" << endl;
	}
}
