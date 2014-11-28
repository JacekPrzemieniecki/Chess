#include "SpriteSet.h"


 void SpriteSet::Init()
{
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