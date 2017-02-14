#pragma once

/*
定义Window类主要负责窗口的创建、显示与更新，加载资源
*/

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include "GameObject.h"
#include "Defines.h"
#include "Enums.h"

using namespace std;

class Window
{
public:
	Window(void);
	~Window(void);

public:
	void Start();				// 进入消息循环
	void Refresh();				// refresh到屏幕上
	void Update();				// 更新图像信息
	void LoadResouceFile();		// 加载资源文件
	SDL_Texture* Load_image(std::string FilePath);	// 加载图像
	void Init();				// 初始化游戏对象
	void ResetGame();			// 重置游戏
	SDL_Texture* Draw_text(std::string message);	// 绘制文字

// 游戏逻辑
public:
	// 移动小球
	void MoveBall();
	// 检查球是否会碰到Wall
	bool CheckWallCollisions(GameObject& Object, Directions dir);
	// 检查球是否会碰撞到浆
	bool CheckBallCollisions(GameObject& Object);
	// 移动小球，做相关碰撞事件的处理逻辑，例如如果碰到浆改变小球速度等
	void HandleBall();
	// 处理电脑的浆，使之智能
	void HandleComputerPaddle();
	// 处理玩家分数
	void HandlePlayerScore();
	// 处理电脑分数
	void HandleComputerScore();

private:
	SDL_Window* win;			// 窗口
	SDL_Renderer* renderer;		// 渲染器
	SDL_Texture* bg;			// 背景纹理
	TTF_Font* Font;				// 字体

	GameObject Ball;			// 小球
	GameObject PaddleC;			// 板（电脑）
	GameObject PaddleP;			// 板（玩家）

	bool IsStarted;				// 游戏是否开始
	bool Pause;					// 暂停游戏

	int PlayerScore;			// 玩家分数
	int ComputerScore;			// 电脑分数
};

