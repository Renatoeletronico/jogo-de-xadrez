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
    void drawHighlights(sf::RenderWindow& window);

    std::vector<std::pair<int,int>> getMoves(int x, int y);
    std::vector<std::pair<int,int>> getValidMoves(int x, int y);

    std::pair<int,int> findKing(bool isWhite);
    bool isSquareUnderAttack(int x, int y, bool byWhite);
    bool isKingInCheck(bool isWhite);

private:
    int boardState[8][8];
    int size;

    int selectedRow;
    int selectedCol;

    bool hasSelection;

    std::vector<std::pair<int,int>> possibleMoves;

    void addDirectionalMoves(
        std::vector<std::pair<int,int>>& moves,
        int x, int y,
        int directions[][2],
        int dirCount,
        bool isWhite,
        bool repeat
    );
};