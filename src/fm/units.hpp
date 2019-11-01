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
			Flat,
			DoubleSharp,
			DoubleFlat,
			TrippleSharp,
			TrippleFlat,			
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

			Iisis = (DoubleSharp << 8) | 1,
			IIisis = (DoubleSharp << 8) | 2,
			IIIisis = (DoubleSharp << 8) | 3,
			IVisis = (DoubleSharp << 8) | 4,
			Visis = (DoubleSharp << 8) | 5,
			VIisis = (DoubleSharp << 8) | 6,
			VIIisis = (DoubleSharp << 8) | 7,

			Ieses = (DoubleFlat << 8) | 1,
			IIeses = (DoubleFlat << 8) | 2,
			IIIeses = (DoubleFlat << 8) | 3,
			IVeses = (DoubleFlat << 8) | 4,
			Veses = (DoubleFlat << 8) | 5,
			VIeses = (DoubleFlat << 8) | 6,
			VIIeses = (DoubleFlat << 8) | 7,

			Iisisis = (TrippleSharp << 8) | 1,
			IIisisis = (TrippleSharp << 8) | 2,
			IIIisisis = (TrippleSharp << 8) | 3,
			IVisisis = (TrippleSharp << 8) | 4,
			Visisis = (TrippleSharp << 8) | 5,
			VIisisis = (TrippleSharp << 8) | 6,
			VIIisisis = (TrippleSharp << 8) | 7,

			Ieseses = (TrippleFlat << 8) | 1,
			IIeseses = (TrippleFlat << 8) | 2,
			IIIeseses = (TrippleFlat << 8) | 3,
			IVeseses = (TrippleFlat << 8) | 4,
			Veseses = (TrippleFlat << 8) | 5,
			VIeseses = (TrippleFlat << 8) | 6,
			VIIeseses = (TrippleFlat << 8) | 7,
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