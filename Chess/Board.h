#pragma once
#include "Move.h"
#include "Piece.h"
#include <set>
#include <list>

class Board
{
public:
	Board(void);
	Board(Board*, Move&);
	~Board(void);
	int FindKing(bool white);
	PieceType operator[](int index);
	Move GetLastMove();
	PieceType board[128];
	std::set<int> whitePieces;
	std::set<int> blackPieces;
	int enPassant = -1;
	bool whiteToMove;

	void MakeMove(const Move&);

	void Print();

	// Castle rights:
	bool bkcastle = true;
	bool bqcastle = true;
	bool wkcastle = true;
	bool wqcastle = true;
private:
	std::list<Move> moveHistory;
	void Place(PieceType piece, int x, int y);
	void Place(PieceType piece, int position);
};
