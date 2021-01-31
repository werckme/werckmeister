#include "parser.h"
#include "lexer.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <fm/literals.hpp>
#include <fm/units.hpp>
#include "error.hpp"
#include <sstream>
#include "parserSymbols.h"
#include "parserPositionIt.h"
#include <sheet/DocumentUsing.h>
#include <sheet/AliasPitchDef.h>
#include <sheet/objects/Grouped.h>
#include <fm/tools.h>

BOOST_FUSION_ADAPT_STRUCT(
	sheet::DocumentUsing,
	(sheet::DocumentUsing::Usings, usings)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Argument,
	(fm::String, name)
	(fm::String, value)
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
	sheet::AliasPitchDef,
	(fm::String, alias)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Event,
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(sheet::Event::Type, type)
	(sheet::Event::Tags, tags)
	(sheet::Event::Pitches, pitches)
	(sheet::Event::Duration, duration)
	(fm::String, stringValue)
	(sheet::Event::Args, metaArgs)
	(unsigned int, sourcePositionEnd)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Grouped,
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(sheet::Event::EventGroup, eventGroup)
	(sheet::Event::Duration, duration)
)

namespace {
	enum EventFields {
		EvSourcePosBegin,
		EvSourceId,
		EvType,
		EvTags,
		EvPitches,
		EvDuration,
		EvStringValue,
		EvMetaArgs,
		EvSourcePosEnd
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	sheet::TrackConfig,
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(fm::String, name)
	(sheet::Event::Args, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::Track,
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(sheet::Track::TrackConfigs, trackConfigs)
	(sheet::Track::Voices, voices)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::DocumentConfig,
	(unsigned int, sourcePositionBegin)
	(sheet::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(fm::String, name)
	(sheet::Event::Args, args)
)

BOOST_FUSION_ADAPT_STRUCT(
	sheet::SheetDef,
	(sheet::DocumentUsing, documentUsing)
	(sheet::SheetDef::DocumentConfigs, documentConfigs)
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
			const std::string ALLOWED_TAG_ARGUMENT = "a-zA-Z0-9";
		}

		namespace {
			namespace qi = boost::spirit::qi;
			namespace ascii = boost::spirit::ascii;
			///////////////////////////////////////////////////////////////////
			template <typename Iterator>
			struct _SheetParser : qi::grammar<Iterator, SheetDef(), ascii::space_type>
			{

				void initArgumentParser() 
				{
					using qi::eps;
					using qi::attr;
					using qi::lexeme;
					using ascii::char_;
					argument_ = ((("_" >> +char_(ALLOWED_TAG_ARGUMENT) >> "=") | attr("")) >> meta_arg_value_) ;
					expression_argument_ = attr("") >> expressionSymbols_;
				}

				template<class DocumentConfigRules>
				void createDocumentConfigRules(DocumentConfigRules &documentConfig) const
				{
					using qi::lexeme;
					using ascii::char_;
					using qi::eol;
					using qi::attr;
					documentConfig %= 
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> +char_("a-zA-Z") 
						>> ":" 
						>> +argument_
						> ";";
				}

				template<class TrackConfigRules>
				void createTrackConfigRules(TrackConfigRules &trackConfig) const
				{
					using qi::lexeme;
					using ascii::char_;
					using qi::eol;
					using qi::attr;
					trackConfig %=
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> +char_("a-zA-Z") 
						>> ":" 
						>> +argument_
						>> ";";
				}

				template<class TrackRules, class VoiceRules, class TrackConfigRules>
				void createTrackRules(TrackRules &track, VoiceRules &voice, TrackConfigRules &trackConfig) const
				{
					using qi::attr;
					createTrackConfigRules(trackConfig);
					track %= "[" > current_pos_.current_pos > attr(sourceId_) > *trackConfig > +voice > "]";
				}
				void initDocumentUsingParser()
				{
					using qi::int_;
					using qi::lit;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::on_error;
					using qi::fail;
					quoted_string.name("quoted string");
					meta_arg_value_.name("argument value");
					
					quoted_string %= lexeme['"' > +(char_ - '"') > '"'];
					meta_arg_value_ = quoted_string | lexeme[+char_(ALLOWED_META_ARGUMENT)];

					using_ %= "using" > quoted_string > ";";
					usings_ %= *using_;
					documentUsing_ %= usings_;
				}



				void initSheetParser()
				{
					using qi::int_;
					using qi::_1;
					using qi::lit;
					using qi::string;
					using qi::space;
					using qi::_val;
					using qi::double_;
					using qi::lexeme;
					using ascii::char_;
					using qi::attr;
					using qi::on_error;
					using qi::fail;
					using boost::phoenix::at_c;
					using boost::phoenix::push_back;
					using boost::phoenix::insert;

					start.name("sheet");
					bar_volta_.name("bar jump mark");
					event_.name("event");
					groupedEvent_.name("eventGroup");
					track.name("track");
					voice.name("voice");
					events.name("events");
					pitch_.name("pitch");
					degree_.name("pitch");
					bar_volta_ %= lexeme['^' >> +char_("0-9")];
					degree_ %= (degreeSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave)) >> attr(false));
					pitch_ %= pitchSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave));
					alias_ %= lexeme['"' >> +(char_ - '"') >> '"'];
					pitchOrAlias_ %= pitch_ | alias_;
					event_ %= 
					( // NOTE
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Note)
						>> (("\"" >> +(lexeme[+char_(ALLOWED_TAG_ARGUMENT)]) >> "\"" >> "@") | attr(Event::Tags()))
						>> (pitchOrAlias_ | ("<" >> +pitchOrAlias_ >> ">"))
						>> (durationSymbols_ | attr(Event::NoDuration))  
						>> attr("")
						>> attr(Event::Args())
						>> current_pos_.current_pos
						>> -(
								lit("~")[at_c<EvType>(_val) = Event::TiedNote] 
								| (lit("->")[at_c<EvType>(_val) = Event::Meta][at_c<EvStringValue>(_val) = FM_STRING("addVorschlag")])
							)
					)
					|
					( // DEGREE
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Degree)
						>> (("\"" >> +(lexeme[+char_(ALLOWED_TAG_ARGUMENT)]) >> "\"" >> "@") | attr(Event::Tags()))
						>> (degree_ | ("<" >> +degree_ >> ">"))
						>> (durationSymbols_ | attr(Event::NoDuration))  
						>> attr("")
						>> attr(Event::Args())
						>> current_pos_.current_pos						
						>> -(
								lit("~")[at_c<EvType>(_val) = Event::TiedDegree] 
								| (lit("->")[at_c<EvType>(_val) = Event::Meta][at_c<EvStringValue>(_val) = FM_STRING("addVorschlag")])
							)
					)
					|
					( // REPEAT SHORTCUT (x)
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Repeat) 
						>> "&"
						>> attr(Event::Tags())
						>> attr(PitchDef())
						>> (durationSymbols_ | attr(Event::NoDuration))
						>> attr("")
						>> attr(Event::Args())
						>> current_pos_.current_pos						
						>> -(
								lit("~")[at_c<EvType>(_val) = Event::TiedRepeat] 
								| (lit("->")[at_c<EvType>(_val) = Event::Meta][at_c<EvStringValue>(_val) = FM_STRING("addVorschlag")])
							)
					)					
					|
					( // CHORD
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Chord)
						>> attr(Event::Tags())
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
						>> attr(Event::Tags())
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr("expression") 
						>> expression_argument_
					)
					| 
					( // EXPRESSION PERFORMED ONCE
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "!" 
						>> attr(Event::Meta)
						>> attr(Event::Tags())
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr("expressionPlayedOnce") 
						>> expression_argument_
					)
					| 
					( // REST
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "r" 
						>> attr(Event::Rest)
						>> attr(Event::Tags())
						>> attr(PitchDef()) 
						>> (durationSymbols_ | attr(Event::NoDuration))
						>> attr("")
						>> attr(Event::Args())
						>> current_pos_.current_pos	
					)
					| 
					( // END OF BAR
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "|"
						>> attr(Event::EOB)
						>> attr(Event::Tags())
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration)
						>> attr("")
						>> attr(Event::Args())
						>> current_pos_.current_pos
						>> -bar_volta_[ (insert(at_c<EvTags>(_val), qi::_1))]
						>> -(lit(":"))[at_c<EvStringValue>(_val) = FM_STRING("__repeat_begin_")] 
					)
					|
					( // END OF BAR WITH REPEAT
						current_pos_.current_pos
						>> attr(sourceId_)
						>> ":"
						>> attr(Event::EOB)
						>> attr(Event::Tags())
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr("__repeat_end_")
						>> attr(Event::Args())
						>> current_pos_.current_pos	
						>> "|"
						>> -bar_volta_[insert(at_c<EvTags>(_val), qi::_1)]
						>> -(lit(":"))[at_c<EvStringValue>(_val) = FM_STRING("__repeat_begin_and_end_")]
					)
					|								
					( // META COMMANDS
						current_pos_.current_pos 
						>> attr(sourceId_)
						>>  "/" 
						>> attr(Event::Meta)
						>> attr(Event::Tags())
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> +char_("a-zA-Z") >> ":" 
						>> +(argument_)
						>> "/"
					)
					;

					groupedEvent_ %= 
						attr(sourceId_)
						>> "(" > *( event_ | groupedEvent_ ) > ")"
						>> (durationSymbols_ | attr(Event::NoDuration)) 
					;

					events %= *(event_ | groupedEvent_);


					voice %= "{" > events > "}";

					createTrackRules(track, voice, trackConfig_);
					createDocumentConfigRules(documentConfig_);
				}

				_SheetParser(Iterator begin, Event::SourceId sourceId = Event::UndefinedSource) : 
					 _SheetParser::base_type(start, "sheet")
					,sourceId_(sourceId)
				{
					using qi::on_error;
					using qi::fail;
					using qi::attr;
					initDocumentUsingParser();
					initArgumentParser();
					initSheetParser();

					current_pos_.setStartPos(begin);

					documentConfig_.name("document config");
					documentUsing_.name("document config");
					track.name("track");

					start %= (documentUsing_ | attr(DocumentUsing()))
							> *documentConfig_ 
							> *track
							> boost::spirit::eoi;

					auto onError = boost::bind(&handler::errorHandler<Iterator>, _1, sourceId_);
					on_error<fail>(start, onError);

				}
			private:
				Event::SourceId sourceId_ = Event::UndefinedSource;
				qi::rule<Iterator, PitchDef(), ascii::space_type> degree_;
				qi::rule<Iterator, SheetDef(), ascii::space_type> start;
				qi::rule<Iterator, PitchDef(), ascii::space_type> pitch_;
				qi::rule<Iterator, PitchDef(), ascii::space_type> pitchOrAlias_;
				qi::rule<Iterator, Argument(), ascii::space_type> argument_;
				qi::rule<Iterator, Argument(), ascii::space_type> expression_argument_;
				qi::rule<Iterator, Track(), ascii::space_type> track;
				qi::rule<Iterator, AliasPitchDef(), ascii::space_type> alias_;
				qi::rule<Iterator, Voice(), ascii::space_type> voice;
				qi::rule<Iterator, Voice::Events(), ascii::space_type> events;
				qi::rule<Iterator, Event(), ascii::space_type> event_;
				qi::rule<Iterator, Grouped(), ascii::space_type> groupedEvent_;
				qi::rule<Iterator, DocumentConfig(), ascii::space_type> documentConfig_;
				qi::rule<Iterator, TrackConfig(), ascii::space_type> trackConfig_;
				qi::rule<Iterator, DocumentUsing(), ascii::space_type> documentUsing_;
				qi::rule<Iterator, fm::String(), ascii::space_type> quoted_string;
				qi::rule<Iterator, fm::String(), ascii::space_type> meta_arg_value_;
				qi::rule<Iterator, fm::String(), ascii::space_type> using_;
				qi::rule<Iterator, DocumentUsing::Usings, ascii::space_type> usings_;
				qi::rule<Iterator, fm::String(), ascii::space_type> bar_volta_;
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
			fm::removeComments(source.begin(), source.end());
			_parse(source, result, sourceId);
			return result;
		}
	}
}