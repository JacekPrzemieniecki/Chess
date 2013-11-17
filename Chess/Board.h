#pragma once
#include "Move.h"
#include "Piece.h"
#include <string>
#include <set>
#include <unordered_map>
#include <list>
#include <array>

class Board
{
public:
	// methods
	Board(void);
	Board(string position);
	int FindKing(bool white);
	PieceType operator[](int index);
	Move GetLastMove();
	void MakeMove(Move);
	void UndoMove();
	void Print();

	// fields
	std::array<PieceType, 128> board;
	std::set<int> whitePieces;
	std::set<int> blackPieces;
	int enPassant;
	int turn;
	bool whiteToMove;	
	std::unordered_map<PieceType, int> castleRights;

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
