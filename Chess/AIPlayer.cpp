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
	const int searchDepth = 5;

	static unordered_map<PieceType, int> pieceValues{
		{ W_PAWN, 1 },
		{ B_PAWN, -1 },
		{ W_BISHOP, 3 },
		{ B_BISHOP, -3 },
		{ W_KNIGHT, 3 },
		{ B_KNIGHT, -3 },
		{ W_ROCK, 5 },
		{ B_ROCK, -5 },
		{ W_QUEEN, 9 },
		{ B_QUEEN, -9 },
		{ W_KING, 1000 },
		{ B_KING, -1000 }
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

		return maximizing? value : -value;
	}

	int EvaluateMove(Board& board, int alpha, int beta, int depth, bool maximizing)
	{
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

	void MakeMove(Game& game)
	{
		Board board = game.board;
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
		game.TryMakeMove(chosenMove);
	}
}