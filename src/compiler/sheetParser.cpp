#include "parser.h"
#include "lexer.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <fm/literals.hpp>
#include <fm/units.hpp>
#include "error.hpp"
#include <sstream>
#include "parserSymbols.h"
#include "parserPositionIt.h"

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Voice,
	(sheet::Voice::Events, events)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::PitchDef,
	(sheet::PitchDef::Pitch, pitch)
	(sheet::PitchDef::Octave, octave)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::AliasPitch,
	(fm::String, alias)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Event,
	(unsigned int, sourcePositionBegin)
	(sheet::Event::Type, type)
	(sheet::Event::Pitches, pitches)
	(sheet::Event::Duration, duration)
	(fm::String, stringValue)
	(sheet::Event::Args, metaArgs)
)

namespace {
	enum EventFields {
		EvSourcePosBegin,
		EvType,
		EvPitches,
		EvDuration,
		EvStringValue,
		EvMetaArgs
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	sheet::TrackInfo,
	(fm::String, name)
	(sheet::Event::Args, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Track,
	(sheet::Track::TrackInfos, trackInfos)
	(sheet::Track::Voices, voices)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::SheetInfo,
	(fm::String, name)
	(sheet::Event::Args, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::SheetDef,
	(sheet::SheetDef::SheetInfos, sheetInfos)
	(sheet::SheetDef::Tracks, tracks)
)


namespace sheet {
	namespace compiler {

		namespace {
			DegreeSymbols degreeSymbols_;
			PitchSymbols pitchSymbols_;
			OctaveSymbols octaveSymbols_;
			DurationSymbols durationSymbols_;
			ExpressionSymbols expressionSymbols_;
		}

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;
		
			struct ASheetParser
			{
				virtual ~ASheetParser() = default;
				ASheetParser()
				{
				}

				template<class SheetInfoRules>
				void createSheetInfoRules(SheetInfoRules &sheetInfo) const
				{
					using qi::lexeme;
					using ascii::char_;
					using qi::eol;
					sheetInfo %= 
						+char_("a-zA-Z") 
						>> ":" 
						>> +(lexeme[+char_("a-zA-Z0-9")])
						>> ";";
				}

				template<class TrackInfoRules>
				void createTrackInfoRules(TrackInfoRules &trackInfo) const
				{
					using qi::lexeme;
					using ascii::char_;
					using qi::eol;
					trackInfo %= 
						+char_("a-zA-Z") 
						>> ":" 
						>> +(lexeme[+char_("a-zA-Z0-9")])
						>> ";";
				}

				template<class TrackRules, class VoiceRules, class TrackInfoRules>
				void createTrackRules(TrackRules &track, VoiceRules &voice, TrackInfoRules &trackInfo) const
				{
					createTrackInfoRules(trackInfo);
					track %= "[" > *trackInfo > +voice > "]";
				}
			};
			///////////////////////////////////////////////////////////////////
			template <typename Iterator>
			struct _SheetParser : public ASheetParser,
							      qi::grammar<Iterator, SheetDef(), ascii::space_type>
			{

				_SheetParser() : _SheetParser::base_type(start, "sheet")
				{
					using qi::int_;
					using qi::lit;
					using qi::_val;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::attr;
					using qi::on_error;
					using qi::fail;
					using boost::phoenix::at_c;

					start.name("sheet");

					event_.name("event");
					track.name("track");
					voice.name("voice");
					events.name("events");
					pitch_.name("pitch");
					degree_.name("pitch");

					degree_ %= degreeSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave));

					pitch_ %= pitchSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave));
					alias_ %= lexeme['"' >> +(char_ - '"') >> '"'];
					pitchOrAlias_ %= pitch_ | alias_;
					event_ %= 
					(
						current_pos_.current_pos >> attr(Event::Note)
						>> (pitchOrAlias_ | ("<" >> +pitchOrAlias_ >> ">"))
						>> (durationSymbols_ | attr(Event::NoDuration))  
						>> -(
								lit("~")[at_c<EvType>(_val) = Event::TiedNote] 
								| (lit("`")[at_c<EvType>(_val) = Event::Meta][at_c<EvStringValue>(_val) = FM_STRING("vorschlag")])
							)
					)
					|
					(
						current_pos_.current_pos >>  attr(Event::Degree) 
						>> (degree_ | ("<" >> +degree_ >> ">"))
						>> (durationSymbols_ | attr(Event::NoDuration))  
						>> -(
								lit("~")[at_c<EvType>(_val) = Event::TiedDegree] 
								| (lit("`")[at_c<EvType>(_val) = Event::Meta][at_c<EvStringValue>(_val) = FM_STRING("vorschlag")])
							)
					)
					|
					(
						current_pos_.current_pos >> attr(Event::Chord)
						>> attr(PitchDef())
						>> attr(Event::NoDuration) 
						>> lexeme[
							char_("a-gA-G")
							> *char_(ChordDefParser::ALLOWED_CHORD_SYMBOLS_REGEX)
						]
					)
					|
					(
						current_pos_.current_pos >> "\\" 
						>> attr(Event::Meta) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr("expression") 
						>> expressionSymbols_
					)
					| 
					(
						current_pos_.current_pos >> "!" 
						>> attr(Event::Meta) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr("singleExpression") 
						>> expressionSymbols_
					)
					| 
					(
						current_pos_.current_pos >> "r" 
						>> attr(Event::Rest) 
						>> attr(PitchDef()) 
						>> (durationSymbols_ | attr(Event::NoDuration)))
					| 
					(
						current_pos_.current_pos >> "|"
						>> attr(Event::EOB) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration)
					)
					| 
					(
						current_pos_.current_pos >>  "/" 
						>> attr(Event::Meta) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> +char_("a-zA-Z") >> ":" >> +(lexeme[+char_("a-zA-Z0-9")]) >> "/"
					)
					;
					events %= *event_;


					voice %= "{" >> events >> "}";

					createTrackRules(track, voice, trackInfo_);
					createSheetInfoRules(sheetInfo_);
					start %= current_pos_.save_start_pos >> *sheetInfo_ >> *track;

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1);
					on_error<fail>(start, onError);

				}
				qi::rule<Iterator, PitchDef(), ascii::space_type> degree_;
				qi::rule<Iterator, SheetDef(), ascii::space_type> start;
				qi::rule<Iterator, PitchDef(), ascii::space_type> pitch_;
				qi::rule<Iterator, PitchDef(), ascii::space_type> pitchOrAlias_;
				qi::rule<Iterator, Track(), ascii::space_type> track;
				qi::rule<Iterator, AliasPitch(), ascii::space_type> alias_;
				qi::rule<Iterator, Voice(), ascii::space_type> voice;
				qi::rule<Iterator, Voice::Events(), ascii::space_type> events;
				qi::rule<Iterator, Event(), ascii::space_type> event_;
				qi::rule<Iterator, SheetInfo(), ascii::space_type> sheetInfo_;
				qi::rule<Iterator, TrackInfo(), ascii::space_type> trackInfo_;
				CurrentPos<Iterator> current_pos_;
			};


			void _parse(const fm::String &defStr, SheetDef &def)
			{
				using boost::spirit::ascii::space;
				typedef _SheetParser<fm::String::const_iterator> SheetParserType;
				
				SheetParserType g;
				phrase_parse(defStr.begin(), defStr.end(), g, space, def);
			}
		}


		SheetDef SheetDefParser::parse(fm::CharType const* first, fm::CharType const* last)
		{

			SheetDef result;
			SheetDefTokenizer<LexerType> sheetDefTok;
			LexerType::iterator_type iter = sheetDefTok.begin(first, last);
			LexerType::iterator_type end = sheetDefTok.end();
			boost::spirit::lex::tokenize(first, last, sheetDefTok);
			_parse(sheetDefTok.tracks.str(), result);
			return result;
		}
	}
}