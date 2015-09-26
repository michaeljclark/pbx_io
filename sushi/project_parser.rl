//
//  project_parser.rl
//

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cctype>
#include <cstring>

#include "sushi.h"

#include "project_parser.h"

%%{
	
	machine project_parser;

	action mark             { mark = fpc; }
	action w_begin_block    { begin_block(); }
	action w_end_block      { end_block(); }
	action w_symbol         { symbol(mark, fpc - mark); }
	action w_qsymbol        { symbol(mark + 1, fpc - mark - 2); }
	action w_end_statement  { end_statement(); }

	action done { 
		project_done();
		fbreak;
	}

	squote = "'";
	dquote = '"';
	escape = /\\./;
	not_squote_or_escape = [^'\\];
	not_dquote_or_escape = [^"\\];
	symbol_special = ( '~' | '!' | '@' | '$' | '%' | '^' | '&' | '*' | '(' | ')' | '-' | '_' |
		'=' |'+' | '[' | ']' | '|' | ':' | '<' | '>' | '.' | ',' | '/' | '?' );
	unquote_symbol_chars = alnum | symbol_special;
	squote_symbol = ( squote ( not_squote_or_escape | escape )* squote ) >mark %w_qsymbol;
	dquote_symbol = ( dquote ( not_dquote_or_escape | escape )* dquote ) >mark %w_qsymbol;
	unquote_symbol = ( unquote_symbol_chars | escape )+ >mark %w_symbol;
	symbol = ( squote_symbol | dquote_symbol | unquote_symbol );

	Eol = ';' %w_end_statement;
	newline = ('\r' '\n' ) | '\n';
	ws = (' ' | '\t' | '\r' | '\n' )+;
	comment = '#' ( any - '\n' )* '\n';
	statement = ( symbol ( ws symbol)* ) ws* Eol;
	begin_block = ( symbol ( ws symbol)* ) ws+ '{' %w_begin_block;
	end_block = '}' %w_end_block;
	project = ( comment | begin_block | end_block | statement | ws )* %done;

	main := project;

}%%

%% write data;

bool project_parser::parse(const char *buffer, size_t len)
{
	int cs = project_parser_en_main;
	
	const char *mark = NULL;
	const char *p = buffer;
	const char *pe = buffer + strlen(buffer);
	const char *eof = pe;

	%% write init;
	%% write exec;

	return (cs != project_parser_error || cs == project_parser_first_final);
}
