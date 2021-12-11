#pragma once

#include <algorithm>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <com/common.hpp>
#include <exception>

#include <iostream>

#define BEGIN_DOCUMENT_CONFIG_LINE_CHAR '@'

namespace documentModel {

	namespace compiler {
		typedef com::CharType CharType;
		typedef com::String StringType;
		typedef com::StringStream StringStream;
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
				: documentUsing(FM_STRING("\\s*@.+?;"))     // define tokens
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
			TokenDef documentUsing
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
			typename Base::Tokens documentUsings;
			typename Base::Tokens chordDefs;
		};

		template <typename Lexer>
		ChordDefTokenizer<Lexer>::ChordDefTokenizer()
		{
			auto addConfigs = boost::bind(&Base::add, this, _1, _2, boost::ref(documentUsings));
			auto addDef = boost::bind(&Base::add, this, _1, _2, boost::ref(chordDefs));
			this->self
				= (Base::documentUsing[addConfigs] | Base::comment | Base::chordDef[addDef])
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
			typename Base::Tokens documentUsings;
			typename Base::Tokens pitchdefs;
		};

		template <typename Lexer>
		PitchmapTokenizer<Lexer>::PitchmapTokenizer()
		{
			auto addConfigs = boost::bind(&Base::add, this, _1, _2, boost::ref(documentUsings));
			auto addDef = boost::bind(&Base::add, this, _1, _2, boost::ref(pitchdefs));
			this->self
				= (Base::documentUsing[addConfigs] | Base::comment | Base::pitchDef[addDef])
				| Base::eol
				| Base::any
				;
		}
		/////////////////////////////////////////////////////////////////////////////
		template <typename Lexer>
		struct DocumentUsingTokenizer : public ASheetTokenizer<Lexer>
		{
			typedef ASheetTokenizer<Lexer> Base;
			DocumentUsingTokenizer();
			StringStream documentUsings;
			void onDocDef(CharType const *begin, CharType const *end) 
			{
				documentUsings << com::String(begin, end) << std::endl;
			}
		};

		template <typename Lexer>
		DocumentUsingTokenizer<Lexer>::DocumentUsingTokenizer()
		{
			auto addConfigs = boost::bind(&DocumentUsingTokenizer::onDocDef, this, _1, _2);
			this->self
				= (Base::documentUsing[addConfigs] | Base::comment)
				| Base::eol
				| Base::any
				;
		}
	}
}


