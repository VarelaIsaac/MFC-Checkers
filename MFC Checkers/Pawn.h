#pragma once

enum class PawnType { EMPTY, RED, BLACK, RED_KING, BLACK_KING };


class Pawn {
public:
    Pawn(PawnType type = PawnType::EMPTY) : m_type(type) {}

    PawnType GetType() const { return m_type; }
    void SetType(PawnType type) { m_type = type; }

    bool IsBlack() const {
        return m_type == PawnType::BLACK || m_type == PawnType::BLACK_KING;
    }

    bool IsRed() const {
        return m_type == PawnType::RED || m_type == PawnType::RED_KING;
    }

private:
    PawnType m_type;
};
