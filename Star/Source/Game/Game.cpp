#include "Game.h"

#include "Engine/IGraphics.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"
#include "Engine/IRenderable.h"
#include "Engine/IInput.h"

#include <ctime>
#include <math.h>

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

constexpr float Pie = 3.14159265359f;
constexpr float TwoPies = Pie * 2.0f;
constexpr float DeltaTime = 0.016f;
constexpr float SpinSpeed = 0.1f;
constexpr float WinTolerance = Pie / 10.0f;
bool IsGameOver;
int RingColourToFind;
int RingToCheck;






IApplication* GetApplication(IGraphics* Graphics, IInput* Input)
{
	return new Game(Graphics, Input);
}

Game::Game(IGraphics* GraphicsIn, IInput* InputIn) : IApplication(GraphicsIn, InputIn), Rings(), OuterRings(), InnerRings(), MiddleRings(), GameOver(), Arrow(nullptr), SelectedRing(), State()
{

}

Game::~Game()
{
}

bool Game::IsValid()
{
	return true;
}

bool Game::Load()
{

	ITexture* ArrowTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow.dds");
	ITexture* ArrowGreenTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow-green.dds");
	ITexture* ArrowRedTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow-red.dds");
	ITexture* ArrowPinkTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow-pink.dds");
	IShader* ArrowShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowTexture);
	IShader* ArrowGreenShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowGreenTexture);
	IShader* ArrowRedShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowRedTexture);
	IShader* ArrowPinkShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowPinkTexture);

	IShader* ArrowList[4] = { ArrowShader ,ArrowRedShader,ArrowGreenShader,ArrowPinkShader };
	
	

	

	ITexture* BlueArcTexture = Graphics->CreateTexture(L"Resource/Textures/Arc-blue.dds");
	IShader* BlueArcShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", BlueArcTexture);
	ITexture* RedArcTexture = Graphics->CreateTexture(L"Resource/Textures/Arc-red.dds");
	IShader* RedArcShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", RedArcTexture);
	ITexture* GreenArcTexture = Graphics->CreateTexture(L"Resource/Textures/Arc-green.dds");
	IShader* GreenArcShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", GreenArcTexture);
	ITexture* PinkArcTexture = Graphics->CreateTexture(L"Resource/Textures/Arc-pink.dds");
	IShader* PinkArcShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", PinkArcTexture);

	IShader* ShaderList[4] = { BlueArcShader,RedArcShader,GreenArcShader,PinkArcShader };

	ITexture* GameOverTexture = Graphics->CreateTexture(L"Resource/Textures/Game-Over.dds");
	IShader* GameOverShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", GameOverTexture);
	



	float radius = 250;
	int randomNum;
	int angle;
	float circleX;
	float circleY;
	srand(time(0));

	for (int x = 0; x <= OuterRing::NumOuterRings - 1; x++)
	{
	
		
		
		angle = (360/OuterRing::NumOuterRings *x +1);
		circleX=radius*cos(angle *((Pie *2)/360 ) );
		circleY=radius*sin(angle * ((Pie * 2) / 360));
	
		randomNum =rand() % 4;
		OuterRings[static_cast<unsigned int>(x)] = Graphics->CreateBillboard(ShaderList[randomNum]);
		OuterRings[static_cast<unsigned int>(x)]->SetScale(0.9, 1.1);
		OuterRings[static_cast<unsigned int>(x)]->SetPosition(circleX, circleY);
		OuterRings[static_cast<unsigned int>(x)]->SetRotation ((((Pie*2)/ OuterRing::NumOuterRings) * x)+1.5);
		OuterRings[static_cast<unsigned int>(x)]->SetColorNum(randomNum);
	
	}

	radius = 150;
	for (int x = 0; x <= MiddleRing::NumMiddleRings - 1; x++)
	{



		angle = (360 / MiddleRing::NumMiddleRings * x + 1);
		circleX = radius * cos(angle * ((Pie * 2) / 360));
		circleY = radius * sin(angle * ((Pie * 2) / 360));

		randomNum = rand() % 4;
		MiddleRings[static_cast<unsigned int>(x)] = Graphics->CreateBillboard(ShaderList[randomNum]);
		MiddleRings[static_cast<unsigned int>(x)]->SetScale(0.5, 0.7);
		MiddleRings[static_cast<unsigned int>(x)]->SetPosition(circleX, circleY);
		MiddleRings[static_cast<unsigned int>(x)]->SetRotation((((Pie * 2) / MiddleRing::NumMiddleRings) * x) + 1.5);
		MiddleRings[static_cast<unsigned int>(x)]->SetColorNum(randomNum);

	}

	radius = 75;
	for (int x = 0; x <= InnerRing::NumInnerRings - 1; x++)
	{



		angle = (360 / InnerRing::NumInnerRings * x + 1);
		circleX = radius * cos(angle * ((Pie * 2) / 360));
		circleY = radius * sin(angle * ((Pie * 2) / 360));

		randomNum = rand() % 4;
		InnerRings[static_cast<unsigned int>(x)] = Graphics->CreateBillboard(ShaderList[randomNum]);
		InnerRings[static_cast<unsigned int>(x)]->SetScale(0.275, 0.4);
		InnerRings[static_cast<unsigned int>(x)]->SetPosition(circleX, circleY);
		InnerRings[static_cast<unsigned int>(x)]->SetRotation((((Pie * 2) / InnerRing::NumInnerRings) * x) + 1.5);
		InnerRings[static_cast<unsigned int>(x)]->SetColorNum(randomNum);
		
	}
	randomNum = rand() % (OuterRing::NumOuterRings - 1);

	Arrow = Graphics->CreateBillboard(ArrowList[OuterRings[static_cast<unsigned int>(randomNum)]->GetColorNum()]);
	RingColourToFind = OuterRings[static_cast<unsigned int>(randomNum)]->GetColorNum();
	Arrow->SetPosition(0, 100);
	Arrow->SetRotation(3.15);
	
	GameOver = Graphics->CreateBillboard(GameOverShader);
	GameOver->SetScale(0.7,0.7);
	GameOver->SetPosition(0, 3000);
	GameOver->SetRotation(Pie);

	std::srand(static_cast<unsigned int>(std::time(0)));
	RingToCheck = 2;
	IsGameOver = false;
	SelectedRing = RingLayer::Outer;
	State = GameState::Playing;
	
	return true;
}

void Game::Update()
{

	if (State == GameState::Playing)
	{
		UpdateRingSelection();
		CheckRingColor();
		RestartGame();
	}

}

void Game::Cleanup()
{
	
}

void Game::UpdateRingSelection()
{

	if (Input->IsPressed(InputAction::ShoulderButtonLeft))
	{

		UpdateRingPostionBackwards();
	}
	else if (Input->IsPressed(InputAction::ShoulderButtonRight))
	{
		UpdateRingPostionForward();
	}
		
}

void Game::UpdateRingPostionForward()
{
	int tempX;
	int tempY;
	float tempRotation;
	if (IsGameOver == false)
	{
		if (SelectedRing == RingLayer::Outer)
		{
			tempX = OuterRings[static_cast<unsigned int>(OuterRing::NumOuterRings - 1)]->GetTransform().PositionX;
			tempY = OuterRings[static_cast<unsigned int>(OuterRing::NumOuterRings - 1)]->GetTransform().PositionY;
			tempRotation = OuterRings[static_cast<unsigned int>(OuterRing::NumOuterRings - 1)]->GetTransform().Rotation;

			for (int x = OuterRing::NumOuterRings - 1; x >= 0; x--)
			{
				if (x == 0)
				{
					OuterRings[static_cast<unsigned int>(x)]->SetPosition(tempX, tempY);
					OuterRings[static_cast<unsigned int>(x)]->SetRotation(tempRotation);
				}
				else
				{
					OuterRings[static_cast<unsigned int>(x)]->SetPosition(OuterRings[static_cast<unsigned int>(x - 1)]->GetTransform().PositionX, OuterRings[static_cast<unsigned int>(x - 1)]->GetTransform().PositionY);
					OuterRings[static_cast<unsigned int>(x)]->SetRotation(OuterRings[static_cast<unsigned int>(x - 1)]->GetTransform().Rotation);
				}

			}
		}

		if (SelectedRing == RingLayer::Middle)
		{
			tempX = MiddleRings[static_cast<unsigned int>(MiddleRing::NumMiddleRings - 1)]->GetTransform().PositionX;
			tempY = MiddleRings[static_cast<unsigned int>(MiddleRing::NumMiddleRings - 1)]->GetTransform().PositionY;
			tempRotation = MiddleRings[static_cast<unsigned int>(MiddleRing::NumMiddleRings - 1)]->GetTransform().Rotation;

			for (int x = MiddleRing::NumMiddleRings - 1; x >= 0; x--)
			{
				if (x == 0)
				{
					MiddleRings[static_cast<unsigned int>(x)]->SetPosition(tempX, tempY);
					MiddleRings[static_cast<unsigned int>(x)]->SetRotation(tempRotation);
				}
				else
				{
					MiddleRings[static_cast<unsigned int>(x)]->SetPosition(MiddleRings[static_cast<unsigned int>(x - 1)]->GetTransform().PositionX, MiddleRings[static_cast<unsigned int>(x - 1)]->GetTransform().PositionY);
					MiddleRings[static_cast<unsigned int>(x)]->SetRotation(MiddleRings[static_cast<unsigned int>(x - 1)]->GetTransform().Rotation);
				}

			}
		}
		if (SelectedRing == RingLayer::Inner)
		{
			tempX = InnerRings[static_cast<unsigned int>(InnerRing::NumInnerRings - 1)]->GetTransform().PositionX;
			tempY = InnerRings[static_cast<unsigned int>(InnerRing::NumInnerRings - 1)]->GetTransform().PositionY;
			tempRotation = InnerRings[static_cast<unsigned int>(InnerRing::NumInnerRings - 1)]->GetTransform().Rotation;

			for (int x = InnerRing::NumInnerRings - 1; x >= 0; x--)
			{
				if (x == 0)
				{
					InnerRings[static_cast<unsigned int>(x)]->SetPosition(tempX, tempY);
					InnerRings[static_cast<unsigned int>(x)]->SetRotation(tempRotation);
				}
				else
				{
					InnerRings[static_cast<unsigned int>(x)]->SetPosition(InnerRings[static_cast<unsigned int>(x - 1)]->GetTransform().PositionX, InnerRings[static_cast<unsigned int>(x - 1)]->GetTransform().PositionY);
					InnerRings[static_cast<unsigned int>(x)]->SetRotation(InnerRings[static_cast<unsigned int>(x - 1)]->GetTransform().Rotation);
				}

			}

		}




		RingToCheck++;
		if (RingToCheck > OuterRing::NumOuterRings - 1)
		{
			RingToCheck = 0;
		}
	}
}

void Game::UpdateRingPostionBackwards()
{
	int tempX;
	int tempY;
	float tempRotation;
	if (IsGameOver == false)
	{
		if (SelectedRing == RingLayer::Outer)
		{
			tempX = OuterRings[static_cast<unsigned int>(OuterRing::O1)]->GetTransform().PositionX;
			tempY = OuterRings[static_cast<unsigned int>(OuterRing::O1)]->GetTransform().PositionY;
			tempRotation = OuterRings[static_cast<unsigned int>(OuterRing::O1)]->GetTransform().Rotation;
			for (int x = 0; x <= OuterRing::NumOuterRings - 1; x++)
			{
				if (x == OuterRing::NumOuterRings - 1)
				{
					OuterRings[static_cast<unsigned int>(x)]->SetPosition(tempX, tempY);
					OuterRings[static_cast<unsigned int>(x)]->SetRotation(tempRotation);
				}
				else
				{
					OuterRings[static_cast<unsigned int>(x)]->SetPosition(OuterRings[static_cast<unsigned int>(x + 1)]->GetTransform().PositionX, OuterRings[static_cast<unsigned int>(x + 1)]->GetTransform().PositionY);
					OuterRings[static_cast<unsigned int>(x)]->SetRotation(OuterRings[static_cast<unsigned int>(x + 1)]->GetTransform().Rotation);
				}

			}
		}
		if (SelectedRing == RingLayer::Middle)
		{
			tempX = MiddleRings[static_cast<unsigned int>(MiddleRing::M1)]->GetTransform().PositionX;
			tempY = MiddleRings[static_cast<unsigned int>(MiddleRing::M1)]->GetTransform().PositionY;
			tempRotation = MiddleRings[static_cast<unsigned int>(MiddleRing::M1)]->GetTransform().Rotation;
			for (int x = 0; x <= MiddleRing::NumMiddleRings - 1; x++)
			{
				if (x == OuterRing::NumOuterRings - 1)
				{
					MiddleRings[static_cast<unsigned int>(x)]->SetPosition(tempX, tempY);
					MiddleRings[static_cast<unsigned int>(x)]->SetRotation(tempRotation);
				}
				else
				{
					MiddleRings[static_cast<unsigned int>(x)]->SetPosition(MiddleRings[static_cast<unsigned int>(x + 1)]->GetTransform().PositionX, MiddleRings[static_cast<unsigned int>(x + 1)]->GetTransform().PositionY);
					MiddleRings[static_cast<unsigned int>(x)]->SetRotation(MiddleRings[static_cast<unsigned int>(x + 1)]->GetTransform().Rotation);
				}

			}
		}
		if (SelectedRing == RingLayer::Inner)
		{
			tempX = InnerRings[static_cast<unsigned int>(InnerRing::I1)]->GetTransform().PositionX;
			tempY = InnerRings[static_cast<unsigned int>(InnerRing::I1)]->GetTransform().PositionY;
			tempRotation = InnerRings[static_cast<unsigned int>(InnerRing::I1)]->GetTransform().Rotation;
			for (int x = 0; x <= InnerRing::NumInnerRings - 1; x++)
			{
				if (x == OuterRing::NumOuterRings - 1)
				{
					InnerRings[static_cast<unsigned int>(x)]->SetPosition(tempX, tempY);
					InnerRings[static_cast<unsigned int>(x)]->SetRotation(tempRotation);
				}
				else
				{
					InnerRings[static_cast<unsigned int>(x)]->SetPosition(InnerRings[static_cast<unsigned int>(x + 1)]->GetTransform().PositionX, InnerRings[static_cast<unsigned int>(x + 1)]->GetTransform().PositionY);
					InnerRings[static_cast<unsigned int>(x)]->SetRotation(InnerRings[static_cast<unsigned int>(x + 1)]->GetTransform().Rotation);
				}

			}
		}





		RingToCheck--;
		if (RingToCheck < 0)
		{
			RingToCheck = 7;
		}
	}
}

void Game::CheckRingColor()
{

	ITexture* ArrowTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow.dds");
	ITexture* ArrowGreenTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow-green.dds");
	ITexture* ArrowRedTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow-red.dds");
	ITexture* ArrowPinkTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow-pink.dds");
	IShader* ArrowShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowTexture);
	IShader* ArrowGreenShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowGreenTexture);
	IShader* ArrowRedShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowRedTexture);
	IShader* ArrowPinkShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowPinkTexture);

	IShader* ArrowList[4] = { ArrowShader ,ArrowRedShader,ArrowGreenShader,ArrowPinkShader };

	int randomNum;

	if (Input->IsPressed(InputAction::ButtonBottom) && IsGameOver == false)
	{
		if (SelectedRing == RingLayer::Outer)
		{
			if (OuterRings[static_cast<unsigned int>(RingToCheck)]->GetColorNum() == RingColourToFind)
			{
				for (int x = 0; x <= OuterRing::NumOuterRings - 1; x++)
				{
					OuterRings[static_cast<unsigned int>(x)]->SetPosition(1000, 3000);

				}
				randomNum = rand() % (MiddleRing::NumMiddleRings - 1);
				Arrow->SetPosition(0, 20000);
				Arrow = Graphics->CreateBillboard(ArrowList[MiddleRings[static_cast<unsigned int>(randomNum)]->GetColorNum()]);
				RingColourToFind = MiddleRings[static_cast<unsigned int>(randomNum)]->GetColorNum();
				Arrow->SetPosition(0, -10);
				Arrow->SetRotation(3.15);
				

				SelectedRing = RingLayer::Middle;
				RingToCheck = 2;
			}
			else
			{
				IsGameOver = true;
				GameOver->SetPosition(0,380);
			}
		}
		else if (SelectedRing == RingLayer::Middle)
		{
			if (MiddleRings[static_cast<unsigned int>(RingToCheck)]->GetColorNum() == RingColourToFind)
			{
				for (int x = 0; x <= MiddleRing::NumMiddleRings - 1; x++)
				{
					MiddleRings[static_cast<unsigned int>(x)]->SetPosition(1000, 3000);

				}

				randomNum = rand() % (InnerRing::NumInnerRings - 1);
				Arrow->SetPosition(0, 20000);
				Arrow = Graphics->CreateBillboard(ArrowList[InnerRings[static_cast<unsigned int>(randomNum)]->GetColorNum()]);
				RingColourToFind = InnerRings[static_cast<unsigned int>(randomNum)]->GetColorNum();
				Arrow->SetPosition(0, -100);
				Arrow->SetRotation(3.15);


				SelectedRing = RingLayer::Inner;
				RingToCheck = 2;
			}
			else
			{
				IsGameOver = true;
				GameOver->SetPosition(0, 380);
			}
		}
		else if (SelectedRing == RingLayer::Inner)
		{
			if (InnerRings[static_cast<unsigned int>(RingToCheck)]->GetColorNum() == RingColourToFind)
			{
				for (int x = 0; x <= InnerRing::NumInnerRings - 1; x++)
				{
					InnerRings[static_cast<unsigned int>(x)]->SetPosition(1000, 3000);

				}
				
				Graphics->clearStuff();
				Game::Load();
				
			}
			else
			{
				IsGameOver = true;
				GameOver->SetPosition(0, 380);
			}
		}
	}
}
void Game::RestartGame()
{
	if (Input->IsPressed(InputAction::ButtonLeft) && IsGameOver == true)
	{
		Graphics->clearStuff();
		Game::Load();
	}
}

