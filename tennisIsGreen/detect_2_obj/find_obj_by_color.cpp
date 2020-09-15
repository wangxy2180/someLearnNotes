#include"find_obj_by_color.h"
/*@ brief根据轮廓计算面积
@param	contours 包含轮廓的vector
@return area	 包含所有轮廓面积的vector
*/
vector<double> calc_area_by_contours(vector<vector<cv::Point>> contours)
{
	if (!contours.empty())
	{
		vector<double> area;
		for (int i = 0; i < contours.size(); i++)
		{
			//这句可以优化成一句
			double temp_area;
			temp_area = cv::contourArea(contours[i]);
			area.push_back(temp_area);
		}
		return area;
	}
}

/*@brief 图像预处理
@param	*frame		 等待预处理的帧，直接对其进行修改
@param	color_lower  hsv的下界
@param	color_upper	 hsv的上界
@return mask		 二值图，在颜色范围内为1，否则0
*/
cv::Mat pre_proc(cv::Mat frame, cv::Scalar color_lower, cv::Scalar color_upper, bool isRed,
	cv::Scalar red_lower, cv::Scalar red_upper)
{
	cv::Mat blurred;
	cv::Mat hsv;
	cv::Mat mask;
	

	//把高斯核尺寸从11改为5，大概能节省0.004s，反正也能用。
	cv::GaussianBlur(frame, blurred, cv::Size(11, 11), 0);
	//cv::GaussianBlur(frame, blurred, cv::Size(5, 5),0);

	cv::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);
	//二值化，阈值内白色，其他黑色
	cv::inRange(hsv, color_lower, color_upper, mask);

	if (isRed)
	{//有种奇技淫巧，把BGR当作RGB去处理，然后BR通道就互换，就可以用蓝色去识别
		cv::Mat mask_red_180;
		cv::inRange(hsv, red_lower, red_upper, mask_red_180);
		cv::addWeighted(mask,1.0,mask,1.0,0.0,mask);
	}

	//经过试验，二者时间差不多，0.001s左右
	//事实上，不做这一步，结果好像也没啥差别，原文用的None，不知道为啥这样做
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	//cv::erode(mask, mask, element, cv::Point(-1, -1), 2);
	//cv::dilate(mask, mask, element, cv::Point(-1, -1), 2);
	cv::morphologyEx(mask, mask, cv::MORPH_OPEN, element, cv::Point(-1, -1), 2);

	return mask;
}

/*@brief 找到最大面积，并从轮廓与面积两个vector中删除
@param	*contours		待处理的边界
@param	*contours_area  待处理的面积
@param	*center			求得的当前最大面积最小包围圆的圆心
@param	*radius			求得的当前最大面积最小包围圆的半径
*/
void find_max_area(vector<vector<cv::Point>> *contours, vector<double> *contours_area,
	cv::Point *center, int *radius)
{
	//总觉得这么写会出问题，但是，就这样吧
	if (!(*contours).empty())
	{
		cv::Point2f cen;
		float rad;
		//找面积最大的轮廓
		int max_index = max_element((*contours_area).begin(), (*contours_area).end()) - (*contours_area).begin();
		vector<cv::Point> max_contour = (*contours)[max_index];

		//阅后即焚，用完即删
		(*contours_area).erase((*contours_area).begin() + max_index);
		(*contours).erase((*contours).begin() + max_index);

		//最小内接圆中心点和半径
		cv::minEnclosingCircle(max_contour, cen, rad);
		*center = cen;
		*radius = rad + 0.5;
	}
}

/*@brief 画出路径
@param	frame		待画的图
@param	center_path 存储圆心路径的队列
@return	frame		画好的图
*/
cv::Mat draw_path(cv::Mat frame, deque<cv::Point> center_path)
{
	//有没有可能换个方法，不管画几个路径，都只调用一次
	for (int i = 1; i < center_path.size(); i++)
	{
		if (center_path.empty())continue;
		int tickness = sqrt(PATH_LENGTH / (i + 1.0)*2.5);
		cv::line(frame, center_path[i - 1], center_path[i], cv::Scalar(0, 255, 255), tickness);
	}
	return frame;
}
