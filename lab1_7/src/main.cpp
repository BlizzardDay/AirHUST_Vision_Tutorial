#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
	Mat background = imread("../data/background.jpg"), logo = imread("../data/logo.jpg"), pic = imread("../data/pic.avif");

	Mat logo_mask = Mat::zeros(logo.size(), CV_8UC1), logo_mask_inv;
	rectangle(logo_mask, Rect(logo.cols / 4, logo.rows / 4, logo.cols / 2, logo.rows / 2), Scalar(255), -1);
	cvtColor(logo, logo, COLOR_RGB2GRAY);
	threshold(logo, logo, 127, 255, THRESH_BINARY);
	bitwise_not(logo_mask, logo_mask_inv);
	bitwise_and(logo, logo_mask, logo);
	imwrite("../Output/mask.jpg", logo_mask);
	imwrite("../Output/mask_inv.jpg", logo_mask_inv);
	imwrite("../Output/logo.jpg", logo);

	Mat pic_mask, pic_mask_inv;
	cvtColor(pic, pic, COLOR_RGB2HSV);
	return 0;
}