#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace cv;

//int main() {
//	Mat image(1440, 1920, CV_8UC3);
//	std::ifstream text_file("E:/Git/AirHUST_Vision_Tutorial/data/pic1.txt");
//	int s;
//	std::string line;
//	std::getline(text_file, line);
//	for (int i = 0; i < image.rows; i++) {
//		for (int j = 0; j < image.cols; j++) {
//			for (int c = 0; c < 3; c++) {
//				text_file >> s;
//				text_file.ignore();
//				image.at<Vec3b>(i, j)[c] = s;
//			}
//		}
//	}
//	imshow("Image", image);
//	imwrite("Output1.png", image);
//	waitKey(0);
//	return 0;
//}

//int main() {
//	Mat image(161, 108, CV_8UC1);
//	std::ifstream text_file("E:/Git/AirHUST_Vision_Tutorial/data/pic2.txt");
//	int s;
//	std::string line;
//	std::getline(text_file, line);
//	for (int i = 0; i < image.rows; i++) {
//		for(int j = 0; j < image.cols; j++) {
//			for (int c = 0; c < 1; c++) {
//				text_file >> s;
//				text_file.ignore();
//				image.at<uchar>(i, j) = (s == 1 ? 255 : 0);
//			}
//		}
//	}
//	imshow("Image", image);
//	imwrite("Output2.png", image);
//	waitKey(0);
//	return 0;
//}

int main() {
	Mat image(1202, 776, CV_8UC1);
	std::ifstream text_file("E:/Git/AirHUST_Vision_Tutorial/data/pic3.txt");
	int s;
	std::string line;
	std::getline(text_file, line);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			for (int c = 0; c < 1; c++) {
				text_file >> s;
				text_file.ignore();
				image.at<uchar>(i, j) = s;
			}
		}
	}
	imshow("Image", image);
	imwrite("Output3.png", image);
	waitKey(0);
	return 0;
}