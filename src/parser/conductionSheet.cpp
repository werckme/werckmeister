#include "parser.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include "error.hpp"
#include "parserSymbols.h"
#include <fm/tools.h>
#include "parserPositionIt.h"

BOOST_FUSION_ADAPT_STRUCT(
	sheet::ConductionSelector,
	(sheet::ConductionSelector::Type, type)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::ConductionRule,
	(sheet::ConductionSelector, selector)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::ConductionSheetDef,
	(sheet::ConductionSheetDef::Rules, rules)
)
namespace sheet {
	namespace compiler {

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;

			template <typename Iterator>
			struct _ConductionParser : qi::grammar<Iterator, ConductionSheetDef(), ascii::space_type>
			{
				_ConductionParser(Iterator begin, ConductionSheetDef::SourceId sourceId = ConductionSheetDef::UndefinedSource) : 
					_ConductionParser::base_type(start, "conduction"),
					sourceId(sourceId)
				{
					using qi::int_;
					using qi::lit;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::on_error;
					using qi::fail;

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1);
					on_error<fail>(start, onError);
				}
				ConductionSheetDef::SourceId sourceId_ = ConductionSheetDef::UndefinedSource;
				qi::rule<Iterator, ConductionSheetDef(), ascii::space_type> start;
				CurrentPos<Iterator> current_pos_;
			};


			void _parse(const fm::String &source, ConductionSheetDef &def, ConductionSheetDef::SourceId sourceId)
			{
				using boost::spirit::ascii::space;
				typedef _ConductionParser<fm::String::const_iterator> ConductionParserType;
				
				ConductionParserType g(source.begin(), sourceId);
				phrase_parse(source.begin(), source.end(), g, space, def);
			}
		}


		ConductionSheetDef ConductionSheetParser::parse(fm::CharType const* first, fm::CharType const* last, ConductionSheetDef::SourceId sourceId)
		{
			
			ConductionSheetDef result;
			fm::String source(first, last);
			fm::removeComments(source.begin(), source.end());
			_parse(source, result, sourceId);
			return result;
		}
	}
}