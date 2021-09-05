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

BOOST_AUTO_TEST_CASE(parse_invalid_fails)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("0123456");
	sheet::compiler::ConductionSheetParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);
}