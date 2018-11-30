#ifndef FM_UNITS_HPP
#define FM_UNITS_HPP

namespace fm {
	typedef unsigned long long Ticks;
	typedef int Pitch;
	typedef int Expression;
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
			PPPPP = -5,
			PPPP = -4,
			PPP = -3,
			PP = -2,
			P = -1,
			Default = 0,
			F = 1,
			FF = 2,
			FFF = 3,
			FFFF = 4,
			FFFFF = 5
		};
	}
}

#endif