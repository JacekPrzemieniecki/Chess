#include "MoveGenerator.h"
#include "MoveValidator.h"
#include "Piece.h"
#include "PieceMoves.h"
#include <iostream>
using namespace std;

void KnightOrKingMove(int position, Board& board, vector<Move*>& result, vector<int>& tab)
{
	for (vector<int>::iterator it = tab.begin(); it != tab.end(); it++)
	{
		char target = position + *it;
		if (target & 0x88) continue; // target position out of board
		if (board[target] * board[position] > 0) continue; // attacking own piece is invalid
		result.push_back(new Move(position, target));
	}
}

void CastleMoves(int position, Board& board, vector<Move*>& result)
{
	static castleMoves castleTables[] {wKingCastle, wQueenCastle, bKingCastle, bQueenCastle};
	for (int i = 0; i < 4; i++)
	{
		castleMoves cInfo = castleTables[i];
		if (cInfo.kingFrom != position || !board.castleRights[cInfo.type]) continue;
		Move* new_move = new Move(cInfo.kingFrom, cInfo.kingTo);
		if (ValidateCastle(board, *new_move))
		{
			result.push_back(new_move);
		}
		else
		{
			delete new_move;
		}
	}
}

void GenerateOnRay(int start, int current, Board& board, vector<Move*>& result, int delta)
{
	// Attacking out of bonds and attacking own pieces is illegal.
	if ((current & 0x88) || board[current] * board[start] > 0 ) return;
	
	result.push_back(new Move(start, current));

	if (board[current] == EMPTY)
	{
		GenerateOnRay(start, current + delta, board, result, delta);
	}
}

void BishopOrRockMove(int position, Board& board, vector<Move*>& result, vector<int>& tab)
{
	for (vector<int>::iterator it = tab.begin(); it != tab.end(); it++)
	{
		GenerateOnRay(position, position + *it, board, result, *it);
	}
}


vector<PieceType> promoteTab = { W_ROCK, W_BISHOP, W_KNIGHT, W_QUEEN };
void CheckForPromoteAndAdd(int position, int to, vector<Move*>& result, bool isWhite, bool isDoublePush)
{
	int rank = to / 16;
	if (rank == 0 || rank == 7)
	{
		for (vector<PieceType>::iterator it = promoteTab.begin(); it != promoteTab.end(); it++)
		{
			Move* move = new Move(position, to);
			move->promoteTo = (isWhite ? *it : (PieceType)-*it);
			result.push_back(move);
		}
	}
	else
	{
		Move* move = new Move(position, to);
		move->isPawnDoublePush = isDoublePush;
		result.push_back(move);
	}
}

void PawnMove(int position, Board& board, vector<Move*>& result, bool isWhite)
{
	int direction = isWhite ? -1 : 1;
	int forward = position + 16 * direction;
	if (!(forward & 0x88) && board[forward] == EMPTY)
	{
		CheckForPromoteAndAdd(position, position + 16 * direction, result, isWhite, false);
	}
	int captureLeft = position + 15 * direction;
	if (!(captureLeft & 0x88) && (board[captureLeft] * direction > 0 || board.enPassant == captureLeft) )
	{
		CheckForPromoteAndAdd(position, position + 15 * direction, result, isWhite, false);
	}
	int captureRight = position + 17 * direction;
	if (!(captureRight & 0x88) && (board[captureRight] * direction > 0 || board.enPassant == captureRight))
	{
		CheckForPromoteAndAdd(position, position + 17 * direction, result, isWhite, false);
	}
	// Pawn double push
	if (((isWhite && position / 16 == 6) || //for white
		(!isWhite && position / 16 == 1)) &&  // and black
		board[forward] == EMPTY && 
		board[position + 32 * direction] == EMPTY)
	{
		CheckForPromoteAndAdd(position, position + 32 * direction, result, isWhite, true);
	}
}

void Generate(int position, Board& board, vector<Move*>& result)
{
	switch (board[position])
	{
	case B_KNIGHT:
	case W_KNIGHT:
		KnightOrKingMove(position, board, result, knightTab);
		break;
	case B_KING:
	case W_KING:
		KnightOrKingMove(position, board, result, kingTab);
		CastleMoves(position, board, result);
		break;
	case B_BISHOP:
	case W_BISHOP:
		BishopOrRockMove(position, board, result, bishopTab);
		break;
	case B_ROCK:
	case W_ROCK:
		BishopOrRockMove(position, board, result, rockTab);
		break;
	case B_QUEEN:
	case W_QUEEN:
		BishopOrRockMove(position, board, result, bishopTab);
		BishopOrRockMove(position, board, result, rockTab);
		break;
	case B_PAWN:
		PawnMove(position, board, result, false);
		break;
	case W_PAWN:
		PawnMove(position, board, result, true);
		break;
	}
}

void GenerateAll(Board& board, vector<Move*>& result)
{
	set<int> pieces = board.whiteToMove ? board.whitePieces : board.blackPieces;
	for (int i : pieces)
	{
		Generate(i, board, result);
	}
}

bool IsMovePossible(Board& board)
{
	//cout << "Checking for possible moves" << endl;
	vector<Move*> possibleMoves;
	possibleMoves.reserve(100);
	bool possible = false;
	GenerateAll(board, possibleMoves);
	//cout << "Found " << possibleMoves.size() << " moves:" << endl;
	for (vector<Move*>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); it++)
	{
		//cout << (*it)->from << "->" << (*it)->to << " ";
		if (IsMoveLegal(board, **it))
		{
			possible = true;
			//cout << "legal" << endl;
			break;
		}
		//cout << "ill; ";
	}
	for (vector<Move*>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); it++)
	{
		delete *it;
	}

	return possible;
}