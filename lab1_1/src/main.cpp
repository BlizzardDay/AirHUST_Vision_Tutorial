#include <opencv2/opencv.hpp>

int main() {
	cv::Mat image = cv::imread("E:/Git/AirHUST_Vision_Tutorial/data/hesiqi.png");
	if (!image.data) {
		std::cout << "Could not open or find the image!" << std::endl;
		return 0;
	}
	cv::imshow("Image", image);
	cv::imwrite("Output.png", image);
	cv::waitKey(0);
	return 0;
}