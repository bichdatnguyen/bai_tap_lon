#ifndef _LOGIC__CPP
#define _LOGIC__CPP
#include "def.h"

struct Coordinate
{
    int x, y;
};

struct logicGame
{
    string board[BOARD_SIZE][BOARD_SIZE];
    bool whiteTurn, blackTurn;
    bool wkHadMove, bkHadMove, wQuRoHadMove, wKiRoHadMove, bQuRoHadMove, bKiRoHadMove;
    int countMoves;
    vector<pair<string, pair<Coordinate, Coordinate>>> moveHadPlay;

    void init()
    {
        whiteTurn = true, blackTurn = false;
        wkHadMove = false, bkHadMove = false, wQuRoHadMove = false, wKiRoHadMove = false, bQuRoHadMove = false, bKiRoHadMove = false;
        countMoves = 0;
        for(int i=0; i<BOARD_SIZE; i++)
            for(int j=0; j<BOARD_SIZE; j++)
                board[i][j] = EMPTY_CELL;

        for(int i=0; i<BOARD_SIZE; i++) {
            board[1][i] = "bPa";
            board[6][i] = "wPa";
        }
        board[0][0] = "bRo";
        board[0][7] = "bRo";
        board[7][0] = "wRo";
        board[7][7] = "wRo";
        board[0][1] = "bKn";
        board[0][6] = "bKn";
        board[7][1] = "wKn";
        board[7][6] = "wKn";
        board[0][2] = "bBi";
        board[0][5] = "bBi";
        board[7][2] = "wBi";
        board[7][5] = "wBi";
        board[0][3] = "bQu";
        board[7][3] = "wQu";
        board[0][4] = "bKi";
        board[7][4] = "wKi";
    }

    void choose(Coordinate &pos)
    {
        SDL_GetMouseState(&pos.x, &pos.y);
        pos.x /= CELL_SIZE;
        pos.y /= CELL_SIZE;
    }

    bool isCheckmate()
    {
        bool wCanMove = false, bCanMove = false;
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                if(board[i][j] == EMPTY_CELL) {
                    continue;
                }
                if(canMove({j, i})) {
                    if(board[i][j][0] == 'w') {
                        wCanMove = true;
                    }
                    if(board[i][j][0] == 'b') {
                        bCanMove = true;
                    }
                }
                else continue;
            }
        }
        Coordinate wKingPos, bKingPos;
        wKingPos = kingPosition('w');
        bKingPos = kingPosition('b');
        if(!wCanMove && whiteTurn && isCheck(wKingPos, 'w')) return true;
        if(!bCanMove && blackTurn && isCheck(bKingPos, 'b')) return true;
        return false;
    }

    bool stalematePAT()
    {
        bool wCanMove = false, bCanMove = false;
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                if(board[i][j] == EMPTY_CELL) continue;
                if(canMove({j, i})) {
                    if(board[i][j][0] == 'w') wCanMove = true;
                    if(board[i][j][0] == 'b') bCanMove = true;
                }
            }
        }
        Coordinate wKingPos, bKingPos;
        wKingPos = kingPosition('w');
        bKingPos = kingPosition('b');
        if((!wCanMove && whiteTurn && !isCheck(wKingPos, 'w')) ||
           (!bCanMove && blackTurn && !isCheck(bKingPos, 'b'))) {
            return true;
        }
        return false;
    }

    bool stalemateDueToPieces()
    {
        int countPieces = 0, knight = 0, bishop = 0;
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                if(board[j][i] != EMPTY_CELL && board[j][i] != "wKi" && board[j][i] != "bKi") {
                    if(board[j][i] == "wKn" || board[j][i] == "bKn") knight++;
                    if(board[j][i] == "wBi" || board[j][i] == "bBi") bishop++;
                    countPieces++;
                }
            }
        }
        if(countPieces == 0) return true;
        if(countPieces == 1) {
            if(knight == 1 || bishop == 1) return true;
            return false;
        }
        return false;
    }

    bool stalemateDueToLimitedMoves()
    {
        if(countMoves == 100) return true;
        return false;
    }

    bool sameMove(pair<Coordinate, Coordinate> a, pair<Coordinate, Coordinate> b)
    {
        if(a.first.x == b.first.x && a.first.y == b.first.y && a.second.x == b.second.x && a.second.y == b.second.y)
            return true;
        return false;
    }

    bool reverseMove(pair<Coordinate, Coordinate> a, pair<Coordinate, Coordinate> b)
    {
        if(a.first.x == b.second.x && a.first.y == b.second.y && a.second.x == b.first.x && a.second.y == b.first.y)
            return true;
        return false;
    }


    bool stalemateDueToSameMoves()
    {
        int moves = moveHadPlay.size();
        if(moves < 12)
            return false;
        if((moveHadPlay[moves - 12].first == moveHadPlay[moves - 10].first &&
           moveHadPlay[moves - 10].first == moveHadPlay[moves - 8].first &&
           moveHadPlay[moves - 8].first == moveHadPlay[moves - 6].first &&
           moveHadPlay[moves - 6].first == moveHadPlay[moves - 4].first &&
           moveHadPlay[moves - 4].first == moveHadPlay[moves - 2].first) &&
           (moveHadPlay[moves - 11].first == moveHadPlay[moves - 9].first &&
           moveHadPlay[moves - 9].first == moveHadPlay[moves - 7].first &&
           moveHadPlay[moves - 7].first == moveHadPlay[moves - 5].first &&
           moveHadPlay[moves - 5].first == moveHadPlay[moves - 3].first &&
           moveHadPlay[moves - 3].first == moveHadPlay[moves - 1].first) &&
           (sameMove(moveHadPlay[moves - 12].second, moveHadPlay[moves - 8].second) &&
           sameMove(moveHadPlay[moves - 8].second, moveHadPlay[moves - 4].second)) &&
           (sameMove(moveHadPlay[moves - 11].second, moveHadPlay[moves - 7].second) &&
           sameMove(moveHadPlay[moves - 7].second, moveHadPlay[moves - 3].second)) &&
           (sameMove(moveHadPlay[moves - 10].second, moveHadPlay[moves - 6].second) &&
           sameMove(moveHadPlay[moves - 6].second, moveHadPlay[moves - 2].second)) &&
           (sameMove(moveHadPlay[moves - 9].second, moveHadPlay[moves - 5].second) &&
           sameMove(moveHadPlay[moves - 5].second, moveHadPlay[moves - 1].second)) &&
           (reverseMove(moveHadPlay[moves - 12].second, moveHadPlay[moves - 10].second) &&
           reverseMove(moveHadPlay[moves - 11].second, moveHadPlay[moves - 9].second)))
            return true;
        return false;
    }

    bool isStalemate()
    {
        return (stalematePAT() || stalemateDueToPieces() || stalemateDueToLimitedMoves() || stalemateDueToSameMoves());
    }

    bool legalMoveVertical(Coordinate piecePos, Coordinate moveTo)
    {
        if(board[piecePos.y][piecePos.x][0] == board[moveTo.y][moveTo.x][0])
            return false;

        if(piecePos.x == moveTo.x) {
            if(piecePos.y < moveTo.y) {
                for(int i = 1; i < (moveTo.y - piecePos.y); i++) {
                    if(board[piecePos.y + i][piecePos.x] != EMPTY_CELL)
                        return false;
                }
                return true;
            }
            else {
                for(int i = 1; i < (piecePos.y - moveTo.y); i++) {
                    if(board[piecePos.y - i][piecePos.x] != EMPTY_CELL)
                        return false;
                }
                return true;
            }
        }
        return false;
    }

    bool legalMoveHorizontal(Coordinate piecePos, Coordinate moveTo)
    {
        if(board[piecePos.y][piecePos.x][0] == board[moveTo.y][moveTo.x][0])
            return false;
        if(piecePos.y == moveTo.y) {
            if(piecePos.x < moveTo.x) {
                for(int i = 1; i < (moveTo.x - piecePos.x); i++) {
                    if(board[piecePos.y][piecePos.x + i] != EMPTY_CELL)
                        return false;
                }
                return true;
            }
            if(piecePos.x > moveTo.x) {
                for(int i = 1; i < (piecePos.x - moveTo.x); i++) {
                    if(board[piecePos.y][piecePos.x - i] != EMPTY_CELL)
                        return false;
                }
                return true;
            }
        }
        return false;
    }

    bool legalMoveLshape(Coordinate piecePos, Coordinate moveTo)
    {
        if(board[piecePos.y][piecePos.x][0] == board[moveTo.y][moveTo.x][0])
            return false;
        if(abs(piecePos.x - moveTo.x) == 1 && abs(piecePos.y - moveTo.y) == 2)
            return true;
        else if(abs(piecePos.x - moveTo.x) == 2 && abs(piecePos.y - moveTo.y) == 1)
            return true;
        return false;
    }

    bool legalMoveDiagonal(Coordinate piecePos, Coordinate moveTo)
    {
        if(board[moveTo.y][moveTo.x][0] == board[piecePos.y][piecePos.x][0])
            return false;
        if((piecePos.x - moveTo.x) == (piecePos.y - moveTo.y)) {
            if(piecePos.x < moveTo.x) {
                for(int i=1; i<(moveTo.x - piecePos.x); i++) {
                    if(board[piecePos.y + i][piecePos.x + i] != EMPTY_CELL)
                        return false;
                }
            }
            else {
                for(int i=1; i<(piecePos.x - moveTo.x); i++) {
                    if(board[piecePos.y - i][piecePos.x - i] != EMPTY_CELL)
                        return false;
                }
            }
            return true;
        }
        else if((moveTo.x - piecePos.x) == (piecePos.y - moveTo.y)) {
            if(piecePos.x < moveTo.x) {
                for(int i=1; i<(moveTo.x - piecePos.x); i++) {
                    if(board[piecePos.y - i][piecePos.x + i] != EMPTY_CELL)
                        return false;
                }
            }
            else {
                for(int i=1; i<(piecePos.x - moveTo.x); i++) {
                    if(board[piecePos.y + i][piecePos.x - i] != EMPTY_CELL)
                        return false;
                }
            }
            return true;
        }
        return false;
    }

    bool rookLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        return (legalMoveVertical(piecePos, moveTo) || legalMoveHorizontal(piecePos, moveTo));
    }

    bool knightLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        return legalMoveLshape(piecePos, moveTo);
    }

    bool bishopLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        return legalMoveDiagonal(piecePos, moveTo);
    }

    bool queenLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        return (legalMoveVertical(piecePos, moveTo) || legalMoveHorizontal(piecePos, moveTo) || legalMoveDiagonal(piecePos, moveTo));
    }

    bool kingLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        if(board[moveTo.y][moveTo.x][0] == board[piecePos.y][piecePos.x][0]) {
            return false;
        }
        if(board[piecePos.y][piecePos.x][0] == 'w') {
            if(checkIfCanCastle('w', 'q') && moveTo.x == 2 && moveTo.y == 7) {
                return true;
            }
            if(checkIfCanCastle('w', 'k') && moveTo.x == 6 && moveTo.y == 7) {
                return true;
            }
        }
        else {
            if(checkIfCanCastle('b', 'q') && moveTo.x == 2 && moveTo.y == 0) {
                return true;
            }
            if(checkIfCanCastle('b', 'k') && moveTo.x == 6 && moveTo.y == 0) {
                return true;
            }
        }
        if(abs(moveTo.x - piecePos.x) <= 1 && abs(moveTo.y - piecePos.y) <= 1) {
            char color = board[piecePos.y][piecePos.x][0];
            if(!checkKingSaveAfterMove(piecePos, moveTo, color)) {
                return false;
            }
            return true;
        }
        return false;
    }

    bool checkKingSaveAfterMove(Coordinate piecePos, Coordinate moveTo, char color)
    {
        string save = board[moveTo.y][moveTo.x];
        board[moveTo.y][moveTo.x] = board[piecePos.y][piecePos.x];
        board[piecePos.y][piecePos.x] = EMPTY_CELL;
        Coordinate posKing = kingPosition(color);
        bool check = isCheck(posKing, color);
        board[piecePos.y][piecePos.x] = board[moveTo.y][moveTo.x];
        board[moveTo.y][moveTo.x] = save;
        if(check) return false;
        return true;
    }

    bool pawnMoveVertical(Coordinate piecePos, Coordinate moveTo, char color)
    {
        if(piecePos.x == moveTo.x) {
            if(board[moveTo.y][moveTo.x] != EMPTY_CELL)
                return false;
            if(color == 'w' && piecePos.y == 6) {
                if(moveTo.y < 4) return false;
                else if(moveTo.y == 4) {
                    if(board[5][moveTo.x] != EMPTY_CELL) return false;
                    return true;
                }
                else if(moveTo.y == 5) return true;
                return false;
            }
            if(color == 'w') {
                if((piecePos.y - moveTo.y) == 1) return true;
                return false;
            }
            if(color == 'b' && piecePos.y == 1) {
                if(moveTo.y > 3) return false;
                else if(moveTo.y == 3) {
                    if(board[2][moveTo.x] != EMPTY_CELL) return false;
                    return true;
                }
                else if(moveTo.y == 2) return true;
                return false;
            }
            if(color == 'b') {
                if((moveTo.y - piecePos.y) == 1) return true;
                return false;
            }
        }
        return false;
    }

    bool pawnMoveDiagonal(Coordinate piecePos, Coordinate moveTo, char color)
    {
        if((piecePos.y - 1) == moveTo.y) {
            if(color == 'w' && (moveTo.x == (piecePos.x - 1) || moveTo.x == (piecePos.x + 1)) && board[moveTo.y][moveTo.x][0] == 'b')
                return true;
            return false;
        }
        if((moveTo.y - 1) == piecePos.y) {
            if(color == 'b' && (moveTo.x == (piecePos.x - 1) || moveTo.x == (piecePos.x + 1)) && board[moveTo.y][moveTo.x][0] == 'w')
                return true;
            return false;
        }
        return false;
    }

    bool pawnEnPassantMove(Coordinate piecePos, Coordinate moveTo)
    {
        int dx[2] = {-1, 1};
        int moves = moveHadPlay.size();
        if(moves == 0) return false;
        Coordinate from = moveHadPlay[moves - 1].second.first;
        Coordinate to = moveHadPlay[moves - 1].second.second;
        if(board[piecePos.y][piecePos.x] == "wPa" && piecePos.y == 3 && (piecePos.x == (moveTo.x - 1) || piecePos.x == (moveTo.x + 1))
           && moveTo.y == 2 && board[moveTo.y][moveTo.x] == EMPTY_CELL) {
            for(int i=0; i<2; i++) {
                if(from.x == moveTo.x && to.x == moveTo.x && from.y == 1 && to.y == 3 && board[to.y][to.x] == "bPa")
                    return true;
            }
        }
        if(board[piecePos.y][piecePos.x] == "bPa" && piecePos.y == 4 && (piecePos.x == (moveTo.x - 1) || piecePos.x == (moveTo.x + 1))
           && moveTo.y == 5 && board[moveTo.y][moveTo.x] == EMPTY_CELL) {
            for(int i=0; i<2; i++) {
                if(from.x == moveTo.x && to.x == moveTo.x && from.y == 6 && to.y == 4 && board[to.y][to.x] == "wPa")
                    return true;
            }
        }
        return false;
    }

    bool pawnLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        char color = board[piecePos.y][piecePos.x][0];
        return (pawnMoveVertical(piecePos, moveTo, color) || pawnMoveDiagonal(piecePos, moveTo, color) || pawnEnPassantMove(piecePos, moveTo));
    }

    bool legalPick(Coordinate pos)
    {
        if(board[pos.y][pos.x][0] == 'w' && whiteTurn && canMove(pos))
            return true;
        if(board[pos.y][pos.x][0] == 'b' && blackTurn && canMove(pos))
            return true;
        return false;
    }

    bool legalMove(Coordinate piecePos, Coordinate moveTo)
    {
        if(piecePos.x == moveTo.x && piecePos.y == moveTo.y) return false;

        if(board[piecePos.y][piecePos.x] == EMPTY_CELL) return false;

        char color = board[piecePos.y][piecePos.x][0];
        if(!checkKingSaveAfterMove(piecePos, moveTo, color)) return false;

        if(board[piecePos.y][piecePos.x] == "wPa" || board[piecePos.y][piecePos.x] == "bPa") {
            if(pawnLegalMove(piecePos, moveTo)) return true;
        }
        if(board[piecePos.y][piecePos.x] == "wRo" || board[piecePos.y][piecePos.x] == "bRo") {
            if(rookLegalMove(piecePos, moveTo)) return true;
        }
        if(board[piecePos.y][piecePos.x] == "wKn" || board[piecePos.y][piecePos.x] == "bKn") {
            if(knightLegalMove(piecePos, moveTo)) return true;
        }
        if(board[piecePos.y][piecePos.x] == "wBi" || board[piecePos.y][piecePos.x] == "bBi") {
            if(bishopLegalMove(piecePos, moveTo)) return true;
        }
        if(board[piecePos.y][piecePos.x] == "wQu" || board[piecePos.y][piecePos.x] == "bQu") {
            if(queenLegalMove(piecePos, moveTo)) return true;
        }
        if(board[piecePos.y][piecePos.x] == "wKi" || board[piecePos.y][piecePos.x] == "bKi") {
            if(kingLegalMove(piecePos, moveTo)) return true;
        }
        return false;
    }

    bool canMove(Coordinate pos)
    {
        Coordinate moveTo;
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                moveTo.x = j;
                moveTo.y = i;
                if(legalMove(pos, moveTo)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool insideBoard(int x, int y)
    {
        if(x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE)
            return true;
        return false;
    }

    bool checkFromDiagonalByKing(Coordinate pos, char color)
    {
        int dx[3] = {-1, 0, 1};
        int dy[3] = {-1, 0, 1};
        for(int i=0; i<3; i++) {
            for(int j=0; j<3; j++) {
                if(i == 0 && j == 0) continue;
                if(insideBoard(pos.x + dx[i], pos.y + dy[j]) && color == 'w' && board[pos.y + dy[j]][pos.x + dx[i]] == "bKi")
                    return true;
                if(insideBoard(pos.x + dx[i], pos.y + dy[j]) && color == 'b' && board[pos.y + dy[j]][pos.x + dx[i]] == "wKi")
                    return true;
            }
        }
        return false;
    }

    bool checkFromDiagonalByPawn(Coordinate pos, char color)
    {
        if(color == 'w' && ((insideBoard(pos.x - 1, pos.y - 1) && board[pos.y - 1][pos.x - 1] == "bPa")
            || (insideBoard(pos.x + 1, pos.y - 1) && board[pos.y - 1][pos.x + 1] == "bPa")))
            return true;
        if(color == 'b' && ((insideBoard(pos.x - 1, pos.y + 1) && board[pos.y + 1][pos.x - 1] == "wPa")
            || (insideBoard(pos.x + 1, pos.y + 1) && board[pos.y + 1][pos.x + 1] == "wPa")))
            return true;
        return false;
    }

    bool checkFromDiagonalByBishopAndQueen(Coordinate pos, char color)
    {
        for(int i=1; i<BOARD_SIZE; i++) {
            if(!insideBoard(pos.x - i, pos.y - i)) break;
            if(board[pos.y - i][pos.x - i] != EMPTY_CELL) {
                if(board[pos.y - i][pos.x - i][0] == color) break;
                else if(board[pos.y - i][pos.x - i][1] == 'B' || board[pos.y - i][pos.x - i][1] == 'Q') return true;
                else break;
            }
        }
        for(int i=1; i<BOARD_SIZE; i++) {
            if(!insideBoard(pos.x - i, pos.y + i)) break;
            if(board[pos.y + i][pos.x - i] != EMPTY_CELL) {
                if(board[pos.y + i][pos.x - i][0] == color) break;
                else if(board[pos.y + i][pos.x - i][1] == 'B' || board[pos.y + i][pos.x - i][1] == 'Q') return true;
                else break;
            }
        }
        for(int i=1; i<BOARD_SIZE; i++) {
            if(!insideBoard(pos.x + i, pos.y - i)) break;
            if(board[pos.y - i][pos.x + i] != EMPTY_CELL) {
                if(board[pos.y - i][pos.x + i][0] == color) break;
                else if(board[pos.y - i][pos.x + i][1] == 'B' || board[pos.y - i][pos.x + i][1] == 'Q') return true;
                else break;
            }
        }
        for(int i=1; i<BOARD_SIZE; i++) {
            if(!insideBoard(pos.x + i, pos.y + i)) break;
            if(board[pos.y + i][pos.x + i] != EMPTY_CELL) {
                if(board[pos.y + i][pos.x + i][0] == color) break;
                else if(board[pos.y + i][pos.x + i][1] == 'B' || board[pos.y + i][pos.x + i][1] == 'Q') return true;
                else break;
            }
        }
        return false;
    }

    bool checkFromDiagonal(Coordinate pos, char color)
    {
        return (checkFromDiagonalByPawn(pos, color) || checkFromDiagonalByKing(pos, color) || checkFromDiagonalByBishopAndQueen(pos, color));
    }

    bool checkFromVertical(Coordinate pos, char color)
    {
        for(int i=1; i<BOARD_SIZE; i++) {
            if(!insideBoard(pos.x, pos.y - i)) break;
            if(board[pos.y - i][pos.x] != EMPTY_CELL) {
                if(board[pos.y - i][pos.x][0] == color) break;
                else if(board[pos.y - i][pos.x][1] == 'R' || board[pos.y - i][pos.x][1] == 'Q') return true;
                else break;
            }
        }
        for(int i=1; i<BOARD_SIZE; i++) {
            if(!insideBoard(pos.x, pos.y + i)) break;
            if(board[pos.y + i][pos.x] != EMPTY_CELL) {
                if(board[pos.y + i][pos.x][0] == color) break;
                else if(board[pos.y + i][pos.x][1] == 'R' || board[pos.y + i][pos.x][1] == 'Q') return true;
                else break;
            }
        }
        return false;
    }

    bool checkFromHorizontal(Coordinate pos, char color)
    {
        for(int i=1; i<BOARD_SIZE; i++) {
            if(!insideBoard(pos.x - i, pos.y)) break;
            if(board[pos.y][pos.x - i] != EMPTY_CELL) {
                if(board[pos.y][pos.x - i][0] == color) break;
                else if(board[pos.y][pos.x - i][1] == 'R' || board[pos.y][pos.x - i][1] == 'Q') return true;
                else break;
            }
        }
        for(int i=1; i<BOARD_SIZE; i++) {
            if(!insideBoard(pos.x + i, pos.y)) break;
            if(board[pos.y][pos.x + i] != EMPTY_CELL) {
                if(board[pos.y][pos.x + i][0] == color) break;
                else if(board[pos.y][pos.x + i][1] == 'R' || board[pos.y][pos.x + i][1] == 'Q') return true;
                else break;
            }
        }
        return false;
    }

    bool checkFromLMove(Coordinate pos, char color)
    {
        int dx[4] = {-2, -1, 1, 2};
        int dy[4] = {-2, -1, 1, 2};
        for(int i=0; i<4; i++) {
            for(int j=0; j<4; j++) {
                if(abs(dx[i] * dy[j]) == 2) {
                    int px = pos.x + dx[i];
                    int py = pos.y + dy[j];
                    if(color == 'w' && insideBoard(px, py) && board[py][px] == "bKn")
                        return true;
                    if(color == 'b' && insideBoard(px, py) && board[py][px] == "wKn")
                        return true;
                }
                else continue;
            }
        }
        return false;
    }

    bool isCheck(Coordinate pos, char color)
    {
        return (checkFromDiagonal(pos, color) || checkFromVertical(pos, color) ||
                checkFromHorizontal(pos, color) || checkFromLMove(pos, color));
    }

    void updateRookAndKingCondition(Coordinate piecePos)
    {
        if(board[piecePos.y][piecePos.x] == "wKi")
            wkHadMove = true;
        if(board[piecePos.y][piecePos.x] == "bKi")
            bkHadMove = true;
        if(board[piecePos.y][piecePos.x] == "wRo" && piecePos.x == 0 && piecePos.y == 7)
            wQuRoHadMove = true;
        if(board[piecePos.y][piecePos.x] == "wRo" && piecePos.x == 7 && piecePos.y == 7)
            wKiRoHadMove = true;
        if(board[piecePos.y][piecePos.x] == "bRo" && piecePos.x == 0 && piecePos.y == 0)
            bQuRoHadMove = true;
        if(board[piecePos.y][piecePos.x] == "bRo" && piecePos.x == 7 && piecePos.y == 0)
            bKiRoHadMove = true;
    }

    bool checkIfCanCastle(char color, char side)
    {
        if(color == 'w' && !isCheck({4, 7}, 'w')) {
            if(!wkHadMove && !wQuRoHadMove && board[7][0] == "wRo" && board[7][4] == "wKi") {
                if(board[7][1] == board[7][2] && board[7][2] == board[7][3] && board[7][3] == EMPTY_CELL) {
                    if(!isCheck({2, 7}, 'w') && !isCheck({3, 7}, 'w'))
                        return true;
                }
            }
            if(!wkHadMove && !wKiRoHadMove && board[7][7] == "wRo" && board[7][4] == "wKi") {
                if(board[7][5] == board[7][6] && board[7][6] == EMPTY_CELL) {
                    if(!isCheck({5, 7}, 'w') && !isCheck({6, 7}, 'w'))
                        return true;
                }
            }
        }
        if(color == 'b' && !isCheck({4, 0}, 'b')) {
            if(!bkHadMove && !bQuRoHadMove && board[0][0] == "bRo" && board[0][4] == "bKi") {
                if(board[0][1] == board[0][2] && board[0][2] == board[0][3] && board[0][3] == EMPTY_CELL) {
                    if(!isCheck({2, 0}, 'b') && !isCheck({3, 0}, 'b'))
                        return true;
                }
            }
            if(!bkHadMove && !bKiRoHadMove && board[0][7] == "bRo" && board[0][4] == "bKi") {
                if(board[0][5] == board[0][6] && board[0][6] == EMPTY_CELL) {
                    if(!isCheck({5, 0}, 'b') && !isCheck({6, 0}, 'b'))
                        return true;
                }
            }
        }
        return false;
    }

    Coordinate kingPosition(char color)
    {
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                if(color == 'w' && board[i][j] == "wKi") return {j, i};
                if(color == 'b' && board[i][j] == "bKi") return {j, i};
            }
        }
        return {-1, -1};
    }

    void updateWhiteKingCastleMove(Coordinate piecePos, Coordinate moveTo)
    {
        if(board[piecePos.y][piecePos.x] == "wKi") {
            if(piecePos.x == 4 && piecePos.y == 7 && moveTo.x == 2 && moveTo.y == 7) {
                board[7][2] = "wKi";
                board[7][3] = "wRo";
                board[7][0] = EMPTY_CELL;
                board[7][4] = EMPTY_CELL;
                wQuRoHadMove = true;
            }
            if(piecePos.x == 4 && piecePos.y == 7 && moveTo.x == 6 && moveTo.y == 7) {
                board[7][6] = "wKi";
                board[7][5] = "wRo";
                board[7][7] = EMPTY_CELL;
                board[7][4] = EMPTY_CELL;
                wKiRoHadMove = true;
            }
            wkHadMove = true;
        }
    }

    void updateBlackKingCastleMove(Coordinate piecePos, Coordinate moveTo)
    {
        if(board[piecePos.y][piecePos.x] == "bKi") {
            if(piecePos.x == 4 && piecePos.y == 0 && moveTo.x == 2 && moveTo.y == 0) {
                board[0][2] = "bKi";
                board[0][3] = "bRo";
                board[0][0] = EMPTY_CELL;
                board[0][4] = EMPTY_CELL;
                bQuRoHadMove = true;
            }
            if(piecePos.x == 4 && piecePos.y == 0 && moveTo.x == 6 && moveTo.y == 0) {
                board[0][6] = "bKi";
                board[0][5] = "bRo";
                board[0][7] = EMPTY_CELL;
                board[0][4] = EMPTY_CELL;
                bKiRoHadMove = true;
            }
            bkHadMove = true;
        }
    }

    void updateCastleMove(Coordinate piecePos, Coordinate moveTo)
    {

        updateWhiteKingCastleMove(piecePos, moveTo);
        updateBlackKingCastleMove(piecePos, moveTo);
    }

    bool pawnGoToBottom(char color, Coordinate moveTo)
    {
        if(color == 'w' && moveTo.y == 0 && board[moveTo.y][moveTo.x][1] == 'P') return true;
        if(color == 'b' && moveTo.y == 7 && board[moveTo.y][moveTo.x][1] == 'P') return true;
        return false;
    }

    void updateNormalMove(Coordinate piecePos, Coordinate moveTo)
    {
        board[moveTo.y][moveTo.x] = board[piecePos.y][piecePos.x];
        board[piecePos.y][piecePos.x] = EMPTY_CELL;
    }

    void updateEnPassantMove(Coordinate piecePos, Coordinate moveTo)
    {
        board[moveTo.y][moveTo.x] = board[piecePos.y][piecePos.x];
        board[piecePos.y][piecePos.x] = EMPTY_CELL;
        if(whiteTurn) board[moveTo.y + 1][moveTo.x] = EMPTY_CELL;
        if(blackTurn) board[moveTo.y - 1][moveTo.x] = EMPTY_CELL;
    }

    void updateMove(Coordinate piecePos, Coordinate moveTo)
    {
        bool castle = false, enPassant = false;;
        if(board[piecePos.y][piecePos.x] == "wKi" || board[piecePos.y][piecePos.x] == "bKi") {
            if((piecePos.x == 4 && piecePos.y == 7 && moveTo.x == 2 && moveTo.y == 7) ||
                (piecePos.x == 4 && piecePos.y == 7 && moveTo.x == 6 && moveTo.y == 7) ||
                (piecePos.x == 4 && piecePos.y == 0 && moveTo.x == 2 && moveTo.y == 0) ||
                (piecePos.x == 4 && piecePos.y == 0 && moveTo.x == 6 && moveTo.y == 0)) {
                castle = true;
                updateCastleMove(piecePos, moveTo);
            }
        }
        if(board[piecePos.y][piecePos.x][1] == 'P' && moveTo.x != piecePos.x && board[moveTo.y][moveTo.x] == EMPTY_CELL) {
            enPassant = true;
            updateEnPassantMove(piecePos, moveTo);
        }
        if(!castle && !enPassant) updateNormalMove(piecePos, moveTo);
    }

    void updateTurn()
    {
        whiteTurn = !whiteTurn;
        blackTurn = !blackTurn;
    }

    void saveMove(Coordinate piecePos, Coordinate moveTo)
    {
        moveHadPlay.push_back({board[piecePos.y][piecePos.x], {piecePos, moveTo}});
    }

    void setUpForNextMove(Coordinate piecePos, Coordinate moveTo)
    {
        updateRookAndKingCondition(piecePos);
        saveMove(piecePos, moveTo);
        if(board[moveTo.y][moveTo.x] != EMPTY_CELL || pawnEnPassantMove(piecePos, moveTo)) countMoves = 0;
        else countMoves++;
    }
};

#endif
