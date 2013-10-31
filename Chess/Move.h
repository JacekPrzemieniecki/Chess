#pragma once
#include "Piece.h"
struct Move
{
public:
	Move();
	Move(int _from, int _to);
	short int from;
	short int to;
	short int castleFrom;
	short int castleTo;
	bool isCastle = false;

	PieceType promoteTo = EMPTY;
	short int enPassantPosition;
	bool isPawnDoublePush = false;
};
