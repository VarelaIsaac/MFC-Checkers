#pragma once

#include "Board.h"
#include <vector>

// CChildView window

class CChildView : public CWnd {
public:
    CChildView();

    // Attributes
public:

    // Operations
public:

    // Overrides
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    // Implementation
public:
    virtual ~CChildView();

    // Generated message map functions
protected:
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
    Board m_board;
    bool m_isSelected;
    int m_selectedRow, m_selectedCol;
    std::vector<Move> m_validMoves;
    bool m_isBlackTurn; // true for black player's turn, false for red player's turn

    void DrawBoard(CDC* pDC);
    void DrawPawn(CDC* pDC, int row, int col, PawnType type);
    void HandleClick(int row, int col);
};
