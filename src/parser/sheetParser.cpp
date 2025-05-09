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

#include <com/literals.hpp>
#include <com/units.hpp>
#include "error.hpp"
#include <sstream>
#include "parserSymbols.h"
#include "parserPositionIt.h"
#include <documentModel/AliasPitchDef.h>
#include <documentModel/objects/Grouped.h>
#include <documentModel/objects/Phrase.h>
#include <com/tools.h>
#include "valueParser.h"


BOOST_FUSION_ADAPT_STRUCT(
	documentModel::Argument,
	(com::String, name)
	(com::String, value))

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::Voice,
	(documentModel::Voice::Events, events))

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::Event,
	(unsigned int, sourcePositionBegin)
	(documentModel::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(documentModel::Event::Type, type)
	(documentModel::Event::Type, subType)
	(documentModel::Event::Tags, tags)
	(documentModel::Event::Pitches, pitches)
	(documentModel::Event::Duration, duration)
	(int, numberOfRepeats)
	(com::String, stringValue)
	(documentModel::Event::Args, metaArgs)
	(unsigned int, sourcePositionEnd))
	

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::Grouped,
	(documentModel::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(documentModel::Event::Tags, tags)
	(documentModel::Event::EventGroup, eventGroup)
	(documentModel::Event::Duration, duration))

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::Phrase,
	(unsigned int, sourcePositionBegin)
	(documentModel::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(documentModel::Event::Type, type)
	(documentModel::Event::Tags, tags)
	(bool, isPhraseStrechedPlayback)
	(com::String, stringValue)
	(documentModel::Event::Duration, duration)
	(unsigned int, sourcePositionEnd))

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::TrackConfig,
	(unsigned int, sourcePositionBegin)
	(documentModel::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(com::String, name)
	(documentModel::Event::Args, args))

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::Track,
	(unsigned int, sourcePositionBegin)
	(documentModel::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(documentModel::Track::TrackConfigs, trackConfigs)
	(documentModel::Track::Voices, voices))

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::DocumentConfig,
	(unsigned int, sourcePositionBegin)
	(documentModel::ASheetObjectWithSourceInfo::SourceId, sourceId)
	(documentModel::DocumentConfig::Type, type)
	(com::String, name)
	(documentModel::Event::Args, args)
	(documentModel::DocumentConfig::Events, events)
)

BOOST_FUSION_ADAPT_STRUCT(
	documentModel::SheetDef,
	(documentModel::SheetDef::Usings, documentUsings)
	(documentModel::SheetDef::DocumentConfigs, documentConfigs)
	(documentModel::SheetDef::Tracks, tracks)
)

namespace
{
	enum EventFields
	{
		EvSourcePosBegin,
		EvSourceId,
		EvType,
		EvSubType,
		EvTags,
		EvPitches,
		EvDuration,
		EvNumberOfEvents,
		EvStringValue,
		EvMetaArgs,
		EvSourcePosEnd
	};
	enum SheetDefFields
	{
		SdDocumentUsings,
		SdDocumentConfigs,
		SdTracks
	};
	enum DocumentConfigFields
	{
		DcsourcePositionBegin,
		DcSourceId,
		DcType,
		DcName,
		DcArgs,
		DcEvents
	};
	enum PhraseConfigFields
	{
		PhSourceBegin,
		PhSourceId,
		PhEvType,
		PhEvTag,
		PhIsStrechedPlayback,
		PhStringValue,
		PhDuration,
		PhSourceEnd
	};
	const int DefaultNumberOfBarRepeats = 0;
}

namespace parser
{

	namespace
	{
		DegreeSymbols degreeSymbols_;
		PitchSymbols pitchSymbols_;
		OctaveSymbols octaveSymbols_;
		DurationSymbols durationSymbols_;
		ExpressionSymbols expressionSymbols_;
		const std::string ALLOWED_PHRASE_NAME = "a-zA-Z0-9_";
		const std::string ALLOWED_ARGUMENT_NAME_SYMBOLS = "a-zA-Z0-9";
		const std::string ALLOWED_EVENT_TAG_ARGUMENT = "a-zA-Z0-9,`':;?.!()[]<>+*/=$%&@\\^_|~-";
	}

	namespace
	{
		using namespace documentModel;
		namespace qi = boost::spirit::qi;
		namespace ascii = boost::spirit::ascii;
		///////////////////////////////////////////////////////////////////
		template <typename Iterator>
		struct _SheetParser : ValueParser, qi::grammar<Iterator, SheetDef(), ascii::space_type>
		{

			void initArgumentParser()
			{
				using ascii::char_;
				using qi::attr;
				using qi::eps;
				using qi::lexeme;
				argument_ = ((("_" >> +char_(ALLOWED_ARGUMENT_NAME_SYMBOLS) >> "=") | attr("")) >> meta_arg_value_);
				expression_argument_ = attr("") >> expressionSymbols_;
			}

			template <class DocumentConfigRules, class EventsRule>
			void createDocumentConfigRules(DocumentConfigRules &documentConfig, EventsRule &events) const
			{
				using ascii::char_;
				using qi::attr;
				using qi::eol;
				using qi::lexeme;
				using boost::spirit::eps;
				using boost::phoenix::at_c;
				using qi::_val;
				documentConfig %= 
				(
					current_pos_.current_pos 
					>> attr(sourceId_) 
					>> attr(DocumentConfig::TypeConfigDef)
					>> +char_("a-zA-Z")
					>> ":" >> +argument_ >> attr(Event())
					>> ";"
				)
				|
				(
					current_pos_.current_pos 
					>> attr(sourceId_) 
					>> attr(DocumentConfig::TypePhraseDef)
					>> eps[at_c<DcName>(_val) = FM_STRING("")] // reset name, will be appended from name above instead
					>> +char_(ALLOWED_PHRASE_NAME)
					>> "=" >> attr(Argument()) >> events
					>> ";"
				)
				;
			}

			template <class TrackConfigRules>
			void createTrackConfigRules(TrackConfigRules &trackConfig) const
			{
				using ascii::char_;
				using qi::attr;
				using qi::eol;
				using qi::lexeme;
				trackConfig %=
					current_pos_.current_pos >> attr(sourceId_) >> +char_("a-zA-Z") >> ":" >> +argument_ >> ";";
			}

			template <class TrackRules, class VoiceRules, class TrackConfigRules>
			void createTrackRules(TrackRules &track, VoiceRules &voice, TrackConfigRules &trackConfig) const
			{
				using qi::attr;
				createTrackConfigRules(trackConfig);
				track %= "[" > current_pos_.current_pos > attr(sourceId_) > *trackConfig > +voice > "]";
			}
			void initDocumentUsingParser()
			{
				using ascii::char_;
				using qi::double_;
				using qi::fail;
				using qi::int_;
				using qi::lexeme;
				using qi::lit;
				using qi::on_error;
				
				meta_arg_value_.name("argument value");
				meta_arg_value_ = quoted_string | lexeme[+char_(ALLOWED_META_ARGUMENT)];
				using_ %= "using" > quoted_string > ";";
			}

			void initSheetParser()
			{
				using ascii::char_;
				using boost::phoenix::at_c;
				using boost::phoenix::insert;
				using boost::phoenix::push_back;
				using qi::_1;
				using qi::_a;
				using qi::_val;
				using qi::attr;
				using qi::double_;
				using qi::fail;
				using qi::int_;
				using qi::lexeme;
				using qi::lit;
				using qi::on_error;
				using qi::space;
				using qi::string;
				using boost::spirit::eps;
			
				start.name("documentModel");
				bar_volta_.name("bar jump mark");
				event_.name("event");
				groupedEvent_.name("eventGroup");
				track.name("track");
				voice.name("voice");
				events.name("events");
				pitch_.name("pitch");
				degree_.name("pitch");
				bar_volta_ %= lexeme['^' >> +char_("0-9")];
				bar_repeat_number_ %= lit("(") >> "x" >> int_ >> ")";
				degree_ %= -lit("!")[at_c<PitchDefFieldForceDegree>(_val) = true] >> (degreeSymbols_ >>  (octaveSymbols_ | attr(PitchDef::DefaultOctave))  );
				pitch_ %= pitchSymbols_ >> (octaveSymbols_ | attr(PitchDef::DefaultOctave));
				alias_ %= lexeme['"' >> +(char_ - '"') >> '"'];
				pitchOrAlias_ %= pitch_ | alias_ | extendedPitch_;
				event_ %=
					( // NOTE
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Note)
						>> attr(Event::Unknown)
						>> (("\"" >> +(lexeme[+char_(ALLOWED_EVENT_TAG_ARGUMENT)]) >> "\"" >> "@") | attr(Event::Tags())) 
						>> (pitchOrAlias_ | ("<" >> +pitchOrAlias_ >> ">")) >> (durationSymbols_ | attr(Event::NoDuration))
						>> attr(DefaultNumberOfBarRepeats)
						>> attr("") 
						>> attr(Event::Args()) >> current_pos_.current_pos 
						>> -(
							   lit("~")[at_c<EvType>(_val) = Event::TiedNote] 
							| (lit("->")[at_c<EvType>(_val) = Event::Meta]
										[at_c<EvSubType>(_val) = Event::Note]
										[at_c<EvStringValue>(_val) = FM_STRING("addVorschlag")])
						)
					) 
					|
					( // DEGREE
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> attr(Event::Degree)
						>> attr(Event::Unknown)
						>> (("\"" >> +(lexeme[+char_(ALLOWED_EVENT_TAG_ARGUMENT)]) >> "\"" >> "@") | attr(Event::Tags())) 
						>> (degree_ | ("<" >> +degree_ >> ">")) 
						>> (durationSymbols_ | attr(Event::NoDuration))
						>> attr(DefaultNumberOfBarRepeats)
						>> attr("") 
						>> attr(Event::Args()) 
						>> current_pos_.current_pos >> -(
							lit("~")[at_c<EvType>(_val) = Event::TiedDegree] 
							| (lit("->")[at_c<EvType>(_val) = Event::Meta]
										[at_c<EvSubType>(_val) = Event::Degree]
							            [at_c<EvStringValue>(_val) = FM_STRING("addVorschlag")])
						)
					) 
					|
					( // REPEAT SHORTCUT (&)
						current_pos_.current_pos
						>> attr(sourceId_)
						>> attr(Event::Repeat)
						>> attr(Event::Unknown)
						>> (("\"" >> +(lexeme[+char_(ALLOWED_EVENT_TAG_ARGUMENT)]) >> "\"" >> "@") | attr(Event::Tags())) 
						>> "&" 
						>> attr(PitchDef()) >> (durationSymbols_ | attr(Event::NoDuration)) 
						>> attr(DefaultNumberOfBarRepeats)
						>> attr("") >> attr(Event::Args()) >> current_pos_.current_pos 
						>> -(
							lit("~")[at_c<EvType>(_val) = Event::TiedRepeat] | (lit("->")[at_c<EvType>(_val) = Event::Meta][at_c<EvStringValue>(_val) = FM_STRING("addVorschlag")])
						)
					) 
					|
					( // CHORD
						current_pos_.current_pos 
						>> attr(sourceId_) 
						>> attr(Event::Chord) 
						>> attr(Event::Unknown)
						>> (("\"" >> +(lexeme[+char_(ALLOWED_EVENT_TAG_ARGUMENT)]) >> "\"" >> "@") | attr(Event::Tags())) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr(DefaultNumberOfBarRepeats)
						>> lexeme[char_("a-gA-G") > *char_(ChordDefParser::ALLOWED_CHORD_SYMBOLS_REGEX)] 
						>> attr(Event::Args()) >> current_pos_.current_pos
					) 
					|
					( // EXPRESSIONS
						current_pos_.current_pos 
						>> attr(sourceId_) 
						>> "\\" 
						>> attr(Event::Meta) 
						>> attr(Event::Unknown)
						>> attr(Event::Tags()) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr(DefaultNumberOfBarRepeats)
						>> attr("expression") 
						>> expression_argument_
					) 
					|
					( // EXPRESSION PERFORMED ONCE
						current_pos_.current_pos 
						>> attr(sourceId_) 
						>> "!" 
						>> attr(Event::Meta) 
						>> attr(Event::Unknown)
						>> attr(Event::Tags()) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr(DefaultNumberOfBarRepeats)
						>> attr("expressionPlayedOnce") 
						>> expression_argument_
					) 
					|
					( // REST
						current_pos_.current_pos 
						>> attr(sourceId_)
						>> "r" 
						>> attr(Event::Rest) 
						>> attr(Event::Unknown)
						>> attr(Event::Tags()) 
						>> attr(PitchDef()) 
						>> (durationSymbols_ | attr(Event::NoDuration)) 
						>> attr(DefaultNumberOfBarRepeats)
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
						>> attr(Event::Unknown)
						>> attr(Event::Tags()) 
						>> attr(PitchDef()) >> attr(Event::NoDuration) 
						>> attr(DefaultNumberOfBarRepeats)
						>> attr("") 
						>> attr(Event::Args()) >> current_pos_.current_pos >> -bar_volta_[(insert(at_c<EvTags>(_val), qi::_1))] 
						>> -(lit(":"))[at_c<EvStringValue>(_val) = FM_STRING("__repeat_begin_")]
					) 
					|
					( // END OF BAR WITH REPEAT
						current_pos_.current_pos 
						>> attr(sourceId_) 
						>> ":" 
						>> attr(Event::EOB) 
						>> attr(Event::Unknown)
						>> attr(Event::Tags()) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration)
						>> -bar_repeat_number_
						>> attr("__repeat_end_") 
						>> attr(Event::Args()) >> current_pos_.current_pos >> "|" >> -bar_volta_[insert(at_c<EvTags>(_val), qi::_1)] 
						>> -(lit(":"))[at_c<EvStringValue>(_val) = FM_STRING("__repeat_begin_and_end_")]
					)
					|
					( // META COMMANDS
						current_pos_.current_pos 
						>> attr(sourceId_) 
						>> "/" 
						>> attr(Event::Meta) 
						>> attr(Event::Unknown)
						>> attr(Event::Tags()) 
						>> attr(PitchDef()) 
						>> attr(Event::NoDuration) 
						>> attr(DefaultNumberOfBarRepeats)
						>> +char_("a-zA-Z") 
						>> ":"
						>> +(argument_)
						>> current_pos_.current_pos
						>> "/"
					);

				phrase_ %= 
						current_pos_.current_pos 
						>> attr(sourceId_) 
						>> attr(Event::Phrase) 
						>> (("\"" >> +(lexeme[+char_(ALLOWED_EVENT_TAG_ARGUMENT)]) >> "\"" >> "@") | attr(Event::Tags()))
						>> (lit(">>") >> attr(false) | lit(">") >> attr(true))
						>> lexeme["\"" >> +char_(ALLOWED_PHRASE_NAME) >> "\""]
						>> (durationSymbols_ | attr(Event::NoDuration))
						>> current_pos_.current_pos
						>> -(
							lit("~")[at_c<EvType>(_val) = Event::TiedPhrase]
						)
				;

				groupedEvent_ %=
					attr(sourceId_)
					>> (("\"" >> +(lexeme[+char_(ALLOWED_EVENT_TAG_ARGUMENT)]) >> "\"" >> "@") | attr(Event::Tags()))
					>> ("(" > *(event_ | phrase_ | groupedEvent_) > ")")
					>> (durationSymbols_ | attr(Event::NoDuration))
				;

				events %= *(event_ | groupedEvent_ | phrase_);

				voice %= "{" > events > "}";

				createTrackRules(track, voice, trackConfig_);
				createDocumentConfigRules(documentConfig_, events);
			}

			_SheetParser(Iterator begin, Event::SourceId sourceId = Event::UndefinedSource) : ValueParser(),
																							  _SheetParser::base_type(start, "documentModel"),
																							  sourceId_(sourceId)
			{
				using qi::attr;
				using qi::fail;
				using qi::on_error;
				using boost::phoenix::at_c;
				using boost::phoenix::push_back;
				using qi::_val;
				using boost::spirit::qi::omit;
				using namespace boost::placeholders;
				initDocumentUsingParser();
				initArgumentParser();
				initSheetParser();

				current_pos_.setStartPos(begin);

				documentConfig_.name("document config");
				documentUsing_.name("document config");
				track.name("track");

				start %= attr(SheetDef::Usings())
					> attr(SheetDef::DocumentConfigs())
					> attr(SheetDef::Tracks())
					> -(*(
							  using_[(push_back(at_c<SdDocumentUsings>(_val), qi::_1))]
							| documentConfig_[(push_back(at_c<SdDocumentConfigs>(_val), qi::_1))]
							| track[(push_back(at_c<SdTracks>(_val), qi::_1))]
						))
					> boost::spirit::eoi;

				auto onError = boost::bind(&compiler::handler::errorHandler<Iterator>, _1, sourceId_);
				on_error<fail>(start, onError);
			}

		protected:
			Event::SourceId sourceId_ = Event::UndefinedSource;
			qi::rule<Iterator, PitchDef(), ascii::space_type> degree_;
			qi::rule<Iterator, SheetDef(), ascii::space_type> start;
			qi::rule<Iterator, Argument(), ascii::space_type> argument_;
			qi::rule<Iterator, Argument(), ascii::space_type> expression_argument_;
			qi::rule<Iterator, Track(), ascii::space_type> track;
			qi::rule<Iterator, AliasPitchDef(), ascii::space_type> alias_;
			qi::rule<Iterator, Voice(), ascii::space_type> voice;
			qi::rule<Iterator, Voice::Events(), ascii::space_type> events;
			qi::rule<Iterator, Event(), ascii::space_type> event_;
			qi::rule<Iterator, Grouped(), ascii::space_type> groupedEvent_;
			qi::rule<Iterator, Phrase(), ascii::space_type> phrase_;
			qi::rule<Iterator, DocumentConfig(), ascii::space_type> documentConfig_;
			qi::rule<Iterator, TrackConfig(), ascii::space_type> trackConfig_;
			qi::rule<Iterator, SheetDef::Using(), ascii::space_type> documentUsing_;
			qi::rule<Iterator, com::String(), ascii::space_type> meta_arg_value_;
			qi::rule<Iterator, com::String(), ascii::space_type> using_;
			qi::rule<Iterator, com::String(), ascii::space_type> bar_volta_;
			qi::rule<Iterator, int, ascii::space_type> bar_repeat_number_;
			CurrentPos<Iterator> current_pos_;
		};

		template <typename Iterator>
		struct _ConfigParser : public _SheetParser<Iterator>
		{
			typedef _SheetParser<Iterator> Base;
			_ConfigParser(Iterator begin, Event::SourceId sourceId = Event::UndefinedSource) : _SheetParser<Iterator>(begin, sourceId)
			{
				using qi::attr;
				using qi::fail;
				using qi::on_error;
				using boost::phoenix::at_c;
				using boost::phoenix::push_back;
				using qi::_val;
				using namespace boost::placeholders;
				Base::initArgumentParser();
				Base::createDocumentConfigRules(Base::documentConfig_, Base::events);
				Base::initDocumentUsingParser();
				Base::current_pos_.setStartPos(begin);
				Base::documentConfig_.name("document config");
				Base::documentUsing_.name("document usings");

				Base::start %= attr(SheetDef::Usings())
					> attr(SheetDef::DocumentConfigs())
					> attr(SheetDef::Tracks())
					> -(*(
							  Base::using_[(push_back(at_c<SdDocumentUsings>(_val), qi::_1))]
							| Base::documentConfig_[(push_back(at_c<SdDocumentConfigs>(_val), qi::_1))]
						))
					> boost::spirit::eoi;

				auto onError = boost::bind(&compiler::handler::errorHandler<Iterator>, _1, Base::sourceId_);
				on_error<fail>(Base::start, onError);
			}
		};

		void _parse(const com::String &source, SheetDef &def, Event::SourceId sourceId)
		{
			using boost::spirit::ascii::space;
			typedef _SheetParser<com::String::const_iterator> SheetParserType;

			SheetParserType g(source.begin(), sourceId);
			phrase_parse(source.begin(), source.end(), g, space, def);
		}

	}

	SheetDef SheetDefParser::parse(com::CharType const *first, com::CharType const *last, Event::SourceId sourceId)
	{
		SheetDef result;
		com::String source(first, last);
		com::removeComments(source.begin(), source.end());
		_parse(source, result, sourceId);
		return result;
	}

	SheetDef ConfigParser::parse(com::CharType const *first, com::CharType const *last, Event::SourceId sourceId)
	{
		SheetDef result;
		com::String source(first, last);
		com::removeComments(source.begin(), source.end());

		using boost::spirit::ascii::space;
		typedef _ConfigParser<com::String::const_iterator> ConfigParserType;

		ConfigParserType g(source.begin(), sourceId);
		phrase_parse(source.cbegin(), source.cend(), g, space, result);

		return result;
	}
}