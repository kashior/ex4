

#ifndef EX4_GAMEBOARD_H
#define EX4_GAMEBOARD_H


#include "ex4_header.h"

template<typename GAME_PIECE>
using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;


template<int ROWS, int COLS, class GAME_PIECE, int playersNum = 2>
class GameBoard {
    PieceInfo<GAME_PIECE> board[ROWS][COLS];
    bool isPlayer;
    bool isPiece;
    int player;
    GAME_PIECE _piece;
public:
    GameBoard():isPiece(false),isPlayer(false) {}

    PieceInfo<GAME_PIECE> getPiece(int row, int col) {
        if (board[row][col] == NULL)
            return nullptr;

        return std::move(std::make_unique<const std::pair<int, GAME_PIECE>>
                                 (board[row][col]->first, board[row][col]->second));
    }

    PieceInfo<GAME_PIECE> setPiece(int row, int col, GAME_PIECE piece, int player) {
        auto oldPiece = move(board[row][col]);
        board[row][col] = std::make_unique<const std::pair<int, GAME_PIECE>>(player, piece);
        return oldPiece == NULL ? nullptr : std::move(oldPiece);

    }

    class AllIterator {
        int row;
        int col;
        bool playerCheck;
        bool pieceCheck;
        GameBoard<ROWS, COLS, GAME_PIECE, playersNum> *b;
        std::tuple<int, int, GAME_PIECE, int> curPiece;

        friend class GameBoard;

        AllIterator(GameBoard<ROWS, COLS, GAME_PIECE, playersNum> &_board, int i, int j, GAME_PIECE piece,
                    int playerNum,bool _isPlayer,bool _isPiece):playerCheck(_isPlayer),pieceCheck(_isPiece) {
            b = &_board;
            curPiece = std::make_tuple(i, j, piece, playerNum);
            row = i;
            col = j;
        }

    public:
        AllIterator operator++() {
            col++;
            for (; row < ROWS; row++) {
                for (; col < COLS; col++) {
                    if (b->board[row][col] != NULL) {
                        if ((playerCheck && b->player == b->board[row][col]->first) || !playerCheck) {
                            if ((pieceCheck && b->_piece == b->board[row][col]->second )|| !pieceCheck) {
                                curPiece = std::make_tuple(row, col, b->board[row][col]->second,
                                                           b->board[row][col]->first);
                                return *this;
                            }
                        }
                    }
                }
                col = 0;
            }
            *this = b->end();
            return *this;
        }

        const std::tuple<int, int, GAME_PIECE, int> operator*() {
            curPiece = std::make_tuple(row, col, std::get<2>(curPiece), std::get<3>(curPiece));
            return curPiece;
        }

        bool operator!=(AllIterator other) {
            return !(other.curPiece == curPiece);
        }
    };


    GameBoard& allPiecesOfPlayer(int playerNum) {
        isPlayer = true;
        player = playerNum;
        return *this;

    }
    GameBoard& allOccureneceOfPiece(GAME_PIECE piece) {
        isPiece = true;
        _piece = piece;
        return *this;

    }


    AllIterator begin() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j] != NULL) {
                    if ((isPlayer && player == board[i][j]->first) || !isPlayer) {
                        if ((isPiece && _piece == board[i][j]->second )|| !isPiece) {
                            AllIterator it(*this, i, j, board[i][j]->second, board[i][j]->first,isPlayer,isPiece);
                            isPlayer=false;
                            isPiece=false;
                            return it;
                        }
                    }

                }
            }
        }
        return end();
    }

    AllIterator end() {
        AllIterator it(*this, ROWS, COLS, board[0][0]->second, -1,isPlayer,isPiece);
        return it;
    }

    ~GameBoard() {}

};


#endif //EX4_GAMEBOARD_H
