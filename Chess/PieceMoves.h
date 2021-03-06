#pragma once
#include "Piece.h"
#include <vector>

struct castleMoves
{
	PieceType type;
	PieceType rockType;
	int kingFrom;
	int kingTo;
	int rockFrom;
	int rockTo;
	int kingDirection;
};
//										{ Type, kFr, kTo, rFr, rTo }
static const castleMoves wKingCastle	{ W_KING, W_ROCK, 116, 118, 119, 117, 1 };
static const castleMoves wQueenCastle	{ W_QUEEN, W_ROCK, 116, 114, 112, 115, -1 };
static const castleMoves bKingCastle	{ B_KING, B_ROCK, 4, 6, 7, 5, 1 };
static const castleMoves bQueenCastle	{ B_QUEEN, B_ROCK, 4, 2, 0, 3, -1 };

static castleMoves castleTables[] {wKingCastle, wQueenCastle, bKingCastle, bQueenCastle};

static std::vector<int> knightTab = { -33, -31, -18, -14, 33, 31, 18, 14 };
static std::vector<int> kingTab = { -1, -15, -16, -17, 1, 15, 16, 17 };
static std::vector<int> bishopTab = { -15, -17, 15, 17 };
static std::vector<int> rockTab = { -1, -16, 1, 16 };