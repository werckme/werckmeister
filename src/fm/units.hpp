#ifndef FM_UNITS_HPP
#define FM_UNITS_HPP

namespace fm {
	typedef long double Ticks;
	typedef int Pitch;
	typedef int Expression;
	typedef double BPM;
	namespace notes {
		enum Note {
			C = 0,
			CIS = 1,
			DES = 1,
			D = 2,
			DIS = 3,
			ES = 3,
			E = 4,
			FES = 4,
			F = 5,
			FIS = 6,
			GES = 6,
			G = 7,
			GIS = 8,
			AS = 8,
			A = 9,
			AIS = 10,
			BES = 10,
			B = 11,
			CES = 11,
		};
	}
	namespace degrees {
		enum Degree {
			I = 1,
			II = 2,
			III = 3,
			IV = 4,
			V = 5,
			VI = 6,
			VII = 7
		};
	}
	namespace expression {
		enum {
			PPPPP = 1,
			PPPP = 2,
			PPP = 3,
			PP = 4,
			P = 5,
			Default = 0,
			F = 6,
			FF = 7,
			FFF = 8,
			FFFF = 9,
			FFFFF = 10
		};
	}
}

#endif