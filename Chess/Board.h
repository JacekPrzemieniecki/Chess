#pragma once
#include "Move.h"
#include "Piece.h"
#include <set>
#include <list>
#include <limits>

class Board
{
public:
	// methods
	Board(void);
	Board(Board*, Move&);
	int FindKing(bool white);
	PieceType operator[](int index);
	Move GetLastMove();
	void MakeMove(Move);
	void Print();

	// fields
	PieceType board[128];
	std::set<int> whitePieces;
	std::set<int> blackPieces;
	int enPassant = -1;
	int turn = 0;
	bool whiteToMove;

	// Castle rights:
	int bkcastle = INT_MAX;
	int bqcastle = INT_MAX;
	int wkcastle = INT_MAX;
	int wqcastle = INT_MAX;
private:
	std::list<Move> moveHistory;
	void Place(PieceType piece, int x, int y);
	void Place(PieceType piece, int position);
};
