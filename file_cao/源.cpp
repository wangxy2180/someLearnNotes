#include <array>
#include <iostream>

// OpenCV Header
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// Kinect for Windows SDK Header
#include <Kinect.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	cout << "Try to get default sensor" << endl;
	IKinectSensor* pSensor = nullptr;
	{
		if (GetDefaultKinectSensor(&pSensor) != S_OK)
		{
			cerr << "Get Sensor failed" << endl;
			return -1;
		}

		cout << "Try to open sensor" << endl;
		if (pSensor->Open() != S_OK)
		{
			cerr << "Can't open sensor" << endl;
			return -1;
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	IColorFrameReader* pColorFrameReader = nullptr;
	cv::Mat	imgColor;
	UINT uColorBufferSize = 0;
	UINT uColorPointNum = 0;
	cout << "Try to get color source" << endl;
	{
		// Get frame source
		IColorFrameSource* pFrameSource = nullptr;
		if (pSensor->get_ColorFrameSource(&pFrameSource) != S_OK)
		{
			cerr << "Can't get color frame source" << endl;
			return -1;
		}

		// Get frame description
		cout << "get color frame description" << endl;
		IFrameDescription* pFrameDescription = nullptr;
		int iWidth = 0,
			iHeight = 0;
		if (pFrameSource->get_FrameDescription(&pFrameDescription) == S_OK)
		{
			pFrameDescription->get_Width(&iWidth);
			pFrameDescription->get_Height(&iHeight);
		}
		pFrameDescription->Release();
		pFrameDescription = nullptr;

		// get frame reader
		cout << "Try to get color frame reader" << endl;
		if (pFrameSource->OpenReader(&pColorFrameReader) != S_OK)
		{
			cerr << "Can't get color frame reader" << endl;
			return -1;
		}

		// release Frame source
		cout << "Release frame source" << endl;
		pFrameSource->Release();
		pFrameSource = nullptr;
		//彩色8位4通道
		imgColor = cv::Mat(iHeight, iWidth, CV_8UC4);
		uColorPointNum = iWidth * iHeight;
		uColorBufferSize = uColorPointNum * 4 * sizeof(unsigned char);
	}

	IDepthFrameReader* pDepthFrameReader = nullptr;
	cv::Mat imgDepth;
	UINT uDepthPointNum = 0;
	cout << "Try to get depth source" << endl;
	{
		// Get frame source
		IDepthFrameSource* pFrameSource = nullptr;
		if (pSensor->get_DepthFrameSource(&pFrameSource) != S_OK)
		{
			cerr << "Can't get depth frame source" << endl;
			return -1;
		}

		// Get frame description
		cout << "get depth frame description" << endl;
		IFrameDescription* pFrameDescription = nullptr;
		int iWidth = 0,
			iHeight = 0;
		if (pFrameSource->get_FrameDescription(&pFrameDescription) == S_OK)
		{
			pFrameDescription->get_Width(&iWidth);
			pFrameDescription->get_Height(&iHeight);
		}
		pFrameDescription->Release();
		pFrameDescription = nullptr;
		cout << "深度图宽高" << iWidth << " " << iHeight << endl;
		// get frame reader
		cout << "Try to get depth frame reader" << endl;
		if (pFrameSource->OpenReader(&pDepthFrameReader) != S_OK)
		{
			cerr << "Can't get depth frame reader" << endl;
			return -1;
		}

		// release Frame source
		cout << "Release frame source" << endl;
		pFrameSource->Release();
		pFrameSource = nullptr;
		//深度16位
		imgDepth = cv::Mat(iHeight, iWidth, CV_16UC1);
		uDepthPointNum = iWidth * iHeight;
	}

	ICoordinateMapper* pCoordinateMapper = nullptr;
	if (pSensor->get_CoordinateMapper(&pCoordinateMapper) != S_OK)
	{
		cerr << "get_CoordinateMapper failed" << endl;
		return -1;
	}

	UINT32 uTableSize = 0;
	PointF* aTable = nullptr;
	auto res = pCoordinateMapper->GetDepthFrameToCameraSpaceTable(&uTableSize, &aTable);
	// Enter main loop
	DepthSpacePoint* pPointArray = new DepthSpacePoint[uColorPointNum];
	while (true)
	{
		//获取彩色帧的数据并保存
		IColorFrame* pColorFrame = nullptr;
		if (pColorFrameReader->AcquireLatestFrame(&pColorFrame) == S_OK)
		{
			pColorFrame->CopyConvertedFrameDataToArray(uColorBufferSize, imgColor.data, ColorImageFormat_Bgra);
			pColorFrame->Release();
		}
		//获取深度帧的数据并保存
		IDepthFrame* pDepthFrame = nullptr;
		if (pDepthFrameReader->AcquireLatestFrame(&pDepthFrame) == S_OK)
		{
			pDepthFrame->CopyFrameDataToArray(uDepthPointNum, reinterpret_cast<UINT16*>(imgDepth.data));
			pDepthFrame->Release();
		}





		//开始对齐
		if (pCoordinateMapper->MapColorFrameToDepthSpace(uDepthPointNum, reinterpret_cast<UINT16*>(imgDepth.data), uColorPointNum, pPointArray) == S_OK)
		{
			for (size_t y = 0; y < imgColor.rows; ++y)
				for (size_t x = 0; x < imgColor.cols; ++x)
				{
					const DepthSpacePoint& rPoint = pPointArray[y * imgColor.cols + x];
					if (rPoint.X >= 0 && rPoint.X < imgDepth.cols && rPoint.Y >= 0 && rPoint.Y < imgDepth.rows)
					{
						imgColor.at<cv::Vec4b>(y, x) = /*255 * */imgDepth.at<UINT16>((int)rPoint.Y, (int)rPoint.X) / 8000;
					}
				}	

		//	Mat *img = reinterpret_cast<Mat*>(&imgDepth);

			//用来存储center的int类型数值
			/*cv::Point center1_int;
			center1_int.x = pPointArray[540 * 1920 + 960].X;
			center1_int.y = pPointArray[540 * 1920 + 960].Y;
			cout << center1_int.x <<"......."<< center1_int.y<<endl;*/
		/*	int z = imgDepth.at<unsigned short>(cv::Point(center1_int.x, center1_int.y));
		
			cout << z << endl;*/

			cout << "(960,540)" << pPointArray[540 * 1920 + 960].X << " " << pPointArray[540 * 1920 + 960].Y<<"  "<< endl;
		
	/*		int x = 121;
			int y = 204;*/

			int x = (int)(pPointArray[540 * 1920 + 960].X);
			int y = (int)(pPointArray[540 * 1920 + 960].Y);
			cout << x << y << endl;

			//因为kinect传入的深度图数据格式是CV_16UC1，因此存储和获得深度数据也用16的数据格式
		//	Mat *img = reinterpret_cast<Mat*>(&imgDepth);
			cout << imgDepth.at<unsigned short>(cv::Point(x, y)) << endl;

			cv::imshow("Image", imgColor);

			cv::imshow("depth", imgDepth);
		}

		// check keyboard input
		if (cv::waitKey(30) == VK_ESCAPE) {
			break;
		}
	}
	delete pPointArray;

	// 3b. release frame reader
	cout << "Release frame reader" << endl;
	pColorFrameReader->Release();
	pColorFrameReader = nullptr;
	pDepthFrameReader->Release();
	pDepthFrameReader = nullptr;

	// 1c. Close Sensor
	cout << "close sensor" << endl;
	pSensor->Close();

	// 1d. Release Sensor
	cout << "Release sensor" << endl;
	pSensor->Release();
	pSensor = nullptr;

	return 0;
}

////Color Frame 获取彩色图，为了避免尺寸太大显示不便，用resize把彩色图的宽、高都缩小为原来的一半，存在CV_8UC4的colorMat内
//... ...
//resize(colorBufferMat, colorMat, Size(), 0.5, 0.5);
//
////Depth Frame 获取彩色图，convertTo函数的作用是转换数据类型不同的Mat，这里的意义是把深度数据转化为像素值
//... ...
//depthBufferMat.convertTo(depthMat, CV_8U, -255.0f / 8000.0f, 255.0f);
//
//// Mapping ( Depth to Color ) 配准
//if (SUCCEEDED(hResult))
//{
//	// 深度空间坐标点集
//	vector<DepthSpacePoint> depthSpacePoints(colorWidth *colorHeight);
//
//	// 实际的配准语句，这里就用了MapColorFrameToDepthSpace函数，函数说明在下面
//	hResult = pCoordinateMapper->MapColorFrameToDepthSpace(depthWidth * depthHeight,
//		reinterpret_cast<UINT16*>(depthBufferMat.data), colorWidth *colorHeight,
//		&depthSpacePoints[0]);
//
//	if (SUCCEEDED(hResult))
//	{
//		CoordinateMapperMat = Scalar(0, 0, 0, 0); // 定义为Mat(colorHeight, colorWidth, CV_8UC4)
//		// 遍历彩色图所有的点
//		for (int y = 0; y < colorHeight; y++)
//		{
//			for (int x = 0; x < colorWidth; x++)
//			{
//				// 按照彩色图像素坐标生成深度坐标点集的索引index，根据索引得到对应深度点
//				unsigned int index = y * colorWidth + x;
//				DepthSpacePoint p = depthSpacePoints[index];
//				// 如果对应深度点坐标没有超限，就把对应彩色点赋给新Mat CoordinateMapperMat（？）
//				if (p.X != -std::numeric_limits<float>::infinity() && p.Y != -std::numeric_limits<float>::infinity())
//				{
//					int depthX = static_cast<int>(p.X + 0.2f);
//					int depthY = static_cast<int>(p.Y + 0.2f);
//
//					if ((depthX >= 0) && (depthX < depthWidth) && (depthY >= 0) && (depthY < depthHeight))
//					{
//						CoordinateMapperMat.at<Vec4b>(y, x) = colorBufferMat.at<Vec4b>(y, x);
//					}
//				}
//			}
//		}
//	}
//}
//imshow("depth", depthMat);
//imshow("color", colorMat);
//// CoordinateMapperMat就是要显示的配准结果
//imshow("CoordinateMapper", CoordinateMapperMat);