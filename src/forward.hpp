#ifndef SHEET_FORWARD_HPP
#define SHEET_FORWARD_HPP

#include <memory>

namespace sheet {
	namespace compiler {
		class Compiler;
		typedef std::shared_ptr<Compiler> CompilerPtr;
		class AContext;
		typedef std::shared_ptr<AContext> AContextPtr;
		class ASpielanweisung;
		typedef std::shared_ptr<ASpielanweisung> ASpielanweisungPtr;
		class AModification;
		typedef std::shared_ptr<AModification> AModificationPtr;		
	}
	class Document;
	typedef std::shared_ptr<Document> DocumentPtr;
	class VoicingStrategy;
	typedef std::shared_ptr<VoicingStrategy> VoicingStrategyPtr;
}

namespace fm {
	namespace midi {
		class Midi;
		typedef std::shared_ptr<Midi> MidiPtr;
	}
	class IConfigServer;
	typedef IConfigServer *IConfigServerPtr;
}
#endif
