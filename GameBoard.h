

#ifndef EX4_GAMEBOARD_H
#define EX4_GAMEBOARD_H


#include "ex4_header.h"

template<typename GAME_PIECE>
using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;

template<int ROWS, int COLS,class GAME_PIECE, int playersNum=2>
class GameBoard {

    PieceInfo<GAME_PIECE> board[ROWS][COLS];
    int firstRowIndex;
    int firstColIndex;
public:
    GameBoard(){


    }
    PieceInfo<GAME_PIECE> getPiece(int row, int col){
        if(board[row][col]==NULL)
            return nullptr;

        return std::move(std::make_unique<const std::pair<int, GAME_PIECE>>
                                 (board[row][col]->first, board[row][col]->second));
    }
    PieceInfo<GAME_PIECE> setPiece(int row, int col, GAME_PIECE piece, int player){
        auto oldPiece=move(board[row][col]);
        board[row][col]=std::make_unique<const std::pair<int, GAME_PIECE>>(player, piece);
        return oldPiece==NULL ? nullptr:std::move(oldPiece);

    }

    class AllIterator{
        int row;
        int col;
        GameBoard<ROWS,COLS,GAME_PIECE,playersNum> * b;
        std::tuple<int,int,GAME_PIECE,int> curPiece;
        friend class GameBoard;

        AllIterator(GameBoard<ROWS,COLS,GAME_PIECE,playersNum>&  _board, int i, int j, GAME_PIECE piece,int playerNum) {
            b=&_board;
            curPiece=std::make_tuple(i,j,piece,playerNum);
            row=i;
            col=j;
        }
    public:
        AllIterator operator++()  {
            col++;
            for (; row< ROWS; row++) {
                for (; col < COLS; col++) {
                    if (b->board[row][col] != NULL) {
                        curPiece=std::make_tuple(row, col, b->board[row][col]->second, b->board[row][col]->first);
                        return *this;
                    }
                }
                col=0;
            }
            *this= b->end();
            return *this;
        }
        const std::tuple<int, int, GAME_PIECE, int> operator*() {
            curPiece=std::make_tuple(row,col,std::get<2>(curPiece),std::get<3>(curPiece));
            return curPiece;
        }
        bool operator!=(AllIterator other) {
            return !(other.curPiece==curPiece);
        }
    };

    AllIterator begin() {
        for(int i=0;i<ROWS;i++){
            for(int j=0;j<COLS;j++){
                if(board[i][j]!=NULL){
                    firstRowIndex=i;
                    firstColIndex=j;
                    AllIterator it(*this,i,j,board[i][j]->second,board[i][j]->first);
                    return it;
                }
            }
        }
        return end();
    }

    AllIterator end() {
        AllIterator it(*this,ROWS,COLS,board[0][0]->second,-1);
        return it;
//        for(int i=ROWS-1;i>=0;i--){
//            for(int j=COLS-1;j>=0;j--){
//                if(board[i][j]!=NULL){
//                    AllIterator it(*this,i,j,board[i][j]->second,board[i][j]->first);
//                    return it;
//                }
//            }
//        }
    }

};




#endif //EX4_GAMEBOARD_H
