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
#include <com/tools.h>
#include "parserPositionIt.h"
#include "pitchParser.h"
#include <conductor/conductorNames.h>

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::ConductionSelector::ArgumentValue,
	(com::Ticks, numberValue)
	(documentModel::PitchDef, pitch)
	(com::String, name)
	(documentModel::ConductionSelector::ArgumentValue::ValueContext, valueContext))

namespace
{
	enum SelectorArgumentValueFields
	{
		ArNumberValue,
		ArPitch,
		ArString,
		ArValueContext
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::ConductionSelector,
	(unsigned int, sourcePositionBegin)
	(documentModel::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(com::String, type)
	(documentModel::ConductionSelector::Arguments, arguments))

namespace
{
	enum ConductionSelectorFields
	{
		CoSourcePosBegin,
		CoType,
		CoArguments
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::ConductionRule::Declaration,
	(unsigned int, sourcePositionBegin)
	(documentModel::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(com::String, property)(documentModel::ConductionRule::Declaration::OperationType, operation)
	(double, value)
	(documentModel::ConductionRule::Declaration::ValueUnit, unit))

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::ConductionRule,
	(unsigned int, sourcePositionBegin)
	(documentModel::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(documentModel::ConductionRule::Selectors, selectors)
	(documentModel::ConductionRule::Declarations, declarations))

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::ConductionSheetDef,
	(unsigned int, sourcePositionBegin)
	(documentModel::ConductionSheetDef::RulesSet, rulesSet))

namespace parser
{

	namespace
	{
		namespace qi = boost::spirit::qi;
		namespace ascii = boost::spirit::ascii;
		ExpressionSymbols expressionSymbols_;
		DegreeSymbols degreeSymbols_;
		template <typename Iterator>
		struct _ConductionParser : PitchParser, qi::grammar<Iterator, documentModel::ConductionSheetDef(), ascii::space_type>
		{
			_ConductionParser(Iterator begin, documentModel::ConductionSheetDef::SourceId sourceId = documentModel::ConductionSheetDef::UndefinedSource) : PitchParser(),
				_ConductionParser::base_type(start, "conduction"),
				sourceId_(sourceId)
			{
				using ascii::char_;
				using boost::phoenix::at_c;
				using boost::phoenix::insert;
				using boost::phoenix::push_back;
				using qi::_val;
				using qi::attr;
				using qi::double_;
				using qi::fail;
				using qi::int_;
				using qi::lexeme;
				using qi::lit;
				using qi::on_error;
				using namespace documentModel;
				typedef ConductionSelector::ArgumentValue ArgumentValue;
				current_pos_.setStartPos(begin);
				start.name("conduction documentModel");
				selector_.name("selector");
				declaration_.name("declaration");
				numberArgument_ %=
					double_[at_c<ArNumberValue>(_val) = qi::_1] >> attr(documentModel::PitchDef()) >> attr(com::String()) >> attr(ArgumentValue::ValueContext(ArgumentValue::Unspecified));

				pitchArgument_ %=
					attr(0) >> pitchOrAlias_[at_c<ArPitch>(_val) = qi::_1] >> attr(com::String()) >> attr(ArgumentValue::ValueContext(ArgumentValue::Unspecified));

				degreeArgument_ %=
					attr(0) >> degreeSymbols_[at_c<ArPitch>(_val) = qi::_1] >> attr(com::String()) >> attr(ArgumentValue::ValueContext(ArgumentValue::Unspecified));

				stringArgument_ %=
					attr(0) >> attr(documentModel::PitchDef()) >> lexeme[+char_("a-zA-Z0-9")] >> attr(ArgumentValue::ValueContext(ArgumentValue::Unspecified));

				chordNameArgument_ %=
					attr(0) >> attr(documentModel::PitchDef()) >> lexeme[+char_(ChordDefParser::ALLOWED_CHORD_SYMBOLS_REGEX)] >> attr(ArgumentValue::ValueContext(ArgumentValue::Unspecified));

				cueArgument_ %=
					attr(0) >> attr(documentModel::PitchDef()) >> "@" >> +char_("a-zA-Z0-9") >> attr(ArgumentValue::ValueContext(ArgumentValue::CueReference));

				selector_ %=
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__FROM_POSITION >> attr(SHEET_CONDUCTOR_SEL__FROM_POSITION) >> "(" >> (numberArgument_ | cueArgument_) >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__TO_POSITION >> attr(SHEET_CONDUCTOR_SEL__TO_POSITION) >> "(" >> (numberArgument_ | cueArgument_) >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__PITCH >> attr(SHEET_CONDUCTOR_SEL__PITCH) >> "(" >> +pitchArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__ON_BEAT >> attr(SHEET_CONDUCTOR_SEL__ON_BEAT) >> "(" >> +numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__NOT_ON_BEAT >> attr(SHEET_CONDUCTOR_SEL__NOT_ON_BEAT) >> "(" >> +numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__FROM_PITCH >> attr(SHEET_CONDUCTOR_SEL__FROM_PITCH) >> "(" >> pitchArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__TO_PITCH >> attr(SHEET_CONDUCTOR_SEL__TO_PITCH) >> "(" >> pitchArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__INSTRUMENT >> attr(SHEET_CONDUCTOR_SEL__INSTRUMENT) >> "(" >> +stringArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__PITCH >> attr(SHEET_CONDUCTOR_SEL__PITCH) >> "(" >> +pitchArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__FROM_BEAT >> attr(SHEET_CONDUCTOR_SEL__FROM_BEAT) >> "(" >> numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__TO_BEAT >> attr(SHEET_CONDUCTOR_SEL__TO_BEAT) >> "(" >> numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__FROM_BAR >> attr(SHEET_CONDUCTOR_SEL__FROM_BAR) >> "(" >> numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__TO_BAR >> attr(SHEET_CONDUCTOR_SEL__TO_BAR) >> "(" >> numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__ON_BAR >> attr(SHEET_CONDUCTOR_SEL__ON_BAR) >> "(" >> +numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__NTH_BAR >> attr(SHEET_CONDUCTOR_SEL__NTH_BAR) >> "(" >> +numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__CHANNEL >> attr(SHEET_CONDUCTOR_SEL__CHANNEL) >> "(" >> +numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__EXPRESSION >> attr(SHEET_CONDUCTOR_SEL__EXPRESSION) >> "(" >> +stringArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__ALL >> attr(SHEET_CONDUCTOR_SEL__ALL) >> "(" >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__WITHTAG >> attr(SHEET_CONDUCTOR_SEL__WITHTAG) >> "(" >> +stringArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__DEGREE >> attr(SHEET_CONDUCTOR_SEL__DEGREE) >> "(" >> +degreeArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__OCTAVE >> attr(SHEET_CONDUCTOR_SEL__OCTAVE) >> "(" >> +numberArgument_ >> ")") |
					(current_pos_.current_pos >> attr(sourceId_) >> SHEET_CONDUCTOR_SEL__CHORD >> attr(SHEET_CONDUCTOR_SEL__CHORD) >> "(" >> +chordNameArgument_ >> ")");
				operationType_ %=
					("+=" >> attr(ConductionRule::Declaration::OperationAdd)) |
					("-=" >> attr(ConductionRule::Declaration::OperationSubstract)) |
					("=" >> lit("&") >> lit("+") >> attr(ConductionRule::Declaration::OperationFollowUpAdd)) |
					("=" >> lit("&") >> lit("-") >> attr(ConductionRule::Declaration::OperationFollowUpSubstract)) |
					("=" >> attr(ConductionRule::Declaration::OperationSet));

				valueUnit_ %=
					("%" >> attr(ConductionRule::Declaration::UnitPercent)) |
					(attr(ConductionRule::Declaration::UnitAbsolute));

				declaration_ %=
					(current_pos_.current_pos >> attr(sourceId_) >> +char_("a-zA-Z") >> operationType_ >> double_ >> valueUnit_ >> ";");

				rules_ %=
					current_pos_.current_pos >> attr(sourceId_) >> +selector_ >> "{" >> *declaration_ > "}";

				rulesSet_ %= rules_ >> *( "," >> rules_);

				start %=
					current_pos_.current_pos > *rulesSet_ > boost::spirit::eoi;

				auto onError = boost::bind(&compiler::handler::errorHandler<Iterator>, _1, sourceId_);
				on_error<fail>(start, onError);
			}
			documentModel::ConductionSheetDef::SourceId sourceId_ = documentModel::ConductionSheetDef::UndefinedSource;
			qi::rule<Iterator, documentModel::ConductionSheetDef(), ascii::space_type> start;
			qi::rule<Iterator, documentModel::ConductionSelector(), ascii::space_type> selector_;
			qi::rule<Iterator, documentModel::ConductionRule(), ascii::space_type> rules_;
			qi::rule<Iterator, documentModel::ConductionSheetDef::Rules(), ascii::space_type> rulesSet_;
			qi::rule<Iterator, documentModel::ConductionSelector::ArgumentValue(), ascii::space_type> numberArgument_;
			qi::rule<Iterator, documentModel::ConductionSelector::ArgumentValue(), ascii::space_type> pitchArgument_;
			qi::rule<Iterator, documentModel::ConductionSelector::ArgumentValue(), ascii::space_type> degreeArgument_;
			qi::rule<Iterator, documentModel::ConductionSelector::ArgumentValue(), ascii::space_type> stringArgument_;
			qi::rule<Iterator, documentModel::ConductionSelector::ArgumentValue(), ascii::space_type> chordNameArgument_;
			qi::rule<Iterator, documentModel::ConductionSelector::ArgumentValue(), ascii::space_type> cueArgument_;
			qi::rule<Iterator, documentModel::ConductionRule::Declaration(), ascii::space_type> declaration_;
			qi::rule<Iterator, documentModel::ConductionRule::Declaration::OperationType(), ascii::space_type> operationType_;
			qi::rule<Iterator, documentModel::ConductionRule::Declaration::ValueUnit(), ascii::space_type> valueUnit_;
			CurrentPos<Iterator> current_pos_;
		};

		void _parse(const com::String &source, documentModel::ConductionSheetDef &def, documentModel::ConductionSheetDef::SourceId sourceId)
		{
			using boost::spirit::ascii::space;
			typedef _ConductionParser<com::String::const_iterator> ConductionParserType;

			ConductionParserType g(source.begin(), sourceId);
			phrase_parse(source.begin(), source.end(), g, space, def);
		}
	}

	documentModel::ConductionSheetDef ConductionSheetParser::parse(com::CharType const *first, com::CharType const *last, documentModel::ConductionSheetDef::SourceId sourceId)
	{

		documentModel::ConductionSheetDef result;
		com::String source(first, last);
		com::removeComments(source.begin(), source.end());
		_parse(source, result, sourceId);
		return result;
	}
}