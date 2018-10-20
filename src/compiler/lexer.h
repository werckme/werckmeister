#ifndef FM_LEXER_HPP
#define FM_LEXER_HPP

#include <algorithm>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/bind.hpp>
#include <vector>

namespace sheet {

	namespace compiler {

		typedef
			boost::spirit::lex::lexertl::token<char const*, boost::spirit::lex::omit, boost::mpl::false_> token_type;

		typedef boost::spirit::lex::lexertl::actor_lexer<token_type> lexer_type;

		template <typename Lexer>
		struct ASheetTokenizer : boost::spirit::lex::lexer<Lexer>
		{
			typedef std::string Token;
			typedef std::vector<std::string> Tokens;
			typedef boost::spirit::lex::token_def<> TokenDef;

			void add(char const *begin, char const *end, Tokens &container) {
				container.push_back(std::string(begin, end));
			}

			ASheetTokenizer()
				: documentConfig("^\\s*@.+?;")     // define tokens
				, comment("--.+$")
				, eol("\\s*\n")
				, any(".")
				, chordDef("^.*$")
			{
			}
			TokenDef documentConfig, eol, any, comment, chordDef;
		};

		/////////////////////////////////////////////////////////////////////////////
		template <typename Lexer>
		struct ChordDefTokenizer : public ASheetTokenizer<Lexer>
		{
			typedef ASheetTokenizer<Lexer> Base;
			ChordDefTokenizer();
			Tokens comments;
			Tokens documentConfigs;
			Tokens chordDefs;
		};

		template <typename Lexer>
		ChordDefTokenizer<Lexer>::ChordDefTokenizer()
		{
			auto addComments = boost::bind(&Base::add, this, _1, _2, boost::ref(comments));
			auto addConfigs = boost::bind(&Base::add, this, _1, _2, boost::ref(documentConfigs));
			auto addDef = boost::bind(&Base::add, this, _1, _2, boost::ref(chordDefs));
			// associate tokens with the lexer
			this->self
				= (documentConfig[addConfigs] | comment[addComments] | chordDef[addDef])
				| eol
				| any
				;
		}
	}
}

#endif