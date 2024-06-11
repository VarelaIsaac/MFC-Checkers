#include "pch.h"
#include "Board.h"
#include "Pawn.h"
#include "iostream"

using std::vector;
using std::pair;
using std::cout;
using std::endl;

Board::Board() {
    Initialize();
}

void Board::Initialize() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (row < 3 && (row + col) % 2 != 0) {
                m_board[row][col] = Pawn(PawnType::BLACK);
            }
            else if (row > 4 && (row + col) % 2 != 0) {
                m_board[row][col] = Pawn(PawnType::RED);
            }
            else {
                m_board[row][col] = Pawn(PawnType::EMPTY);
            }
        }
    }
}

Pawn Board::GetPawnAt(int row, int col) const {
    if (IsWithinBounds(row, col)) {
        return m_board[row][col];
    }
    return Pawn();
}

void Board::SetPawnAt(int row, int col, PawnType type) {
    if (IsWithinBounds(row, col)) {
        m_board[row][col].SetType(type);
    }
}

bool Board::IsValidMove(const Move& move) const {
    int startRow = move.GetStartRow();
    int startCol = move.GetStartCol();
    int endRow = move.GetEndRow();
    int endCol = move.GetEndCol();

    if (!IsWithinBounds(startRow, startCol) || !IsWithinBounds(endRow, endCol)) {
        return false;
    }

    Pawn startPawn = GetPawnAt(startRow, startCol);
    Pawn endPawn = GetPawnAt(endRow, endCol);

    if (startPawn.GetType() == PawnType::EMPTY || endPawn.GetType() != PawnType::EMPTY) {
        return false;
    }

    int rowDiff = endRow - startRow;
    int colDiff = endCol - startCol;

    // Regular pawn movement (non-capture)
    if (abs(rowDiff) == 1 && abs(colDiff) == 1) {
        if ((startPawn.GetType() == PawnType::RED && rowDiff == -1) ||
            (startPawn.GetType() == PawnType::BLACK && rowDiff == 1) ||
            startPawn.GetType() == PawnType::RED_KING ||
            startPawn.GetType() == PawnType::BLACK_KING) {
            return true;
        }
    }

    // Capture movement
    int capturedRow, capturedCol;
    if (CanCapture(startRow, startCol, endRow, endCol, capturedRow, capturedCol)) {
        return true;
    }

    return false;
}

bool Board::ApplyMove(const Move& move) {
    if (!IsValidMove(move)) {
        return false;
    }

    int startRow = move.GetStartRow();
    int startCol = move.GetStartCol();
    int endRow = move.GetEndRow();
    int endCol = move.GetEndCol();

    int capturedRow, capturedCol;
    bool isCapture = CanCapture(startRow, startCol, endRow, endCol, capturedRow, capturedCol); // Determine if it's a capture

    if (isCapture) {
        SetPawnAt(capturedRow, capturedCol, PawnType::EMPTY);
    }

    PawnType startPawnType = GetPawnAt(startRow, startCol).GetType();
    SetPawnAt(startRow, startCol, PawnType::EMPTY);
    SetPawnAt(endRow, endCol, startPawnType);

    // Check for promotion to king
    if ((startPawnType == PawnType::RED && endRow == 0) ||
        (startPawnType == PawnType::BLACK && endRow == 7)) {
        SetPawnAt(endRow, endCol, (startPawnType == PawnType::RED) ? PawnType::RED_KING : PawnType::BLACK_KING);
    }

    SetLastMove(Move(startRow, startCol, endRow, endCol, isCapture)); // Set the move with isCapture flag

    return true;
}


vector<Move> Board::GetValidMoves(int row, int col) const {
    std::vector<Move> moves;
    Pawn pawn = GetPawnAt(row, col);
    PawnType type = pawn.GetType();

    if (type == PawnType::EMPTY) {
        return moves;
    }

    // Check if the last move was a capture
    bool lastMoveWasCapture = GetLastMove().IsCaptureMove();

    // Define movement directions
    std::vector<std::pair<int, int>> directions;

    if (type == PawnType::RED || type == PawnType::RED_KING) {
        directions = { {-1, -1}, {-1, 1} }; // Red moves up
    }
    else if (type == PawnType::BLACK || type == PawnType::BLACK_KING) {
        directions = { {1, -1}, {1, 1} }; // Black moves down
    }

    // Add king's directions
    if (type == PawnType::RED_KING || type == PawnType::BLACK_KING) {
        directions = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    }

    for (const auto& direction : directions) {
        int dr = direction.first;
        int dc = direction.second;

        // Check for regular moves
        int newRow = row + dr;
        int newCol = col + dc;

        // Allow king to move multiple steps in all directions
        if (type == PawnType::RED_KING || type == PawnType::BLACK_KING) {
            while (IsWithinBounds(newRow, newCol) && GetPawnAt(newRow, newCol).GetType() == PawnType::EMPTY) {
                if (!lastMoveWasCapture) {  // Only allow single jumps if the last move wasn't a capture
                    moves.push_back(Move(row, col, newRow, newCol));
                }
                newRow += dr;
                newCol += dc;
            }
        }
        else {
            if (IsWithinBounds(newRow, newCol) && GetPawnAt(newRow, newCol).GetType() == PawnType::EMPTY) {
                if (!lastMoveWasCapture) {  // Only allow single jumps if the last move wasn't a capture
                    moves.push_back(Move(row, col, newRow, newCol));
                }
            }
        }

        // Check for capture moves
        newRow = row + dr;
        newCol = col + dc;
        while (IsWithinBounds(newRow, newCol)) {
            Pawn adjacentPawn = GetPawnAt(newRow, newCol);
            int jumpRow = newRow + dr;
            int jumpCol = newCol + dc;

            if (adjacentPawn.GetType() != PawnType::EMPTY && !IsSameColor(adjacentPawn, pawn)) {
                if (IsWithinBounds(jumpRow, jumpCol) && GetPawnAt(jumpRow, jumpCol).GetType() == PawnType::EMPTY) {
                    moves.push_back(Move(row, col, jumpRow, jumpCol));

                    // Update lastMoveWasCapture flag for subsequent jumps
                    lastMoveWasCapture = true;

                    // Allow kings to continue jumping
                    if (type == PawnType::RED_KING || type == PawnType::BLACK_KING) {
                        int furtherJumpRow = jumpRow + dr;
                        int furtherJumpCol = jumpCol + dc;
                        while (IsWithinBounds(furtherJumpRow, furtherJumpCol) && GetPawnAt(furtherJumpRow, furtherJumpCol).GetType() == PawnType::EMPTY) {
                            moves.push_back(Move(row, col, furtherJumpRow, furtherJumpCol));
                            furtherJumpRow += dr;
                            furtherJumpCol += dc;
                        }
                    }
                }

                break; // Stop after the first capture in this direction
            }
            newRow += dr;
            newCol += dc;
        }
    }

    return moves;
}



std::vector<Move> Board::GetCaptureMoves(int row, int col, int newRow, int newCol) const {
    std::vector<Move> moves;

    while (IsWithinBounds(newRow, newCol)) {
        Pawn adjacentPawn = GetPawnAt(newRow, newCol);
        int jumpRow = newRow + (newRow - row);
        int jumpCol = newCol + (newCol - col);

        if (adjacentPawn.GetType() != PawnType::EMPTY && !IsSameColor(adjacentPawn, GetPawnAt(row, col))) {
            if (IsWithinBounds(jumpRow, jumpCol) && GetPawnAt(jumpRow, jumpCol).GetType() == PawnType::EMPTY) {
                // Add the capture move
                moves.push_back(Move(row, col, jumpRow, jumpCol));

                // Update row and col for next jump
                newRow = jumpRow;
                newCol = jumpCol;
            }
            else {
                break; // No more captures in this direction
            }
        }
        else {
            break; // No captures possible in this direction
        }
    }

    return moves;
}


bool Board::IsSameColor(const Pawn& pawn1, const Pawn& pawn2) const {
    return (pawn1.IsBlack() && pawn2.IsBlack()) || (pawn1.IsRed() && pawn2.IsRed());
}


bool Board::IsWithinBounds(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool Board::CanCapture(int startRow, int startCol, int endRow, int endCol, int& capturedRow, int& capturedCol) const {
    if (!IsWithinBounds(startRow, startCol) || !IsWithinBounds(endRow, endCol)) {
        return false;
    }

    Pawn startPawn = GetPawnAt(startRow, startCol);
    Pawn endPawn = GetPawnAt(endRow, endCol);

    if (startPawn.GetType() == PawnType::EMPTY || endPawn.GetType() != PawnType::EMPTY) {
        return false;
    }

    int rowDiff = endRow - startRow;
    int colDiff = endCol - startCol;

    if (abs(rowDiff) != 2 || abs(colDiff) != 2) {
        return false;
    }

    capturedRow = startRow + rowDiff / 2;
    capturedCol = startCol + colDiff / 2;

    Pawn capturedPawn = GetPawnAt(capturedRow, capturedCol);
    if (capturedPawn.GetType() == PawnType::EMPTY || capturedPawn.GetType() == startPawn.GetType() ||
        (startPawn.GetType() == PawnType::RED_KING && capturedPawn.GetType() == PawnType::RED) ||
        (startPawn.GetType() == PawnType::BLACK_KING && capturedPawn.GetType() == PawnType::BLACK)) {
        return false;
    }

    return true;
}

bool Board::HasCaptureMove(int row, int col, PawnType type) const {
    // Regular movement directions
    vector<pair<int, int>> directions = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    // Add multiple step directions for kings
    if (type == PawnType::RED_KING || type == PawnType::BLACK_KING) {
        directions.push_back({ -2, -2 });
        directions.push_back({ -2, 2 });
        directions.push_back({ 2, -2 });
        directions.push_back({ 2, 2 });
    }

    for (const auto& direction : directions) {
        int dr = direction.first;
        int dc = direction.second;

        int newRow = row + dr;
        int newCol = col + dc;

        if (IsWithinBounds(newRow, newCol)) {
            Pawn adjacentPawn = GetPawnAt(newRow, newCol);
            int jumpRow = row + 2 * dr;
            int jumpCol = col + 2 * dc;

            if (adjacentPawn.GetType() != PawnType::EMPTY && IsSameColor(adjacentPawn.GetType(), type)) {
                continue; // Skip if the adjacent pawn is the same color
            }

            if (adjacentPawn.GetType() != PawnType::EMPTY && IsWithinBounds(jumpRow, jumpCol) && GetPawnAt(jumpRow, jumpCol).GetType() == PawnType::EMPTY) {
                return true; // Return true if there's at least one capture move
            }
        }
    }

    return false;
}
