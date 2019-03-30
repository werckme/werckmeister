#include <boost/test/unit_test.hpp>
#include <iostream>
#include "compiler/error.hpp"
#include <sheet/Document.h>


BOOST_AUTO_TEST_CASE(test_source_manager_add)
{
	using namespace sheet;
	Document doc;
	auto id1 = doc.addSource(FM_STRING("a"));
	auto id2 = doc.addSource(FM_STRING("b"));
	BOOST_CHECK(id1 != id2);
	BOOST_CHECK(id1 != Event::UndefinedSource);
	BOOST_CHECK(id2 != Event::UndefinedSource);
}

BOOST_AUTO_TEST_CASE(test_source_manager_find_by_path)
{
	using namespace sheet;
	Document doc;
	auto id1 = doc.addSource(FM_STRING("a"));
	auto id2 = doc.addSource(FM_STRING("b"));
	auto id3 = doc.addSource(FM_STRING("c"));
	BOOST_CHECK(doc.findSourceId(FM_STRING("a")) == id1);
	BOOST_CHECK(doc.findSourceId(FM_STRING("b")) == id2);
	BOOST_CHECK(doc.findSourceId(FM_STRING("c")) == id3);
}
