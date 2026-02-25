

## lab1_7_mask

现提供三张图片：`logo`、`pic` 和 `background`。请基于 `logo` 图生成 logo 的掩膜（mask），用于提取出 logo 部分；基于 `pic` 提取出主要人物；最后将提取出的人物以及 logo 合成到 `background` 的合适位置上，并保存最终结果。

### 实验要求：

1. **Logo 掩膜生成与提取**：
   
   * 读取 `logo` 图片，考虑其黑白差距明显，可以直接将其转换为灰度图。
   * 对灰度图进行阈值分割，生成一个二值掩膜（mask）：logo 主体区域为白色（255），背景区域为黑色（0）。
   * 使用 `cv2.bitwise_not` 生成反向掩膜（mask_inv）。
   * 利用掩膜与位运算（`cv2.bitwise_and`）提取出 logo 的前景像素。

2. **人物提取**：
   
   * 读取 `pic` 图片。
   * 选择以下任意一种方法将人物从背景中分离：
     - **基于颜色空间**：将图片转换到 HSV 空间，根据背景颜色范围（如纯色背景）使用 `cv2.inRange` 生成掩膜。
     - **基于阈值分割**：如果背景与人物对比度较大，可用阈值方法生成掩膜。
     - **基于 GrabCut**（进阶可选）：使用 `cv2.grabCut` 进行交互式/半自动前景提取。
   * 利用生成的掩膜提取人物前景。可以使用形态学操作（开运算/闭运算）清理掩膜边缘的噪点和毛刺。

3. **图像合成**：
   
   * 读取 `background` 图片。
   * 确定人物和 logo 在背景图中的放置位置，根据需要对人物和 logo 进行 **缩放**（`cv2.resize`），使其大小与背景图协调。
   * 在背景图的目标区域（ROI）中，利用掩膜进行合成：
     - 用 `mask_inv` 在背景 ROI 上"挖掉"前景将要占据的区域。
     - 用 `mask` 提取前景像素。
     - 将两者用 `cv2.add` 相加，得到无缝合成的结果。
   * 将合成后的 ROI 写回背景图的对应位置。

4. **结果输出**：展示每一步的中间结果（掩膜、提取的前景、合成过程），并保存最终的合成图像。

### 你可能要用到的工具函数原型：

如果你是C++选手：

```cpp
// 颜色空间转换
void cv::cvtColor(cv::InputArray src, cv::OutputArray dst, int code, int dstCn = 0);

// 固定阈值（用于生成掩膜）
double cv::threshold(cv::InputArray src, cv::OutputArray dst, double thresh, double maxval, int type);

// HSV颜色范围筛选（用于按颜色生成掩膜）
void cv::inRange(cv::InputArray src, cv::InputArray lowerb, cv::InputArray upperb, cv::OutputArray dst);

// 位运算 - 与（配合掩膜提取感兴趣区域）
void cv::bitwise_and(cv::InputArray src1, cv::InputArray src2, cv::OutputArray dst, cv::InputArray mask = cv::noArray());

// 位运算 - 或
void cv::bitwise_or(cv::InputArray src1, cv::InputArray src2, cv::OutputArray dst, cv::InputArray mask = cv::noArray());

// 位运算 - 取反（生成反向掩膜）
void cv::bitwise_not(cv::InputArray src, cv::OutputArray dst, cv::InputArray mask = cv::noArray());

// 图像加法（合成前景与背景）
void cv::add(cv::InputArray src1, cv::InputArray src2, cv::OutputArray dst, cv::InputArray mask = cv::noArray(), int dtype = -1);

// 图像缩放
void cv::resize(cv::InputArray src, cv::OutputArray dst, cv::Size dsize, double fx = 0, double fy = 0, int interpolation = INTER_LINEAR);

// 形态学操作（清理掩膜噪点）
void cv::morphologyEx(cv::InputArray src, cv::OutputArray dst, int op, cv::InputArray kernel, cv::Point anchor = cv::Point(-1,-1), int iterations = 1, int borderType = BORDER_CONSTANT, const cv::Scalar& borderValue = cv::morphologyDefaultBorderValue());

// 创建结构元素（形态学操作的核）
cv::Mat cv::getStructuringElement(int shape, cv::Size ksize, cv::Point anchor = cv::Point(-1,-1));
```

如果你是Python选手：

```python
# 颜色空间转换 (BGR -> GRAY / BGR -> HSV)
cv2.cvtColor(src: MatLike, code: int, ...) -> MatLike

# 固定阈值 (生成二值掩膜)
cv2.threshold(src: MatLike, thresh: float, maxval: float, type: int) -> tuple[float, MatLike]

# HSV颜色范围筛选 (返回二值掩膜：在范围内为255，否则为0)
cv2.inRange(src: MatLike, lowerb: MatLike, upperb: MatLike) -> MatLike

# 位运算 - 与 (使用mask参数控制作用区域)
cv2.bitwise_and(src1: MatLike, src2: MatLike, mask: MatLike = ...) -> MatLike

# 位运算 - 或
cv2.bitwise_or(src1: MatLike, src2: MatLike, mask: MatLike = ...) -> MatLike

# 位运算 - 取反 (白变黑，黑变白)
cv2.bitwise_not(src: MatLike, mask: MatLike = ...) -> MatLike

# 图像加法 (饱和运算，不会溢出)
cv2.add(src1: MatLike, src2: MatLike, mask: MatLike = ..., dtype: int = ...) -> MatLike

# 图像缩放
cv2.resize(src: MatLike, dsize: tuple[int, int], fx: float = ..., fy: float = ..., interpolation: int = ...) -> MatLike

# 形态学操作 (开运算/闭运算/膨胀/腐蚀)
cv2.morphologyEx(src: MatLike, op: int, kernel: MatLike, ...) -> MatLike

# 创建结构元素
cv2.getStructuringElement(shape: int, ksize: tuple[int, int], anchor: tuple[int, int] = ...) -> MatLike
```

**注意**：

1. **掩膜（mask）必须是单通道的 8 位图像**（`dtype=np.uint8`），像素值只有 0 和 255。在 `bitwise_and` 中，mask 参数决定了"哪些像素参与运算"——mask 为 255 的地方保留，为 0 的地方置零。
2. **ROI 的尺寸必须与前景图一致**。在将缩放后的 logo/人物放到背景上之前，先确认 `roi = background[y:y+h, x:x+w]` 中的 `h, w` 与前景图的尺寸完全匹配，否则位运算会因形状不一致而报错。
3. **`cv2.add` 与 `+` 运算符不同**：`cv2.add` 执行饱和运算（上限 255），而 NumPy 的 `+` 会发生溢出取模（如 200+100=44）。合成图像时务必使用 `cv2.add`。
4. 如果 logo 本身有白色/浅色背景，阈值分割后要注意**前景和背景哪个是白哪个是黑**，可能需要调换 `mask` 和 `mask_inv` 的角色。

### 实验提示：

1. **Logo 掩膜生成的典型流程**：
   
   ```
   logo_gray = cv2.cvtColor(logo, cv2.COLOR_BGR2GRAY)
   _, mask = cv2.threshold(logo_gray, 阈值, 255, cv2.THRESH_BINARY)
   # 或使用 cv2.THRESH_BINARY_INV，取决于logo背景是亮还是暗
   mask_inv = cv2.bitwise_not(mask)
   ```

2. **合成的核心思路（以 logo 为例）**：
   
   ```
   # 1. 在背景ROI上"擦除"logo将要占据的区域
   bg_roi = background[y:y+h, x:x+w]
   bg_part = cv2.bitwise_and(bg_roi, bg_roi, mask=mask_inv)
   
   # 2. 从logo中提取前景像素
   fg_part = cv2.bitwise_and(logo, logo, mask=mask)
   
   # 3. 合成
   combined = cv2.add(bg_part, fg_part)
   background[y:y+h, x:x+w] = combined
   ```

3. **人物提取提示**：
   
   * 如果 `pic` 是纯色背景（如绿幕、蓝幕、白色背景），转到 HSV 空间后用 `cv2.inRange` 把背景颜色选出来，再取反就得到人物掩膜。
   * 如果背景不是纯色，可以尝试使用 `cv2.THRESH_OTSU` 自动确定阈值，或者使用边缘检测+轮廓查找的方式。
   * 不过本场景中色彩非常鲜明，如果你已经完成了`lab1_6`，那么想必做这个是小菜一碟。
   * 提取后的掩膜边缘可能有锯齿，可用 `cv2.GaussianBlur` 对掩膜进行轻微模糊，再重新二值化，以获得更平滑的边缘。

4. **调试技巧**：
   
   * 每一步都把 mask 显示出来看看效果——mask 看起来应该像剪影。
   * 合成前，先单独查看 `bg_part` 和 `fg_part`，确认"擦除"和"提取"都正确。
   * 如果合成结果出现"黑边"或"白边"，通常是掩膜的边缘不够精确，尝试调整阈值或用形态学腐蚀（`cv2.erode`）缩小掩膜边界。

### 实验思考：

1. 在合成时，直接用 `cv2.add` 拼合前景和背景，接缝处可能会出现不自然的硬边。有没有办法实现**羽化（feathering**效果，让边缘过渡更柔和？提示：考虑对 mask 做高斯模糊后作为**alpha 通道**进行加权混合。你也可以试试`cv2.seamlessClone` (泊松融合)。
2. `cv2.bitwise_and(img, img, mask=mask)` 与直接用 NumPy 操作 `img[mask == 0] = 0` 的效果是否等价？各自有什么优缺点？
3. 如果 logo 包含**半透明区域**（如 PNG 格式的 alpha 通道），上述二值掩膜方法还适用吗？应该如何利用 alpha 通道实现更精确的合成？提示：`cv2.imread("logo.png", cv2.IMREAD_UNCHANGED)` 可以读取含 alpha 通道的 4 通道图像。

## 实验完毕后，记得提交修改（命令行中-m后的字符串可自行确定），以供检查：

```
git commit -a -m "my work on lab1_7 is done."
```
