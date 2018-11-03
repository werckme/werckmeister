#include "parser.h"
#include "lexer.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include "error.hpp"

BOOST_FUSION_ADAPT_STRUCT(
	sheet::DocumentConfig,
	(sheet::DocumentConfig::Usings, usings)
)

namespace sheet {
	namespace compiler {

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;

			template <typename Iterator>
			struct _ConfigParser : qi::grammar<Iterator, DocumentConfig(), ascii::space_type>
			{
				typedef ChordDef::Intervals Intervals;
				_ConfigParser() : _ConfigParser::base_type(start, "document config")
				{
					using qi::int_;
					using qi::lit;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::on_error;
					using qi::fail;

					start.name("document config");
					quoted_string.name("quoted string");

					quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];
					using_ %= "@load" >> quoted_string >> ";";
					usings_ %= *using_;
					start %= usings_;

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1);
					on_error<fail>(start, onError);
				}
				qi::rule<Iterator, DocumentConfig(), ascii::space_type> start;
				qi::rule<Iterator, fm::String(), ascii::space_type> quoted_string;
				qi::rule<Iterator, fm::String(), ascii::space_type> using_;
				qi::rule<Iterator, DocumentConfig::Usings, ascii::space_type> usings_;
			};


			void _parse(const fm::String &defStr, DocumentConfig &conf)
			{
				using boost::spirit::ascii::space;
				typedef _ConfigParser<fm::String::const_iterator> ChordParserType;
				ChordParserType g;
				bool r = phrase_parse(defStr.begin(), defStr.end(), g, space, conf);
			}
		}


		DocumentConfig DocumentConfigParser::parse(fm::CharType const* first, fm::CharType const* last)
		{
			
			DocumentConfig result;
			DocumentConfigTokenizer<LexerType> docTok;
			LexerType::iterator_type iter = docTok.begin(first, last);
			LexerType::iterator_type end = docTok.end();
			boost::spirit::lex::tokenize(first, last, docTok);
			auto docDefsStr = docTok.documentConfigs.str();
			_parse(docDefsStr, result);
			return result;
		}
	}
}