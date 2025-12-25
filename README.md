# CS106L Assignments 

- 克隆自 https://github.com/cs106l/cs106l-assignments/

# Progress
- [x] Assigment 1
  - 作业 1 包含了 streams 和 initialization 章节的内容，即实例的初始化和文件流交互。
- [x] Assigment 2
  - 作业 2 包含了文件的读入，STL 的迭代器遍历和函数使用（重点为 `set` 和 `queue`）
- [x] Assigment 3
  - 作业 3 包含了类的构造，内容涉及 `getter` 和 `setter` 函数，以及基础的构造函数设计
- [x] Assigment 4
  - 作业 4 包含了 lambda 表达式、`ranges` 和 `view` 特性的使用，学习使用这些特性替代 `while`、`for` 循环，进行更简洁的循环操作。
- [x] Assigment 5
  - 作业 5 包含了 SMFs 的构建和运算符的重载操作，深入挖掘类本身的特性，进行更高自由度的类实现。
- [x] Assigment 6
  - 作业 6 包含了 `std::optional` 中类型安全的检查
- [x] Assigment 7
  - 作业 7 包含对 `std::unique_ptr` 智能指针的简单实现，简单地介绍了智能指针如何安全、自动地完成动态内存管理。
- [x] HashMap([CS106L 2023 STL HashMap](https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1226/assignment3.html))
  - HashMap 作业进一步实践了 **模板编程**
  - 通过设置一份基础的 HashMap 原始代码（包含 `.h` 和 `.cpp`）文件，以 `.h` 中的注释代码指导学生完成 **初始化（构造、析构）、运算符重载、拷贝（构造和赋值）、移动（构造和赋值）**
  - 并通过预设的 benchmark 比较 from scratch 和 STL 库内的 HashMap 的性能，分析 from scratch 版本的特性
- [x] GapBuffer([CS106L 2020 GapBuffer](https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1204/assignments.html))
  - GapBuffer 作业也是要求学生实践 **模板编程**
  - 通过引入文本编辑器在光标处插入内容的需求，探讨了文本存储的方式，并引入了本作业的核心，即 **buffer**，在光标后，都应该有 **buffer** 处理可能到来的符号插入，避免频繁的内存操作导致过长的处理时间
  - 以注释引导学生完成 **初始化（构造、析构）、运算符重载、拷贝和移动语义、迭代器的简单实现、RAII和可变参数初始化** 的功能。
- [x] KDTree([CS106L 2018 KDTree](https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1192/assignments.html))
  - KDTree 作业要求学生基于指针实现KDTree和基础的创建、搜索（KNN）和复制操作
  - 以 handbook 方式引导学生实现**简单的 KDTree**，并实践部分 C++ 模板编程中的 **拷贝构造、赋值** 操作。

# Acknowledgment
以下列上相关的 Stanford 课程主页和参考的代码仓库
- 课程主页
  - [CS106L 2018](https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1192/index.html)
  - [CS106L 2023](https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1234/index.html)
  - [CS106L 2025 Spring](https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1256/)
  - [CS106L Textbook](https://cs106l.github.io/textbook/)
- 代码仓库
  - [Self-Learning Materials of CS106L from 'wenwz'](https://github.com/wengwz/CS106L-Self-Learning)
  - [CS106L Assignment from 'Chena2003'](https://github.com/Chena2003/CS106L)
- 博客
  - [C++ 学习! from 'NoughtQ'](https://note.noughtq.top/lang/cpp/)
