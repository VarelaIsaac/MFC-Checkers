#pragma once

class Move {
public:
    Move() = default;
    Move(int startRow, int startCol, int endRow, int endCol, bool isCapture = false)
        : m_startRow(startRow), m_startCol(startCol), m_endRow(endRow), m_endCol(endCol), m_isCapture(isCapture) {}

    int GetStartRow() const { return m_startRow; }
    int GetStartCol() const { return m_startCol; }
    int GetEndRow() const { return m_endRow; }
    int GetEndCol() const { return m_endCol; }

    bool IsCapture() const {
        return m_isCapture;
    }

    bool IsCaptureMove() const {
        return abs(m_startRow - m_endRow) == 2 && abs(m_startCol - m_endCol) == 2;
    }

private:
    int m_startRow, m_startCol, m_endRow, m_endCol;
    bool m_isCapture = false;
};
