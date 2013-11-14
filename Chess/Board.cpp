#include "Board.h"
#include "Piece.h"
#include "MoveValidator.h"
#include "PieceMoves.h"
#include <iostream>
#include <iomanip>

using namespace std;

Board::Board(void) :
whiteToMove(true)
{
	Place(W_ROCK, 0, 7);
	Place(W_ROCK, 7, 7);
	Place(W_KNIGHT, 1, 7);
	Place(W_KNIGHT, 6, 7);
	Place(W_BISHOP, 2, 7);
	Place(W_BISHOP, 5, 7);
	Place(W_QUEEN, 3, 7);
	Place(W_KING, 4, 7);

	Place(B_ROCK, 0, 0);
	Place(B_ROCK, 7, 0);
	Place(B_KNIGHT, 1, 0);
	Place(B_KNIGHT, 6, 0);
	Place(B_BISHOP, 2, 0);
	Place(B_BISHOP, 5, 0);
	Place(B_QUEEN, 3, 0);
	Place(B_KING, 4, 0);

	for (int i = 0; i < 8; i++)
	{
		Place(W_PAWN, i, 6);
		Place(B_PAWN, i, 1);
	}
}

PieceType Board::operator[](int index)
{
	return board[index];
}

int Board::FindKing(bool white)
{
	set<int>& tab = white ? whitePieces : blackPieces;
	PieceType lookingFor = white ? W_KING : B_KING;
	for (set<int>::iterator it = tab.begin(); it != tab.end(); it++)
	{
		if (board[*it] == lookingFor)
		{
			cout << "Found king at " << *it << endl;
			return *it;
		}
	}
	throw exception();
}

void Board::Place(PieceType type, int x, int y)
{
	Place(type, x + 16 * y);
}

void Board::Place(PieceType type, int position)
{
	//cout << "Placing piece: " << type << " On position: " << position << endl;
	if (board[position] > 0)
	{
		whitePieces.erase(position);
	}
	else if (board[position] < 0)
	{
		blackPieces.erase(position);
	}

	board[position] = type;
	if (type > 0)
	{
		whitePieces.insert(position);
	}
	else if (type < 0)
	{
		blackPieces.insert(position);
	}
}

Move Board::GetLastMove()
{
	return moveHistory.front();
}

void Board::MakeMove(Move move)
{
	move.capturedPiece = board[move.to];

	for (int i = 0; i < 4; i++)
	{
		castleMoves cInfo = castleTables[i];
		if (castleRights[cInfo.type] < turn) continue;

		if (move.from == cInfo.kingFrom)
		{
			castleRights[cInfo.type] = turn;
		}
	}

	if (move.castleInfo != NULL)
	{
		Place(move.castleInfo->rockType, move.castleInfo->rockTo);
		Place(EMPTY, move.castleInfo->rockFrom);
	}

	if (move.isPawnDoublePush)
	{
		enPassant = move.enPassantPosition;
	}
	else
	{
		enPassant = -1;
	}

	if (move.promoteTo != EMPTY)
	{
		Place(move.promoteTo, move.to);
	}
	else
	{
		Place(board[move.from], move.to);
	}

	Place(EMPTY, move.from);

	moveHistory.push_front(move);
	whiteToMove = !whiteToMove;
	turn++;
}

void Board::UndoMove()
{
	Move last_move = moveHistory.front();
	//cout << "Undoing move from " << last_move.from << " to: " << last_move.to << endl;
	whiteToMove = !whiteToMove;
	turn--;

	// If castle rights were lost this turn, restore them.
	for (int i = 0; i < 4; i++)
	{
		castleMoves cInfo = castleTables[i];
		if (castleRights[cInfo.type] == turn)
		{
			castleRights[cInfo.type] = INT_MAX;
		}
	}

	// If it was a castle, undo the rock move.
	if (last_move.castleInfo != NULL)
	{
		//cout << "Undoing castle" << endl;
		Place(last_move.castleInfo->rockType, last_move.castleInfo->rockFrom);
		Place(EMPTY, last_move.castleInfo->rockTo);
	}

	if (last_move.promoteTo != EMPTY)
	{
		Place(whiteToMove ? W_PAWN : B_PAWN, last_move.from);
	}
	else
	{
		Place(board[last_move.to], last_move.from);
	}

	Place(last_move.capturedPiece, last_move.to);
	moveHistory.pop_front();
	enPassant = moveHistory.front().isPawnDoublePush ? moveHistory.front().enPassantPosition : -1;
}

void Board::Print()
{
	return;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cout << setw(2) << board[j + 16 * i] << " ";
		}
		cout << endl;
	}
}