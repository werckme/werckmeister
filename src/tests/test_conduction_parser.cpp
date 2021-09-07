#include <boost/test/unit_test.hpp>
#include <iostream>
#include "parser/parser.h"
#include "compiler/error.hpp"
#include <fm/literals.hpp>
#include <fm/units.hpp>
#include "testhelper.h"

BOOST_AUTO_TEST_CASE(parse_empty_succeeds)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("");
	sheet::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
}

// BOOST_AUTO_TEST_CASE(parse_invalid_fails)
// {
// 	using namespace fm;
// 	using sheet::PitchDef;
// 	fm::String text = FM_STRING("0123456");
// 	sheet::compiler::ConductionSheetParser parser;
// 	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);
// }

BOOST_AUTO_TEST_CASE(parse_oneSelector_emptyDeclarations)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
	atBeat(1){} \
");
	sheet::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "atBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].tickValue, fm::Ticks(1));
}

BOOST_AUTO_TEST_CASE(parse_twoSlector_emptyDeclarations)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
	atBeat(1) pitch(c d' x \"bd\"){} \
");
	sheet::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "atBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].tickValue, fm::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].type, "pitch");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments.size(), size_t(4));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[0].pitch.pitch, sheet::PitchDef::Pitch(0));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[0].pitch.octave, sheet::PitchDef::Octave(0));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[1].pitch.pitch, sheet::PitchDef::Pitch(2));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[1].pitch.octave, sheet::PitchDef::Octave(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[2].pitch.alias, fm::String("x"));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[1].arguments[3].pitch.alias, fm::String("bd"));
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_oneDeclaration)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
	atBeat(1) {\
		velocity+=10;\
	}\
");
	sheet::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "atBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].tickValue, fm::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, sheet::ConductionRule::Declaration::PropertyVelocity);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, sheet::ConductionRule::Declaration::OperationAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].value, double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, sheet::ConductionRule::Declaration::UnitAbsolute);
}

BOOST_AUTO_TEST_CASE(parse_oneSelector_twoDeclarations)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
	atBeat(1) {\
		velocity+=10;\
		time=2%;\
	}\
");
	sheet::compiler::ConductionSheetParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK_EQUAL(defs.rules.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].type, "atBeat");
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments.size(), size_t(1));
	BOOST_CHECK_EQUAL(defs.rules[0].selectors[0].arguments[0].tickValue, fm::Ticks(1));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations.size(), size_t(2));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].property, sheet::ConductionRule::Declaration::PropertyVelocity);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].operation, sheet::ConductionRule::Declaration::OperationAdd);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].value, double(10));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[0].unit, sheet::ConductionRule::Declaration::UnitAbsolute);

	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].property, sheet::ConductionRule::Declaration::PropertyTime);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].operation, sheet::ConductionRule::Declaration::OperationSet);
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].value, double(2));
	BOOST_CHECK_EQUAL(defs.rules[0].declarations[1].unit, sheet::ConductionRule::Declaration::UnitPercent);
}

BOOST_AUTO_TEST_CASE(parse_noDeclarationBody_fails)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("atBeat(1)");
	sheet::compiler::ConductionSheetParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);
}