/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include <random>
#include "SpriteEffect.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	textHandler("Images/Fixedsys16x28.bmp")
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	board.Update();
	if(wnd.kbd.KeyIsPressed(VK_RIGHT) && !keyIsDown){
		board.MoveAllCells(Directions::RIGHT);
		keyIsDown = true;
	}
	else if(wnd.kbd.KeyIsPressed(VK_LEFT) && !keyIsDown){
		board.MoveAllCells(Directions::LEFT);
		keyIsDown = true;
	}
	else if(wnd.kbd.KeyIsPressed(VK_UP) && !keyIsDown){
		board.MoveAllCells(Directions::UP);
		keyIsDown = true;
	}
	else if(wnd.kbd.KeyIsPressed(VK_DOWN) && !keyIsDown){
		board.MoveAllCells(Directions::DOWN);
		keyIsDown = true;
	}else if(!CheckControlKeys() && keyIsDown)
		keyIsDown = false;
}

bool Game::CheckControlKeys(){
	return wnd.kbd.KeyIsPressed(VK_RIGHT) || wnd.kbd.KeyIsPressed(VK_LEFT) 
		|| wnd.kbd.KeyIsPressed(VK_UP) || wnd.kbd.KeyIsPressed(VK_DOWN);
}

void Game::ComposeFrame()
{
	textHandler.DrawText("FPS:" + std::to_string((int)(1000 / ft.Mark())), { 0,0 }, Colors::White, gfx);
	board.Draw(gfx, textHandler);
}
