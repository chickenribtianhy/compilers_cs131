# Programing Assignment III 文档

<!-- TOC -->

- [Programing Assignment III 文档](#programing-assignment-iii-文档)
- [lab3-实验文档](#lab3-实验文档)
  - [0. 基础知识](#0-基础知识)
    - [0.1 Light IR 类型设计](#01-light-ir-类型设计)
    - [0.1 Light IR Function](#01-light-ir-function)
    - [0.2 Light IR class](#02-light-ir-class)
    - [0.3 Light IR stdlib 调用](#03-light-ir-stdlib-调用)
  - [1. 实验要求](#1-实验要求)
    - [1.1 目录结构](#11-目录结构)
    - [1.2 主要工作](#12-主要工作)
      - [1.2.1  几点说明](#121--几点说明)
    - [1.3 Bonus](#13-bonus)
    - [1.4 编译、运行和验证](#14-编译运行和验证)

<!-- /TOC -->

# lab3-实验文档

本次实验是组队实验，请仔细阅读组队要求，并合理进行分工合作。 本次实验和 Lab3 一样，需要使用 `LightIR` 框架自动产生 `ChocoPy` 语言的LLVM IR。 建议在实验前队伍内进行讨论，以确保成员都理解 Lab3 中的问题。

相信大家已经掌握了 LightIR 的使用，并且对于 LLVM IR 也有了一定理解。在本次实验中，我们要使用访问者模式来实现 IR 自动生成。 对于产生的IR， 我们可以调用 clang 生成可执行文件，这样一个初级的 ChocoPy 编译器就完成啦！

注意：组队实验意味着合作，但是小组间的交流是受限的，且严格**禁止**代码的共享。除此之外，如果小组和其它组进行了交流，必须在根目录 `README.md` 中记录下来交流的小组和你们之间交流内容。同时，需要在此文件中更新两位同学的邮箱和代码WriteUp，WriteUp要求详见[Code Review.pdf]()。

## 0. 基础知识

### 0.1 Light IR 类型设计

### 0.1 Light IR Function

### 0.2 Light IR class

### 0.3 Light IR stdlib 调用

Class代码框架
## 1. 实验要求

### 1.1 目录结构

详见[common/structure.md](./doc/common/structure.md)
### 1.2 主要工作

1. 阅读[LightIR 核心类介绍](../common/LightIR.md)
2. 阅读[实验框架](#1-实验框架)，理解如何使用框架以及注意事项
3. 修改 [chocopy_lightir.cpp](../../src/ir-optimizer/chocopy_lightir.cpp) 来实现自动 IR 产生的算法，使得它能正确编译任何合法的 ChocoPy 程序
4. 在 `README.md` 中解释你们的设计，遇到的困难和解决方案

#### 1.2.1  几点说明
由于 LLVM IR 是强类型的，以及无法接收生成 fp action record 的参数，有很多部分和伯克利生成的代码不太一样

1. char* 还是 .string
2. activation record 不使用 frame pointer 回滚 function local 变量。
3. 几种 call convention，本 project 完全采用 c calling convention.
```bash
 *******************
 * CALL CONVENTION *
 *******************
NAME        CLEAN       PARA_DIRECTION
---------------------------------------------
cdecl       caller      <-
stdcall     callee      <-
pascal      callee      ->
fastcall    callee      2reg, <-
USING **pascal**
```
4. 

### 1.3 Bonus

1. 完成基本Pass[60 pts]
2. 完成优化Pass[$\infty$ pts]

### 1.4 编译、运行和验证

* 编译

  若编译成功，则将在 `./[build_dir]/` 下生成 `ir-optimizer` 命令。

* 运行

  本次实验的 `ir-optimizer` 命令使用命令行参数来完成编译和运行。

  ```shell
  $ cd chocopy
  $ ./build/ir-optimizer test.py -run # 直接用clang编译器编译到elf给qemu执行。
  $ ./build/ir-optimizer test.py -emit # 输出。
  $ ./build/ir-optimizer test.py -assem # 输出汇编。
  $ ./build/ir-optimizer test.py -pass [PassName] # 运行pass。
  <以上的选项可以同时使用>
  ```

  通过灵活使用重定向，可以比较方便地完成各种各样的需求，请同学们务必掌握这个 shell 功能。

* 验证

  本次试验测试案例较多，为此我们将这些测试分为两类：

    1. sample: 这部分测试均比较简单且单纯，适合开发时调试。
    2. fuzz: 由fuzzer生成的正确的python文件，此项不予开源。
    3. student: 这部分由同学提供。

  我们使用python中的 `json.load()` 命令进行验证。将自己的生成结果和助教提供的 `xxx.typed.ast` 进行比较。

  ```shell
  $ python3 ./duipai.py --pa 3
  # 如果结果完全正确，则全 PASS，且有分数提示，一个正确的case 1 pts，此项评分按比例算入总评。选择chocopy的同学会在project部分分数上*1.2计入总评。
  # 如果有不一致，则会汇报具体哪个文件哪部分不一致，且有详细输出。
  ```

  **请注意助教提供的`testcase`并不能涵盖全部的测试情况，完成此部分仅能拿到基础分，请自行设计自己的`testcase`进行测试。**
  