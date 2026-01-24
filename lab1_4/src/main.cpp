#include <opencv2/opencv.hpp>

using namespace cv;

Mat Rotate(Mat img, double angle, double scale, double width, double height, double lean);

int main() {
	Mat img = imread("../data/ranjinle.jpg"), pic1, pic2;
	pic1 = Rotate(img, 30.0, 1.0, img.cols, img.rows, 0.0);
	pic2 = Rotate(pic1, -30.0, 1.0, img.cols, img.rows, 30.0);
	imwrite("../Output/pic1.jpg", pic1);
	imwrite("../Output/pic2.jpg", pic2);
	waitKey(0);
	return 0;
}

Mat Rotate(Mat img, double angle, double scale, double width, double height, double lean) {
	Point2f center = Point2f(img.cols / 2.0, img.rows / 2.0);
	Mat change = getRotationMatrix2D(center, angle, scale), dst;
	double alpha = (angle + lean) / 180.0 * CV_PI;
	int newcols = max(width * cos(alpha) + height * sin(alpha), -width * cos(alpha) + height * sin(alpha));
	int newrows = max(height * cos(alpha) - width * sin(alpha), height * cos(alpha) + width * sin(alpha));
	change.at<double>(0, 2) += (newcols - img.cols) / 2.0;
	change.at<double>(1, 2) += (newrows - img.rows) / 2.0;
	warpAffine(img, dst, change, Size(newcols, newrows), INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
	return dst;
}