

## lab1_8_morphological_process

对于给定的二值图片，请依次选择合适的核形状与大小，分别进行**腐蚀、膨胀、开操作、闭操作、形态学梯度、顶帽和黑帽**运算，输出全部结果图片，并观察各操作的效果差异。

而后，请使用**形态学操作 + 轮廓筛选**（不要手动框选），精准截取出图片中的 `Morphology` 单词整体，以及其中每个单独的字母，分别保存这些图片。

### 背景知识

形态学操作是图像处理中最基础也最实用的工具之一，主要作用于**二值图像**（或灰度图像），通过一个称为**结构元素**（Structuring Element / Kernel)的小矩阵在图像上滑动，根据不同规则修改像素值。

七种基本形态学操作的直觉理解如下：

| 操作                  | 效果       | 直觉描述              |
| ------------------- | -------- | ----------------- |
| **腐蚀（Erode）**       | 白色区域缩小   | "啃掉"白色边缘，消除细小白色噪点 |
| **膨胀（Dilate）**      | 白色区域扩大   | "长胖"白色边缘，填补细小黑色空洞 |
| **开运算（Opening）**    | 先腐蚀后膨胀   | 去除白色小噪点，保持主体大小不变  |
| **闭运算（Closing）**    | 先膨胀后腐蚀   | 填补黑色小空洞，保持主体大小不变  |
| **形态学梯度（Gradient）** | 膨胀 − 腐蚀  | 提取物体的轮廓/边缘        |
| **顶帽（Top Hat）**     | 原图 − 开运算 | 提取比周围亮的细小结构（亮细节）  |
| **黑帽（Black Hat）**   | 闭运算 − 原图 | 提取比周围暗的细小结构（暗细节）  |

```
原图 (白色文字，黑色背景):        腐蚀后:              膨胀后:
┌────────────────────┐          ┌──────────────────┐  ┌──────────────────┐
│                    │          │                  │  │                  │
│    ████  ██████    │          │    ██    ████    │  │   ██████ ████████│
│   ██  ██ ██       │          │   █  █  █       │  │  ████ ████████   │
│   ██  ██ ████     │          │   █  █  ███     │  │  ████ ████████   │
│   ██  ██ ██       │          │   █  █  █       │  │  ████ ████████   │
│    ████  ██       │          │    ██   █       │  │   ██████ ████    │
│                    │          │                  │  │                  │
└────────────────────┘          └──────────────────┘  └──────────────────┘
  字母笔画正常粗细                  笔画变细/断裂          笔画变粗/粘连
```

### 实验要求：

1. **结构元素实验**：
   
   * 使用 `cv2.getStructuringElement` 分别创建以下三种核形状：
     - **矩形核**（`cv2.MORPH_RECT`）
     - **椭圆核**（`cv2.MORPH_ELLIPSE`）
     - **十字核**（`cv2.MORPH_CROSS`）
   * 选择合适的核大小（如 3×3、5×5、7×7），观察不同大小对操作效果的影响。

2. **七种形态学操作**：
   
   * 对给定的二值图像，依次执行以下操作，并将结果图像全部保存/展示：
     - 腐蚀（`cv2.erode`）
     - 膨胀（`cv2.dilate`）
     - 开运算（`cv2.morphologyEx` + `cv2.MORPH_OPEN`）
     - 闭运算（`cv2.morphologyEx` + `cv2.MORPH_CLOSE`）
     - 形态学梯度（`cv2.morphologyEx` + `cv2.MORPH_GRADIENT`）
     - 顶帽（`cv2.morphologyEx` + `cv2.MORPH_TOPHAT`）
     - 黑帽（`cv2.morphologyEx` + `cv2.MORPH_BLACKHAT`）
   * 建议将全部结果拼成一张大图进行直观对比。

3. **文字提取实战**：
   
   * 读取包含 `Morphology` 单词的二值图，你可以进行任意的预处理。
   * **提取整个单词**：
     - 使用形态学操作（如膨胀或闭运算），将相邻字母**连通**成一个整体区域。
     - 查找轮廓，筛选出或符合条件的轮廓。
     - 用 `cv2.boundingRect` 获取外接矩形，截取该区域并保存。
   * **提取每个字母**：
     - 在**不进行字母连通**的条件下，查找出**分离的**轮廓。
     - 筛选出每个字母的轮廓（过滤噪点）。
     - 对筛选出的轮廓按 **x 坐标排序**（从左到右），依次截取每个字母的外接矩形区域并保存。
   * 保存格式建议：整个单词保存为 `word_morphology.png`，各字母保存为 `letter_0_M.png`、`letter_1_o.png`、...（也可简单地按序号命名）。

4. **结果输出**：将七种形态学操作的对比图以及截取的单词/字母图片全部保存。

### 你可能要用到的工具函数原型：

如果你是C++选手：

```cpp
// 创建结构元素
// shape: MORPH_RECT / MORPH_ELLIPSE / MORPH_CROSS
cv::Mat cv::getStructuringElement(int shape, cv::Size ksize, cv::Point anchor = cv::Point(-1,-1));

// 腐蚀
void cv::erode(cv::InputArray src, cv::OutputArray dst, cv::InputArray kernel,
               cv::Point anchor = cv::Point(-1,-1), int iterations = 1,
               int borderType = BORDER_CONSTANT,
               const cv::Scalar& borderValue = cv::morphologyDefaultBorderValue());

// 膨胀
void cv::dilate(cv::InputArray src, cv::OutputArray dst, cv::InputArray kernel,
                cv::Point anchor = cv::Point(-1,-1), int iterations = 1,
                int borderType = BORDER_CONSTANT,
                const cv::Scalar& borderValue = cv::morphologyDefaultBorderValue());

// 高级形态学操作 (开/闭/梯度/顶帽/黑帽)
// op: MORPH_OPEN / MORPH_CLOSE / MORPH_GRADIENT / MORPH_TOPHAT / MORPH_BLACKHAT
void cv::morphologyEx(cv::InputArray src, cv::OutputArray dst, int op, cv::InputArray kernel,
                      cv::Point anchor = cv::Point(-1,-1), int iterations = 1,
                      int borderType = BORDER_CONSTANT,
                      const cv::Scalar& borderValue = cv::morphologyDefaultBorderValue());

// 查找轮廓
void cv::findContours(cv::InputArray image, cv::OutputArrayOfArrays contours,
                      cv::OutputArray hierarchy, int mode, int method,
                      cv::Point offset = cv::Point());

// 外接矩形
cv::Rect cv::boundingRect(cv::InputArray array);

// 轮廓面积
double cv::contourArea(cv::InputArray contour, bool oriented = false);

// 绘制轮廓
void cv::drawContours(cv::InputArray image, cv::InputArrayOfArrays contours,
                      int contourIdx, const cv::Scalar& color, int thickness = 1, ...);
```

如果你是Python选手：

```python
# 创建结构元素 (核)
# shape: cv2.MORPH_RECT / cv2.MORPH_ELLIPSE / cv2.MORPH_CROSS
cv2.getStructuringElement(shape: int, ksize: tuple[int, int]) -> MatLike

# 腐蚀 (iterations 控制迭代次数)
cv2.erode(src: MatLike, kernel: MatLike, iterations: int = 1, ...) -> MatLike

# 膨胀
cv2.dilate(src: MatLike, kernel: MatLike, iterations: int = 1, ...) -> MatLike

# 高级形态学操作
# op: cv2.MORPH_OPEN / MORPH_CLOSE / MORPH_GRADIENT / MORPH_TOPHAT / MORPH_BLACKHAT
cv2.morphologyEx(src: MatLike, op: int, kernel: MatLike, iterations: int = 1, ...) -> MatLike

# 查找轮廓
cv2.findContours(image: MatLike, mode: int, method: int) -> tuple[Sequence[MatLike], MatLike]

# 外接矩形 (返回 x, y, w, h)
cv2.boundingRect(array: MatLike) -> tuple[int, int, int, int]

# 轮廓面积
cv2.contourArea(contour: MatLike) -> float

# 绘制轮廓
cv2.drawContours(image: MatLike, contours: Sequence[MatLike], contourIdx: int,
                 color: Sequence[float], thickness: int = ...) -> MatLike
```

**注意**：

1. `cv2.erode` 和 `cv2.dilate` 的 `iterations` 参数可以控制操作重复次数。`iterations=2` 等价于连续执行两次该操作，效果比单次使用更大核更平滑。
2. `cv2.morphologyEx` 是统一入口，通过 `op` 参数选择具体操作。开运算和闭运算**不等价于**简单地对膨胀/腐蚀的结果调换顺序——它们的组合顺序是固定的。
3. `cv2.findContours` 在检索模式上，`cv2.RETR_EXTERNAL` 只返回最外层轮廓（适合本实验），`cv2.RETR_TREE` 返回完整层级结构。
4. 使用 `cv2.boundingRect` 获取的是**正立的外接矩形**（不旋转），对于本实验中水平排列的文字已经足够。

### 实验提示：

1. **如何将分散的字母连成一个整体？**
   
   * 使用**水平方向的膨胀核**，例如 `cv2.getStructuringElement(cv2.MORPH_RECT, (25, 1))`，让相邻字母在水平方向上粘连。
   * 核的宽度取决于字母之间的间距——太小连不上，太大会把不相关的内容也连进来。需要你观察图片后自行调整。

2. **字母提取的排序**：
   
   ```python
   # 按外接矩形的 x 坐标排序，保证从左到右
   contours_sorted = sorted(contours, key=lambda c: cv2.boundingRect(c)[0])
   ```

3. **面积筛选的参考逻辑**：
   
   ```python
   for cnt in contours:
       x, y, w, h = cv2.boundingRect(cnt)
       area = cv2.contourArea(cnt)
       # 过滤太小的噪点和太大的外框
       if area < 100:
           continue
       # 可选：通过长宽比过滤非字母区域
       aspect_ratio = w / h
       if aspect_ratio > 3 or aspect_ratio < 0.1:
           continue
       # 截取字母
       letter = img[y:y+h, x:x+w]
   ```

4. **调试建议**：
   
   * 在形态学操作后、轮廓查找后，都把中间结果画出来看看。
   * 用 `cv2.drawContours` 在原图副本上画出找到的所有轮廓，确认筛选逻辑是否正确。
   * 如果字母 "o"、"p"、"h" 等含有封闭空洞的字母被检测出了内外两层轮廓，记得使用 `cv2.RETR_EXTERNAL` 只取最外层。

### 实验思考：

1. 对于腐蚀和膨胀操作，使用**矩形核、椭圆核、十字核**分别会产生什么不同的效果？在什么场景下应该选择哪种核形状？
2. 开运算（先腐蚀后膨胀）为什么能去除白色噪点而不明显改变主体大小？闭运算（先膨胀后腐蚀）又为什么能填补黑色空洞？试从操作的先后顺序角度分析。
3. 在提取单个字母时，如果图片中存在标点符号（如句号、逗号），它们也会被当作"字母"提取出来。除了面积筛选，还有哪些特征可以帮助区分字母和标点？
4. 形态学梯度提取的"边缘"与 Canny 边缘检测的结果有什么区别？各自适合什么场景？(考虑到之后很快就会有边缘检测的实验...)

## 实验完毕后，记得提交修改（命令行中-m后的字符串可自行确定），以供检查：

```
git commit -a -m "my work on lab1_8 is done."
```


