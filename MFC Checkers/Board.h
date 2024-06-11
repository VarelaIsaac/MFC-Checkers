#pragma once
#include "Pawn.h"
#include "Move.h"
#include <vector>

using std::vector;

class Board {
public:
    Board();
    void Initialize();

    Pawn GetPawnAt(int row, int col) const;
    void SetPawnAt(int row, int col, PawnType type);

    bool IsValidMove(const Move& move) const;
    bool ApplyMove(const Move& move);
    vector<Move> GetValidMoves(int row, int col) const;
    bool HasCaptureMove(int row, int col, PawnType type) const;

    vector<Move> GetCaptureMoves(int row, int col, int newRow, int newCol) const;

    bool IsSameColor(const Pawn& pawn1, const Pawn& pawn2) const;

    void SetLastMove(const Move& move) {
        lastMove = move;
    }

    Move Board::GetLastMove() const {   
        return lastMove;
    }

private:
    Pawn m_board[8][8];
    Move lastMove; // Variable to store the last move

    bool IsWithinBounds(int row, int col) const;
    bool CanCapture(int startRow, int startCol, int endRow, int endCol, int& capturedRow, int& capturedCol) const;

};
