#ifndef FM_LEXER_HPP
#define FM_LEXER_HPP

#include <algorithm>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <fm/common.hpp>
#include <exception>

#include <iostream>

#define BEGIN_DOCUMENT_CONFIG_LINE_CHAR '@'

namespace sheet {

	namespace compiler {
		typedef fm::CharType CharType;
		typedef fm::String StringType;
		typedef fm::StringStream StringStream;
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

			StringType withoutComment(CharType const *begin, CharType const *end)
			{
				auto it = begin;
				while (it != end) {
					if (*it == '-') {
						if ((it + 1) != end && *(it + 1) == '-') {
							break;
						}
					}
					++it;
				}
				return StringType(begin, it);
			}

			ASheetTokenizer()
				: documentConfig(FM_STRING("\\s*@.+?;"))     // define tokens
				, eol(FM_STRING("\\s*\n"))
				, any(FM_STRING("."))
				, comment(FM_STRING("^\\s*--.+$"))
				, chordDef(FM_STRING("^.+$"))
				, pitchDef(FM_STRING("^.+$"))
				, beginSection(FM_STRING("\\s*section\\s+[a-zA-Z0-9]+"))
				, line(FM_STRING("^.+$"))
				, endSection(FM_STRING("\\s*end"))
			{
			}
			TokenDef documentConfig
				, eol
				, any
				, comment
				, chordDef
				, pitchDef
				, beginSection
				, line
				, endSection;

			virtual ~ASheetTokenizer() = default;
		};

		/////////////////////////////////////////////////////////////////////////////
		template <typename Lexer>
		struct ChordDefTokenizer : public ASheetTokenizer<Lexer>
		{
			typedef ASheetTokenizer<Lexer> Base;
			ChordDefTokenizer();
			typename Base::Tokens documentConfigs;
			typename Base::Tokens chordDefs;
		};

		template <typename Lexer>
		ChordDefTokenizer<Lexer>::ChordDefTokenizer()
		{
			auto addConfigs = boost::bind(&Base::add, this, _1, _2, boost::ref(documentConfigs));
			auto addDef = boost::bind(&Base::add, this, _1, _2, boost::ref(chordDefs));
			this->self
				= (Base::documentConfig[addConfigs] | Base::comment | Base::chordDef[addDef])
				| Base::eol
				| Base::any
				;
		}
		/////////////////////////////////////////////////////////////////////////////
		template <typename Lexer>
		struct PitchmapTokenizer : public ASheetTokenizer<Lexer>
		{
			typedef ASheetTokenizer<Lexer> Base;
			PitchmapTokenizer();
			typename Base::Tokens documentConfigs;
			typename Base::Tokens pitchdefs;
		};

		template <typename Lexer>
		PitchmapTokenizer<Lexer>::PitchmapTokenizer()
		{
			auto addConfigs = boost::bind(&Base::add, this, _1, _2, boost::ref(documentConfigs));
			auto addDef = boost::bind(&Base::add, this, _1, _2, boost::ref(pitchdefs));
			this->self
				= (Base::documentConfig[addConfigs] | Base::comment | Base::pitchDef[addDef])
				| Base::eol
				| Base::any
				;
		}
		/////////////////////////////////////////////////////////////////////////////
		template <typename Lexer>
		struct DocumentConfigTokenizer : public ASheetTokenizer<Lexer>
		{
			typedef ASheetTokenizer<Lexer> Base;
			DocumentConfigTokenizer();
			StringStream documentConfigs;
			void onDocDef(CharType const *begin, CharType const *end) 
			{
				documentConfigs << fm::String(begin, end) << std::endl;
			}
		};

		template <typename Lexer>
		DocumentConfigTokenizer<Lexer>::DocumentConfigTokenizer()
		{
			auto addConfigs = boost::bind(&DocumentConfigTokenizer::onDocDef, this, _1, _2);
			this->self
				= (Base::documentConfig[addConfigs] | Base::comment)
				| Base::eol
				| Base::any
				;
		}


		/////////////////////////////////////////////////////////////////////////////
		template <typename Lexer>
		struct SheetDefTokenizer : public ASheetTokenizer<Lexer>
		{
			typedef ASheetTokenizer<Lexer> Base;
			SheetDefTokenizer();
			typename Base::Tokens documentConfigs;
			StringStream lines;
		private:
			void onLine_(CharType const *begin, CharType const *end)
			{
				auto line = Base::withoutComment(begin, end);
				lines << line << std::endl;
			}
		};

		template <typename Lexer>
		SheetDefTokenizer<Lexer>::SheetDefTokenizer()
		{
			auto onLine = boost::bind(&SheetDefTokenizer::onLine_, this, _1, _2);
			this->self
				= 
				  Base::line[onLine]
				| Base::comment
				| Base::eol
				| Base::any
				;
		}
	}
}

#endif
