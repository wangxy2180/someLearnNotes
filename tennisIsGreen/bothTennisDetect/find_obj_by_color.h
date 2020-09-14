#pragma once
//#pragma comment(lib,"opencv_world411d.lib")
#pragma comment(lib,"opencv_world411.lib")

#include<vector>
#include<opencv2/opencv.hpp>
//定义轨迹长度为32
#define PATH_LENGTH 32


using namespace std;

/*@ brief根据轮廓计算面积
@param	contours 包含轮廓的vector
@return area	 包含所有轮廓面积的vector
*/
vector<double> calc_area_by_contours(vector<vector<cv::Point>> contours);

/*@brief 图像预处理
@param	*frame		 等待预处理的帧，直接对其进行修改
@param	color_lower  hsv的下界
@param	color_upper	 hsv的上界
@return mask		 二值图，在颜色范围内为1，否则0
*/
cv::Mat pre_proc(cv::Mat *frame, cv::Scalar color_lower, cv::Scalar color_upper);

/*@brief 找到最大面积，并从轮廓与面积两个vector中删除
@param	*contours		待处理的边界
@param	*contours_area  待处理的面积
@param	*center			求得的当前最大面积最小包围圆的圆心
@param	*radius			求得的当前最大面积最小包围圆的半径
*/
void find_max_area(vector<vector<cv::Point>> *contours, vector<double> *contours_area,
	cv::Point *center, int *radius);

/*@brief 画出路径
@param	frame		待画的图
@param	center_path 存储圆心路径的队列
@return	frame		画好的图
*/
cv::Mat draw_path(cv::Mat frame, deque<cv::Point> center_path);
