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

Board::Board(Board* b, Move& m) :
whiteToMove(!b->whiteToMove),
wkcastle(b->wkcastle),
wqcastle(b->wqcastle),
bkcastle(b->bkcastle),
bqcastle(b->bqcastle)
{
	if (m.from == 115)
	{
		wkcastle = wqcastle = false;
		//cout << "White castle no longer possible" << endl;
	}
	if (m.from == 119 || m.to == 119)
	{
		wqcastle = false;
		//cout << "White queen-side castle no longer possible" << endl;
	}
	if (m.from == 112 || m.to == 112)
	{
		wkcastle = false;
		//cout << "White king-side castle no longer possible" << endl;
	}
	if (m.isCastle)
	{
		Place((*b)[m.castleFrom], m.castleTo);
		//cout << "Castling moves piece: " << (*b)[m.castleFrom] << "to: " << m.castleTo << endl;
	}

	if (m.isPawnDoublePush)
	{
		enPassant = m.enPassantPosition;
	}

	if (m.promoteTo != EMPTY)
	{
		Place(m.promoteTo, m.to);
	}
	else
	{
		Place((*b)[m.from], m.to);
	}
	for (int i = 0; i < 127; i++)
	{
		if ((*b)[i] == EMPTY || i == m.to || i == m.from || i == m.castleFrom || i == m.castleTo)
		{
			continue;
		}
		Place((*b)[i], i);
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
	if (board[move.to] != 0)
	{
		move.isCapture = true;
		move.capturedPiece = board[move.to];
	}

	static castleMoves castleTables[] {wKingCastle, wQueenCastle, bKingCastle, bQueenCastle};

	for (int i = 0; i < 4; i++)
	{
		castleMoves cInfo = castleTables[i];
		if (castleRights[cInfo.type] < turn) continue;

		if (move.from == cInfo.kingFrom)
		{
			castleRights[cInfo.type] = turn;
		}
	}

	if (move.isCastle)
	{
		Place(board[move.castleFrom], move.castleTo);
		Place(EMPTY, move.castleFrom);
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