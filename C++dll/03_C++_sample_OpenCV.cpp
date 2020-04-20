#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
using namespace std;
using namespace cv;


//这个中的返回值不能用，只是懒得改了，能用返回值得请见04_C++_OpenCV_RET
extern "C" {

	__declspec(dllexport)
		void welcome() {
		cout << "welcome" << endl;
	}

	__declspec(dllexport)
		void img_to_gray()
	{
		Mat img_src;
		img_src = imread("imgTest.jpg");
		imshow("src", img_src);

		time_t start_time, end_time, interval_time;

		start_time = clock();
		if (waitKey(2000) == 'q')return;
		end_time = clock();
		interval_time = end_time - start_time;
		cout << "interval time is :" << interval_time << "ms" << endl;

		Mat img_gray;
		cvtColor(img_src, img_gray, COLOR_RGB2GRAY);
		imshow("gray", img_gray);
		start_time = clock();
		if (waitKey(2000) == 'q')return;
		end_time = clock();
		interval_time = end_time - start_time;
		cout << "interval time 2 is :" << interval_time << "ms" << endl;
		return;
	}

}