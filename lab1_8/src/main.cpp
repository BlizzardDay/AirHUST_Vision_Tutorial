#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

bool LetterInWord(Rect letter_roi, Rect word_roi) {
	if (letter_roi.x > word_roi.x &&
		letter_roi.y > word_roi.y &&
		letter_roi.x + letter_roi.width < word_roi.x + word_roi.width &&
		letter_roi.y + letter_roi.height < word_roi.y + word_roi.height) {
		return true;
	}
	return false;
}

int main() {
	Mat img = imread("../data/binary_img.png"), img_dilate, clone = img.clone();

	Mat word_morphology;
	Rect word_roi;
	cvtColor(img, img, COLOR_BGR2GRAY);
	Mat kernel_rect = getStructuringElement(MORPH_RECT, Size(5, 5));
	dilate(img, img_dilate, kernel_rect, Point(-1, -1), 3);
	vector<vector<Point2i>> contours;
	findContours(img_dilate, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
		if (contourArea(contours[i]) > 16000) {
			word_roi = boundingRect(contours[i]);
			word_morphology = Mat(clone, word_roi).clone();
			break;
		}
	}

	int id = 0;
	float pos[15];
	Mat letters[15];
	vector<vector<Point2i>> contours_word;
	findContours(img, contours_word, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours_word.size(); i++) {
		Rect letter_roi = boundingRect(contours_word[i]);
		if (LetterInWord(letter_roi, word_roi)) {
			pos[id] = letter_roi.x;
			letters[id++] = Mat(clone, letter_roi).clone();
		}
	}

	for (int i = 0; i < id - 1; i++) {
		for (int j = i + 1; j < id; j++) {
			if (pos[j] < pos[i]) {
				swap(pos[i], pos[j]);
				swap(letters[i], letters[j]);
			}
		}
	}

	for (int i = 0; i < id; i++) {
		string letter_path = "../Output/letter_" + to_string(i) + ".png";
		imwrite(letter_path, letters[i]);
	}

	imwrite("../Output/word_morphology.png", word_morphology);
	return 0;
}