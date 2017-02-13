#pragma once

/*
定义Window类主要负责窗口的创建、显示与更新，加载资源
*/

#include <SDL_image.h>
#include <iostream>
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

// 游戏逻辑
public:
	void MoveBall();
	bool CheckWallCollisions(GameObject& Object, Directions dir);

private:
	SDL_Window* win;			// 窗口
	SDL_Renderer* renderer;		// 渲染器
	SDL_Texture* bg;			// 背景纹理

	GameObject Ball;			// 小球
	GameObject PaddleC;			// 板（电脑）
	GameObject PaddleP;			// 板（玩家）

	bool IsStarted;				// 游戏是否开始
};

