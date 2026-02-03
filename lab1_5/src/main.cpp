#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
	Mat img = imread("../data/chessboard.png");
	Mat img_clone = img.clone();
	Point2f corners[4] = { Point2f(0, 0), Point2f(200, 0), Point2f(0, 200), Point2f(200, 200) };
	Point2f new_corners[4] = { Point2f(0, 0), Point2f(150, 0), Point2f(0, 100), Point2f(150, 100) };
	Mat perspective_Mat = getPerspectiveTransform(corners, new_corners);
	Mat perspective_result;
	warpPerspective(img(Rect(90, 110, 200, 200)), perspective_result, perspective_Mat, Size(150, 100));
	imwrite("../Output/perspective_result.jpg", perspective_result);
	Mat inverse_Mat = perspective_Mat.inv();
	Mat inverse_result;
	warpPerspective(perspective_result, inverse_result, inverse_Mat, Size(200, 200));
	imwrite("../Output/inverse_result.jpg", inverse_result);
	waitKey(0);
	return 0;
}