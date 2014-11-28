#pragma once

#include "Board.h"
#include "Move.h"
#include <set>

class Game
{
public:
	Game();
	void TryMakeMove(Move& move);
	void Undo();
    void Refresh();
    PieceType GetPiece(int position);
	std::set<int>& GetWhitePieces();
	std::set<int>& GetBlackPieces();
    bool MoveWasMade();
	Move& GetLastMove();

    bool dirty = false;
    Move AwaitingAIMove;

	bool gameOver = false;
	Board board;
};
