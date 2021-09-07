#include "parser.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/bind.hpp>
#include "error.hpp"
#include "parserSymbols.h"
#include <fm/tools.h>
#include "parserPositionIt.h"
#include "pitchParser.h"

BOOST_FUSION_ADAPT_STRUCT(
	sheet::ConductionSelector::ArgumentValue,
	(fm::Ticks, tickValue)
	(sheet::PitchDef, pitch)
)

namespace {
	enum SelectorArgumentValueFields {
		ArTickValue,
		ArPitch
	};
}


BOOST_FUSION_ADAPT_STRUCT(
	sheet::ConductionSelector,
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(sheet::ConductionSelector::Type, type)
	(sheet::ConductionSelector::Arguments, arguments)
)

namespace {
	enum ConductionSelectorFields {
		CoSourcePosBegin,
		CoType,
		CoArguments
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	sheet::ConductionRule::Declaration,
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(sheet::ConductionRule::Declaration::Property, property)
	(sheet::ConductionRule::Declaration::OperationType, operation)
	(double, value)
	(sheet::ConductionRule::Declaration::ValueUnit, unit)
)


BOOST_FUSION_ADAPT_STRUCT(
	sheet::ConductionRule,
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(sheet::ConductionRule::Selectors, selectors)
	(sheet::ConductionRule::Declarations, declarations)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::ConductionSheetDef,
	(unsigned int, sourcePositionBegin)
	(sheet::ConductionSheetDef::Rules, rules)
)


namespace sheet {
	namespace compiler {

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;

			template <typename Iterator>
			struct _ConductionParser : PitchParser, qi::grammar<Iterator, ConductionSheetDef(), ascii::space_type>
			{
				_ConductionParser(Iterator begin, ConductionSheetDef::SourceId sourceId = ConductionSheetDef::UndefinedSource) : 
					PitchParser(),
					_ConductionParser::base_type(start, "conduction"),
					sourceId_(sourceId)
				{
					using qi::int_;
					using qi::lit;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::on_error;
					using qi::fail;
					using qi::attr;
					using boost::phoenix::at_c;
					using boost::phoenix::push_back;
					using boost::phoenix::insert;
					using qi::_val;
			
					current_pos_.setStartPos(begin);
					start.name("conduction sheet");

					argument_ %= 
						  (double_[at_c<ArTickValue>(_val) = qi::_1] >> attr(PitchDef()))
						| (attr(0) >> pitchOrAlias_[at_c<ArPitch>(_val) = qi::_1] )
					;

					selector_ %= 
					(
						current_pos_.current_pos
						>> attr(sourceId_)
						>> "fromPosition" >> attr(ConductionSelector::TypeFromPosition) >> "(" >> argument_ >> ")"
					)
					|
					(
						current_pos_.current_pos
						>> attr(sourceId_)
						>> "toPosition" >> attr(ConductionSelector::TypeToPosition) >> "(" >> argument_ >> ")"
					)
					|
					(
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "pitch" >> attr(ConductionSelector::TypePitch) >> "(" >> +argument_ >> ")"
					)
					|
					(
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "atBeat" >> attr(ConductionSelector::TypeAtBeat) >> "(" >> +argument_ >> ")"
					)					
					;

					operationType_ %= 
					(
						"+=" >> attr(ConductionRule::Declaration::OperationAdd)
					)
					|
					(
						"=" >> attr(ConductionRule::Declaration::OperationSet)
					)
					;

					valueUnit_ %= 
					(
						"%" >> attr(ConductionRule::Declaration::UnitPercent)
					)
					|
					(
						attr(ConductionRule::Declaration::UnitAbsolute)
					)
					;

					declaration_ %= 
						current_pos_.current_pos
						>> attr(sourceId_)
						>> ( 
							  "velocity" >> attr(ConductionRule::Declaration::PropertyVelocity)
							| "time"  	 >> attr(ConductionRule::Declaration::PropertyTime)
						)
						>> operationType_
						>> double_
						>> valueUnit_
						>> ";"
					;

					rules_ %= 
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> +selector_
						>> "{" >> *declaration_ > "}"
					;

					start %= 
						current_pos_.current_pos 
						> *rules_
						> boost::spirit::eoi
					;

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1, sourceId_);
					on_error<fail>(start, onError);
				}
				ConductionSheetDef::SourceId sourceId_ = ConductionSheetDef::UndefinedSource;
				qi::rule<Iterator, ConductionSheetDef(), ascii::space_type> start;
				qi::rule<Iterator, ConductionSelector(), ascii::space_type> selector_;
				qi::rule<Iterator, ConductionRule(), ascii::space_type> rules_;
				qi::rule<Iterator, sheet::ConductionSelector::ArgumentValue(), ascii::space_type> argument_;
				qi::rule<Iterator, sheet::ConductionRule::Declaration(), ascii::space_type> declaration_;
				qi::rule<Iterator, sheet::ConductionRule::Declaration::OperationType(), ascii::space_type> operationType_;
				qi::rule<Iterator, sheet::ConductionRule::Declaration::ValueUnit(), ascii::space_type> valueUnit_;
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