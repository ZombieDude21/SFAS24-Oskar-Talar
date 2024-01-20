
#pragma once

#include "Engine/IApplication.h"

class IGraphics;
class ITexture;
class IShader;
class IRenderable;


enum OuterRing { O1, O2, O3, O4,O5,O6,O7,O8, NumOuterRings };
enum InnerRing { I1, I2, I3, I4, I5, I6, I7, I8, NumInnerRings };
enum MiddleRing { M1, M2, M3, M4, M5, M6, M7, M8, NumMiddleRings };
enum RingLayer { Outer, Middle, Inner, NumRings };
enum GameState { Setup, Playing, Test,Stop, NumModes };
static const unsigned int NumberOfRings = static_cast<int>(NumRings);
static const unsigned int NumberOfOuterRings = static_cast<int>(NumOuterRings);
static const unsigned int NumberOfInnerRings = static_cast<int>(NumInnerRings);
static const unsigned int NumberOfMiddleRings = static_cast<int>(NumMiddleRings);

class Game : public IApplication
{
public:

	Game(IGraphics* Graphics, IInput* InputIn);
	virtual ~Game();

	virtual bool IsValid();
	virtual bool Load();
	virtual void Update();
	virtual void Cleanup();

private:

	void SetupEachRing();
	void UpdateRingSelection();
	void UpdateRingPostionForward();
	void UpdateRingPostionBackwards();
	void CheckRingColor();
	void RestartGame();

	IRenderable* Rings[NumberOfRings];
	IRenderable* Arrow;
	IRenderable* OuterRings[NumberOfOuterRings];
	IRenderable* InnerRings[NumberOfInnerRings];
	IRenderable* MiddleRings[NumberOfMiddleRings];
	IRenderable* GameOver;
	RingLayer SelectedRing;
	GameState State;
};

