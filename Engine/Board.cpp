#include "Board.h"
#include <cmath>
#include <random>

Board::Board(){
	InitSpawn();
}

void Board::Draw(Graphics &gfx, Font ft){
	for(int y = 0; y < height; y++)
		for(int x = 0; x < width; x++)
			gfx.DrawFilledRectBorder(Vei2(x, y) * cellSize + pos, {cellSize, cellSize}, Colors::Gray, Colors::LightGray);

	ft.DrawText("Score:" + std::to_string(score), pos + Vei2(0, -28), Colors::White, gfx);

	if(!cellsAvalible && !moovesAvalible)
		ft.DrawText("Game Over!", pos + Vei2{240, -28}, Colors::White, gfx);
	for(auto &i : cells )
		for(auto &j : i)
			j.Draw(gfx, ft, pos);
}

void Board::InitSpawn(){
	for(int y = 0; y < height; y++){
		cells.push_back({});
		for(int x = 0; x < width; x++)
			cells[y].push_back({{x, y}, 0});
	}
	SpawnCell();
	SpawnCell();
}

void Board::SpawnCell(){
	CheckCells();
	if(cellsAvalible){
		while(true){
			std::mt19937 rng(std::random_device{}());
			std::uniform_int_distribution<int> pos(0, width-1);
			std::uniform_int_distribution<int> val(1, 2);
			int x = pos(rng), y = pos(rng);
			if(cells[y][x].value == 0){
				cells[y][x].value = (int)exp2(val(rng));
				return;
			}
		}
	}
}

void Board::CheckCells(){
	for(auto &i : cells)
		for(auto &c : i)
			if(c.value == 0){
				cellsAvalible = true;
				return;
			}
	cellsAvalible = false;
}

void Board::CheckIfMovesAvalible()
{
	for(int y = 0; y < height; y++)
		for(int x = 0; x < width; x++){
			Cell currentCell = cells[y][x];
			if (cells[y][x].value == 0 || GetClosestCell(currentCell, Directions::RIGHT) != 0
				|| GetClosestCell(currentCell, Directions::LEFT) != 0
				|| GetClosestCell(currentCell, Directions::UP) != 0
				|| GetClosestCell(currentCell, Directions::DOWN) != 0) {
				moovesAvalible = true;
				return;
			}
		}
	moovesAvalible = false;
}

size_t Board::GetClosestCell(Cell &movable, Directions dir){
	size_t offset = 0;
	switch(dir){
		case Directions::RIGHT:
			for(int i = 1; i < width; i++){
				if(movable.pos.x + i >= width)
					return offset;
				else{
					Cell possibleCell = cells[movable.pos.y][movable.pos.x + (size_t)i];
					if(possibleCell.value == 0 || possibleCell.value == movable.value)
						offset = i;
					else
						return offset;
				}
			}
			break;
		case Directions::LEFT:
			for(int i = 1; i < width; i++){
				if(movable.pos.x - i < 0)
					return offset;
				else{
					Cell possibleCell = cells[movable.pos.y][movable.pos.x - (size_t)i];
					if(possibleCell.value == 0 || possibleCell.value == movable.value)
						offset = i;
					else
						return offset;
				}
			}
			break;
		case Directions::UP:
			for(int i = 1; i < width; i++){
				if(movable.pos.y - i < 0)
					return offset;
				else{
					Cell possibleCell = cells[movable.pos.y - (size_t)i][movable.pos.x];
					if(possibleCell.value == 0 || possibleCell.value == movable.value)
						offset = i;
					else
						return offset;
				}
			}
			break;
		case Directions::DOWN:
			for(int i = 1; i < width; i++){
				if(movable.pos.y + i >= width)
					return offset;
				else{
					Cell possibleCell = cells[movable.pos.y  + (size_t)i][movable.pos.x];
					if(possibleCell.value == 0 || possibleCell.value == movable.value)
						offset = i;
					else
						return offset;
				}
			}
			break;
		default:
			break;
	}
	return offset;
}

void Board::MoveCell(Cell &original, Cell &destination){
	destination.finishedAnimation = false;
	destination.oldPos = original.pos;
	if(destination.value == original.value)
		MergeCells(destination, original);
	else{
		destination.value = original.value;
		original.value = 0;
	}
}

void Board::MoveAllCells(Directions dir){
	if(moovesAvalible){
		switch(dir){
			case Directions::RIGHT:
				for(int y = 0; y < height; y++)
					for(int x = width - 1; x >= 0; x--){
						if(cells[y][x].value != 0){
							size_t offset = GetClosestCell(cells[y][x], dir);
							if(offset == 0)
								continue;
							MoveCell(cells[y][x], cells[y][x + offset]);
						}
					}
				break;
			case Directions::LEFT:
				for(int y = 0; y < height; y++)
					for(int x = 0; x < width; x++){
						if(cells[y][x].value != 0){
							size_t offset = GetClosestCell(cells[y][x], dir);
							if(offset == 0)
								continue;
							MoveCell(cells[y][x], cells[y][x - offset]);
						}
					}
				break;
			case Directions::UP:
				for(int x = 0; x < width; x++)
					for(int y = 0; y < height; y++){
						if(cells[y][x].value != 0){
							size_t offset = GetClosestCell(cells[y][x], dir);
							if(offset == 0)
								continue;
							MoveCell(cells[y][x], cells[y - offset][x]);
						}
					}
				break;
			case Directions::DOWN:
				for(int x = 0; x < width; x++)
					for(int y = height - 1; y >= 0; y--){
						if(cells[y][x].value != 0){
							size_t offset = GetClosestCell(cells[y][x], dir);
							if(offset == 0)
								continue;
							MoveCell(cells[y][x], cells[y + offset][x]);
						}
					}
				break;
			default:
				break;
		}
		SpawnCell();
	}
}

void Board::Update()
{
	CheckIfMovesAvalible();
	CheckCells();
}

void Board::MergeCells(Cell & destination, Cell & donor){
	destination.value += donor.value;
	score += destination.value;
	donor.value = 0;
}

void Board::Cell::Draw(Graphics &gfx, Font ft, Vei2 boardPos){
	Vei2 drawPos;
	if(!finishedAnimation){
		drawPos = Vei2((pos.x + oldPos.x) / 2, (pos.y + oldPos.y) / 2) * cellSize + boardPos;
		finishedAnimation = true;
	}else
		drawPos = pos * cellSize + boardPos;
	if(value != 0){
		int nSymbols = 0, temp = value;
		while(temp > 0){
			nSymbols++;
			temp /= 10;
		}
		gfx.DrawFilledRectBorder(drawPos, {cellSize, cellSize}, Colors::MakeRGB(220, 220 - 10 * (int)log2(value), 0), Colors::LightGray);
		ft.DrawText(std::to_string(value), drawPos + Vei2(50 - 8 * nSymbols, 36), Colors::White, gfx);
	}
}

Board::Cell::Cell(Vei2 _pos, int _value) : 
	pos(_pos), value(_value), oldPos(_pos)
{}
