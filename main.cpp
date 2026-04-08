#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.h"
#include <windows.h>
#include <locale>

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
    // Configura console UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale(""));

    // Cria janela SFML
    RenderWindow window(VideoMode(Vector2u(512, 512)), "Xadrez");

    // Carrega texturas do tabuleiro e peças
    Texture boardTexture;
    if (!boardTexture.loadFromFile("assets/tabuleiro-xadrez.png")) {
        std::cout << "Erro ao carregar tabuleiro-xadrez.png\n";
        return -1;
    }

    // Carrega peças brancas
    wKing.loadFromFile("assets/rei_branco.png");
    wQueen.loadFromFile("assets/rainha_branca.png");
    wBishop.loadFromFile("assets/bispo_branco.png");
    wKnight.loadFromFile("assets/cavalo_branco.png");
    wRook.loadFromFile("assets/torre_branca.png");
    wPawn.loadFromFile("assets/peao_branco.png");

    // Carrega peças pretas
    bKing.loadFromFile("assets/rei_preto.png");
    bQueen.loadFromFile("assets/rainha_preta.png");
    bBishop.loadFromFile("assets/bispo_preto.png");
    bKnight.loadFromFile("assets/cavalo_preto.png");
    bRook.loadFromFile("assets/torre_preta.png");
    bPawn.loadFromFile("assets/peao_preto.png");

    Sprite boardSprite(boardTexture);

    while (window.isOpen()) {
        // Processa eventos
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

        // Se há promoção pendente, solicita escolha via console
        if (board.isPromotionPending()) {
            window.setTitle("1-Torre 2-Cavalo 3-Bispo 4-Rainha");

            int choice;
            std::cout << "Promover peão (1-Torre 2-Cavalo 3-Bispo 4-Rainha): ";
            while (true) {
                std::cin >> choice;
                if (choice >= 1 && choice <= 4)
                    break;
                std::cout << "Escolha inválida, tente novamente: ";
            }

            // Chama seu método de promoção existente
            board.promotePawn(choice);
        } else {
            window.setTitle("Jogo de Xadrez");
        }

        // Desenha tabuleiro
        window.clear();
        window.draw(boardSprite);

        // Desenha peças
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int p = board.getPiece(i, j);
                if (p == 0) continue;

                Texture* tex = getTexture(p);
                if (!tex) continue;

                Sprite pieceSprite(*tex);
                pieceSprite.setPosition(Vector2f(j * board.getSize(), i * board.getSize()));
                pieceSprite.setScale(Vector2f(
                    board.getSize() / (float)tex->getSize().x,
                    board.getSize() / (float)tex->getSize().y
                ));
                window.draw(pieceSprite);
            }
        }

        // Desenha destaques (seleção/movimentos válidos)
        board.drawHighlights(window);

        window.display();
    }

    return 0;
}