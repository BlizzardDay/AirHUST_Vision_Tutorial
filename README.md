## lab1_6_simple_threshold

现提供若干车牌图片（包含不同光照条件和拍摄角度），请你截取出其中车牌的部分 (提取相关ROI) 并规格化，最后保存图片。

### 实验要求：

1. **预处理**：读取图像，将其转换为灰度图，并进行适当的高斯模糊以去噪。你可以任意地添加预处理，反正做出来就行
2. **阈值分割**：
   * 尝试使用**固定阈值**（如 `cv2.threshold`）进行二值化。
   * 尝试使用**自适应阈值**（如 `cv2.adaptiveThreshold`）进行二值化。
   * **对比**两种方法的效果，选择能更清晰分离出车牌矩形轮廓的那种。
3. **轮廓查找与筛选**：
   * 查找二值图中的所有轮廓。
   * 通过面积大小、形状（矩形）或长宽比筛选出最可能是车牌的轮廓。
   * 使用多边形拟合（ApproxPolyDP）获取该轮廓的4个顶点。
4. **透视矫正**：
   * 根据拟合出的4个顶点，构建透视变换矩阵。
   * 将车牌区域变换为标准的矩形（建议尺寸：440x140 或比例接近 3.14:1）。
5. **结果输出**：展示二值化过程图，并保存最终矫正后的几张车牌图像。

备注：(可选) 使用形态学操作（如闭运算）连接断裂的边缘，使车牌轮廓更完整。如果**透视矫正**难住你了，你可以选择一个简化版本，即用矩形框出车牌的位置。
如果觉得给出的车牌不够，难度过低，你可任意添加车牌图片。


### 你可能要用到的工具函数原型：

如果你是C++选手：

```cpp
// 颜色空间转换
void cv::cvtColor(cv::InputArray src, cv::OutputArray dst, int code, int dstCn = 0);

// 高斯模糊
void cv::GaussianBlur(cv::InputArray src, cv::OutputArray dst, cv::Size ksize, double sigmaX, double sigmaY = 0, int borderType = BORDER_DEFAULT);

// 固定阈值
double cv::threshold(cv::InputArray src, cv::OutputArray dst, double thresh, double maxval, int type);

// 自适应阈值
void cv::adaptiveThreshold(cv::InputArray src, cv::OutputArray dst, double maxValue, int adaptiveMethod, int thresholdType, int blockSize, double C);

// 颜色范围分割
void cv::inRange(cv::InputArray src, cv::InputArray lowerb, cv::InputArray upperb, cv::OutputArray dst);

// 查找轮廓
void cv::findContours(cv::InputArray image, cv::OutputArrayOfArrays contours, cv::OutputArray hierarchy, int mode, int method, cv::Point offset = cv::Point());

// 多边形拟合（用于将复杂的轮廓简化为四边形）
void cv::approxPolyDP(cv::InputArray curve, cv::OutputArray approxCurve, double epsilon, bool closed);

// 绘制轮廓（用于调试）
void cv::drawContours(cv::InputArray image, cv::InputArrayOfArrays contours, int contourIdx, const cv::Scalar& color, int thickness = 1, int lineType = LINE_8, cv::InputArray hierarchy = cv::noArray(), int maxLevel = INT_MAX, cv::Point offset = cv::Point());

// 轮廓面积
double cv::contourArea(cv::InputArray contour, bool oriented = false);
```

如果你是Python选手：

```python
# 颜色空间转换 (BGR -> GRAY)
cv2.cvtColor(src: MatLike, code: int, ...) -> MatLike

# 高斯模糊
cv2.GaussianBlur(src: MatLike, ksize: tuple[int, int], sigmaX: float, ...) -> MatLike

# 固定阈值 (返回值为: 实际使用的阈值, 二值化图)
cv2.threshold(src: MatLike, thresh: float, maxval: float, type: int) -> tuple[float, MatLike]

# 自适应阈值 (自动处理光照不均)
cv2.adaptiveThreshold(src: MatLike, maxValue: float, adaptiveMethod: int, thresholdType: int, blockSize: int, C: float) -> MatLike

# 颜色范围分割 (通常用于HSV空间)
cv2.inRange(src: MatLike, lowerb: Sequence[int], upperb: Sequence[int]) -> MatLike

# 查找轮廓
cv2.findContours(image: MatLike, mode: int, method: int, ...) -> tuple[Sequence[MatLike], MatLike]

# 多边形拟合 (epsilon通常设为周长的0.02倍左右)
cv2.approxPolyDP(curve: MatLike, epsilon: float, closed: bool) -> MatLike

# 绘制轮廓
cv2.drawContours(image: MatLike, contours: Sequence[MatLike], contourIdx: int, color: Sequence[float], thickness: int = ...) -> MatLike

# 轮廓面积计算
cv2.contourArea(contour: MatLike, oriented: bool = ...) -> float
```

**注意**：

1. 阈值分割前建议先进行高斯模糊去噪，以减少噪声对分割结果的干扰。
2. 形态学开运算可去除小噪点，闭运算可填充区域内部小孔洞，合理搭配使用可显著改善分割质量。
3. 通过 `approxPolyDP` 拟合出的多边形，其顶点顺序是不固定的。在进行透视变换前，你需要编写逻辑将这4个点排序为：左上、右上、左下、右下（反正得有一个顺序，你自己调个顺序也行）。
4. 车牌是蓝底白字，在灰度图上对比度明显，但容易受阴影影响。你需要尽可能找到一种鲁棒的方法处理全部图片，而不能依赖于手工调整。当然，也没必要强求完美，后边有更好用的工具。

### 实验提示：

1. **关于阈值选择**：对于光照均匀的图，`cv2.THRESH_OTSU` 很好用；但对于有阴影的实拍图，`cv2.ADAPTIVE_THRESH_GAUSSIAN_C` 往往能更好地保留边缘。到底该选哪个最好呢?
2. **关于筛选策略**：
   * `cv2.boundingRect(cnt)` 可以获取轮廓的外接矩形。
   * 通过 `w / h` (宽高比) 过滤：中国车牌的长宽比通常在 3 到 4 之间。
   * 通过 `cv2.contourArea` 过滤：太小的噪点轮廓直接忽略。
3. **关于多边形拟合**：
   * 使用 `cv2.arcLength` 计算轮廓周长。
   * 设定 `epsilon = 0.02 * perimeter` ，允许一定误差进行拟合。
   * 如果拟合结果 `len(approx) == 4`，那么恭喜你，可能找到了车牌！
4. **调试技巧**：每一步都用 `cv2.imshow` 把中间结果显示出来（例如显示二值化后的图，显示画了轮廓的图），不要只看最后结果。

### 实验思考：

1. 如果车牌颜色与车身颜色非常接近（例如白车白牌），仅靠灰度或BGR定值阈值分割还能奏效吗？如果不奏效，可以利用 HSV 或YUV颜色空间进行分割吗？
2. 在二值化后，车牌区域的内部经常会有很多黑色空洞（文字部分），如何使用**形态学操作**（膨胀/腐蚀/闭运算）将车牌变成一个实心的白色矩形块，以便更容易被检测到？
3. 自适应阈值中的 `blockSize` 和 `C` 参数分别对结果有什么影响？调大或调小会发生什么？

## 实验完毕后，记得提交修改（命令行中-m后的字符串可自行确定），以供检查：

```bash
git commit -a -m "my work on lab1_6 is done."
```
