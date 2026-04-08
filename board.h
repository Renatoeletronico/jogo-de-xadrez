#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Board {
public:
    Board();

    bool isPromotionPending() const;
    int promotionRow, promotionCol;
    bool whiteTurn;
    int size;

    int getSize();
    int getPiece(int row, int col);

    void handleClick(int x, int y);
    void draw(sf::RenderWindow& window);
    void drawHighlights(sf::RenderWindow& window);

    std::vector<std::pair<int,int>> getMoves(int x, int y);
    std::vector<std::pair<int,int>> getValidMoves(int x, int y);
    std::vector<std::pair<int,int>> getAttackMoves(int x, int y);

    std::pair<int,int> findKing(bool isWhite);
    bool isSquareUnderAttack(int x, int y, bool byWhite);
    bool isKingInCheck(bool isWhite);
    bool isInside(int x, int y);

    bool isCheckmate(bool isWhite);
    bool hasAnyValidMove(bool isWhite);

    void updateWindowTitle(sf::RenderWindow& window);
    bool checkPawnPromotion(int x, int y, int piece);
    void promotePawn(int choice);


private:
    int boardState[8][8];

    int selectedRow;
    int selectedCol;

    bool hasSelection;
    bool xequeAlert;

    bool promotionPending = false;
    int promotionX = -1;
    int promotionY = -1;
    bool promotionWhite = true;
    

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

