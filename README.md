## 决策树实现：基于信息增益的ID3算法
**实现了决策树的学习以及分类，并采用后向剪枝进行优化，采用信息增益作为学习过程的属性选择依据。**

文件目录：
encode.txt：对各个属性的编码说明
ID3.cpp：源代码
TestData.txt：测试数据
TrainData.txt：训练数据

### 编码
把每一种属性都用数字来表示。

 ```
青绿:0　乌黑:1　浅白:2
蜷缩:0　稍蜷:1　硬挺:2
浊响:0　沉闷:1　清脆:2
清晰:0　稍糊:1　模糊:2
凹陷:0　稍凹:1　平坦:2
硬滑:0　软粘:1　
 ```

### 训练
训练数据：
第1行：训练样本个数及其属性个数
第2行：各个属性的属性值的个数
第3-12行：训练样本

```
10 6
3 3 3 3 3 2
0 0 0 0 0 0 1  
1 0 1 0 0 0 1
1 0 0 0 0 0 1
0 1 0 0 1 1 1
1 1 0 1 1 1 1
0 2 2 0 2 1 0
2 1 1 1 0 0 0
1 1 0 0 1 1 0
2 0 0 2 2 0 0
0 0 1 1 1 0 0
```

### 测试
测试数据：
第1行：测试样本个数及其属性个数
第2-8行：测试样本

```
7 6
0 0 1 0 0 0 1
2 0 0 0 0 0 1
1 1 0 0 1 0 1
1 1 1 1 1 0 0
2 2 2 2 2 0 0
2 0 0 2 2 1 0
0 1 0 1 0 0 0
```

### 函数功能简介
```C++
void InputData(TrainData& data);    //输入训练数据
Node* BuildTree(TrainData data, vector<int> vis);   //建树（核心），即学习过程
int MostValue(TrainData data);      //统计数据中种类最多的那一类
int BestAttribute(TrainData data, vector<int>vis);  //找出信息增益最大的那一个属性
double CalGain(TrainData data, int Attr);   //计算各个属性的信息增益
double CalEntropy(TrainData data);      //计算信息熵
void GetTestData(TestData& Tdata);      //输入测试数据
int Judge(Node* root, TestData Tdata, int state);   //对测试数据分类
void Prune(Node* root, Node* u, TestData Tdata);    //后向剪枝
void PrintTreeStruct(Node* root);       //打印树的结构(测试代码用，可以删去这部分)
```