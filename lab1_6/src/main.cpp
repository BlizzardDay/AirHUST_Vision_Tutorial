#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

bool cmp(const Point2i a, const Point2i b) {
	if (a.x == b.x) {
		return a.y < b.y;
	}
	return a.x < b.x;
}

double dis(Point2i a, Point2i b) {
	return sqrt((double)((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

void perspective(Rect rec, vector<Point2i> card, Mat origin, Mat &result) {
	vector<Point2f> dstcard = { Point2f(0, 0), Point2f(0, 140), Point2f(440, 0), Point2f(440, 140) }, srccard;
	for (int i = 0; i < card.size(); ++i) {
		srccard.push_back(Point2f(card[i].x - rec.x, card[i].y - rec.y));
	}
	Mat trans = getPerspectiveTransform(srccard, dstcard);
	warpPerspective(origin(rec), result, trans, Size(440, 140));
	return;
}

int main() {
	Mat img = imread("../data/plate3.png"), binary, clone = img.clone(), result;
	cvtColor(img, img, COLOR_BGR2HSV);
	inRange(img, Scalar(90, 80, 70), Scalar(150, 255, 255), binary);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(binary, binary, MORPH_CLOSE, kernel);
	vector<vector<Point2i>> contours;
	findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); ++i) {
		if (contourArea(contours[i]) < 1000) {
			continue;
		}
		vector<Point2i> approx;
		approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.04, true);
		if (approx.size() == 4) {
			sort(approx.begin(), approx.end(), cmp);
			if (approx[0].y > approx[1].y) {
				swap(approx[0], approx[1]);
			}
			if (approx[2].y > approx[3].y) {
				swap(approx[2], approx[3]);
			}
			double len1 = dis(approx[0], approx[1]), len2 = dis(approx[2], approx[3]);
			double len3 = dis(approx[0], approx[2]), len4 = dis(approx[1], approx[3]);
			Rect rec = boundingRect(contours[i]);
			if (abs(len3 / len1 - 3.5) < 1 && abs(len4 / len1 - 3.5) < 1
			&& abs(len3 / len2 - 3.5) < 1 && abs(len4 / len2 - 3.5) < 1) {
				//drawContours(clone, contours, i, Scalar(0, 255, 0), 3);
				perspective(rec, approx, clone, result);
			}
		}
	}
	//imwrite("../Output/plate1/binary.jpg", binary);
	//imwrite("../Output/plate1/clone.jpg", clone);
	imwrite("../Output/plate3/result.jpg", result);
	waitKey(0);
	return 0;
}

//#include <opencv2/opencv.hpp>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//Mat img, hsv_img;
//
//// 鼠标回调函数：点击像素显示HSV值
//void onMouseClick(int event, int x, int y, int flags, void* userdata) {
//    if (event == EVENT_LBUTTONDOWN) { // 左键点击
//        // 获取点击位置的HSV值
//        Vec3b hsv = hsv_img.at<Vec3b>(y, x); // (行,列)对应(y,x)
//        int H = (int)hsv[0];
//        int S = (int)hsv[1];
//        int V = (int)hsv[2];
//
//        cout << "点击位置(" << x << "," << y << ")的HSV值：" << endl;
//        cout << "H=" << H << ", S=" << S << ", V=" << V << endl;
//        // 推荐的阈值范围（以该像素为中心，H±10，S±50，V±50）
//        cout << "推荐阈值范围：" << endl;
//        cout << "下限：Scalar(" << max(0, H - 10) << ", " << max(0, S - 50) << ", " << max(0, V - 50) << ")" << endl;
//        cout << "上限：Scalar(" << min(179, H + 10) << ", " << min(255, S + 50) << ", " << min(255, V + 50) << ")" << endl;
//    }
//}
//
//int main() {
//    string img_path = "../data/plate3.png";
//    img = imread(img_path);
//    if (img.empty()) {
//        cerr << "错误：无法读取图片！" << endl;
//        return -1;
//    }
//    cvtColor(img, hsv_img, COLOR_BGR2HSV);
//
//    namedWindow("点击图片获取HSV值", WINDOW_NORMAL);
//    setMouseCallback("点击图片获取HSV值", onMouseClick);
//    imshow("点击图片获取HSV值", img);
//
//    waitKey(0);
//    destroyAllWindows();
//    return 0;
//}
