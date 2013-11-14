#pragma once
#include <SFML/Graphics.hpp>

#include "Board.h"
#include "Move.h"
#include "Piece.h"
#include "Game.h"
#include "PromoteWindow.h"


class BoardInterface
{
public:
	BoardInterface(sf::RenderWindow&);
	void OnClick(int x, int y);
	void Draw();
	void Undo();
private:
	void BoardInterface::DrawPiece(PieceType type, int boardPosition);
	void DrawLastMove(Move& move);
	void DrawMoveSuggestion();
	int ScreenToBoard(int screenX, int screenY);
	void BoardToScreen(int board, int& x, int& y);
	void LoadTextures();

	Game game;
	int tileSize;
	static const int border = 50;
	int screenW;
	int screenH;
	int selectedPosition = -1;
	bool pendingPromote = false;
	int promoteTo;
	PromoteWindow promoteWindow;

	void DrawGizmo(int boardPosition);
	void DrawGizmo(int x, int y);

	sf::RenderWindow& window;

	sf::Texture tx_boardTexture;
	sf::Texture tx_highlightlast;
	sf::Texture tx_highlight;

	sf::Texture tx_wking;
	sf::Texture tx_wqueen;
	sf::Texture tx_wbishop;
	sf::Texture tx_wknight;
	sf::Texture tx_wrock;
	sf::Texture tx_wpawn;

	sf::Texture tx_bking;
	sf::Texture tx_bqueen;
	sf::Texture tx_bbishop;
	sf::Texture tx_bknight;
	sf::Texture tx_brock;
	sf::Texture tx_bpawn;

	sf::Sprite boardTexture;
	sf::Sprite highlightLastFrom;
	sf::Sprite highlightLastTo;
	sf::Sprite highlightPiece;
	sf::Sprite highlightMoveSuggestion;

	sf::Sprite wking;
	sf::Sprite wqueen;
	sf::Sprite wbishop;
	sf::Sprite wknight;
	sf::Sprite wrock;
	sf::Sprite wpawn;

	sf::Sprite bking;
	sf::Sprite bqueen;
	sf::Sprite bbishop;
	sf::Sprite bknight;
	sf::Sprite brock;
	sf::Sprite bpawn;
};
