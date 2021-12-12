#include <boost/test/unit_test.hpp>
#include <iostream>
#include "parser/parser.h"
#include "compiler/error.hpp"
#include <com/literals.hpp>
#include <com/units.hpp>
#include "testhelper.h"

BOOST_AUTO_TEST_CASE(parse_empty_succeeds)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("");
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
}

BOOST_AUTO_TEST_CASE(parse_invalid_fails)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("0123456");
	documentModel::compiler::ConductionSheetParser parser;
	BOOST_CHECK_THROW(parser.parse(text), documentModel::compiler::Exception);
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_emptyDeclarations)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1){} \
");
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].numberValue, com::Ticks(1));
}

BOOST_AUTO_TEST_CASE(parse_twoSlector_emptyDeclarations)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	onBeat(1) pitch(c d' x \"bd\"){} \
");
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].type, "pitch");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments.size(), size_t(4));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[0].pitch.pitch, documentModel::PitchDef::Pitch(0));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[0].pitch.octave, documentModel::PitchDef::Octave(0));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[1].pitch.pitch, documentModel::PitchDef::Pitch(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[1].pitch.octave, documentModel::PitchDef::Octave(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[2].pitch.alias, com::String("x"));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[3].pitch.alias, com::String("bd"));
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
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].value, double(10));
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
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].value, double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, documentModel::ConductionRule::Declaration::UnitAbsolute);

	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].property, "time");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].operation, documentModel::ConductionRule::Declaration::OperationSet);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].value, double(2));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].unit, documentModel::ConductionRule::Declaration::UnitPercent);
}

BOOST_AUTO_TEST_CASE(parse_noDeclarationBody_fails)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("onBeat(1)");
	documentModel::compiler::ConductionSheetParser parser;
	BOOST_CHECK_THROW(parser.parse(text), documentModel::compiler::Exception);
}

BOOST_AUTO_TEST_CASE(parse_oneSelectorWithNameArgument)
{
	using namespace com;
	using documentModel::PitchDef;
	com::String text = FM_STRING("\
	instrument(myInstrument) {} \
");
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "instrument");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].name, com::String("myInstrument"));
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
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].value, double(10));
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
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].value, double(10));
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
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].value, double(10));
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
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpSubstract);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].value, double(10));
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
	documentModel::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "onBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].numberValue, com::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, "velocity");
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, documentModel::ConductionRule::Declaration::OperationFollowUpSubstract);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].value, double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, documentModel::ConductionRule::Declaration::UnitAbsolute);
}