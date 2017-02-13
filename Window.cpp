#include "stdafx.h"
#include "Window.h"
#include <Windows.h>
#include <stdint.h>


Window::Window(void):
	win(nullptr), renderer(nullptr)
{
	IsStarted = true;
	// ��ʼ��SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(1);
	}
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	// ��������
	win = SDL_CreateWindow(WINDOW_CAPTION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	// ������Ⱦ��(ʹ��Ӳ��������Ⱦ���뵱ǰ��ʾ��ͬ��)
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// ������Դ�ļ�
	LoadResouceFile();
	// ��ʼ����Ϸ����
	Init();
}

void Window::Init()
{
	// �������١���ʼλ�õ�
	Ball.setSpeed_x(2);
	Ball.setSpeed_y(0);
	int Position_x = (WINDOW_WIDTH / 2);
	int Position_y = (WINDOW_HEIGHT / 2);
	SDL_Rect rect0 = {
		Position_x, Position_y, Ball.getGeometry().w, Ball.getGeometry().h
	};
	Ball.setLocation(rect0);

	// ���õ�����ҽ��١�λ�õ�
	PaddleC.setSpeed_x(COMPUTER_SPEED);
	Position_x = (WINDOW_WIDTH / 2) - (PaddleC.getGeometry().w / 2);
	SDL_Rect rect = {
		Position_x, COMPUTER_Y, PaddleC.getGeometry().w, PaddleC.getGeometry().h
	};
	PaddleC.setLocation(rect);

	// ������ҽ��١�λ�õ�
	PaddleP.setSpeed_x(PLAYER_SPEED);
	rect.x = (WINDOW_WIDTH / 2) - (PaddleC.getGeometry().w / 2);
	rect.y = PLAYER_Y;
	rect.w = PaddleP.getGeometry().w;
	rect.h = PaddleP.getGeometry().h;
	PaddleP.setLocation(rect);
}

// ������Ϣѭ��
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

// refresh����Ļ��
void Window::Refresh()
{
	SDL_RenderPresent(renderer);
}

// ����ͼ��λ����Ϣ
void Window::Update()
{
	SDL_RenderClear(renderer);
	// do something
	if (IsStarted)
	{
		// ��ȡ����ͼ�����ԣ���С��
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
	// ��������
	bg = IMG_LoadTexture(renderer, "res/background.bmp");
	// ��
	Ball.setTexture(Load_image("res/ball.png"));
	// ���ԡ���Ұ�
	SDL_Texture* texture = Load_image("res/paddle.png");
	PaddleC.setTexture(texture);
	PaddleP.setTexture(texture);
}

// ����ͼ������
SDL_Texture* Window::Load_image(std::string FilePath)
{
	SDL_Surface* picture = nullptr;

	picture = IMG_Load(FilePath.c_str());
	// ���ñ���͸��
	Uint32 colorkey = SDL_MapRGB(picture->format, 0, 0, 0);
	SDL_SetColorKey(picture, SDL_TRUE, colorkey);

	SDL_Texture* result = SDL_CreateTextureFromSurface(renderer, picture);
	SDL_FreeSurface(picture);

	return result;
}

///////////////////////////////////////////////////////////////
// ��Ϸ�߼�

// �ƶ�С��
void Window::MoveBall()
{
	// ����ĵ�ǰ�ٶ���ӵ�����λ�����ƶ�
	// Ball.GainLocation().x += Ball.getSpeed_x();
	// Ball.GainLocation().y += Ball.getSpeed_y();
	Ball.setLocation_XY(Ball.getLocation().x + Ball.getSpeed_x(), Ball.getLocation().y + Ball.getSpeed_y());

	// ����������ƶ����ж����Ƿ������ǽ������ǣ��ı����ķ���������ͬ������
	if (((Ball.getSpeed_x() <0) && CheckWallCollisions(Ball, LEFT)) ||
		((Ball.getSpeed_x() > 0) && CheckWallCollisions(Ball, RIGHT)))
	{
		Ball.setSpeed_x(-Ball.getSpeed_x());
	}
}

// ����Ƿ�����Wall
bool Window::CheckWallCollisions(GameObject& Object, Directions dir)
{
	int temp_x;		// �洢�ƶ���ʵ���λ��
	switch (dir)
	{
	case LEFT:
		temp_x = Object.getLocation().x - Object.getSpeed_x();
		break;
	case RIGHT:
		{
			// ��Ҫ���ʵ��Ŀ������������ҷ�������
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
