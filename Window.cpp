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
	// ��ʼ��SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(1);
	}
	// ��ʼ��ͼ���
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	// ��ʼ�������
	TTF_Init();
	Font = TTF_OpenFont("res/SourceSansPro-Regular.ttf", 20);

	// ��������
	win = SDL_CreateWindow(WINDOW_CAPTION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	// ������Ⱦ��(ʹ��Ӳ��������Ⱦ���뵱ǰ��ʾ��ͬ��)
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// ��ʼ�����������
	srand(unsigned int (time(0)));

	// ������Դ�ļ�
	LoadResouceFile();
	// ��ʼ����Ϸ����
	Init();
}

void Window::Init()
{
	ResetGame();

	// ���õ�����ҽ��١�λ�õ�
	PaddleC.setSpeed_x(COMPUTER_SPEED);
	int Position_x = (WINDOW_WIDTH / 2) - (PaddleC.getGeometry().w / 2);
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

void Window::ResetGame()
{
	// �������١���ʼλ�õ�
	Ball.setSpeed_x(5);
	Ball.setSpeed_y(-3);
	int Position_x = (WINDOW_WIDTH / 2);
	int Position_y = (WINDOW_HEIGHT / 2);
	SDL_Rect rect0 = {
		Position_x, Position_y, Ball.getGeometry().w, Ball.getGeometry().h
	};
	Ball.setLocation(rect0);
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
			int speed = 0;
			switch (event.type)
			{
			case SDL_QUIT:
				gameRunning = false;
				break;
			case SDL_KEYDOWN:	// ���̰����¼�
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
								// ������Ϸ
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

		// ������Ժ���ҷ���
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

// ������Ƿ����ײ����
bool Window::CheckBallCollisions(GameObject& Object)
{
	// ��ʱ�������Ա��ִ�������Ϳɶ���
	int ball_x = Ball.getLocation().x;
	int ball_y = Ball.getLocation().y;
	int ball_width = Ball.getLocation().w;
	int ball_height = Ball.getLocation().h;
	int ball_speed = Ball.getSpeed_y();

	int paddle_x = Object.getLocation().x;
	int paddle_y = Object.getLocation().y;
	int paddle_width = Object.getLocation().w;
	int paddle_height = Object.getLocation().h;

	// �����ҵĽ�(ͨ���������λ���ж�)
	if (Object.getLocation().y == PLAYER_Y)
	{
		// ������Ƿ�����ҽ���Y��Χ��
		if ((ball_speed > 0) && (ball_y + ball_height >= paddle_y) &&
			(ball_y + ball_height <= paddle_y + paddle_height))
		{
			// ���ball�ڽ���X��Χ�ڣ�����true
			if ((ball_x <= paddle_x + paddle_width) &&
				(ball_x + ball_width >= paddle_x))
			{
				return true;
			}
		}
	}
	else  // ���ԵĽ�
	{
		// ������Ƿ��ڵ��Խ���Y��Χ��
		if ((ball_speed < 0) && (ball_y >= paddle_y) &&
			(ball_y <= paddle_y + paddle_height))
		{
			// ������ڽ���x��Χ�ڣ�����true
			if ((ball_x <= paddle_x + paddle_width) &&
				(ball_x + ball_width >= paddle_x))
			{
				return true;
			}
		}
	}

	return false;
}

// �ƶ�С���������ײ�¼��Ĵ����߼�����������������ı�С���ٶȵ�
void Window::HandleBall()
{
	// ��ʼ�ƶ�С��
	MoveBall();

	// ����Ƿ�����ҵĽ�������ײ�����������ײ��Ҫ�ı�С����ٶ�
	if (CheckBallCollisions(PaddleP))
	{
		// ��ȡ��ͽ��ڵ�ǰλ�õ�����λ��
		int ball_center = Ball.getLocation().x + Ball.getLocation().w / 2;
		int paddle_center = PaddleP.getLocation().x + PaddleP.getLocation().w / 2;

		// ball_center��paddle_center����Ծ���
		int distance = ball_center - paddle_center;

		// ���ݾཬ���ĵľ�������x�ٶ�
		Ball.setSpeed_x(distance / BALL_SPEED_MODIFIER);
		Ball.setSpeed_y(-Ball.getSpeed_y());
	}

	// ����Ƿ�����ԵĽ�������ײ�����������ײ�ı�С����ٶ�
	if (CheckBallCollisions(PaddleC))
	{
		int ball_center = Ball.getLocation().x + Ball.getLocation().w / 2;
		int paddle_center = PaddleC.getLocation().x + PaddleC.getLocation().w / 2;

		int distance = ball_center - paddle_center;

		Ball.setSpeed_x(distance / BALL_SPEED_MODIFIER);
		Ball.setSpeed_y(-Ball.getSpeed_y());
	}

	// ����С��Y����С��0�����WINDOW_HEIGHTʱ���߼�
	if (Ball.getLocation().y < 0)
	{
		// �������
		HandlePlayerScore();
	}
	if ((Ball.getLocation().y + Ball.getLocation().h) > WINDOW_HEIGHT)
	{
		// ����������
		HandleComputerScore();
	}
}

// ���������ҵĽ���ʹ֮���ܻ�
/*
ѡ��һ������1��3֮����������1��ʾ������ߣ�2��ʾ���ģ�3��ʾ�ұߣ���������������ǲ��ֻ����������
���С���Ƿ�ı����ٶȡ�����ٶȸı䣬��Ӧ�������µľ��ߣ�����ֻ�Ǽ�����������Ľ������ƶ���ֱ�������ʵ����������·��
�л������ٴθı䷽��
*/
void Window::HandleComputerPaddle()
{
	// ��һ�ε����������ʱ����¼��ĵ�ǰ�ٶȣ������������ǲ��ֻ���
	static int last_speed = Ball.getSpeed_x();
	static int decision = rand() % 3 + 1;

	int computer_x;
	int ball_center = Ball.getLocation().x + Ball.getLocation().w / 2;

	// ���С���Ƿ�ı��˷���
	if (last_speed != Ball.getSpeed_x())
	{
		// ���С���ٶȣ�x���������˸ı䣬���¾���
		decision = rand() % 3 + 1;
		last_speed = Ball.getSpeed_x();
	}

	// ���ݾ������ý���x����
	switch (decision)
	{
	case 1:		// ��
		computer_x = PaddleC.getLocation().x;
		break;
	case 2:		// ��
		computer_x = PaddleC.getLocation().x + PaddleC.getLocation().w / 2;
		break;
	case 3:		// ��
		computer_x = PaddleC.getLocation().x + PaddleC.getLocation().w ;
		break;
	default:
		break;
	}

	// ��ֹ��������������ƶ�
	if (abs(computer_x - ball_center) < 10)
		return ;

	// ���ڽ������
	if (computer_x > ball_center)
	{
		// ȷ��û��������Ļ�����
		if (!(CheckWallCollisions(PaddleC, LEFT)))
			PaddleC.setLocation_XY(PaddleC.getLocation().x - COMPUTER_SPEED, PaddleC.getLocation().y);
	}
	// ���ڽ����ұ�
	if (computer_x < ball_center)
	{
		// ȷ��û��������Ļ���ұ�
		if (!(CheckWallCollisions(PaddleC, RIGHT)))
			PaddleC.setLocation_XY(PaddleC.getLocation().x + COMPUTER_SPEED, PaddleC.getLocation().y);
	}
}

// ������ҷ���
void Window::HandlePlayerScore()
{
	// Increase score
	PlayerScore++;
	
	// Reset ball
	Ball.setSpeed_x(0);
	Ball.setSpeed_y(0);
	Ball.setLocation_XY((WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));

	// �������Ƿ�Ӯ��
	if (PlayerScore == 10)
	{
		ComputerScore = 0;
		PlayerScore = 0;
	}
}

// ������Է���
void Window::HandleComputerScore()
{
	// Increase score
	ComputerScore++;

	// Reset ball
	Ball.setSpeed_x(0);
	Ball.setSpeed_y(0);
	Ball.setLocation_XY((WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));

	// �������Ƿ�Ӯ��
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
