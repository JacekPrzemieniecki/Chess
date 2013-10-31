#include "BoardInterface.h"
#include <iostream>
#include <vector>

// Debug
#include "MoveValidator.h"
#include "MoveGenerator.h"
// /Debug

using namespace std;

void BoardInterface::DrawGizmo(int boardPosition)
{
	int x, y;
	BoardToScreen(boardPosition, x, y);
	DrawGizmo(x, y);
}

void BoardInterface::DrawGizmo(int x, int y)
{
	static sf::CircleShape shape(25);
	shape.setFillColor(sf::Color(100, 250, 50));
	shape.setPosition((float)x, (float)y);
	window.draw(shape);
}

BoardInterface::BoardInterface(sf::RenderWindow& _window) :
window(_window)
{
	sf::Vector2u size = window.getSize();
	screenW = size.x;
	screenH = size.y;
	tileSize = (min(size.x, size.y) - 2 * border) / 8;
	LoadTextures();
}

BoardInterface::~BoardInterface()
{
}

void BoardInterface::OnClick(int x, int y)
{
	if (!game.board->whiteToMove)
	{
		return;
	}

	if (pendingPromote)
	{
		PieceType p = promoteWindow.OnClick(x, y);
		if (p != EMPTY)
		{
			cout << "Promotion to: " << p << endl;
			Move move(selectedPosition, promoteTo);
			move.promoteTo = p;

			game.TryMakeMove(move);
			pendingPromote = false;
			selectedPosition = -1;
			return;
		}
	}

	int clickedPosition = ScreenToBoard(x, y);
	if (clickedPosition & 0x88)
		return; // clicked outside of the board

	// Debug
	//cout << "PieceType " << sizeof(PieceType) << endl;
	//cout << "Piece " << sizeof(Piece) << endl;
	int debug_x = (x - border) / tileSize;
	int debug_y = (y - border) / tileSize;
	// /Debug

	cout << "OnClick: Board position: " << clickedPosition << " X: " << debug_x << " Y : " << debug_y << endl;

	PieceType clickedType = game.GetPiece(clickedPosition);

	// Debug
	if (clickedType != EMPTY)

	if (selectedPosition != -1)
		cout << "Selected position is: " << selectedPosition << endl;
	// /Debug
	if (selectedPosition == -1 && clickedType <= 0)
	{
		return;
	}
	else if (selectedPosition == -1 && clickedType > 0)
	{
		cout << "Selected was NULL, selecting: " << clickedPosition << endl;
		selectedPosition = clickedPosition;
		return;
	}
	else if (selectedPosition == clickedPosition)
	{
		cout << "Canceling selection" << endl;
		selectedPosition = -1;
		return;
	} 
	else if (game.GetPiece(selectedPosition) == W_PAWN && clickedPosition / 16 == 0)
	{
		pendingPromote = true;
		promoteTo = clickedPosition;
	}
	else
	{
		cout << "Trying to move from: " << selectedPosition << " to: " << clickedPosition << endl;
		Move move(selectedPosition, clickedPosition);

		game.TryMakeMove(move);
		selectedPosition = -1;
	}
}

void BoardInterface::Draw()
{
	window.draw(boardTexture);

	//Debug

	vector<Move*> moves;
	GenerateAll(*(game.board), moves);
	for (vector<Move*>::iterator iter = moves.begin(); iter != moves.end(); iter++)
	{
		DrawGizmo((*iter)->to);
	}
	for (vector<Move*>::iterator it = moves.begin(); it != moves.end(); it++)
	{
		delete *it;
	}

	//vector<int> whites = game.GetWhitePieces();

	//for (vector<int>::iterator it = whites.begin(); it != whites.end(); it++)
	//{
	//	if (game.GetPiece(*it) != EMPTY )
	//	{
	//		vector<Move*> moves;
	//		Generate(*it, *(game.board), moves);
	//		for (vector<Move*>::iterator iter = moves.begin(); iter != moves.end(); iter++)
	//		{
	//			DrawGizmo((*iter)->to);
	//		}
	//	}
	//}

	//for (int i = 0; i < 128; i++)
	//{
	//	if (i & 0x88) continue;
	//	if (IsAttacked(*game.board, i, false))
	//		DrawGizmo(i);
	//}

	//int x, y;
	//BoardToScreen(67, x, y);
	//bknight.setPosition(x, y);
	//window.draw(bknight);
	/*
		vector<int> bishopTab = { -15, 15, -17, 17 };

		for (vector<int>::iterator it = bishopTab.begin(); it != bishopTab.end(); it++)
		{
		DrawGizmo(67 + *it);
		}
		*/
	//Debug


	if (game.GetLastMove().from != -1)
	{
		DrawLastMove(game.GetLastMove());
	}

	if (selectedPosition != -1)
	{
		DrawMoveSuggestion();
	}

	for (std::vector<int>::iterator it = game.GetWhitePieces().begin(); it != game.GetWhitePieces().end(); ++it)
	{
		DrawPiece(game.GetPiece(*it), *it);
	}

	for (std::vector<int>::iterator it = game.GetBlackPieces().begin(); it != game.GetBlackPieces().end(); ++it)
	{
		DrawPiece(game.GetPiece(*it), *it);
	}

	if (pendingPromote)
	{
		promoteWindow.Draw(window, (float)(selectedPosition * tileSize) + 0.5f);
	}
}

void BoardInterface::DrawPiece(PieceType type, int boardPosition)
{
	if (type == EMPTY)
		return;
	int delta = 5;

	int pieceX;
	int pieceY;
	BoardToScreen(boardPosition, pieceX, pieceY);

	sf::Sprite sprite;
	switch (type)
	{
	case B_BISHOP:
		sprite = bbishop;
		break;
	case B_KING:
		sprite = bking;
		break;
	case B_KNIGHT:
		sprite = bknight;
		break;
	case B_PAWN:
		sprite = bpawn;
		break;
	case B_QUEEN:
		sprite = bqueen;
		break;
	case B_ROCK:
		sprite = brock;
		break;
	case W_BISHOP:
		sprite = wbishop;
		break;
	case W_KING:
		sprite = wking;
		break;
	case W_KNIGHT:
		sprite = wknight;
		break;
	case W_PAWN:
		sprite = wpawn;
		break;
	case W_QUEEN:
		sprite = wqueen;
		break;
	case W_ROCK:
		sprite = wrock;
		break;
	default:
		break;
	}

	sprite.setPosition((float)(pieceX + delta), (float)(pieceY + delta));
	window.draw(sprite);
}

void BoardInterface::DrawMoveSuggestion()
{
	sf::Vector2i position = sf::Mouse::getPosition(window);
	int mouseX = position.x - (position.x - border) % tileSize;
	int mouseY = position.y - (position.y - border) % tileSize;
	if (border <= position.x && position.x < screenW - border && border <= position.y && position.y < screenH - border)
	{
		highlightMoveSuggestion.setPosition((float)mouseX, (float)mouseY);
		window.draw(highlightMoveSuggestion);
	}

	int selectedX, selectedY;
	BoardToScreen(selectedPosition, selectedX, selectedY);
	highlightPiece.setPosition((float)selectedX, (float)selectedY);

	window.draw(highlightPiece);
}

void BoardInterface::DrawLastMove(Move& move)
{
	int fromX, fromY, toX, toY;
	BoardToScreen(move.from, fromX, fromY);
	BoardToScreen(move.to, toX, toY);
	highlightLastFrom.setPosition((float)fromX, (float)fromY);
	highlightLastTo.setPosition((float)toX, (float)toY);
	window.draw(highlightLastFrom);
	window.draw(highlightLastTo);
}

void BoardInterface::BoardToScreen(int boardPosition, int& x, int& y)
{
	x = boardPosition % 16 * tileSize + border;
	y = boardPosition / 16 * tileSize + border;
}

int BoardInterface::ScreenToBoard(int x, int y)
{
	return  (y - border) / tileSize * 16 + (x - border) / tileSize;
}

void BoardInterface::LoadTextures()
{
	std::cout << "Loading Textures" << std::endl;

	if (!(
		tx_boardTexture.loadFromFile("img/board.png") &&
		tx_highlightlast.loadFromFile("img/highlightlast.png") &&
		tx_highlight.loadFromFile("img/highlight.png") &&

		tx_wking.loadFromFile("img/wking.png") &&
		tx_wqueen.loadFromFile("img/wqueen.png") &&
		tx_wbishop.loadFromFile("img/wbishop.png") &&
		tx_wknight.loadFromFile("img/wknight.png") &&
		tx_wrock.loadFromFile("img/wrock.png") &&
		tx_wpawn.loadFromFile("img/wpawn.png") &&

		tx_bking.loadFromFile("img/bking.png") &&
		tx_bqueen.loadFromFile("img/bqueen.png") &&
		tx_bbishop.loadFromFile("img/bbishop.png") &&
		tx_bknight.loadFromFile("img/bknight.png") &&
		tx_brock.loadFromFile("img/brock.png") &&
		tx_bpawn.loadFromFile("img/bpawn.png")
		))
	{
		fprintf(stderr, "Failed to load images");
	}

	boardTexture.setTexture(tx_boardTexture);
	highlightLastFrom.setTexture(tx_highlightlast);
	highlightLastTo.setTexture(tx_highlightlast);
	highlightPiece.setTexture(tx_highlight);
	highlightMoveSuggestion.setTexture(tx_highlight);

	wking.setTexture(tx_wking);
	wqueen.setTexture(tx_wqueen);
	wbishop.setTexture(tx_wbishop);
	wknight.setTexture(tx_wknight);
	wrock.setTexture(tx_wrock);
	wpawn.setTexture(tx_wpawn);

	bking.setTexture(tx_bking);
	bqueen.setTexture(tx_bqueen);
	bbishop.setTexture(tx_bbishop);
	bknight.setTexture(tx_bknight);
	brock.setTexture(tx_brock);
	bpawn.setTexture(tx_bpawn);
}