#include "pch.h"
#include "framework.h"
#include "MFC Checkers.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
    : m_board(), m_isSelected(false), m_selectedRow(-1), m_selectedCol(-1), m_isBlackTurn(true) {
}

CChildView::~CChildView() {
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    // Define appropriate window class
    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
        ::LoadCursor(nullptr, IDC_ARROW),
        reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
        nullptr);
    return TRUE;
}

void CChildView::OnPaint() {
    CPaintDC dc(this);
    DrawBoard(&dc);
}

void CChildView::DrawBoard(CDC* pDC) {
    CRect rect;
    GetClientRect(&rect);

    int boardSize = min(rect.Width(), rect.Height());
    int cellSize = boardSize / 8;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            CRect cellRect(col * cellSize, row * cellSize, (col + 1) * cellSize, (row + 1) * cellSize);

            // Fill alternate cells with light brown and beige
            COLORREF color = (row + col) % 2 == 0 ? RGB(245, 245, 220) : RGB(222, 184, 135);
            pDC->FillSolidRect(&cellRect, color);

            // Draw pawn if any
            Pawn pawn = m_board.GetPawnAt(row, col);
            DrawPawn(pDC, row, col, pawn.GetType());
        }
    }

    if (m_isSelected) {
        std::vector<Move> validMoves = m_board.GetValidMoves(m_selectedRow, m_selectedCol);
        for (const auto& move : validMoves) {
            int toRow = move.GetEndRow();
            int toCol = move.GetEndCol();

            if (toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8) {
                CRect highlightRect(toCol * cellSize, toRow * cellSize, (toCol + 1) * cellSize, (toRow + 1) * cellSize);

                // Check if the move is a capture move
                if (abs(move.GetEndRow() - move.GetStartRow()) > 1) {
                    pDC->FillSolidRect(&highlightRect, RGB(255, 69, 0)); // Red for capture moves
                }
                else {
                    pDC->FillSolidRect(&highlightRect, RGB(135, 206, 250)); // Light blue for regular moves
                }
            }
        }
    }
}

void CChildView::DrawPawn(CDC* pDC, int row, int col, PawnType type) {
    if (type == PawnType::EMPTY) return;

    CRect rect;
    GetClientRect(&rect);

    int boardSize = min(rect.Width(), rect.Height());
    int cellSize = boardSize / 8;
    int centerX = col * cellSize + cellSize / 2;
    int centerY = row * cellSize + cellSize / 2;
    int radius = cellSize / 3;

    COLORREF color = (type == PawnType::RED || type == PawnType::RED_KING) ? RGB(255, 0, 0) : RGB(0, 0, 0);

    pDC->SelectStockObject(DC_BRUSH);
    pDC->SetDCBrushColor(color);
    pDC->Ellipse(centerX - radius, centerY - radius, centerX + radius, centerY + radius);

    if (type == PawnType::RED_KING || type == PawnType::BLACK_KING) {
        pDC->SetTextColor(RGB(255, 255, 0));
        pDC->DrawText(_T("K"), CRect(centerX - radius, centerY - radius, centerX + radius, centerY + radius), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void CChildView::HandleClick(int row, int col) {
    if (!m_isSelected) {
        Pawn pawn = m_board.GetPawnAt(row, col);
        if (pawn.GetType() != PawnType::EMPTY &&
            ((m_isBlackTurn && pawn.IsBlack()) || (!m_isBlackTurn && pawn.IsRed()))) {
            m_isSelected = true;
            m_selectedRow = row;
            m_selectedCol = col;
            m_validMoves = m_board.GetValidMoves(row, col);
        }
    }
    else {
        Move move(m_selectedRow, m_selectedCol, row, col);
        if (std::find_if(m_validMoves.begin(), m_validMoves.end(), [&move](const Move& m) {
            return m.GetStartRow() == move.GetStartRow() && m.GetStartCol() == move.GetStartCol()
                && m.GetEndRow() == move.GetEndRow() && m.GetEndCol() == move.GetEndCol(); }) != m_validMoves.end()) {

            bool isCapture = m_board.ApplyMove(move);

            if (isCapture && m_board.HasCaptureMove(row, col, m_board.GetPawnAt(row, col).GetType())) {
                // If there are more capture moves available, update validMoves and allow consecutive capture moves
                m_isSelected = true;
                m_selectedRow = row;
                m_selectedCol = col;
                m_validMoves = m_board.GetValidMoves(row, col);
            }
            else {
                // If no more capture moves available, end the turn
                m_isSelected = false;
                m_isBlackTurn = !m_isBlackTurn;
            }
        }
        else {
            m_isSelected = false;
        }
    }

    Invalidate();
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point) {
    CRect rect;
    GetClientRect(&rect);

    int boardSize = min(rect.Width(), rect.Height());
    int cellSize = boardSize / 8;

    int row = point.y / cellSize;
    int col = point.x / cellSize;

    HandleClick(row, col);

    Invalidate();
    CWnd::OnLButtonDown(nFlags, point);
}
