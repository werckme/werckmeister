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
		enum Flag {
			Normal,
			Sharp,
			Flat
		};

		enum Degree {
			// Flag << 8 | value 
			I = (Normal << 8) | 1,
			II = (Normal << 8) | 2,
			III = (Normal << 8) | 3,
			IV = (Normal << 8) | 4,
			V = (Normal << 8) | 5,
			VI = (Normal << 8) | 6,
			VII = (Normal << 8) | 7,

			Iis = (Sharp << 8) | 1,
			IIis = (Sharp << 8) | 2,
			IIIis = (Sharp << 8) | 3,
			IVis = (Sharp << 8) | 4,
			Vis = (Sharp << 8) | 5,
			VIis = (Sharp << 8) | 6,
			VIIis = (Sharp << 8) | 7,

			Ies = (Flat << 8) | 1,
			IIes = (Flat << 8) | 2,
			IIIes = (Flat << 8) | 3,
			IVes = (Flat << 8) | 4,
			Ves = (Flat << 8) | 5,
			VIes = (Flat << 8) | 6,
			VIIes = (Flat << 8) | 7,
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