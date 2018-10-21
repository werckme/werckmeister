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
#include <fm/common.hpp>
#include <exception>

namespace sheet {

	namespace compiler {
		typedef fm::CharType CharType;
		typedef fm::String StringType;
		typedef
			boost::spirit::lex::lexertl::token<CharType const*, boost::spirit::lex::omit, boost::mpl::false_> TokenType;

		typedef boost::spirit::lex::lexertl::actor_lexer<TokenType> LexerType;

		template <typename Lexer>
		struct ASheetTokenizer : boost::spirit::lex::lexer<Lexer>
		{
			typedef StringType Token;
			typedef std::vector<StringType> Tokens;
			typedef boost::spirit::lex::token_def<boost::spirit::unused_type, CharType> TokenDef;

			void add(CharType const *begin, CharType const *end, Tokens &container) {
				container.push_back(StringType(begin, end));
			}

			ASheetTokenizer()
				: documentConfig(FM_STRING("^\\s*@.+?;"))     // define tokens
				, comment(FM_STRING("^\\s*--.+$"))
				, eol(FM_STRING("\\s*\n"))
				, any(FM_STRING("."))
				, chordDef(FM_STRING("^.+$"))
				, beginSection(FM_STRING("\\s*section\\s+[a-zA-Z0-9]+"))
				, line(FM_STRING("^.+$"))
				, endSection(FM_STRING("\\s*end"))
			{
			}
			TokenDef documentConfig, eol, any, comment, chordDef, beginSection, line, endSection;
			virtual ~ASheetTokenizer() = default;
		};

		/////////////////////////////////////////////////////////////////////////////
		template <typename Lexer>
		struct ChordDefTokenizer : public ASheetTokenizer<Lexer>
		{
			typedef ASheetTokenizer<Lexer> Base;
			ChordDefTokenizer();
			Tokens documentConfigs;
			Tokens chordDefs;
		};

		template <typename Lexer>
		ChordDefTokenizer<Lexer>::ChordDefTokenizer()
		{
			auto addConfigs = boost::bind(&Base::add, this, _1, _2, boost::ref(documentConfigs));
			auto addDef = boost::bind(&Base::add, this, _1, _2, boost::ref(chordDefs));
			this->self
				= (documentConfig[addConfigs] | comment | chordDef[addDef])
				| eol
				| any
				;
		}
		/////////////////////////////////////////////////////////////////////////////
		template <typename Lexer>
		struct StyleDefTokenizer : public ASheetTokenizer<Lexer>
		{
			typedef ASheetTokenizer<Lexer> Base;
			StyleDefTokenizer();
			Tokens documentConfigs;
			Tokens sections;
		private:
			void beginSection_(CharType const *begin, CharType const *end)
			{
				if (this->sectionBegin) {
					throw std::runtime_error("invalid section");
				}
				this->sectionBegin = begin;
			}
			void endSection_(CharType const *begin, CharType const *end)
			{
				if (end && begin >= end) {
					throw std::runtime_error("invalid section");
				}
				sections.push_back(StringType(this->sectionBegin, end));
				this->sectionBegin = nullptr;
			}
			CharType const* sectionBegin = nullptr;
		};

		template <typename Lexer>
		StyleDefTokenizer<Lexer>::StyleDefTokenizer()
		{
			auto addConfigs = boost::bind(&Base::add, this, _1, _2, boost::ref(documentConfigs));
			auto onBeginSection = boost::bind(&StyleDefTokenizer::beginSection_, this, _1, _2);
			auto onEndSection = boost::bind(&StyleDefTokenizer::endSection_, this, _1, _2);
			this->self
				= (documentConfig[addConfigs] | comment | line | beginSection[onBeginSection] | endSection[onEndSection])
				| eol
				| any
				;
		}

	}
}

#endif