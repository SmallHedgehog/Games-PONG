#include "stdafx.h"
#include "Window.h"
#include <Windows.h>
#include <stdint.h>


Window::Window(void):
	win(nullptr), renderer(nullptr)
{
	IsStarted = true;
	// 初始化SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(1);
	}
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	// 创建窗口
	win = SDL_CreateWindow(WINDOW_CAPTION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	// 创建渲染器(使用硬件加速渲染和与当前显示器同步)
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// 加载资源文件
	LoadResouceFile();
	// 初始化游戏对象
	Init();
}

void Window::Init()
{
	// 设置球速、初始位置等
	Ball.setSpeed_x(2);
	Ball.setSpeed_y(0);
	int Position_x = (WINDOW_WIDTH / 2);
	int Position_y = (WINDOW_HEIGHT / 2);
	SDL_Rect rect0 = {
		Position_x, Position_y, Ball.getGeometry().w, Ball.getGeometry().h
	};
	Ball.setLocation(rect0);

	// 设置电脑玩家浆速、位置等
	PaddleC.setSpeed_x(COMPUTER_SPEED);
	Position_x = (WINDOW_WIDTH / 2) - (PaddleC.getGeometry().w / 2);
	SDL_Rect rect = {
		Position_x, COMPUTER_Y, PaddleC.getGeometry().w, PaddleC.getGeometry().h
	};
	PaddleC.setLocation(rect);

	// 设置玩家浆速、位置等
	PaddleP.setSpeed_x(PLAYER_SPEED);
	rect.x = (WINDOW_WIDTH / 2) - (PaddleC.getGeometry().w / 2);
	rect.y = PLAYER_Y;
	rect.w = PaddleP.getGeometry().w;
	rect.h = PaddleP.getGeometry().h;
	PaddleP.setLocation(rect);
}

// 进入消息循环
void Window::Start()
{
	// do something
	SDL_Event event;
	bool gameRunning = true;
	while (gameRunning)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				gameRunning = false;
			}
		}

		MoveBall();

		Update();
		Refresh();
	}
}

// refresh到屏幕上
void Window::Refresh()
{
	SDL_RenderPresent(renderer);
}

// 更新图像位置信息
void Window::Update()
{
	SDL_RenderClear(renderer);
	// do something
	if (IsStarted)
	{
		// 获取背景图像属性（大小）
		int width = 0, height = 0;
		SDL_QueryTexture(bg, NULL, NULL, &width, &height);
		SDL_Rect rect = {
			0, 0, width, height
		};

		SDL_RenderCopy(renderer, bg, NULL, &rect);
		SDL_RenderCopy(renderer, Ball.getTexture(), NULL, &Ball.getLocation());
		SDL_RenderCopy(renderer, PaddleC.getTexture(), NULL, &PaddleC.getLocation());
		SDL_RenderCopy(renderer, PaddleP.getTexture(), NULL, &PaddleP.getLocation());
	}
}

void Window::LoadResouceFile()
{
	// 背景纹理
	bg = IMG_LoadTexture(renderer, "res/background.bmp");
	// 球
	Ball.setTexture(Load_image("res/ball.png"));
	// 电脑、玩家板
	SDL_Texture* texture = Load_image("res/paddle.png");
	PaddleC.setTexture(texture);
	PaddleP.setTexture(texture);
}

// 加载图像纹理
SDL_Texture* Window::Load_image(std::string FilePath)
{
	SDL_Surface* picture = nullptr;

	picture = IMG_Load(FilePath.c_str());
	// 设置背景透明
	Uint32 colorkey = SDL_MapRGB(picture->format, 0, 0, 0);
	SDL_SetColorKey(picture, SDL_TRUE, colorkey);

	SDL_Texture* result = SDL_CreateTextureFromSurface(renderer, picture);
	SDL_FreeSurface(picture);

	return result;
}

///////////////////////////////////////////////////////////////
// 游戏逻辑

// 移动小球
void Window::MoveBall()
{
	// 将球的当前速度添加到它的位置以移动
	// Ball.GainLocation().x += Ball.getSpeed_x();
	// Ball.GainLocation().y += Ball.getSpeed_y();
	Ball.setLocation_XY(Ball.getLocation().x + Ball.getSpeed_x(), Ball.getLocation().y + Ball.getSpeed_y());

	// 如果球向左移动，判断它是否击中了墙，如果是，改变它的方向；向右做同样处理
	if (((Ball.getSpeed_x() <0) && CheckWallCollisions(Ball, LEFT)) ||
		((Ball.getSpeed_x() > 0) && CheckWallCollisions(Ball, RIGHT)))
	{
		Ball.setSpeed_x(-Ball.getSpeed_x());
	}
}

// 检测是否碰到Wall
bool Window::CheckWallCollisions(GameObject& Object, Directions dir)
{
	int temp_x;		// 存储移动后实体的位置
	switch (dir)
	{
	case LEFT:
		temp_x = Object.getLocation().x - Object.getSpeed_x();
		break;
	case RIGHT:
		{
			// 需要添加实体的宽度来获得它的右方向坐标
			temp_x = (Object.getLocation().x + Object.getLocation().w) + Object.getSpeed_x();
		}
		break;
	case UP:
		break;
	case DOWN:
		break;
	default:
		break;
	}

	if ((temp_x <= 0) ||
		temp_x >= WINDOW_WIDTH)
	{
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////

Window::~Window(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
