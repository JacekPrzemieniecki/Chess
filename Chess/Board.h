#pragma once
#include "Move.h"
#include "Piece.h"
#include <set>
#include <unordered_map>
#include <list>

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
	std::unordered_map<PieceType, int> castleRights = std::unordered_map<PieceType, int>({
		{ W_KING, INT_MAX },
		{ W_QUEEN, INT_MAX },
		{ B_KING, INT_MAX },
		{ B_QUEEN, INT_MAX }
	});

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
