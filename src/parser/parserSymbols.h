#ifndef PARSER_SYMBOLS_HPP
#define PARSER_SYMBOLS_HPP

#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <fm/common.hpp>
#include <boost/spirit/include/qi.hpp>
#include <sheet/objects/Event.h>

namespace sheet {
	namespace compiler {

		struct DegreeSymbols : boost::spirit::qi::symbols<char, PitchDef::Pitch >
		{
			DegreeSymbols()
			{
				add
				("I", fm::degrees::I)
				("II", fm::degrees::II)
				("III", fm::degrees::III)
				("IV", fm::degrees::IV)
				("V", fm::degrees::V)
				("VI", fm::degrees::VI)
				("VII", fm::degrees::VII)

				("I#", fm::degrees::Iis)
				("II#", fm::degrees::IIis)
				("III#", fm::degrees::IIIis)
				("IV#", fm::degrees::IVis)
				("V#", fm::degrees::Vis)
				("VI#", fm::degrees::VIis)
				("VII#", fm::degrees::VIIis)

				("Ib", fm::degrees::Ies)
				("IIb", fm::degrees::IIes)
				("IIIb", fm::degrees::IIIes)
				("IVb", fm::degrees::IVes)
				("Vb", fm::degrees::Ves)
				("VIb", fm::degrees::VIes)
				("VIIb", fm::degrees::VIIes)

				("I##", fm::degrees::Iisis)
				("II##", fm::degrees::IIisis)
				("III##", fm::degrees::IIIisis)
				("IV##", fm::degrees::IVisis)
				("V##", fm::degrees::Visis)
				("VI##", fm::degrees::VIisis)
				("VII##", fm::degrees::VIIisis)

				("Ibb", fm::degrees::Ieses)
				("IIbb", fm::degrees::IIeses)
				("IIIbb", fm::degrees::IIIeses)
				("IVbb", fm::degrees::IVeses)
				("Vbb", fm::degrees::Veses)
				("VIbb", fm::degrees::VIeses)
				("VIIbb", fm::degrees::VIIeses)

				("I###", fm::degrees::Iisisis)
				("II###", fm::degrees::IIisisis)
				("III###", fm::degrees::IIIisisis)
				("IV###", fm::degrees::IVisisis)
				("V###", fm::degrees::Visisis)
				("VI###", fm::degrees::VIisisis)
				("VII###", fm::degrees::VIIisisis)

				("Ibbb", fm::degrees::Ieseses)
				("IIbbb", fm::degrees::IIeseses)
				("IIIbbb", fm::degrees::IIIeseses)
				("IVbbb", fm::degrees::IVeseses)
				("Vbbb", fm::degrees::Veseses)
				("VIbbb", fm::degrees::VIeseses)
				("VIIbbb", fm::degrees::VIIeseses)				
				;
			}
		};

		struct ExpressionSymbols : boost::spirit::qi::symbols<char, fm::String>
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
				("c", fm::notes::C)
				("c#", fm::notes::CIS)
				("db", fm::notes::DES)
				("d", fm::notes::D)
				("d#", fm::notes::DIS)
				("eb", fm::notes::ES)
				("e", fm::notes::E)
				("fb", fm::notes::FES)
				("e#", fm::notes::F)
				("f", fm::notes::F)
				("f#", fm::notes::FIS)
				("gb", fm::notes::GES)
				("g", fm::notes::G)
				("g#", fm::notes::GIS)
				("ab", fm::notes::AS)
				("a", fm::notes::A)
				("a#", fm::notes::AIS)
				("bb", fm::notes::BES)
				("b", fm::notes::B)
				("cb", fm::notes::CES)
				("b#", fm::notes::C)
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

		struct DurationSymbols : boost::spirit::qi::symbols<char, fm::Ticks>
		{
			DurationSymbols()
			{
				using namespace fm;
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