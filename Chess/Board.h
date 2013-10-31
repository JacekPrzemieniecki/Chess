#pragma once
#include "Move.h"
#include "Piece.h"
#include <vector>

class Board
{
public:
	Board(void);
	Board(Board*, Move&);
	~Board(void);
	int FindKing(bool white);
	PieceType operator[](int index);
	Board* previous;
	Move lastMove;
	PieceType board[128];
	std::vector<int> whitePieces;
	std::vector<int> blackPieces;
	int enPassant = -1;
	bool whiteToMove;

	// Castle rights:
	bool bkcastle = true;
	bool bqcastle = true;
	bool wkcastle = true;
	bool wqcastle = true;
private:
	void Place(PieceType piece, int x, int y);
	void Place(PieceType piece, int position);
};
