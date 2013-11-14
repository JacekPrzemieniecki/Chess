#include "MoveValidator.h"
#include "Piece.h"
#include "PieceMoves.h"
#include <vector>
#include <iostream>

using namespace std;

int rayCast(Board& board, int from, int to, int delta)
{
	//cout << "Raycasting from: " << from << " to: " << to << " with delta: " << delta << endl;
	int cur = from + delta;
	if (cur == to) return cur;
	// if we want do move on and the square is not empty or is invalid (& 0x88)
	else if (board[cur] != EMPTY || cur & 0x88)
	{
		return cur;
	}
	else return rayCast(board, cur, to, delta);
}

vector<int> rayCastAll(Board& board, int from, vector<int> deltas)
{
	vector<int> results;
	for (vector<int>::iterator it = deltas.begin(); it != deltas.end(); it++)
	{
		int cast = rayCast(board, from, 9, *it);
		if (cast & 0x88) continue;
		results.push_back(cast);
	}
	return results;
}

bool KnightMove(int from, int to)
{
	int diff = abs(to - from);
	for (vector<int>::iterator it = knightTab.begin(); it != knightTab.end(); it++)
	{
		if (*it == diff)
		{
			return true;
		}
	}
	return false;
}

bool RockMove(Board& board, int from, int to)
{
	cout << "Validating Rock move from: " << from << " to: " << to << endl;
	int diff = abs(from - to);
	bool up, down, left, right;
	up = down = (diff % 16 == 0);
	left = right = diff < 8;
	up &= from > to;
	right &= from < to;
	cout << "up: " << up << "down: " << down << " left: " << left << " right " << right << endl;
	if (up) return rayCast(board, from, to, -16) == to;
	if (down) return rayCast(board, from, to, 16) == to;
	if (right) return rayCast(board, from, to, 1) == to;
	if (left) return rayCast(board, from, to, -1) == to;
	return false;
}

bool BishopMove(Board& board, int from, int to)
{
	cout << "Validating Bishop move from: " << from << " to: " << to << endl;
	int diff = abs(from - to);
	bool ul, ur, ll, lr; // upper left, upper right...
	ul = (from > to) && (diff % 17 == 0);
	ur = (from > to) && (diff % 15 == 0);
	ll = (from < to) && (diff % 15 == 0);
	lr = (from < to) && (diff % 17 == 0);
	cout << "ul: " << ul << "ur: " << ur << " ll: " << ll << " lr " << lr << endl;
	if (ul) return rayCast(board, from, to, -17) == to;
	if (ur) return rayCast(board, from, to, -15) == to;
	if (ll) return rayCast(board, from, to, 15) == to;
	if (lr) return rayCast(board, from, to, 17) == to;
	return false;
}

bool ValidateCastle(Board& board, Move& move)
{
	static castleMoves castleTables[] {wKingCastle, wQueenCastle, bKingCastle, bQueenCastle};
	for (int i = 0; i < 4; i++)
	{
		castleMoves cInfo = castleTables[i];
		if (move.from == cInfo.kingFrom &&						// King moves from initial position
			move.to == cInfo.kingTo &&							// To a castle field
			board.castleRights[cInfo.type] > board.turn &&  // Player didn't loose castle right
			cInfo.rockTo == rayCast(board, cInfo.kingFrom, cInfo.rockTo, cInfo.kingDirection)) // No field on between king and rock is taken
		{
			cout << "Castling!" << endl;
			move.castleInfo = &castleTables[i];
			return !(
				IsAttacked(board, cInfo.kingFrom, cInfo.type > 0) ||
				IsAttacked(board, cInfo.rockTo, cInfo.type > 0) ||
				IsAttacked(board, cInfo.kingTo, cInfo.type > 0));
		}
	}
}

bool KingMove(Board& board, Move& move)
{
	int from = move.from;
	int to = move.to;
	int diff = to - from;
	for (vector<int>::iterator it = kingTab.begin(); it != kingTab.end(); it++)
	{
		if (*it == diff)
		{
			return true;
		}
	}
	return ValidateCastle(board, move);
}

bool PawnMove(Board& board, Move& move, bool isWhite)
{
	int direction = isWhite ? -1 : 1;
	int diff = direction * (move.to - move.from);
	if (diff == 16 && board[move.to] == EMPTY)
	{
		return true;
	}
	if ((diff == 15 || diff == 17) && (board[move.to] * direction > 0 || board.enPassant == move.to))
	{
		return true;
	}
	cout << board[move.from + 16] << endl;
	if (diff == 32 && board[move.from + 32 * direction] == EMPTY &&
		board[move.from + 16 * direction] == EMPTY && // Pawn double push
		((isWhite && move.from / 16 == 6) || //for white
		(!isWhite && move.from / 16 == 1)))   // and black
	{
		return true;

	}
	return false;
}

bool ValidateMove(Board& board, Move& move)
{
	cout << "Validating move from: " << move.from << " to: " << move.to << endl;
	PieceType moving = board[move.from];
	PieceType target = board[move.to];

	if (moving * target > 0)
		return false; // Attacking own piece is illegal

	// Check if this piece can move like this
	switch (moving)
	{
	case B_KNIGHT:
	case W_KNIGHT:
		return KnightMove(move.from, move.to);
	case B_ROCK:
	case W_ROCK:
		return RockMove(board, move.from, move.to);
	case B_BISHOP:
	case W_BISHOP:
		return BishopMove(board, move.from, move.to);
	case B_QUEEN:
	case W_QUEEN:
		return BishopMove(board, move.from, move.to) || RockMove(board, move.from, move.to);
	case B_KING:
	case W_KING:
		return KingMove(board, move);
	case B_PAWN:
		return PawnMove(board, move, false);
	case W_PAWN:
		return PawnMove(board, move, true);
	}
	return true;
}

bool IsAttacked(Board& board, int position, bool byBlack)
{
	cout << "Checking if position: " << position << " is attacked " << (byBlack ? "by Black" : "by White") << endl;
	int sign = byBlack ? -1 : 1;
	for (vector<int>::iterator it = knightTab.begin(); it != knightTab.end(); it++)
	{
		if (board[position + *it] == W_KNIGHT * sign)
		{
			cout << "It is - by a king" << endl;
			return true;
		}
	}

	for (vector<int>::iterator it = kingTab.begin(); it != kingTab.end(); it++)
	{
		if (board[position + *it] == W_KING * sign)
		{

			cout << "It is - by a knight" << endl;
			return true;
		}
	}

	vector<int> bishopCasts = rayCastAll(board, position, bishopTab);

	for (vector<int>::iterator it = bishopCasts.begin(); it != bishopCasts.end(); it++)
	{
		if (board[*it] == W_BISHOP * sign || board[*it] == W_QUEEN * sign)
		{
			cout << "It is - by a bishop" << endl;
			return true;
		}
	}

	vector<int> rockCasts = rayCastAll(board, position, rockTab);
	for (vector<int>::iterator it = rockCasts.begin(); it != rockCasts.end(); it++)
	{
		if (board[*it] == W_ROCK * sign || board[*it] == W_QUEEN * sign)
		{
			cout << "It is - by a rock" << endl;
			return true;
		}
	}

	if (board[position + (15 * sign)] == W_PAWN * sign) return true;
	if (board[position + (17 * sign)] == W_PAWN * sign) return true;

	return false;
}

bool IsMoveLegal(Board& board, Move& move, bool whiteMove)
{
	Board* newBoard = new Board(&board, move);
	bool legal = true;
	if (IsAttacked(*newBoard, newBoard->FindKing(whiteMove), whiteMove))
	{
		legal = false;
	}
	delete newBoard;
	return legal;
}