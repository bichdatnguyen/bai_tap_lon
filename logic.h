#ifndef _LOGIC__CPP
#define _LOGIC__CPP
#include "def.h"

struct Coordinate
{
    int x, y;
};

struct Chess
{
    string board[BOARD_SIZE][BOARD_SIZE];
    string promoteBoard[BOARD_SIZE][BOARD_SIZE];
    bool whiteTurn = true;
    bool wkHadMove = false, bkHadMove = false, wQuRoHadMove = false, wKiRoHadMove = false, bQuRoHadMove = false, bKiRoHadMove = false;
    queue<pair<Coordinate, Coordinate>> moveHadPlay;

    void show()
    {
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void init()
    {
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
        promoteBoard[0][0] = "Ro";
        promoteBoard[0][1] = "Kn";
        promoteBoard[0][2] = "Bi";
        promoteBoard[0][3] = "Qu";
        show();
    }

    void pick(Coordinate &piecePos)
    {
        SDL_GetMouseState(&piecePos.x, &piecePos.y);
        piecePos.x /= CELL_SIZE;
        piecePos.y /= CELL_SIZE;
    }

    bool isCheckmate()
    {
        bool whiteKing = false, blackKing = false;
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                if(board[i][j] == "wKi") whiteKing = true;
                if(board[i][j] == "bKi") blackKing = true;
            }
        }
        if(!whiteKing || !blackKing)
            return true;
        return false;
    }

    bool legalPick(Coordinate piecePos)
    {
        if(board[piecePos.y][piecePos.x][0] == 'w' && whiteTurn)
            return true;
        if(board[piecePos.y][piecePos.x][0] == 'b' && !whiteTurn)
            return true;
        return false;
    }

    bool moveVertical(Coordinate piecePos, Coordinate moveTo)
    {
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

    bool moveHorizontal(Coordinate piecePos, Coordinate moveTo)
    {
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

    bool moveLshape(Coordinate piecePos, Coordinate moveTo)
    {
        if(abs(piecePos.x - moveTo.x) == 1 && abs(piecePos.y - moveTo.y) == 2)
            return true;
        else if(abs(piecePos.x - moveTo.x) == 2 && abs(piecePos.y - moveTo.y) == 1)
            return true;
        return false;
    }

    bool moveDiagonal(Coordinate piecePos, Coordinate moveTo)
    {
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
        if((piecePos.x == moveTo.x && piecePos.y == moveTo.y) || (piecePos.x != moveTo.x && piecePos.y != moveTo.y))
            return false;
        if(board[moveTo.y][moveTo.x][0] == board[piecePos.y][piecePos.x][0])
            return false;
        if(piecePos.x == moveTo.x)
            return moveVertical(piecePos, moveTo);
        else if(piecePos.y == moveTo.y)
            return moveHorizontal(piecePos, moveTo);
        return false;
    }

    bool knightLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        if(board[moveTo.y][moveTo.x][0] == board[piecePos.y][piecePos.x][0])
            return false;
        return moveLshape(piecePos, moveTo);
    }

    bool bishopLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        if(piecePos.x == moveTo.x && piecePos.y == moveTo.y)
            return false;
        if(board[moveTo.y][moveTo.x][0] == board[piecePos.y][piecePos.x][0])
            return false;
        return moveDiagonal(piecePos, moveTo);
    }

    bool queenLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        if(piecePos.x == moveTo.x && piecePos.y == moveTo.y)
            return false;
        if(board[moveTo.y][moveTo.x][0] == board[piecePos.y][piecePos.x][0])
            return false;
        return (moveVertical(piecePos, moveTo) || moveHorizontal(piecePos, moveTo) || moveDiagonal(piecePos, moveTo));
    }

    bool kingLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        if(piecePos.x == moveTo.x && piecePos.y == moveTo.y)
            return false;
        if(board[piecePos.y][piecePos.x] == "wKi") {
            if(checkIfCanCastle('w', 'q'))
                return true;
            if(checkIfCanCastle('w', 'k'))
                return true;
        }
        if(board[piecePos.y][piecePos.x] == "bKi") {
            if(checkIfCanCastle('b', 'q'))
                return true;
            if(checkIfCanCastle('b', 'k'))
                return true;
        }
        if(abs(moveTo.x - piecePos.x) <= 1 && abs(moveTo.y - piecePos.y) <= 1) {
            if(board[moveTo.y][moveTo.x][0] == board[piecePos.y][piecePos.x][0])
                return false;
            return true;
        }
        return false;
    }

    bool enPassantMove(Coordinate piecePos, Coordinate moveTo, bool isWhite)
    {
        if(piecePos.y == 3 && moveTo.y == 2 && board[piecePos.y][piecePos.x][0] == 'w' && board[moveTo.y][moveTo.x] == EMPTY_CELL) {
            pair<Coordinate, Coordinate> blackLastestMove = moveHadPlay.front();
            Coordinate blackFrom = blackLastestMove.first;
            Coordinate blackTo = blackLastestMove.second;
            if(piecePos.x == moveTo.x - 1) {
                if(blackFrom.y == 1 && blackTo.y == 3 && blackFrom.x == blackTo.x && piecePos.x == blackTo.x - 1)
                    return true;
            }
            if(piecePos.x - 1 == moveTo.x) {
                if(blackFrom.y == 1 && blackTo.y == 3 && blackFrom.x == blackTo.x && piecePos.x == blackTo.x + 1)
                    return true;
            }
        }
        if(piecePos.y == 4 && moveTo.y == 5 && board[piecePos.y][piecePos.x][0] == 'b' && board[moveTo.y][moveTo.x] == EMPTY_CELL) {
            pair<Coordinate, Coordinate> whiteLastestMove = moveHadPlay.front();
            Coordinate whiteFrom = whiteLastestMove.first;
            Coordinate whiteTo = whiteLastestMove.second;
            if(piecePos.x == moveTo.x - 1) {
                if(whiteFrom.y == 6 && whiteTo.y == 4 && whiteFrom.x == whiteTo.x && piecePos.x == whiteTo.x - 1)
                    return true;
            }
            if(piecePos.x - 1 == moveTo.x) {
                if(whiteFrom.y == 6 && whiteTo.y == 4 && whiteFrom.x == whiteTo.x && piecePos.x == whiteTo.x + 1)
                    return true;
            }
        }
        return false;
    }

    bool pawnMoveVertical(Coordinate piecePos, Coordinate moveTo, bool isWhite)
    {
        if(piecePos.x == moveTo.x) {
            if(board[moveTo.y][moveTo.x] != EMPTY_CELL)
                return false;
            if(isWhite && piecePos.y == 6) {
                if(moveTo.y < 4) return false;
                else if(moveTo.y == 4) {
                    if(board[5][moveTo.x] != EMPTY_CELL) return false;
                    return true;
                }
                else if(moveTo.y == 5) return true;
                return false;
            }
            if(isWhite) {
                if((piecePos.y - moveTo.y) == 1) return true;
                return false;
            }
            if(!isWhite && piecePos.y == 1) {
                if(moveTo.y > 3) return false;
                else if(moveTo.y == 3) {
                    if(board[3][moveTo.x] != EMPTY_CELL) return false;
                    return true;
                }
                else if(moveTo.y == 2) return true;
                return false;
            }
            if(!isWhite) {
                if((moveTo.y - piecePos.y) == 1) return true;
                return false;
            }
        }
        return false;
    }

    bool pawnMoveDiagonal(Coordinate piecePos, Coordinate moveTo, bool isWhite)
    {
        if((piecePos.y - 1) == moveTo.y) {
            if(isWhite && (moveTo.x == (piecePos.x - 1) || moveTo.x == (piecePos.x + 1)))
                return true;
            return false;
        }
        if((moveTo.y - 1) == piecePos.y) {
            if(!isWhite && (moveTo.x == (piecePos.x - 1) || moveTo.x == (piecePos.x + 1)))
                return true;
            return false;
        }
        return false;
    }

    bool pawnLegalMove(Coordinate piecePos, Coordinate moveTo)
    {
        bool isWhite = ((board[piecePos.y][piecePos.x][0] == 'w') ? true : false);
        if(enPassantMove(piecePos, moveTo, isWhite))
            return true;
        if(pawnMoveVertical(piecePos, moveTo, isWhite))
            return true;
        if(pawnMoveDiagonal(piecePos, moveTo, isWhite))
            return true;
        return false;
    }

    bool legalMove(const Coordinate piecePos, const Coordinate moveTo)
    {
        if(board[piecePos.y][piecePos.x] == "wPa" || board[piecePos.y][piecePos.x] == "bPa")
            return pawnLegalMove(piecePos, moveTo);

        if(board[piecePos.y][piecePos.x] == "wRo" || board[piecePos.y][piecePos.x] == "bRo")
            return rookLegalMove(piecePos, moveTo);

        if(board[piecePos.y][piecePos.x] == "wKn" || board[piecePos.y][piecePos.x] == "bKn")
            return knightLegalMove(piecePos, moveTo);

        if(board[piecePos.y][piecePos.x] == "wBi" || board[piecePos.y][piecePos.x] == "bBi")
            return bishopLegalMove(piecePos, moveTo);

        if(board[piecePos.y][piecePos.x] == "wQu" || board[piecePos.y][piecePos.x] == "bQu")
            return queenLegalMove(piecePos, moveTo);

        if(board[piecePos.y][piecePos.x] == "wKi" || board[piecePos.y][piecePos.x] == "bKi")
            return kingLegalMove(piecePos, moveTo);

        return false;
    }

    bool insideBoard(int x, int y)
    {
        if(x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE)
            return true;
        return false;
    }

    bool checkFromDiagonal(Coordinate piecePos, char color)
    {
        if(color == 'w') {
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x - i, piecePos.y - i) || board[piecePos.y - i][piecePos.x - i][0] == 'w')
                    break;
                if(board[piecePos.y - 1][piecePos.x - 1] == "bPa")
                    return true;
                if(board[piecePos.y - i][piecePos.x - i] == "bBi" || board[piecePos.y - i][piecePos.x - i] == "bQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x + i, piecePos.y - i) || board[piecePos.y - i][piecePos.x + i][0] == 'w')
                    break;
                if(board[piecePos.y - 1][piecePos.x + 1] == "bPa")
                    return true;
                if(board[piecePos.y - i][piecePos.x + i] == "bBi" || board[piecePos.y - i][piecePos.x + i] == "bQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x + i, piecePos.y + i) || board[piecePos.y + i][piecePos.x + i][0] == 'w')
                    break;
                if(board[piecePos.y + i][piecePos.x + i] == "bBi" || board[piecePos.y + i][piecePos.x + i] == "bQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x - i, piecePos.y + i) || board[piecePos.y + i][piecePos.x - i][0] == 'w')
                    break;
                if(board[piecePos.y + i][piecePos.x - i] == "bBi" || board[piecePos.y + i][piecePos.x - i] == "bQu")
                    return true;
            }
        }
        else {
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x - i, piecePos.y - i) || board[piecePos.y - i][piecePos.x - i][0] == 'b')
                    break;
                if(board[piecePos.y - i][piecePos.x - i] == "wBi" || board[piecePos.y - i][piecePos.x - i] == "wQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x + i, piecePos.y - i) || board[piecePos.y - i][piecePos.x + i][0] == 'b')
                    break;
                if(board[piecePos.y - i][piecePos.x + i] == "wBi" || board[piecePos.y - i][piecePos.x + i] == "wQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x + i, piecePos.y + i) || board[piecePos.y + i][piecePos.x + i][0] == 'b')
                    break;
                if(i == 1 && board[piecePos.y - 1][piecePos.x - 1] == "wPa")
                    return true;
                if(board[piecePos.y + i][piecePos.x + i] == "wBi" || board[piecePos.y + i][piecePos.x + i] == "wQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x - i, piecePos.y + i) || board[piecePos.y + i][piecePos.x - i][0] == 'b')
                    break;
                if(i == 1 && board[piecePos.y - 1][piecePos.x - 1] == "wPa")
                    return true;
                if(board[piecePos.y + i][piecePos.x - i] == "wBi" || board[piecePos.y + i][piecePos.x - i] == "wQu")
                    return true;
            }
        }
        return false;
    }

    bool checkFromVerticalOrHorizontal(Coordinate piecePos, char color)
    {
        if(color == 'w') {
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x, piecePos.y - i) || board[piecePos.y - i][piecePos.x][0] == 'w')
                    break;
                if(board[piecePos.y - i][piecePos.x] == "bRo" || board[piecePos.y - i][piecePos.x - i] == "bQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x + i, piecePos.y) || board[piecePos.y][piecePos.x + i][0] == 'w')
                    break;
                if(board[piecePos.y][piecePos.x + i] == "bRo" || board[piecePos.y][piecePos.x + i] == "bQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x, piecePos.y + i) || board[piecePos.y + i][piecePos.x][0] == 'w')
                    break;
                if(board[piecePos.y + i][piecePos.x] == "bRo" || board[piecePos.y + i][piecePos.x] == "bQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x - i, piecePos.y) || board[piecePos.y][piecePos.x - i][0] == 'w')
                    break;
                if(board[piecePos.y][piecePos.x - i] == "bRo" || board[piecePos.y][piecePos.x - i] == "bQu")
                    return true;
            }
            if(insideBoard(piecePos.x - 1, piecePos.y - 2) && board[piecePos.y - 2][piecePos.x - 1] == "bKn")
                return true;
        }
        else {
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x, piecePos.y - i) || board[piecePos.y - i][piecePos.x][0] == 'b')
                    break;
                if(board[piecePos.y - i][piecePos.x] == "wRo" || board[piecePos.y - i][piecePos.x] == "wQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x + i, piecePos.y) || board[piecePos.y][piecePos.x + i][0] == 'b')
                    break;
                if(board[piecePos.y][piecePos.x + i] == "wRo" || board[piecePos.y][piecePos.x + i] == "wQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x, piecePos.y + i) || board[piecePos.y + i][piecePos.x][0] == 'b')
                    break;
                if(board[piecePos.y + i][piecePos.x] == "wRo" || board[piecePos.y + i][piecePos.x] == "wQu")
                    return true;
            }
            for(int i=1; i<BOARD_SIZE; i++) {
                if(!insideBoard(piecePos.x - i, piecePos.y) || board[piecePos.y][piecePos.x - i][0] == 'b')
                    break;
                if(board[piecePos.y][piecePos.x - i] == "wRo" || board[piecePos.y][piecePos.x - i] == "wQu")
                    return true;
            }
        }
        return false;
    }

    bool checkFromLMove(Coordinate piecePos, char color)
    {
        if(color == 'w')
        {
            if(insideBoard(piecePos.x + 1, piecePos.y - 2) && board[piecePos.y - 2][piecePos.x + 1] == "bKn")
                return true;
            if(insideBoard(piecePos.x - 1, piecePos.y + 2) && board[piecePos.y + 2][piecePos.x - 1] == "bKn")
                return true;
            if(insideBoard(piecePos.x + 1, piecePos.y + 2) && board[piecePos.y + 2][piecePos.x + 1] == "bKn")
                return true;
            if(insideBoard(piecePos.x - 2, piecePos.y - 1) && board[piecePos.y - 1][piecePos.x - 2] == "bKn")
                return true;
            if(insideBoard(piecePos.x - 2, piecePos.y + 1) && board[piecePos.y + 1][piecePos.x - 2] == "bKn")
                return true;
            if(insideBoard(piecePos.x + 2, piecePos.y - 1) && board[piecePos.y - 1][piecePos.x + 2] == "bKn")
                return true;
            if(insideBoard(piecePos.x + 2, piecePos.y + 1) && board[piecePos.y + 1][piecePos.x + 2] == "bKn")
                return true;
        }
        else {
            if(insideBoard(piecePos.x - 1, piecePos.y - 2) && board[piecePos.y - 2][piecePos.x - 1] == "wKn")
                return true;
            if(insideBoard(piecePos.x + 1, piecePos.y - 2) && board[piecePos.y - 2][piecePos.x + 1] == "wKn")
                return true;
            if(insideBoard(piecePos.x - 1, piecePos.y + 2) && board[piecePos.y + 2][piecePos.x - 1] == "wKn")
                return true;
            if(insideBoard(piecePos.x + 1, piecePos.y + 2) && board[piecePos.y + 2][piecePos.x + 1] == "wKn")
                return true;
            if(insideBoard(piecePos.x - 2, piecePos.y - 1) && board[piecePos.y - 1][piecePos.x - 2] == "wKn")
                return true;
            if(insideBoard(piecePos.x - 2, piecePos.y + 1) && board[piecePos.y + 1][piecePos.x - 2] == "wKn")
                return true;
            if(insideBoard(piecePos.x + 2, piecePos.y - 1) && board[piecePos.y - 1][piecePos.x + 2] == "wKn")
                return true;
            if(insideBoard(piecePos.x + 2, piecePos.y + 1) && board[piecePos.y + 1][piecePos.x + 2] == "wKn")
                return true;
        }
        return false;
    }

    bool isCheck(Coordinate piecePos, char color)
    {
        if(checkFromDiagonal(piecePos, color))
            return true;
        if(checkFromVerticalOrHorizontal(piecePos, color))
            return true;
        if(checkFromLMove(piecePos, color))
            return true;
        return false;
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
        if(color == 'w') {
            if(!wkHadMove) return true;
            if(side == 'q' && !wQuRoHadMove && !isCheck({2, 7}, 'w')
                && board[7][1] == board[7][2] && board[7][2] == board[7][3] && board[7][3] == EMPTY_CELL)
                return true;
            if(side == 'k' && !wKiRoHadMove && !isCheck({6, 7}, 'w')
                && board[7][5] == board[7][6] && board[7][6] == EMPTY_CELL)
                return true;
        }
        if(color == 'b') {
            if(!bkHadMove) return true;
            if(side == 'q' && !bQuRoHadMove && !isCheck({2, 0}, 'b')
                && board[0][1] == board[0][2] && board[0][2] == board[0][3] && board[0][3] == EMPTY_CELL)
                return true;
            if(side == 'k' && !bKiRoHadMove && !isCheck({6, 0}, 'b')
                && board[0][5] == board[7][6] && board[7][6] == EMPTY_CELL)
                return true;
        }
        return false;
    }

    Coordinate kingPosition(char color)
    {
        for(int i=0; i<BOARD_SIZE; i++) {
            for(int j=0; j<BOARD_SIZE; j++) {
                if(color == 'w' && board[i][j] == "wKi") return {j, i};
                else if(color == 'b' && board[i][j] == "bKi") return {j, i};
            }
        }
        return {-1, -1};
    }

    void updateEnPassantMove(Coordinate piecePos, Coordinate moveTo)
    {
        board[moveTo.y][moveTo.x] = board[piecePos.y][piecePos.x];
        board[piecePos.y][moveTo.x] = EMPTY_CELL;
        board[piecePos.y][piecePos.x] = EMPTY_CELL;
    }

    void updateCastleMove(Coordinate piecePos, Coordinate moveTo)
    {
        if(board[piecePos.y][piecePos.x] == "wKi") {
            if(piecePos.x == 4 && piecePos.y == 7 && moveTo.x == 2 && moveTo.y == 7) {
                board[7][2] = "wKi";
                board[7][3] = "wRo";
                board[7][0] = EMPTY_CELL;
                board[7][4] = EMPTY_CELL;
            }
            if(piecePos.x == 4 && piecePos.y == 7 && moveTo.x == 6 && moveTo.y == 7) {
                board[7][6] = "wKi";
                board[7][5] = "wRo";
                board[7][7] = EMPTY_CELL;
                board[7][4] = EMPTY_CELL;
            }
        }
        else if(board[piecePos.y][piecePos.x] == "bKi") {
            if(piecePos.x == 4 && piecePos.y == 0 && moveTo.x == 2 && moveTo.y == 0) {
                board[0][2] = "bKi";
                board[0][3] = "bRo";
                board[0][0] = EMPTY_CELL;
                board[0][4] = EMPTY_CELL;
            }
            if(piecePos.x == 4 && piecePos.y == 0 && moveTo.x == 6 && moveTo.y == 0) {
                board[0][6] = "bKi";
                board[0][5] = "bRo";
                board[0][7] = EMPTY_CELL;
                board[0][4] = EMPTY_CELL;
            }
        }
    }

    void promotePawn(Coordinate moveTo)
    {
        Promote p;
        p.init();
        char color = board[moveTo.y][moveTo.x][0];
        p.render(color);
    }

    void updateNormalMove(Coordinate piecePos, Coordinate moveTo)
    {
        board[moveTo.y][moveTo.x] = board[piecePos.y][piecePos.x];
        board[piecePos.y][piecePos.x] = EMPTY_CELL;
        if(board[moveTo.y][moveTo.x][1] == 'P' && (moveTo.y == 0 || moveTo.y == 7)) {
            promotePawn(moveTo);
        }
    }

    void updateMove(Coordinate piecePos, Coordinate moveTo)
    {
        bool castle = false, enPassant = false;
        if(board[piecePos.y][piecePos.x][1] == 'P' && board[moveTo.y][moveTo.x] == EMPTY_CELL && piecePos.x != moveTo.x) {
            enPassant = true;
            updateEnPassantMove(piecePos, moveTo);
        }

        if(board[piecePos.y][piecePos.x] == "wKi" || board[piecePos.y][piecePos.x] == "bKi") {
            if((piecePos.x == 4 && piecePos.y == 7 && moveTo.x == 2 && moveTo.y == 7) ||
                (piecePos.x == 4 && piecePos.y == 7 && moveTo.x == 6 && moveTo.y == 7) ||
                (piecePos.x == 4 && piecePos.y == 0 && moveTo.x == 2 && moveTo.y == 0) ||
                (piecePos.x == 4 && piecePos.y == 0 && moveTo.x == 6 && moveTo.y == 0)) {
                castle = true;
                updateCastleMove(piecePos, moveTo);
            }
        }
        if(!castle && !enPassant)
            updateNormalMove(piecePos, moveTo);
    }

    void checkSituation()
    {
        if(isCheck(kingPosition('w'), 'w'))
            cout << "White king is checked: " << kingPosition('w').x << " " << kingPosition('w').y << endl;
        else if(isCheck(kingPosition('b'), 'b'))
            cout << "Black king is checked: " << kingPosition('w').x << " " << kingPosition('w').y << endl;
    }

    void saveMove(Coordinate piecePos, Coordinate moveTo)
    {
        moveHadPlay.push(make_pair(piecePos, moveTo));
    }

    void updateTurn()
    {
        whiteTurn = !whiteTurn;
    }

    Coordinate move(Coordinate piecePos)
    {
        Coordinate moveTo;
        bool quit = false;
        SDL_Event event;
        while(!quit)
        {
            SDL_PollEvent(&event);
            switch(event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                pick(moveTo);
                cout << "From: " << piecePos.x << " " << piecePos.y << " move to location: " << moveTo.x << " " << moveTo.y << "?" << endl;
                if(!legalMove(piecePos, moveTo)) {
                    cout << "Illegal" << endl;
                    break;
                }
                cout << "Move to: " << moveTo.x << " " << moveTo.y << endl;
                updateRookAndKingCondition(piecePos);
                checkSituation();
                saveMove(piecePos, moveTo);
                updateMove(piecePos, moveTo);
                show();
                updateTurn();
                quit = true;
                break;
            }
            if(quit) break;
        }
        return moveTo;
    }
};

#endif
