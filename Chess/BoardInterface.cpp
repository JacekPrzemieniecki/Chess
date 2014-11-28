#include "BoardInterface.h"
#include <vector>

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
window(_window),
game()
{
	sf::Vector2u size = window.getSize();
	screenW = size.x;
	screenH = size.y;
	tileSize = (min(size.x, size.y) - 2 * border) / 8;
    spriteSet.Init();
}

void BoardInterface::Undo()
{
	game.Undo();
	game.Undo();
}

void BoardInterface::OnClick(int x, int y)
{
	if (!game.board.whiteToMove)
	{
		return;
	}

	if (pendingPromote)
	{
		PieceType p = promoteWindow.OnClick(x, y);
		if (p != EMPTY)
		{
			//cout << "Promotion to: " << p << endl;
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

	//cout << "OnClick: Board position: " << clickedPosition << " X: " << debug_x << " Y : " << debug_y << endl;

	PieceType clickedType = game.GetPiece(clickedPosition);

	if (selectedPosition == -1 && clickedType <= 0)
	{
		return;
	}
	else if (selectedPosition == -1 && clickedType > 0)
	{
		//cout << "Selected was NULL, selecting: " << clickedPosition << endl;
		selectedPosition = clickedPosition;
		return;
	}
	else if (selectedPosition == clickedPosition)
	{
		//cout << "Canceling selection" << endl;
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
		//cout << "Trying to move from: " << selectedPosition << " to: " << clickedPosition << endl;
		Move move(selectedPosition, clickedPosition);

		game.TryMakeMove(move);
		selectedPosition = -1;
	}
}

void BoardInterface::Draw()
{
    window.draw(spriteSet.boardTexture);

	if (game.GetLastMove().from != -1)
	{
		DrawLastMove(game.GetLastMove());
	}

	if (selectedPosition != -1)
	{
		DrawMoveSuggestion();
	}

	set<int> whitePieces = game.GetWhitePieces();
	for (int pos: whitePieces)
	{
		DrawPiece(game.GetPiece(pos), pos);
	}

	set<int> blackPieces = game.GetBlackPieces();
	for (int pos : blackPieces)
	{
		DrawPiece(game.GetPiece(pos), pos);
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
		sprite = spriteSet.bbishop;
		break;
	case B_KING:
        sprite = spriteSet.bking;
		break;
	case B_KNIGHT:
        sprite = spriteSet.bknight;
		break;
	case B_PAWN:
        sprite = spriteSet.bpawn;
		break;
	case B_QUEEN:
        sprite = spriteSet.bqueen;
		break;
	case B_ROCK:
        sprite = spriteSet.brock;
		break;
	case W_BISHOP:
        sprite = spriteSet.wbishop;
		break;
	case W_KING:
        sprite = spriteSet.wking;
		break;
	case W_KNIGHT:
        sprite = spriteSet.wknight;
		break;
	case W_PAWN:
        sprite = spriteSet.wpawn;
		break;
	case W_QUEEN:
        sprite = spriteSet.wqueen;
		break;
	case W_ROCK:
        sprite = spriteSet.wrock;
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
        spriteSet.highlightMoveSuggestion.setPosition((float)mouseX, (float)mouseY);
        window.draw(spriteSet.highlightMoveSuggestion);
	}

	int selectedX, selectedY;
	BoardToScreen(selectedPosition, selectedX, selectedY);
    spriteSet.highlightPiece.setPosition((float)selectedX, (float)selectedY);

    window.draw(spriteSet.highlightPiece);
}

void BoardInterface::DrawLastMove(Move& move)
{
	int fromX, fromY, toX, toY;
	BoardToScreen(move.from, fromX, fromY);
	BoardToScreen(move.to, toX, toY);
    spriteSet.highlightLastFrom.setPosition((float)fromX, (float)fromY);
    spriteSet.highlightLastTo.setPosition((float)toX, (float)toY);
    window.draw(spriteSet.highlightLastFrom);
    window.draw(spriteSet.highlightLastTo);
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