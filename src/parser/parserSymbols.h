#ifndef PARSER_SYMBOLS_HPP
#define PARSER_SYMBOLS_HPP

#include <com/units.hpp>
#include <com/literals.hpp>
#include <com/common.hpp>
#include <boost/spirit/include/qi.hpp>
#include <documentModel/objects/Event.h>

namespace documentModel {
	namespace compiler {

		struct DegreeSymbols : boost::spirit::qi::symbols<char, PitchDef::Pitch >
		{
			DegreeSymbols()
			{
				add
				("I", com::degrees::I)
				("II", com::degrees::II)
				("III", com::degrees::III)
				("IV", com::degrees::IV)
				("V", com::degrees::V)
				("VI", com::degrees::VI)
				("VII", com::degrees::VII)

				("I#", com::degrees::Iis)
				("II#", com::degrees::IIis)
				("III#", com::degrees::IIIis)
				("IV#", com::degrees::IVis)
				("V#", com::degrees::Vis)
				("VI#", com::degrees::VIis)
				("VII#", com::degrees::VIIis)

				("Ib", com::degrees::Ies)
				("IIb", com::degrees::IIes)
				("IIIb", com::degrees::IIIes)
				("IVb", com::degrees::IVes)
				("Vb", com::degrees::Ves)
				("VIb", com::degrees::VIes)
				("VIIb", com::degrees::VIIes)

				("I##", com::degrees::Iisis)
				("II##", com::degrees::IIisis)
				("III##", com::degrees::IIIisis)
				("IV##", com::degrees::IVisis)
				("V##", com::degrees::Visis)
				("VI##", com::degrees::VIisis)
				("VII##", com::degrees::VIIisis)

				("Ibb", com::degrees::Ieses)
				("IIbb", com::degrees::IIeses)
				("IIIbb", com::degrees::IIIeses)
				("IVbb", com::degrees::IVeses)
				("Vbb", com::degrees::Veses)
				("VIbb", com::degrees::VIeses)
				("VIIbb", com::degrees::VIIeses)

				("I###", com::degrees::Iisisis)
				("II###", com::degrees::IIisisis)
				("III###", com::degrees::IIIisisis)
				("IV###", com::degrees::IVisisis)
				("V###", com::degrees::Visisis)
				("VI###", com::degrees::VIisisis)
				("VII###", com::degrees::VIIisisis)

				("Ibbb", com::degrees::Ieseses)
				("IIbbb", com::degrees::IIeseses)
				("IIIbbb", com::degrees::IIIeseses)
				("IVbbb", com::degrees::IVeseses)
				("Vbbb", com::degrees::Veseses)
				("VIbbb", com::degrees::VIeseses)
				("VIIbbb", com::degrees::VIIeseses)				
				;
			}
		};

		struct ExpressionSymbols : boost::spirit::qi::symbols<char, com::String>
		{
			ExpressionSymbols()
			{
				add
				("ppppp", FM_STRING("ppppp"))
				("pppp", FM_STRING("pppp"))
				("ppp", FM_STRING("ppp"))
				("pp", FM_STRING("pp"))
				("p", FM_STRING("p"))
				("f", FM_STRING("f"))
				("ff", FM_STRING("ff"))
				("fff", FM_STRING("fff"))
				("ffff", FM_STRING("ffff"))
				("fffff", FM_STRING("fffff"))
				;
			}

		};

		struct PitchSymbols : boost::spirit::qi::symbols<char, PitchDef::Pitch >
		{
			PitchSymbols()
			{
				add
				("c", com::notes::C)
				("c#", com::notes::CIS)
				("db", com::notes::DES)
				("d", com::notes::D)
				("d#", com::notes::DIS)
				("eb", com::notes::ES)
				("e", com::notes::E)
				("fb", com::notes::FES)
				("e#", com::notes::F)
				("f", com::notes::F)
				("f#", com::notes::FIS)
				("gb", com::notes::GES)
				("g", com::notes::G)
				("g#", com::notes::GIS)
				("ab", com::notes::AS)
				("a", com::notes::A)
				("a#", com::notes::AIS)
				("bb", com::notes::BES)
				("b", com::notes::B)
				("cb", com::notes::CES)
				("b#", com::notes::C)
				;
			}

		};

		struct OctaveSymbols : boost::spirit::qi::symbols<char, PitchDef::Octave >
		{
			OctaveSymbols()
			{
				add
				(",,,,,", -5)
				(",,,,", -4)
				(",,,", -3)
				(",,", -2)
				(",", -1)
				("'", 1)
				("''", 2)
				("'''", 3)
				("''''", 4)
				("'''''", 5)
				;
			}

		};

		struct DurationSymbols : boost::spirit::qi::symbols<char, com::Ticks>
		{
			DurationSymbols()
			{
				using namespace com;
				add
				("1", 1.0_N1)
				("2", 1.0_N2)
				("4", 1.0_N4)
				("8", 1.0_N8)
				("16", 1.0_N16)
				("32", 1.0_N32)
				("64", 1.0_N64)
				("128", 1.0_N128)
				("1.", 1.0_N1p)
				("2.", 1.0_N2p)
				("4.", 1.0_N4p)
				("8.", 1.0_N8p)
				("16.", 1.0_N16p)
				("32.", 1.0_N32p)
				("64.", 1.0_N64p)
				("128.", 1.0_N128p)
				("1..", 1.0_N1pp)
				("2..", 1.0_N2pp)
				("4..", 1.0_N4pp)
				("8..", 1.0_N8pp)
				("16..", 1.0_N16pp)
				("32..", 1.0_N32pp)
				("64..", 1.0_N64pp)
				("128..", 1.0_N128pp)
				("1...", 1.0_N1ppp)
				("2...", 1.0_N2ppp)
				("4...", 1.0_N4ppp)
				("8...", 1.0_N8ppp)
				("16...", 1.0_N16ppp)
				("32...", 1.0_N32ppp)
				("64...", 1.0_N64ppp)
				("128...", 1.0_N128ppp)
				("1....", 1.0_N1pppp)
				("2....", 1.0_N2pppp)
				("4....", 1.0_N4pppp)
				("8....", 1.0_N8pppp)
				("16....", 1.0_N16pppp)
				("32....", 1.0_N32pppp)
				("64....", 1.0_N64pppp)
				("128....", 1.0_N128pppp)												
				("1t", 1.0_N1t)
				("2t", 1.0_N2t)
				("4t", 1.0_N4t)
				("8t", 1.0_N8t)
				("16t", 1.0_N16t)
				("32t", 1.0_N32t)
				("64t", 1.0_N64t)
				("128t", 1.0_N128t)
				("1n5", 1.0_N1n5)
				("2n5", 1.0_N2n5)
				("4n5", 1.0_N4n5)
				("8n5", 1.0_N8n5)
				("16n5", 1.0_N16n5)
				("32n5", 1.0_N32n5)
				("64n5", 1.0_N64n5)
				("128n5", 1.0_N128n5)
				("1n7", 1.0_N1n7)
				("2n7", 1.0_N2n7)
				("4n7", 1.0_N4n7)
				("8n7", 1.0_N8n7)
				("16n7", 1.0_N16n7)
				("32n7", 1.0_N32n7)
				("64n7", 1.0_N64n7)
				("128n7", 1.0_N128n7)
				("1n9", 1.0_N1n9)
				("2n9", 1.0_N2n9)
				("4n9", 1.0_N4n9)
				("8n9", 1.0_N8n9)
				("16n9", 1.0_N16n9)
				("32n9", 1.0_N32n9)
				("64n9", 1.0_N64n9)
				("128n9", 1.0_N128n9)
				;
			}

		};
	}
}

#endif