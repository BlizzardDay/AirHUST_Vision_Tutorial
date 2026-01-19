#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
	Mat img = imread("E:/Git/AirHUST_Vision_Tutorial/data/processed_lena.jpg");
	if (img.empty()) {
		std::cout << "Could not read the image!" << std::endl;
		return 0;
	}
	for (int length = 20; length >= 5; length /= 2) {
		for (int i = 0; i < img.rows; i += length) {
			for (int j = 0; j < img.cols; j += length) {
				int width = (img.rows - i) < length ? (img.rows - i) : length;
				int height = (img.cols - j) < length ? (img.cols - j) : length;
				Mat roi = img(Rect(i, j, width, height));
				Mat channel[3];
				split(roi, channel);
				double aver[3];
				aver[0] = mean(roi)[0]; //B
				aver[1] = mean(roi)[1]; //G
				aver[2] = mean(roi)[2]; //R
				if (aver[2] < aver[0]) {
					swap(aver[2], aver[0]);
					swap(channel[2], channel[0]);
				}
				if (aver[0] < aver[1]) {
					swap(aver[0], aver[1]);
					swap(channel[0], channel[1]);
				}
				if (aver[2] < aver[0]) {
					swap(aver[2], aver[0]);
					swap(channel[2], channel[0]);
				}
				merge(channel, 3, roi);
			}
		}
	}
	imshow("Image", img);
	imwrite("Output.jpg", img);
	waitKey(0);
	return 0;
}