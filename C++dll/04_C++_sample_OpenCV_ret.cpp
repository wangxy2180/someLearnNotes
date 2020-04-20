#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
using namespace std;
using namespace cv;


//这个中的返回值不能用，只是懒得改了，能用返回值得请见04_C++_OpenCV_RET
extern "C" {
	__declspec(dllexport)
		char* res_test()
	{
		char a[20] = "hello";
		return a ;
		//return "nihaoa" ;
	}


	__declspec(dllexport)
		void welcome() {
		cout << "welcome" << endl;
	}

	__declspec(dllexport)
		void print_int(int a)
	{
		printf("input is %d\n", a);
	}

	__declspec(dllexport)
		uchar* img_to_gray_ret()
	{
		Mat a;
		a = imread("imgTest.jpg");
		cout << a.type() << endl;
		printf("ret\n");

		uchar *dst=NULL;
		cout << "22" << endl;
		memcpy(dst, a.data, 660 * 524 * 3);
		cout << "11" << endl;
		Mat aa(524, 660,CV_8UC4, dst);
		imshow("ccc", aa);
		cout << "123" << endl;
		waitKey(2000);

		return dst;
	}

	__declspec(dllexport)
		void img_to_gray()
	{
		Mat img_src;
		img_src = imread("imgTest.jpg");
		imshow("src", img_src);

		time_t start_time, end_time, interval_time;

		start_time = clock();
		if (waitKey(2000) == 'q')return ;
		end_time = clock();
		interval_time = end_time - start_time;
		cout << "interval time is :" << interval_time << "ms" << endl;

		Mat img_gray;
		cvtColor(img_src, img_gray, COLOR_RGB2GRAY);
		imshow("gray", img_gray);
		start_time = clock();
		if (waitKey(2000) == 'q')return ;
		end_time = clock();
		interval_time = end_time - start_time;
		cout << "interval time 2 is :" << interval_time << "ms" << endl;
		return ;
	}

}