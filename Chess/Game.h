#pragma once
#include <set>

#include "Board.h"
#include "Move.h"
#include "AIPlayer.h"

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
    Move awaitingAIMove;

	bool gameOver = false;
	Board board;
    AIPlayer aiPlayer;
};
