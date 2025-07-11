#include <boost/test/unit_test.hpp>
#include <iostream>
#include "parser/parser.h"
#include "compiler/error.hpp"
#include <com/literals.hpp>
#include <com/units.hpp>
#include "testhelper.h"

using namespace parser;

BOOST_AUTO_TEST_CASE(parse_empty_succeeds)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
}

BOOST_AUTO_TEST_CASE(parse_invalid_fails)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("0123456");
	ConductionSheetParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_emptyDeclarations)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1){} \
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
}

BOOST_AUTO_TEST_CASE(parse_twoSlector_emptyDeclarations)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) pitch(c d' x \"bd\"){} \
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0].size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].type, "pitch");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].arguments.size(), size_t(4));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].arguments[0].pitch.pitch, documentModel::PitchDef::Pitch(0));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].arguments[0].pitch.octave, documentModel::PitchDef::Octave(0));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].arguments[1].pitch.pitch, documentModel::PitchDef::Pitch(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].arguments[1].pitch.octave, documentModel::PitchDef::Octave(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].arguments[2].pitch.alias, com::String("x"));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].arguments[3].pitch.alias, com::String("bd"));
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_oneDeclaration)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) {\
		velocity+=10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].numberValue(), double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, documentModel::ConductionRule::Declaration::UnitAbsolute);
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_twoDeclarations)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) {\
		velocity+=10;\
		time=2%;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].numberValue(), double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, documentModel::ConductionRule::Declaration::UnitAbsolute);

	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].property, "time");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].operation, documentModel::ConductionRule::Declaration::OperationSet);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].numberValue(), double(2));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].unit, documentModel::ConductionRule::Declaration::UnitPercent);
}

BOOST_AUTO_TEST_CASE(parse_noDeclarationBody_fails)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("onBeat(1)");
	ConductionSheetParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);
}

BOOST_AUTO_TEST_CASE(parse_oneSelectorWithNameArgument)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	instrument(myInstrument) {} \
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "instrument");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].name, com::String("myInstrument"));
}

BOOST_AUTO_TEST_CASE(parse_oneSelectorWithTwoNameArguments)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	instrument(ia ib) {} \
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "instrument");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].name, com::String("ia"));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[1].name, com::String("ib"));
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_followUpOperator_1)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) {\
		velocity =& +10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].numberValue(), double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, documentModel::ConductionRule::Declaration::UnitAbsolute);
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_followUpOperator_2)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) {\
		velocity = &+10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].numberValue(), double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, documentModel::ConductionRule::Declaration::UnitAbsolute);
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_followUpOperator_3)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) {\
		velocity = & + 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].numberValue(), double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, documentModel::ConductionRule::Declaration::UnitAbsolute);
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_followUpOperator_4)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) {\
		velocity = & - 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpSubstract);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].numberValue(), double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, documentModel::ConductionRule::Declaration::UnitAbsolute);
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_followUpOperator_5)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) {\
		velocity = & -10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpSubstract);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].numberValue(), double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, documentModel::ConductionRule::Declaration::UnitAbsolute);
}

BOOST_AUTO_TEST_CASE(parse_cue_position)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	fromPosition(@myCue) {\
		velocity = 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "fromPosition");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].name, "myCue");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].valueContext, documentModel::ConductionSelector::ArgumentValue::CueReference);
}

BOOST_AUTO_TEST_CASE(parse_quarters_position)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	fromPosition(1) {\
		velocity = 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "fromPosition");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].valueContext, documentModel::ConductionSelector::ArgumentValue::Unspecified);
}

BOOST_AUTO_TEST_CASE(parse_position_fail)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	fromPosition(myCuew) {\
		velocity = 10;\
	}\
");
	ConductionSheetParser parser;
	BOOST_CHECK_THROW(parser.parse(text), Exception);
}


BOOST_AUTO_TEST_CASE(parse_degree_selector)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	degree(I) {\
		velocity = 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "degree");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].pitch.pitch, com::degrees::I);
}

BOOST_AUTO_TEST_CASE(parse_degree_selector_two_args)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	degree(I V) {\
		velocity = 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "degree");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].pitch.pitch, com::degrees::I);
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[1].pitch.pitch, com::degrees::V);
}

BOOST_AUTO_TEST_CASE(parse_degree_selector_accidentals)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	degree(Ib V#) {\
		velocity = 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "degree");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].pitch.pitch, com::degrees::Ies);
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[1].pitch.pitch, com::degrees::Vis);
}

BOOST_AUTO_TEST_CASE(parse_chord_selector)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	chord(myChord7) {\
		velocity = 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "chord");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].name, com::String("myChord7"));
}

BOOST_AUTO_TEST_CASE(parse_chord_selector_two_arguments)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	chord(myChord7 otherChord9) {\
		velocity = 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "chord");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].name, com::String("myChord7"));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[1].name, com::String("otherChord9"));
}

BOOST_AUTO_TEST_CASE(parse_octave_selector)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	octave(0 1 2) {\
		velocity = 10;\
	}\
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "octave");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(3));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, 0.0);
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[1].numberValue, 1.0);
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[2].numberValue, 2.0);
}

BOOST_AUTO_TEST_CASE(parse_selectors_two_selectors_comma_separated)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1), onBar(4) {} \
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][0].type, "onBar");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][0].arguments[0].numberValue, com::Ticks(4));	
}

BOOST_AUTO_TEST_CASE(parse_selectors_two_selector_groups_comma_separated)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) fromPosition(5), onBar(10) toPosition(100) {} \
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0].size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].type, "fromPosition");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][1].arguments[0].numberValue, com::Ticks(5));

	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1].size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][0].type, "onBar");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][0].arguments[0].numberValue, com::Ticks(10));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][1].type, "toPosition");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][1].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][1].arguments[0].numberValue, com::Ticks(100));	
}

BOOST_AUTO_TEST_CASE(parse_selectors_four_selectors_comma_separated)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1), fromPosition(5), onBar(10), toPosition(100) {} \
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(4));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0].size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1));

	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1].size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][0].type, "fromPosition");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[1][0].arguments[0].numberValue, com::Ticks(5));

	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[2].size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[2][0].type, "onBar");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[2][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[2][0].arguments[0].numberValue, com::Ticks(10));
	
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[3].size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[3][0].type, "toPosition");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[3][0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[3][0].arguments[0].numberValue, com::Ticks(100));	
}

BOOST_AUTO_TEST_CASE(parse_selector_fraction_value)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(3/2 1 / 2) {} \
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0].size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[0].numberValue, com::Ticks(1.5));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].arguments[1].numberValue, com::Ticks(0.5));
}

BOOST_AUTO_TEST_CASE(parse_declaration_fraction_value)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) { duration=1/4; } \
");
	ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0].size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectorsSet[0][0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].numberValue(), 1.0/4.0);
}

#include <conductor/declarations/Fade.h>

BOOST_AUTO_TEST_CASE(parse_fade_options_empty)
{
	using namespace conductor;

	Fade::FadeOptions options;
	Fade::parseFadeOptions(options, "");
	BOOST_CHECK_EQUAL(options.type, 0);
	BOOST_CHECK_EQUAL(options.from, 0);
	BOOST_CHECK_EQUAL(options.to, 0);
	BOOST_CHECK_EQUAL(options.offset, 0);
	BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
}

BOOST_AUTO_TEST_CASE(parse_fade_options_type)
{
	using namespace conductor;

	{
		Fade::FadeOptions options;
		Fade::parseFadeOptions(options, "type(cc69)");
		BOOST_CHECK_EQUAL(options.type, 69);
		BOOST_CHECK_EQUAL(options.from, 0);
		BOOST_CHECK_EQUAL(options.to, 0);
		BOOST_CHECK_EQUAL(options.offset, 0);
		BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
	}
	{
		Fade::FadeOptions options;
		Fade::parseFadeOptions(options, "type(Hold2)");
		BOOST_CHECK_EQUAL(options.type, 69);
		BOOST_CHECK_EQUAL(options.from, 0);
		BOOST_CHECK_EQUAL(options.to, 0);
		BOOST_CHECK_EQUAL(options.offset, 0);
		BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
	}
	{
		Fade::FadeOptions options;
		Fade::parseFadeOptions(options, "type(bend)");
		BOOST_CHECK_EQUAL(options.type, Fade::FadeOptions::TypeBend);
		BOOST_CHECK_EQUAL(options.from, 0);
		BOOST_CHECK_EQUAL(options.to, 0);
		BOOST_CHECK_EQUAL(options.offset, 0);
		BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
	}
}

BOOST_AUTO_TEST_CASE(parse_fade_options_from_and_to)
{
	using namespace conductor;

	{
		Fade::FadeOptions options;
		Fade::parseFadeOptions(options, "from(10)");
		BOOST_CHECK_EQUAL(options.type, 0);
		BOOST_CHECK_EQUAL(options.from, 10);
		BOOST_CHECK_EQUAL(options.to, 0);
		BOOST_CHECK_EQUAL(options.offset, 0);
		BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
	}
	{
		Fade::FadeOptions options;
		Fade::parseFadeOptions(options, "to(20)");
		BOOST_CHECK_EQUAL(options.type, 0);
		BOOST_CHECK_EQUAL(options.from, 0);
		BOOST_CHECK_EQUAL(options.to, 20);
		BOOST_CHECK_EQUAL(options.offset, 0);
		BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
	}
}

BOOST_AUTO_TEST_CASE(parse_fade_options_offset)
{
	using namespace conductor;

	{
		Fade::FadeOptions options;
		Fade::parseFadeOptions(options, "offset(1.5)");
		BOOST_CHECK_EQUAL(options.type, 0);
		BOOST_CHECK_EQUAL(options.from, 0);
		BOOST_CHECK_EQUAL(options.to, 0);
		BOOST_CHECK_EQUAL(options.offset, 1.5);
		BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
	}
	{
		Fade::FadeOptions options;
		Fade::parseFadeOptions(options, "offset(2)");
		BOOST_CHECK_EQUAL(options.type, 0);
		BOOST_CHECK_EQUAL(options.from, 0);
		BOOST_CHECK_EQUAL(options.to, 0);
		BOOST_CHECK_EQUAL(options.offset, 2);
		BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
	}
}

BOOST_AUTO_TEST_CASE(parse_fade_options_duration)
{
	using namespace conductor;

	{
		Fade::FadeOptions options;
		Fade::parseFadeOptions(options, "duration(1.5)");
		BOOST_CHECK_EQUAL(options.type, 0);
		BOOST_CHECK_EQUAL(options.from, 0);
		BOOST_CHECK_EQUAL(options.to, 0);
		BOOST_CHECK_EQUAL(options.offset, 0);
		BOOST_CHECK_EQUAL(options.duration, 1.5);
		BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
	}
	{
		Fade::FadeOptions options;
		Fade::parseFadeOptions(options, "duration(2)");
		BOOST_CHECK_EQUAL(options.type, 0);
		BOOST_CHECK_EQUAL(options.from, 0);
		BOOST_CHECK_EQUAL(options.to, 0);
		BOOST_CHECK_EQUAL(options.offset, 0);
		BOOST_CHECK_EQUAL(options.duration, 2);
		BOOST_CHECK_EQUAL(options.curve, com::String("lin"));
	}
}

BOOST_AUTO_TEST_CASE(parse_fade_options_curve)
{
	using namespace conductor;
	Fade::FadeOptions options;
	Fade::parseFadeOptions(options, "curve(exp)");
	BOOST_CHECK_EQUAL(options.type, 0);
	BOOST_CHECK_EQUAL(options.from, 0);
	BOOST_CHECK_EQUAL(options.to, 0);
	BOOST_CHECK_EQUAL(options.offset, 0);
	BOOST_CHECK_EQUAL(options.curve, com::String("exp"));
}

BOOST_AUTO_TEST_CASE(parse_fade_options_multiple_options)
{
	using namespace conductor;
	Fade::FadeOptions options;
	Fade::parseFadeOptions(options, "type(cc10), from(10), to(100), offset(-1.5), curve(exp)");
	BOOST_CHECK_EQUAL(options.type, 10);
	BOOST_CHECK_EQUAL(options.from, 10);
	BOOST_CHECK_EQUAL(options.to, 100);
	BOOST_CHECK_EQUAL(options.offset, -1.5);
	BOOST_CHECK_EQUAL(options.curve, com::String("exp"));
}

BOOST_AUTO_TEST_CASE(parse_fade_options_multiple_options_spaces)
{
	using namespace conductor;
	Fade::FadeOptions options;
	Fade::parseFadeOptions(options, "  type(cc10) , from(10)    , to(100),offset(1.5),    curve(exp)  ");
	BOOST_CHECK_EQUAL(options.type, 10);
	BOOST_CHECK_EQUAL(options.from, 10);
	BOOST_CHECK_EQUAL(options.to, 100);
	BOOST_CHECK_EQUAL(options.offset, 1.5);
	BOOST_CHECK_EQUAL(options.curve, com::String("exp"));
}

BOOST_AUTO_TEST_CASE(parse_fade_options_multiple_options_spaces2)
{
	using namespace conductor;
	Fade::FadeOptions options;
	Fade::parseFadeOptions(options, "  type (cc10),from( 10),to(100 ),offset(  1.5  ),curve( exp )");
	BOOST_CHECK_EQUAL(options.type, 10);
	BOOST_CHECK_EQUAL(options.from, 10);
	BOOST_CHECK_EQUAL(options.to, 100);
	BOOST_CHECK_EQUAL(options.offset, 1.5);
	BOOST_CHECK_EQUAL(options.curve, com::String("exp"));
}