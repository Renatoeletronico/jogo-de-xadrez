#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Board {
public:
    Board();
    int getSize();
    int getPiece(int row, int col);
    void handleClick(int x, int y);
    void draw(sf::RenderWindow& window);
    std::vector<std::pair<int,int>> getPossibleMoves();
    void drawHighlights(sf::RenderWindow& window);
    std::pair<int,int> Board::findKing(bool isWhite);
    bool Board::isSquareUnderAttack(int x, int y, bool byWhite);
    bool Board::isKingInCheck(bool isWhite);
    void Board::addDirectionalMoves(
    std::vector<std::pair<int,int>>& moves,
    int x, int y,
    int directions[][2],
    int dirCount,
    bool isWhite,
    bool repeat // torre/bispo/rainha = true | rei/cavalo = false
) ;

private:
    int boardState[8][8];
    int size;
    
    int selectedRow;
    int selectedCol;

    bool hasSelection;
    int selectedX, selectedY;

    std::vector<std::pair<int,int>> possibleMoves;

    void calculateMoves(int x, int y);
};