#include "pch.h"
#include <fen.h>
#include <position/bit_board.h>
#include <square.h>
#include <regex>
#include <vector>
#include <format>
#include <array>
#include <optional>

namespace uci::fen {

    static const std::regex fenRegex(
        R"(^([pnbrqkPNBRQK1-8]+\/){7}([pnbrqkPNBRQK1-8]+) [wb] [-KQkq]{0,4} [a-h36\-] \d+ \d+$)"
    );

	static constexpr int char_to_index(const char c) {
		switch (c) {
		case 'P': return 0; case 'N': return 1; case 'B': return 2;
		case 'R': return 3; case 'Q': return 4; case 'K': return 5;
		case 'p': return 6; case 'n': return 7; case 'b': return 8;
		case 'r': return 9; case 'q': return 10; case 'k': return 11;
		default:  return -1;
		}
	}

    static constexpr char index_to_char(const unsigned int index) {
        switch (index) {
        case 0:  return 'P'; case 1:  return 'N'; case 2:  return 'B';
        case 3:  return 'R'; case 4:  return 'Q'; case 5:  return 'K';
        case 6:  return 'p'; case 7:  return 'n'; case 8:  return 'b';
        case 9:  return 'r'; case 10: return 'q'; case 11: return 'k';
        default: return '?'; // error indicator
        }
    }

    static void parse_position_string(const char* posString, core::Position& position) {

        int rank = 0; int file = 0;
        int squareIndex = 0;

        while (*posString != '\0') {
            const char c = *posString;
            posString++;

            if (c == '/') {
                rank++;
                file = 0;
                continue;
            }

            if (std::isdigit(c)) {
                file += c - '0';  // Skip that many empty squares
            }
            else {
                int index = char_to_index(c);
                squareIndex = (7 - rank) * 8 + file;  // Map FEN pos to bitboard index
                position.bitBoards[char_to_index(c)] |= core::bit_board::square_to_bit_board(squareIndex);
                file++;
            }
        }

        if (squareIndex != 7) throw std::invalid_argument("Invalid FEN string"); // Square index must end on h1
    }

    static void parse_castling(const char* castleString, core::Position& position) {
        position.w_kingSideCastle =  false;
        position.b_kingSideCastle =  false;
        position.w_queenSideCastle = false;
        position.b_queenSideCastle = false;

        for (int i = 0; i < 4; i++) {
            switch (*(castleString + i))
            {
            case 'K': position.w_kingSideCastle  = true; break;
            case 'Q': position.w_queenSideCastle = true; break;
            case 'k': position.b_kingSideCastle  = true; break;
            case 'q': position.b_queenSideCastle = true; break;
            default:  return;
            }
        }
    }

    static void parse_en_passant_square(const char* sq, core::Position& position) {
        if (*sq == '-') return;
        int squareIndex = uci::uci_to_square_index(std::string(sq, 2));
        position.enPassantSquare = squareIndex;
    }

	std::optional<core::Position> parse(char* fen) {
        if (!std::regex_match(fen, fenRegex)) return std::nullopt;

        core::Position position = core::Position::default_position();
        
        std::array<const char*, 6> fields{};
        int field = 0;
        fields[field++] = fen;

        for (const char* p = fen; *p && field < 6; ++p) {
            if (*p == ' ')
                fields[field++] = p + 1;
        }

        try {
            parse_position_string(fields[0], position);
            position.toMove = *fields[1] == 'w' ? core::Color::White : core::Color::Black;
            parse_castling(fields[2], position);
            parse_en_passant_square(fields[3], position);
            position.halfMoveClock = std::atoi(fields[4]);
            position.fullMoveClock = std::atoi(fields[5]);
        }
        catch (std::invalid_argument e) {
            return std::nullopt;
        }

        return position;
	}

    static void format_position(std::string& fen, core::Position& position) {

        for (int rank = 7; rank >= 0; rank--) {
            int emptyFor = 0;

            for (int file = 0; file < 7; file++) {
                int squareIndex = file + rank * 8;
                int pieceAtSquare = position.pieceIndexAtSquare[squareIndex];

                if (pieceAtSquare == -1) emptyFor++;
                else {
                    char piece = index_to_char(pieceAtSquare);
                    fen += emptyFor == 0 ? piece + "" : std::format("%d%c", emptyFor, piece);
                }
            }

            if (rank != 0) fen += emptyFor == 0 ? "/" : std::format("%d/", emptyFor + 1);;
        }
    }

    static void format_castling(std::string& fen, core::Position& position) {
        bool anyCastle = false;

        if (position.w_kingSideCastle)  fen += 'K'; anyCastle = true;
        if (position.w_queenSideCastle) fen += 'Q'; anyCastle = true;
        if (position.b_kingSideCastle)  fen += 'k'; anyCastle = true;
        if (position.b_queenSideCastle) fen += 'q'; anyCastle = true;

        if (!anyCastle) fen += "-";
    }

    std::string format(core::Position& position) {
        std::string fen = std::string();
        format_position(fen, position); fen += ' ';
        fen += position.toMove == core::Color::White ? "w " : "b ";
        format_castling(fen, position); fen += ' ';
        fen += position.halfMoveClock + ' ';
        fen += position.fullMoveClock;

        return fen;
    }
}