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
#include "sheet/DocumentConfig.h"
#include <sheet/tools.h>

BOOST_FUSION_ADAPT_STRUCT(
	sheet::DocumentConfig,
	(sheet::DocumentConfig::Usings, usings)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Voice,
	(sheet::Voice::Events, events)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::PitchDef,
	(sheet::PitchDef::Pitch, pitch)
	(sheet::PitchDef::Octave, octave)
	(bool, forceDegree)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::AliasPitch,
	(fm::String, alias)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Event,
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(sheet::Event::Type, type)
	(sheet::Event::Pitches, pitches)
	(sheet::Event::Duration, duration)
	(fm::String, stringValue)
	(sheet::Event::Args, metaArgs)
	(unsigned int, sourcePositionEnd)
)

namespace {
	enum EventFields {
		EvSourcePosBegin,
		EvSourceId,
		EvType,
		EvPitches,
		EvDuration,
		EvStringValue,
		EvMetaArgs
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	sheet::TrackInfo,
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
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
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(fm::String, name)
	(sheet::Event::Args, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::SheetDef,
	(sheet::DocumentConfig, documentConfig)
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
			const std::string ALLOWED_META_ARGUMENT = "a-zA-Z0-9."; 
		}

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;
			///////////////////////////////////////////////////////////////////
			template <typename Iterator>
			struct _SheetParser : qi::grammar<Iterator, SheetDef(), ascii::space_type>
			{
				template<class SheetInfoRules>
				void createSheetInfoRules(SheetInfoRules &sheetInfo) const
				{
					using qi::lexeme;
					using ascii::char_;
					using qi::eol;
					using qi::attr;
					sheetInfo %= 
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> +char_("a-zA-Z") 
						>> ":" 
						>> +(lexeme[+char_(ALLOWED_META_ARGUMENT)])
						> ";";
				}

				template<class TrackInfoRules>
				void createTrackInfoRules(TrackInfoRules &trackInfo) const
				{
					using qi::lexeme;
					using ascii::char_;
					using qi::eol;
					using qi::attr;
					trackInfo %=
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> +char_("a-zA-Z") 
						>> ":" 
						>> +(lexeme[+char_(ALLOWED_META_ARGUMENT)])
						>> ";";
				}

				template<class TrackRules, class VoiceRules, class TrackInfoRules>
				void createTrackRules(TrackRules &track, VoiceRules &voice, TrackInfoRules &trackInfo) const
				{
					createTrackInfoRules(trackInfo);
					track %= "[" > *trackInfo > +voice > "]";
				}
				void initDocumentConfigParser()
				{
					using qi::int_;
					using qi::lit;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::on_error;
					using qi::fail;
					quoted_string.name("quoted string");

					quoted_string %= lexeme['"' > +(char_ - '"') > '"'];
					using_ %= "@load" > quoted_string > ";";
					usings_ %= *using_;
					documentConfig_ %= usings_;
				}

				void initSheetParser()
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

					degree_ %= (degreeSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave)) >> attr(false));

					pitch_ %= pitchSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave));
					alias_ %= lexeme['"' >> +(char_ - '"') >> '"'];
					pitchOrAlias_ %= pitch_ | alias_;
					event_ %= 
					( // NOTE
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Note)
						>> (pitchOrAlias_ | ("<" >> +pitchOrAlias_ >> ">"))
						>> (durationSymbols_ | attr(Event::NoDuration))  
						>> attr("")
						>> attr(Event::Args())
						>> current_pos_.current_pos
						>> -(
								lit("~")[at_c<EvType>(_val) = Event::TiedNote] 
								| (lit("`")[at_c<EvType>(_val) = Event::Meta][at_c<EvStringValue>(_val) = FM_STRING("vorschlag")])
							)
						
					)
					|
					( // DEGREE
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Degree) 
						>> (degree_ | ("<" >> +degree_ >> ">"))
						>> (durationSymbols_ | attr(Event::NoDuration))  
						>> attr("")
						>> attr(Event::Args())
						>> current_pos_.current_pos						
						>> -(
								lit("~")[at_c<EvType>(_val) = Event::TiedDegree] 
								| (lit("`")[at_c<EvType>(_val) = Event::Meta][at_c<EvStringValue>(_val) = FM_STRING("vorschlag")])
							)
					)
					|
					( // REPEAT SHORTCUT (x)
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Repeat) 
						>> "x"
						>> attr(PitchDef())
						>> (durationSymbols_ | attr(Event::NoDuration))
						>> attr("")
						>> attr(Event::Args())
						>> current_pos_.current_pos						
						>> -(
								lit("~")[at_c<EvType>(_val) = Event::TiedRepeat] 
								| (lit("`")[at_c<EvType>(_val) = Event::Meta][at_c<EvStringValue>(_val) = FM_STRING("vorschlag")])
							)
					)					
					|
					( // CHORD
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Chord)
						>> attr(PitchDef())
						>> attr(Event::NoDuration)					
						>> lexeme[
							char_("a-gA-G")
							> *char_(ChordDefParser::ALLOWED_CHORD_SYMBOLS_REGEX)
						]
						>> attr(Event::Args())
						>> current_pos_.current_pos	
					)
					|
					( // EXPRESSIONS
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "\\" 
						>> attr(Event::Meta) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr("expression") 
						>> expressionSymbols_
					)
					| 
					( // EXPRESSION PERFORMED ONCE
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "!" 
						>> attr(Event::Meta) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr("singleExpression") 
						>> expressionSymbols_
					)
					| 
					( // REST
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "r" 
						>> attr(Event::Rest) 
						>> attr(PitchDef()) 
						>> (durationSymbols_ | attr(Event::NoDuration)))
						>> attr("")
						>> attr(Event::Args())
						>> current_pos_.current_pos	
					| 
					( // END OF BAR
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "|"
						>> attr(Event::EOB) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration)
					)
					| 
					( // META COMMANDS
						current_pos_.current_pos 
						>> attr(sourceId_)
						>>  "/" 
						>> attr(Event::Meta) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> +char_("a-zA-Z") >> ":" >> +(lexeme[+char_(ALLOWED_META_ARGUMENT)]) >> "/"
					)
					;
					events %= *event_;


					voice %= "{" > events > "}";

					createTrackRules(track, voice, trackInfo_);
					createSheetInfoRules(sheetInfo_);
				}

				_SheetParser(Iterator begin, Event::SourceId sourceId = Event::UndefinedSource) : 
					 _SheetParser::base_type(start, "sheet")
					,sourceId_(sourceId)
				{
					using qi::on_error;
					using qi::fail;
					using qi::attr;
					initSheetParser();
					initDocumentConfigParser();
					current_pos_.setStartPos(begin);
					start %= (documentConfig_ | attr(DocumentConfig()))
							>> *sheetInfo_ 
							>> *track;

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1, sourceId_);
					on_error<fail>(start, onError);

				}
			private:
				Event::SourceId sourceId_ = Event::UndefinedSource;
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
				qi::rule<Iterator, DocumentConfig(), ascii::space_type> documentConfig_;
				qi::rule<Iterator, fm::String(), ascii::space_type> quoted_string;
				qi::rule<Iterator, fm::String(), ascii::space_type> using_;
				qi::rule<Iterator, DocumentConfig::Usings, ascii::space_type> usings_;
				CurrentPos<Iterator> current_pos_;
			};


			void _parse(const fm::String &source, SheetDef &def, Event::SourceId sourceId)
			{
				using boost::spirit::ascii::space;
				typedef _SheetParser<fm::String::const_iterator> SheetParserType;
				
				SheetParserType g(source.begin(), sourceId);
				phrase_parse(source.begin(), source.end(), g, space, def);
			}
		}


		SheetDef SheetDefParser::parse(fm::CharType const* first, fm::CharType const* last, Event::SourceId sourceId)
		{
			SheetDef result;
			fm::String source(first, last);
			removeComments(source.begin(), source.end());
			_parse(source, result, sourceId);
			return result;
		}
	}
}