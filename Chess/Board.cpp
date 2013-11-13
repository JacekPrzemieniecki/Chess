#include "Board.h"
#include "Piece.h"
#include "MoveValidator.h"
#include <iostream>

using namespace std;

Board::Board(void):
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
	Place(B_QUEEN, 4, 0);
	Place(B_KING, 3, 0);

	for (int i = 0; i < 8; i++)
	{
		Place(W_PAWN, i, 6);
		Place(B_PAWN, i, 1);
	}
}


Board::Board(Board* b, Move& m):
lastMove(m),
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

Board::~Board(void)
{
}

int Board::FindKing(bool white)
{
	vector<int>& tab = white ? whitePieces : blackPieces;
	for (vector<int>::iterator it = tab.begin(); it != tab.end(); it++)
	{
		if (white && board[*it] == W_KING)
			return *it;
		if (!white && board[*it] == B_KING)
			return *it;
	}
}

void Board::Place(PieceType type, int x, int y)
{
	Place(type, x + 16 * y);
}

void Board::Place(PieceType type, int position)
{
	//cout << "Placing piece: " << type << " On position: " << position << endl;
	board[position] = type;
	if (type > 0)
	{
		whitePieces.push_back(position);
	}
	else if (type < 0)
	{
		blackPieces.push_back(position);
	}
}

void Board::MakeMove(Move m)
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
		Place(board[m.castleFrom], m.castleTo);
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
		Place(board[m.from], m.to);
	}

	vector<int>& pieceVector = whiteToMove ? whitePieces : blackPieces;
	for (auto it = pieceVector.begin(); it != pieceVector.end(); it++)
	{
		if (*it == m.from)
		{
			pieceVector.erase(it);
			break;
		}

	}
	Place(EMPTY, m.from);

	lastMove = m;
	enPassant = -1;
	whiteToMove = !whiteToMove;
}