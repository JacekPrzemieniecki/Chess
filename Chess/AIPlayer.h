#pragma once
#include "Move.h"
#include "Board.h"
class AIPlayer
{
public:
	AIPlayer();
	~AIPlayer();
	Move MakeMove(Board& board);
};

