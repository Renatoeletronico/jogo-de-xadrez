#include "board.h"
#include <iostream>
#include "debug.h"

using namespace sf;

Board::Board() {
    size = 64;
    selectedRow = -1;
    selectedCol = -1;

    int initial[8][8] = {
        {-5,-4,-3,-2,-1,-3,-4,-5},
        {-6,-6,-6,-6,-6,-6,-6,-6},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 6, 6, 6, 6, 6, 6, 6, 6},
        { 5, 4, 3, 2, 1, 3, 4, 5}
    };

    memcpy(boardState, initial, sizeof(boardState));
    hasSelection = false;
}

void Board::handleClick(int mouseX, int mouseY) {
    int size = 64;

    int x = mouseY / size;
    int y = mouseX / size;

    if (!hasSelection) {
        if (boardState[x][y] != 0) {
            selectedRow = x;
            selectedCol = y;
            hasSelection = true;
        
        LOG("Peca selecionada: X=" << x << " Y=" << y);

            calculateMoves(x, y);
        }
    } else {
        bool validMove = false;

        for (auto move : possibleMoves) {
            if (move.first == x && move.second == y) {
                validMove = true;
                break;
            }
        }

        if (validMove) {
            boardState[x][y] = boardState[selectedRow][selectedCol];
            boardState[selectedRow][selectedCol] = 0;
        }

        hasSelection = false;
        possibleMoves.clear();
    }
}

void Board::draw(RenderWindow& window) {

    RectangleShape highlight(Vector2f(size, size));

    // 🟡 peça selecionada
    if (hasSelection) {
        highlight.setFillColor(Color(255, 255, 0, 100));
        highlight.setPosition(Vector2f(selectedCol * size, selectedRow * size));
        window.draw(highlight);
    }

    // 🟢 movimentos possíveis
    highlight.setFillColor(Color(0, 255, 0, 100));

    for (auto move : possibleMoves) {
        highlight.setPosition(Vector2f(move.second * size, move.first * size));
        window.draw(highlight);
    }
}

int Board::getPiece(int row, int col) {
    return boardState[row][col];
}

int Board::getSize() {
    return size;
}

void Board::calculateMoves(int x, int y) {
    possibleMoves.clear();

    int piece = boardState[x][y];

    // Peça: peão branco
    if (piece == 6 && selectedCol <= 7) {
        if (selectedRow == 6 && boardState[x-2][y] == 0 && boardState[x-1][y] == 0)
            possibleMoves.push_back({x-2, y});
        if (boardState[x-1][y] == 0 )
            possibleMoves.push_back({x-1, y});
        if (x>0 && boardState[x-1][y-1] != 0)
            possibleMoves.push_back({x-1, y-1});
        if (x<7 && boardState[x-1][y+1] != 0)
            possibleMoves.push_back({x-1, y+1});
    }

    // Peça: peão preto
    if (piece == -6 && selectedCol >= 0) {
        if (selectedRow == 1 && boardState[x+2][y] == 0 && boardState[x+1][y] == 0)
            possibleMoves.push_back({x+2, y});
        if (boardState[x+1][y] == 0)
            possibleMoves.push_back({x+1, y});
        if (x>0 && boardState[x+1][y-1] != 0)
            possibleMoves.push_back({x+1, y-1});    
        if (x<7 && boardState[x+1][y+1] != 0)
            possibleMoves.push_back({x+1, y+1});
    }
        // Peça: torre branca
    if (piece == 5 && selectedCol <= 7) {
        for (int i = x-1; i >= 0; i--) {
            if (boardState[i][y] == 0)
                possibleMoves.push_back({i, y});
            else {
                if (boardState[i][y] < 0)
                    possibleMoves.push_back({i, y});
                break;
            }
        }
        for (int i = x+1; i <= 7; i++) {
            if (boardState[i][y] == 0)
                possibleMoves.push_back({i, y});
            else {
                if (boardState[i][y] < 0)
                    possibleMoves.push_back({i, y});
                break;
            }
        }
        for (int j = y-1; j >= 0; j--) {
            if (boardState[x][j] == 0)
                possibleMoves.push_back({x, j});
            else {
                if (boardState[x][j] < 0)
                    possibleMoves.push_back({x, j});
                break;
            }
        }
        for (int j = y+1; j <= 7; j++) {
            if (boardState[x][j] == 0)
                possibleMoves.push_back({x, j});
            else {
                if (boardState[x][j] < 0)
                    possibleMoves.push_back({x, j});
                break;
            }
        }
    }

    // Peça: torre preta
    if (piece == -5 && selectedCol >= 0) {  
        for (int i = x-1; i >= 0; i--) {
            if (boardState[i][y] == 0)
                possibleMoves.push_back({i, y});
            else {
                if (boardState[i][y] > 0)
                    possibleMoves.push_back({i, y});
                break;
            }
        }
        for (int i = x+1; i <= 7; i++) {
            if (boardState[i][y] == 0)
                possibleMoves.push_back({i, y});
            else {
                if (boardState[i][y] > 0)
                    possibleMoves.push_back({i, y});
                break;
            }
        }
        for (int j = y-1; j >= 0; j--) {
            if (boardState[x][j] == 0)
                possibleMoves.push_back({x, j});
            else {
                if (boardState[x][j] > 0)
                    possibleMoves.push_back({x, j});
                break;
            }
        }
        for (int j = y+1; j <= 7; j++) {
            if (boardState[x][j] == 0)
                possibleMoves.push_back({x, j});
            else {
                if (boardState[x][j] > 0)
                    possibleMoves.push_back({x, j});
                break;
            }
        }
    }

    // Peça: bispo branco
    if (piece == 3 && selectedCol <= 7) {
        for (int i = x-1, j = y-1; i >= 0 && j >= 0; i--, j--) {
            if (boardState[i][j] == 0)
                possibleMoves.push_back({i, j});
            else {
                if (boardState[i][j] < 0)
                    possibleMoves.push_back({i, j});
                break;
            }
        }
        for (int i = x-1, j = y+1; i >= 0 && j <= 7; i--, j++) {
            if (boardState[i][j] == 0)
                possibleMoves.push_back({i, j});
            else {
                if (boardState[i][j] < 0)
                    possibleMoves.push_back({i, j});
                break;
            }
        }
        for (int i = x+1, j = y-1; i <= 7 && j >= 0; i++, j--) {
            if (boardState[i][j] == 0)
                possibleMoves.push_back({i, j});
            else {
                if (boardState[i][j] < 0)
                    possibleMoves.push_back({i, j});
                break;
            }
        }
        for (int i = x+1, j = y+1; i <= 7 && j <= 7; i++, j++) {
            if (boardState[i][j] == 0)
                possibleMoves.push_back({i, j});
            else {
                if (boardState[i][j] < 0)
                    possibleMoves.push_back({i, j});
                break;
            }
        }
    }

    // Peça: bispo preto
    if (piece == -3 && selectedCol >= 0) {  
        for (int i = x-1, j = y-1; i >= 0 && j >= 0; i--, j--) {
            if (boardState[i][j] == 0)
                possibleMoves.push_back({i, j});
            else {
                if (boardState[i][j] > 0)
                    possibleMoves.push_back({i, j});
                break;
            }
        }
        for (int i = x-1, j = y+1; i >= 0 && j <= 7; i--, j++) {
            if (boardState[i][j] == 0)
                possibleMoves.push_back({i, j});
            else {
                if (boardState[i][j] > 0)
                    possibleMoves.push_back({i, j});
                break;
            }
        }
        for (int i = x+1, j = y-1; i <= 7 && j >= 0; i++, j--) {
            if (boardState[i][j] == 0)
                possibleMoves.push_back({i, j});
            else {
                if (boardState[i][j] > 0)
                    possibleMoves.push_back({i, j});
                break;
            }
        }
        for (int i = x+1, j = y+1; i <= 7 && j <= 7; i++, j++) {
            if (boardState[i][j] == 0)
                possibleMoves.push_back({i, j});
            else {
                if (boardState[i][j] > 0)
                    possibleMoves.push_back({i, j});
                break;
            }
        }
    }

        // Peça: cavalo branco
    if (piece == 4 && selectedCol <= 7) {   
        int moves[8][2] = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
            {1, -2}, {1, 2}, {2, -1}, {2, 1}
        };

        for (auto move : moves) {
            int newX = x + move[0];
            int newY = y + move[1];

            if (newX >= 0 && newX <= 7 && newY >= 0 && newY <= 7) {
                if (boardState[newX][newY] <= 0)
                    possibleMoves.push_back({newX, newY});
            }
        }
    }

    // Peça: cavalo preto
    if (piece == -4 && selectedCol >= 0) {  
        int moves[8][2] = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
            {1, -2}, {1, 2}, {2, -1}, {2, 1}
        };

        for (auto move : moves) {
            int newX = x + move[0];
            int newY = y + move[1];

            if (newX >= 0 && newX <= 7 && newY >= 0 && newY <= 7) {
                if (boardState[newX][newY] >= 0)
                    possibleMoves.push_back({newX, newY});
            }
        }
    }

    // Peça: rei branco
    if (piece == 1 && selectedCol <= 7) {  
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;

                int newX = x + i;
                int newY = y + j;

                if (newX >= 0 && newX <= 7 && newY >= 0 && newY <= 7) {
                    if (boardState[newX][newY] <= 0)
                        possibleMoves.push_back({newX, newY});
                }
            }
        }
    }

    // Peça: rei preto
    if (piece == -1 && selectedCol >= 0) {

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;

                int newX = x + i;
                int newY = y + j;

                if (newX >= 0 && newX <= 7 && newY >= 0 && newY <= 7) {
                    if (boardState[newX][newY] >= 0)
                        possibleMoves.push_back({newX, newY});
                }
            }
        }
    }

        //Peça: rainha branca
 if (piece == 2 && selectedCol <= 7) { // rainha branca

    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}, // torre
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1} // bispo
    };

    for (auto dir : directions) {
        int i = x + dir[0];
        int j = y + dir[1];

        while (i >= 0 && i < 8 && j >= 0 && j < 8) {

            if (boardState[i][j] == 0) {
                possibleMoves.push_back({i, j});
            } else {
                if (boardState[i][j] < 0) // inimigo
                    possibleMoves.push_back({i, j});
                break;
            }

            i += dir[0];
            j += dir[1];
        }
    }
}
        // Peça: rainha preta
    if (piece == -2 && selectedCol >= 0) { // rainha preta
    
        int directions[8][2] = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}, // torre
            {-1, -1}, {-1, 1}, {1, -1}, {1, 1} // bispo
        };
    
        for (auto dir : directions) {
            int i = x + dir[0];
            int j = y + dir[1];
    
            while (i >= 0 && i < 8 && j >= 0 && j < 8) {
    
                if (boardState[i][j] == 0) {
                    possibleMoves.push_back({i, j});
                } else {
                    if (boardState[i][j] > 0) // inimigo
                        possibleMoves.push_back({i, j});
                    break;
                }
    
                i += dir[0];
                j += dir[1];
            }
        }
}
}

void Board::drawHighlights(sf::RenderWindow& window) {

    RectangleShape highlight(Vector2f(size, size));

    // 🟡 peça selecionada
    if (hasSelection) {
        highlight.setFillColor(Color(255, 255, 0, 100));
        highlight.setPosition(Vector2f(selectedCol * size, selectedRow * size));
        window.draw(highlight);
    }

    // 🟢 movimentos possíveis
    highlight.setFillColor(Color(0, 255, 0, 100));

    for (auto move : possibleMoves) {
        highlight.setPosition(Vector2f(move.second * size, move.first * size));
        window.draw(highlight);
    }
}

std::pair<int,int> Board::findKing(bool isWhite) {
    int king = isWhite ? 1 : -1;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (boardState[i][j] == king)
                return {i, j};
        }
    }

    return {-1, -1}; // erro
}

bool Board::isSquareUnderAttack(int x, int y, bool byWhite) {

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            int piece = boardState[i][j];

            if (piece == 0) continue;

            // verifica apenas peças da cor atacante
            if (byWhite && piece < 0) continue;
            if (!byWhite && piece > 0) continue;

            // gera movimentos dessa peça
            std::vector<std::pair<int,int>> movesBackup = possibleMoves;
            possibleMoves.clear();

            calculateMoves(i, j);

            for (auto move : possibleMoves) {
                if (move.first == x && move.second == y) {
                    possibleMoves = movesBackup;
                    return true;
                }
            }

            possibleMoves = movesBackup;
        }
    }

    return false;
}

bool Board::isKingInCheck(bool isWhite) {
    auto kingPos = findKing(isWhite);

    return isSquareUnderAttack(
        kingPos.first,
        kingPos.second,
        !isWhite // inimigo
    );
}

void Board::addDirectionalMoves(
    std::vector<std::pair<int,int>>& moves,
    int x, int y,
    int directions[][2],
    int dirCount,
    bool isWhite,
    bool repeat // torre/bispo/rainha = true | rei/cavalo = false
) {
    for (int d = 0; d < dirCount; d++) {

        int dx = directions[d][0];
        int dy = directions[d][1];

        int i = x + dx;
        int j = y + dy;

        while (i >= 0 && i < 8 && j >= 0 && j < 8) {

            if (boardState[i][j] == 0) {
                moves.push_back({i, j});
            } else {
                if ((isWhite && boardState[i][j] < 0) ||
                    (!isWhite && boardState[i][j] > 0)) {
                    moves.push_back({i, j});
                }
                break;
            }

            if (!repeat) break;

            i += dx;
            j += dy;
        }
    }
}