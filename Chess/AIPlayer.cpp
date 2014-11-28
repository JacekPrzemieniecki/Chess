#include "AIPlayer.h"
#include "Board.h"
#include "Move.h"
#include "MoveGenerator.h"
#include "MoveValidator.h"
#include "Game.h"
#include <unordered_map>
#include <vector>
#include <future>
#include <iostream>


using namespace std;

namespace AIPlayer
{
	const int searchDepth = 4;

	static unordered_map<PieceType, int> pieceValues{
		{ W_PAWN, 10 },
		{ B_PAWN, -10 },
		{ W_BISHOP, 30 },
		{ B_BISHOP, -30 },
		{ W_KNIGHT, 30 },
		{ B_KNIGHT, -30 },
		{ W_ROCK, 50 },
		{ B_ROCK, -50 },
		{ W_QUEEN, 90 },
		{ B_QUEEN, -90 },
		{ W_KING, 10000 },
		{ B_KING, -10000 }
};

	int EvaluateBoard(Board& board, bool maximizing = true)
	{
		int value = 0;
		for (int piecePosition : board.whitePieces)
		{
			value += pieceValues[board[piecePosition]];
		}
		for (int piecePosition : board.blackPieces)
		{
			value += pieceValues[board[piecePosition]];
		}

		vector<Move*> possibleMoves;
		possibleMoves.reserve(100);
		GenerateAll(board, possibleMoves);
		value += possibleMoves.size();
		for (Move* m : possibleMoves)
		{
			delete m;
		}

		return maximizing? value : -value;
	}

	int EvaluateMove(Board& board, int alpha, int beta, int depth, bool maximizing)
	{
		if (board.kingDead)
		{
			return maximizing ? -999111 : 999111;
		}

		if (depth == 0)
		{
			return EvaluateBoard(board);
		}

		vector<Move*> possibleMoves;
		possibleMoves.reserve(100);
		GenerateAll(board, possibleMoves);

		int returnVal;
		if (maximizing)
		{
			for (Move* move : possibleMoves)
			{
				board.MakeMove(*move);
				alpha = max(alpha, EvaluateMove(board, alpha, beta, depth - 1, false));
				if (beta <= alpha)
				{
					board.UndoMove();
					break;
				}
				board.UndoMove();
			}
			returnVal = alpha;
		}
		else
		{
			for (Move* move : possibleMoves)
			{
				board.MakeMove(*move);
				beta = min(beta, EvaluateMove(board, alpha, beta, depth - 1, true));
				if (beta <= alpha)
				{
					board.UndoMove();
					break;
				}
				board.UndoMove();
			}
			returnVal = beta;
		}

		for (Move* move : possibleMoves)
		{
			delete move;
		}
		return returnVal;
	}

	void MakeMove(Game* game)
	{
		Board board = game->board;
		vector<Move*> possibleMoves;
		possibleMoves.reserve(100);
		GenerateAll(board, possibleMoves);

		if (possibleMoves.size() == 0)
		{
			// Game over
			return;
		}

		int beta = 10001;
		Move* best = possibleMoves[0];
		for (Move* move : possibleMoves)
		{
			if (!IsMoveLegal(board, *move))	continue;

			board.MakeMove(*move);
			int eval = EvaluateMove(board, -10009, beta, searchDepth, true);
			if (eval < beta)
			{
				beta = eval;
				best = move;
			}
			board.UndoMove();
		}

		Move chosenMove = *best;

		for (Move* move : possibleMoves)
		{
			delete move;
		}
        game->AwaitingAIMove = chosenMove;
        game->dirty = true;
	}
}