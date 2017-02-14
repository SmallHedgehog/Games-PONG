#include "stdafx.h"
#include "Window.h"
#include <Windows.h>
#include <stdint.h>


Window::Window(void):
	win(nullptr), renderer(nullptr), Font(nullptr)
{
	IsStarted	  =  true;
	Pause		  =  false;
	PlayerScore	  =  0;
	ComputerScore =  0;
	// 初始化SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(1);
	}
	// 初始化图像库
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	// 初始化字体库
	TTF_Init();
	Font = TTF_OpenFont("res/SourceSansPro-Regular.ttf", 20);

	// 创建窗口
	win = SDL_CreateWindow(WINDOW_CAPTION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	// 创建渲染器(使用硬件加速渲染和与当前显示器同步)
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// 初始化随机数种子
	srand(unsigned int (time(0)));

	// 加载资源文件
	LoadResouceFile();
	// 初始化游戏对象
	Init();
}

void Window::Init()
{
	ResetGame();

	// 设置电脑玩家浆速、位置等
	PaddleC.setSpeed_x(COMPUTER_SPEED);
	int Position_x = (WINDOW_WIDTH / 2) - (PaddleC.getGeometry().w / 2);
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

void Window::ResetGame()
{
	// 设置球速、初始位置等
	Ball.setSpeed_x(5);
	Ball.setSpeed_y(-3);
	int Position_x = (WINDOW_WIDTH / 2);
	int Position_y = (WINDOW_HEIGHT / 2);
	SDL_Rect rect0 = {
		Position_x, Position_y, Ball.getGeometry().w, Ball.getGeometry().h
	};
	Ball.setLocation(rect0);
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
			int speed = 0;
			switch (event.type)
			{
			case SDL_QUIT:
				gameRunning = false;
				break;
			case SDL_KEYDOWN:	// 键盘按键事件
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_UP:
						break;
					case SDLK_LEFT:
						speed = -PLAYER_SPEED;
						break;
					case SDLK_RIGHT:
						speed = PLAYER_SPEED;
						break;
					case SDLK_DOWN:
						break;
					case SDLK_SPACE:
						{
							Pause = !Pause;
							if (Pause)
							{
								// 重置游戏
								ResetGame();
							}
						}
						break;
					default:
						break;
					}
				}
				break;
			default:
				break;
			}

			PaddleP.setLocation_XY(PaddleP.getLocation().x + speed, PaddleP.getLocation().y);
		}

		HandleBall();
		HandleComputerPaddle();

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

		// 输出电脑和玩家分数
		char c_score[50];
		sprintf_s(c_score, "Computer Score: %d", ComputerScore);

		char p_score[50];
		sprintf_s(p_score, "Player Score: %d", PlayerScore);

		width = 0, height = 0;
		SDL_Texture* c_textre = Draw_text(c_score);
		SDL_QueryTexture(c_textre, NULL, NULL, &width, &height);
		rect.x = COMPUTER_SCORE_X;
		rect.y = COMPUTER_SCORE_Y;
		rect.w = width;
		rect.h = height;
		SDL_RenderCopy(renderer, c_textre, NULL, &rect);

		SDL_Texture* p_textre = Draw_text(p_score);
		SDL_QueryTexture(p_textre, NULL, NULL, &width, &height);
		rect.x = PLAYER_SCORE_X;
		rect.y = PLAYER_SCORE_Y;
		rect.w = width;
		rect.h = height;
		SDL_RenderCopy(renderer, p_textre, NULL, &rect);
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

// 检测球是否会碰撞到浆
bool Window::CheckBallCollisions(GameObject& Object)
{
	// 临时变量，以保持代码整洁和可读性
	int ball_x = Ball.getLocation().x;
	int ball_y = Ball.getLocation().y;
	int ball_width = Ball.getLocation().w;
	int ball_height = Ball.getLocation().h;
	int ball_speed = Ball.getSpeed_y();

	int paddle_x = Object.getLocation().x;
	int paddle_y = Object.getLocation().y;
	int paddle_width = Object.getLocation().w;
	int paddle_height = Object.getLocation().h;

	// 检测玩家的浆(通过纵坐标的位置判断)
	if (Object.getLocation().y == PLAYER_Y)
	{
		// 检测球是否在玩家浆的Y范围内
		if ((ball_speed > 0) && (ball_y + ball_height >= paddle_y) &&
			(ball_y + ball_height <= paddle_y + paddle_height))
		{
			// 如果ball在浆的X范围内，返回true
			if ((ball_x <= paddle_x + paddle_width) &&
				(ball_x + ball_width >= paddle_x))
			{
				return true;
			}
		}
	}
	else  // 电脑的浆
	{
		// 检查球是否在电脑浆的Y范围内
		if ((ball_speed < 0) && (ball_y >= paddle_y) &&
			(ball_y <= paddle_y + paddle_height))
		{
			// 如果球在浆的x范围内，返回true
			if ((ball_x <= paddle_x + paddle_width) &&
				(ball_x + ball_width >= paddle_x))
			{
				return true;
			}
		}
	}

	return false;
}

// 移动小球，做相关碰撞事件的处理逻辑，例如如果碰到浆改变小球速度等
void Window::HandleBall()
{
	// 开始移动小球
	MoveBall();

	// 检测是否与玩家的浆发生碰撞，如果发生碰撞需要改变小球的速度
	if (CheckBallCollisions(PaddleP))
	{
		// 获取球和浆在当前位置的中心位置
		int ball_center = Ball.getLocation().x + Ball.getLocation().w / 2;
		int paddle_center = PaddleP.getLocation().x + PaddleP.getLocation().w / 2;

		// ball_center与paddle_center的相对距离
		int distance = ball_center - paddle_center;

		// 根据距浆中心的距离增加x速度
		Ball.setSpeed_x(distance / BALL_SPEED_MODIFIER);
		Ball.setSpeed_y(-Ball.getSpeed_y());
	}

	// 检测是否与电脑的浆发生碰撞，如果发生碰撞改变小球的速度
	if (CheckBallCollisions(PaddleC))
	{
		int ball_center = Ball.getLocation().x + Ball.getLocation().w / 2;
		int paddle_center = PaddleC.getLocation().x + PaddleC.getLocation().w / 2;

		int distance = ball_center - paddle_center;

		Ball.setSpeed_x(distance / BALL_SPEED_MODIFIER);
		Ball.setSpeed_y(-Ball.getSpeed_y());
	}

	// 处理小球Y坐标小于0或大于WINDOW_HEIGHT时的逻辑
	if (Ball.getLocation().y < 0)
	{
		// 处理玩家
		HandlePlayerScore();
	}
	if ((Ball.getLocation().y + Ball.getLocation().h) > WINDOW_HEIGHT)
	{
		// 处理电脑玩家
		HandleComputerScore();
	}
}

// 处理电脑玩家的浆，使之智能化
/*
选择一个介于1和3之间的随机数。1表示浆的左边，2表示中心，3表示右边，计算机做出关于那部分击球随机决定
检测小球是否改变了速度。如果速度改变，就应该做出新的决策，否则，只是继续将计算机的浆向球移动，直到浆的适当部分在球的路径
中或者球再次改变方向
*/
void Window::HandleComputerPaddle()
{
	// 第一次调用这个函数时，记录球的当前速度，并决定浆的那部分击球
	static int last_speed = Ball.getSpeed_x();
	static int decision = rand() % 3 + 1;

	int computer_x;
	int ball_center = Ball.getLocation().x + Ball.getLocation().w / 2;

	// 检测小球是否改变了方向
	if (last_speed != Ball.getSpeed_x())
	{
		// 如果小球速度（x）方向发生了改变，重新决策
		decision = rand() % 3 + 1;
		last_speed = Ball.getSpeed_x();
	}

	// 根据决策设置浆的x坐标
	switch (decision)
	{
	case 1:		// 左
		computer_x = PaddleC.getLocation().x;
		break;
	case 2:		// 中
		computer_x = PaddleC.getLocation().x + PaddleC.getLocation().w / 2;
		break;
	case 3:		// 右
		computer_x = PaddleC.getLocation().x + PaddleC.getLocation().w ;
		break;
	default:
		break;
	}

	// 防止计算机快速来回移动
	if (abs(computer_x - ball_center) < 10)
		return ;

	// 球在浆的左边
	if (computer_x > ball_center)
	{
		// 确保没有碰到屏幕的左边
		if (!(CheckWallCollisions(PaddleC, LEFT)))
			PaddleC.setLocation_XY(PaddleC.getLocation().x - COMPUTER_SPEED, PaddleC.getLocation().y);
	}
	// 球在浆的右边
	if (computer_x < ball_center)
	{
		// 确保没有碰到屏幕的右边
		if (!(CheckWallCollisions(PaddleC, RIGHT)))
			PaddleC.setLocation_XY(PaddleC.getLocation().x + COMPUTER_SPEED, PaddleC.getLocation().y);
	}
}

// 处理玩家分数
void Window::HandlePlayerScore()
{
	// Increase score
	PlayerScore++;
	
	// Reset ball
	Ball.setSpeed_x(0);
	Ball.setSpeed_y(0);
	Ball.setLocation_XY((WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));

	// 检查玩家是否赢了
	if (PlayerScore == 10)
	{
		ComputerScore = 0;
		PlayerScore = 0;
	}
}

// 处理电脑分数
void Window::HandleComputerScore()
{
	// Increase score
	ComputerScore++;

	// Reset ball
	Ball.setSpeed_x(0);
	Ball.setSpeed_y(0);
	Ball.setLocation_XY((WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));

	// 检查玩家是否赢了
	if (ComputerScore == 10)
	{
		ComputerScore = 0;
		PlayerScore = 0;
	}
}

///////////////////////////////////////////////////////////////

SDL_Texture* Window::Draw_text(std::string message)
{
	SDL_Color color = { 0, 0, 0 };
	SDL_Surface* surface = TTF_RenderText_Blended(Font, message.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	return texture;
}

Window::~Window(void)
{
	TTF_CloseFont(Font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
