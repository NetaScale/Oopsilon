#ifndef AST_H_
#define AST_H_

#include <cstddef>

/* Details of the position of some source code. */
class Position {
	size_t m_oldLine, m_oldCol, m_oldPos;
	size_t m_line, m_col, m_pos;

    public:
	Position() {};
	Position(size_t oldLine, size_t oldCol, size_t oldPos, size_t line,
	    size_t col, size_t pos)
	    : m_oldLine(oldLine)
	    , m_oldCol(oldCol)
	    , m_oldPos(oldPos)
	    , m_line(line)
	    , m_col(col)
	    , m_pos(pos) {};
	Position(const Position &a, const Position &b)
	    : m_oldLine(a.m_oldLine)
	    , m_oldCol(a.m_oldCol)
	    , m_oldPos(a.m_oldPos)
	    , m_line(b.m_line)
	    , m_col(b.m_col)
	    , m_pos(b.m_pos) {};

	/* Get line number */
	size_t line() const { return m_oldLine; }
	/* Get column number*/
	size_t col() const { return m_oldCol; }
	/* Get absolute position in source-file */
	size_t pos() const;
};

#endif /* AST_H_ */
