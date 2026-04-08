#include "board.h"
#include <iostream>
#include "debug.h"

using namespace sf;

Board::Board() {
    size = 64;
    selectedRow = -1;
    selectedCol = -1;
    whiteTurn = true;

    int initial[8][8] = {
        {-5,-4,-3,-1,-2,-3,-4,-5},
        {-6,-6,-6,-6,-6,-6,-6,-6},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 6, 6, 6, 6, 6, 6, 6, 6},
        { 5, 4, 3, 1, 2, 3, 4, 5}
    };

    memcpy(boardState, initial, sizeof(boardState));
    hasSelection = false;
}

void Board::handleClick(int mouseX, int mouseY) {

    int size = 64;
    int x = mouseY / size;
    int y = mouseX / size;

    // 🔥 proteção de limite
    if (!isInside(x, y))
        return;

    int piece = boardState[x][y];

    if (!hasSelection) {

        if (piece != 0) {

            // impede pegar peça errada
            if ((whiteTurn && piece < 0) || (!whiteTurn && piece > 0))
                return;

            selectedRow = x;
            selectedCol = y;
            hasSelection = true;

            possibleMoves = getValidMoves(x, y);
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

            // 🔥 promoção de peão
        if (checkPawnPromotion(x, y, boardState[x][y])) {
            return;
        }  

            // troca turno
            whiteTurn = !whiteTurn;

            // 🔥 verifica xeque APENAS UMA VEZ
            bool emXeque = isKingInCheck(whiteTurn);

            if (emXeque && !xequeAlert) {

                if (whiteTurn)
                    LOG("Rei branco em xeque!\n") ;
                else
                    LOG("Rei preto em xeque!\n");

                xequeAlert = true;
            }

            if (!emXeque) {
                xequeAlert = false;
            }

            // xeque-mate
            if (isCheckmate(whiteTurn)) {
                if (whiteTurn)
                    LOG("Xeque-mate! Pretas venceram\n");
                else
                    LOG("Xeque-mate! Brancas venceram\n");
            }
        }

        hasSelection = false;
        possibleMoves.clear();
    }
}

bool Board::checkPawnPromotion(int x, int y, int piece) {
    LOG("Checando promocao... X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Piece: " + std::to_string(piece) + "\n");
    if (abs(piece) != 6) return false;

    if ((piece > 0 && x == 0) || (piece < 0 && x == 7)) {

        LOG("Peao promovido! X: " + std::to_string(x) + " Y: " + std::to_string(y) + "\n");
        promotionPending = true;
        promotionX = x;
        promotionY = y;
        promotionWhite = (piece > 0);

        return true;
    }

    return false;
}

void Board::updateWindowTitle(sf::RenderWindow& window) {

    std::string titulo = "Jogo de Xadrez";

    if (isCheckmate(whiteTurn)) {
        if (whiteTurn)
            titulo += " - Xeque-mate! Pretas venceram";
        else
            titulo += " - Xeque-mate! Brancas venceram";
    }
    else if (isKingInCheck(whiteTurn)) {
        if (whiteTurn)
            titulo += " - Xeque no Rei Branco!";
        else
            titulo += " - Xeque no Rei Preto!";
    }

    window.setTitle(titulo);
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

void Board::drawHighlights(sf::RenderWindow& window) {

    RectangleShape highlight(Vector2f(size, size));

    // 🔴 1. rei em xeque (primeiro = fundo)
    for (bool isWhite : {true, false}) {

        if (isKingInCheck(isWhite)) {
            auto kingPos = findKing(isWhite);

            highlight.setFillColor(Color(255, 0, 0, 120));
            highlight.setPosition(Vector2f(kingPos.second * size, kingPos.first * size));

            window.draw(highlight);
        }
    }


    // 🟢 2. movimentos possíveis
    highlight.setFillColor(Color(0, 255, 0, 100));

    for (auto move : possibleMoves) {
        highlight.setPosition(Vector2f(move.second * size, move.first * size));
        window.draw(highlight);
    }

    // 🟡 3. peça selecionada (por cima de tudo)
    if (hasSelection) {
        highlight.setFillColor(Color(255, 255, 0, 100));
        highlight.setPosition(Vector2f(selectedCol * size, selectedRow * size));
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

bool Board::isInside(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool Board::isSquareUnderAttack(int x, int y, bool byWhite) {

    if (!isInside(x, y)) return false;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            int piece = boardState[i][j];
            if (piece == 0) continue;

            // pega só peças da cor atacante
            if (byWhite && piece < 0) continue;
            if (!byWhite && piece > 0) continue;

            auto attacks = getAttackMoves(i, j);

            for (auto move : attacks) {
                if (move.first == x && move.second == y) {
                    return true;
                }
            }
        }
    }

    return false;
}

std::vector<std::pair<int,int>> Board::getAttackMoves(int x, int y) {

    std::vector<std::pair<int,int>> moves;

    int piece = boardState[x][y];
    if (piece == 0) return moves;

    bool isWhite = piece > 0;

    // 🔥 PEÃO (ATAQUE DIFERENTE DO MOVIMENTO)
    if (abs(piece) == 6) {
        int dir = isWhite ? -1 : 1;

        int attacks[2][2] = {{dir, -1}, {dir, 1}};

        for (auto a : attacks) {
            int i = x + a[0];
            int j = y + a[1];

            if (isInside(i, j)) {
                moves.push_back({i, j});
            }
        }

        return moves;
    }

    // 🔥 TORRE
    if (abs(piece) == 5) {
        int dir[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
        addDirectionalMoves(moves, x, y, dir, 4, isWhite, true);
    }

    // 🔥 BISPO
    if (abs(piece) == 3) {
        int dir[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
        addDirectionalMoves(moves, x, y, dir, 4, isWhite, true);
    }

    // 🔥 RAINHA
    if (abs(piece) == 2) {
        int dir[8][2] = {
            {-1,0},{1,0},{0,-1},{0,1},
            {-1,-1},{-1,1},{1,-1},{1,1}
        };
        addDirectionalMoves(moves, x, y, dir, 8, isWhite, true);
    }

    // 🔥 REI
    if (abs(piece) == 1) {
        int dir[8][2] = {
            {-1,0},{1,0},{0,-1},{0,1},
            {-1,-1},{-1,1},{1,-1},{1,1}
        };
        addDirectionalMoves(moves, x, y, dir, 8, isWhite, false);
    }

    // 🔥 CAVALO
    if (abs(piece) == 4) {
        int dir[8][2] = {
            {-2,-1},{-2,1},{-1,-2},{-1,2},
            {1,-2},{1,2},{2,-1},{2,1}
        };
        addDirectionalMoves(moves, x, y, dir, 8, isWhite, false);
    }

    return moves;
}


std::vector<std::pair<int,int>> Board::getMoves(int x, int y) {

    std::vector<std::pair<int,int>> moves;

    int piece = boardState[x][y];
    if (piece == 0) return moves;

    bool isWhite = piece > 0;

    // TORRE
    if (abs(piece) == 5) {
        int dir[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
        addDirectionalMoves(moves, x, y, dir, 4, isWhite, true);
    }

    // BISPO
    if (abs(piece) == 3) {
        int dir[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
        addDirectionalMoves(moves, x, y, dir, 4, isWhite, true);
    }

    // RAINHA
    if (abs(piece) == 2) {
        int dir[8][2] = {
            {-1,0},{1,0},{0,-1},{0,1},
            {-1,-1},{-1,1},{1,-1},{1,1}
        };
        addDirectionalMoves(moves, x, y, dir, 8, isWhite, true);
    }

    // REI
    if (abs(piece) == 1) {
        int dir[8][2] = {
            {-1,0},{1,0},{0,-1},{0,1},
            {-1,-1},{-1,1},{1,-1},{1,1}
        };
        addDirectionalMoves(moves, x, y, dir, 8, isWhite, false);
    }

    // CAVALO
    if (abs(piece) == 4) {
        int dir[8][2] = {
            {-2,-1},{-2,1},{-1,-2},{-1,2},
            {1,-2},{1,2},{2,-1},{2,1}
        };
        addDirectionalMoves(moves, x, y, dir, 8, isWhite, false);
    }

    // PEÃO
    if (abs(piece) == 6) {

        int dir = isWhite ? -1 : 1;

        // andar 1 casa
        if (isInside(x + dir, y) && boardState[x + dir][y] == 0)
            moves.push_back({x + dir, y});

        // andar 2 casas (apenas no movimento inicial)
        if ((isWhite && x == 6) || (!isWhite && x == 1)) {
            if (isInside(x + dir, y) &&
                isInside(x + 2*dir, y) &&
                boardState[x + dir][y] == 0 &&
                boardState[x + 2*dir][y] == 0) {

                moves.push_back({x + 2*dir, y});
            }
        }

        // ataque
        int attack[2][2] = {{dir,-1},{dir,1}};

        for (auto a : attack) {
            int i = x + a[0];
            int j = y + a[1];

            if (isInside(i, j)) {
                if ((isWhite && boardState[i][j] < 0) ||
                    (!isWhite && boardState[i][j] > 0)) {
                    moves.push_back({i,j});
                }
            }
        }
    }

    return moves;
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

std::vector<std::pair<int,int>> Board::getValidMoves(int x, int y) {

    auto moves = getMoves(x, y);
    std::vector<std::pair<int,int>> valid;

    int piece = boardState[x][y];
    if (piece == 0) return valid;

    bool isWhite = piece > 0;

    for (const auto& move : moves) {

        int toX = move.first;
        int toY = move.second;

        int captured = boardState[toX][toY];

        // 🧠 SIMULA movimento
        boardState[toX][toY] = piece;
        boardState[x][y] = 0;

        // 🔥 testa se ficou em xeque
        if (!isKingInCheck(isWhite)) {
            valid.push_back(move);
        }

        // 🔁 DESFAZ movimento
        boardState[x][y] = piece;
        boardState[toX][toY] = captured;
    }

    return valid;
}

bool Board::hasAnyValidMove(bool isWhite) {

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            int piece = boardState[i][j];
            if (piece == 0) continue;

            // pega só peças da cor
            if (isWhite && piece < 0) continue;
            if (!isWhite && piece > 0) continue;

            auto moves = getValidMoves(i, j);

            if (!moves.empty()) {
                return true; // ainda tem jogada
            }
        }
    }

    return false; // sem jogadas
}

bool Board::isCheckmate(bool isWhite) {

    // 1. precisa estar em xeque
    if (!isKingInCheck(isWhite))
        return false;

    // 2. não pode ter nenhum movimento válido
    if (hasAnyValidMove(isWhite))
        return false;

    return true;
}

bool Board::isKingInCheck(bool isWhite) {
    auto kingPos = findKing(isWhite);

    bool result = isSquareUnderAttack(
        kingPos.first,
        kingPos.second,
        !isWhite
    );        

    return result;
}

void Board::promotePawn(int choice) {
    int newPiece = 0;

    // Mapear escolha 1-4 para peça
    switch(choice) {
        case 1: newPiece = 5; break; // Torre
        case 2: newPiece = 4; break; // Cavalo
        case 3: newPiece = 3; break; // Bispo
        case 4: newPiece = 2; break; // Rainha
    }

    if (!promotionWhite) newPiece = -newPiece;

    boardState[promotionRow][promotionCol] = newPiece;

    // Limpar promoção
    promotionPending = false;

    // Trocar turno apenas agora
    whiteTurn = !whiteTurn;
}

bool Board::isPromotionPending() const {
    return promotionPending;
}