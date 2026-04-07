#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.h"
#include <windows.h>
#include <io.h>
#include <fcntl.h>

Board board;
using namespace sf;

// Texturas globais
Texture wKing, wQueen, wBishop, wKnight, wRook, wPawn;
Texture bKing, bQueen, bBishop, bKnight, bRook, bPawn;

// Retorna textura correta
Texture* getTexture(int piece) {
    switch(piece) {
        case 1: return &wKing;
        case 2: return &wQueen;
        case 3: return &wBishop;
        case 4: return &wKnight;
        case 5: return &wRook;
        case 6: return &wPawn;

        case -1: return &bKing;
        case -2: return &bQueen;
        case -3: return &bBishop;
        case -4: return &bKnight;
        case -5: return &bRook;
        case -6: return &bPawn;
    }
    return nullptr;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::locale::global(std::locale(""));

    // 🔥 SFML 3 mudou isso aqui
    RenderWindow window(VideoMode(Vector2u(512, 512)), "Xadrez");

    Texture boardTexture;

    if (!boardTexture.loadFromFile("assets/tabuleiro-xadrez.png")) {
        std::cout << "Erro ao carregar tabuleiro-de-xadrez.png\n";
        return -1;
    }

    // Carregar peças
    wKing.loadFromFile("assets/rei_branco.png");
    wQueen.loadFromFile("assets/rainha_branca.png");
    wBishop.loadFromFile("assets/bispo_branco.png");
    wKnight.loadFromFile("assets/cavalo_branco.png");
    wRook.loadFromFile("assets/torre_branca.png");
    wPawn.loadFromFile("assets/peao_branco.png");

    bKing.loadFromFile("assets/rei_preto.png");
    bQueen.loadFromFile("assets/rainha_preta.png");
    bBishop.loadFromFile("assets/bispo_preto.png");
    bKnight.loadFromFile("assets/cavalo_preto.png");
    bRook.loadFromFile("assets/torre_preta.png");
    bPawn.loadFromFile("assets/peao_preto.png");

    Sprite boardSprite (boardTexture);


    while (window.isOpen()) {

        // 🔥 SFML 3 usa optional<Event>
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

        if (event->is<Event::MouseButtonPressed>()) {
            auto mouse = event->getIf<Event::MouseButtonPressed>();

            if (mouse->button == Mouse::Button::Left) {
                board.handleClick(mouse->position.x, mouse->position.y);
                }
            }
        }
    

        window.clear();
        window.draw(boardSprite);

        // desenhar peças
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {

                int p = board.getPiece(i, j);
                if (p == 0) continue;

                Texture* tex = getTexture(p);
                if (!tex) continue;

                Sprite pieceSprite(*tex);

                // 🔥 SFML 3 usa Vector2f
                pieceSprite.setPosition(Vector2f(j * board.getSize(), i * board.getSize()));

                pieceSprite.setScale(Vector2f(
                    board.getSize()/ (float)tex->getSize().x,
                    board.getSize() / (float)tex->getSize().y
                ));

                window.draw(pieceSprite);
            }
        }
        board.draw(window);
        window.display();

    }

    return 0;
}