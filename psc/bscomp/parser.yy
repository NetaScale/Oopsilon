%include {
#include <cstdlib>
#include <list>

#define YY_NO_UNISTD_H

#include "ast.hh"
#include "driver.hh"
#include "parser.tab.h"
#include "scanner.ll.hh"

#define LEMON_SUPER MVST_Parser

static std::string removeFirstChar (std::string aString)
{
	return aString.substr(1, aString.size() - 1);
}

static std::string removeFirstAndLastChar(std::string aString)
{
	return aString.substr(1, aString.size() - 2);
}
}

%token_type    {Token}
%token_prefix  TOK_
%left BAR.

%code {
ProgramNode * MVST_Parser::parseFile (std::string fName, std::string src)
{
	MVST_Parser *parser;
	yyscan_t scanner;
	YY_BUFFER_STATE yyb;

	parser = MVST_Parser::create(fName, src);
	if (0)
		parser->trace(stdout, "<parser>: ");

	mvstlex_init_extra(parser, &scanner);
	/* Now we need to scan our string into the buffer. */
	yyb = mvst_scan_string(src.c_str(), scanner);

	while (mvstlex(scanner))
		;
	parser->parse(TOK_EOF);
	parser->parse(0);

	return parser->program;
}

MethodNode * MVST_Parser::parseText (std::string src)
{
	MVST_Parser *parser;
	yyscan_t scanner;
	YY_BUFFER_STATE yyb;

	parser = MVST_Parser::create("<no-file>", src);
	if (1)//0)
		//parser->trace(stdout, "<parser>: ");

	mvstlex_init_extra(parser, &scanner);

	printf("Compiling immediate method: [ %s ]\n", src.c_str());
	/* Now we need to scan our string into the buffer. */
	yyb = mvst_scan_string(src.c_str(), scanner);

	parser->parse(TOK_DIRECTMETH);
	while (mvstlex(scanner))
		;
	parser->parse(TOK_EOF);
	parser->parse(0);

	return parser->meth;
}

MVST_Parser * MVST_Parser::create(std::string fName, std::string & fText)
{
	return new yypParser(fName, fText);
}

Position MVST_Parser::pos()
{
	yypParser *self = (yypParser *)this;
	return Position(self->m_oldLine, self->m_oldCol, self->m_oldPos,
	    self->m_line, self->m_col, self->m_pos);
}

}

%syntax_error {
	const YYACTIONTYPE stateno = yytos->stateno;
	size_t eolPos = fText.find("\n", m_pos);
	std::string eLine = fText.substr(m_pos, eolPos - m_pos);
	size_t i;

#if 0
	std::cerr << "PDST: " << fName << "(" << std::to_string(m_line) + ":" <<
	    std::to_string(m_col) << "): " <<
	    "Error V1001: Syntax error: unexpected " <<
	    yyTokenName[yymajor] << "\n";

	std::cerr << "+ " << eLine << "\n";
	std::cerr << "+ ";

	for (i = 0; i < m_oldCol; i++) {
		std::cerr << ((char)eLine[i] == '\t' ? "\t" : " ");
	}
	for (; i < m_col; i++)
		std::cerr << "^";

	std::cerr << "\n\texpected one of: \n";
#else
	printf("PDST: %d/%d: Syntax error, unexpected %s\n", m_line, m_col,
	    yyTokenName[yymajor]);
	printf("+  %s\n", eLine.c_str());
	printf("+ \n");
	for (i = 0; i < m_oldCol; i++) {
		putchar(((char)eLine[i] == '\t' ? '\t' : ' '));
	}
	for (; i < m_col; i++)
		putchar('^');

	printf("\n\texpected one of: \n");
#endif

	for (unsigned i = 0; i < YYNTOKEN; ++i) {
		int yyact = yy_find_shift_action(i, stateno);
		if (yyact != YY_ERROR_ACTION && yyact != YY_NO_ACTION)
#if 0
			std::cerr << "\t" << yyTokenName[i] << "\n";
#else
			printf("\t%s\n", yyTokenName[i]);
#endif
	}
}

prog ::= EOF.
prog ::= ASSIGN ATinclude CLASSRR SUBCLASSCOLON NAMESPACE CURRENTCOLON CHAR SYMBOL
	FLOAT INTEGER COLONVAR KEYWORD NAMESPACENAME IDENTIFIER STRING PRIMNUM DOT
	LBRACKET RBRACKET SEMICOLON SQB_OPEN SQB_CLOSE LCARET RCARET HASH COMMA BINARY
	UP DIRECTMETH.
