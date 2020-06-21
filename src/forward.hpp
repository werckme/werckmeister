#ifndef SHEET_FORWARD_HPP
#define SHEET_FORWARD_HPP

#include <memory>

namespace sheet {
	namespace compiler {
		class Compiler;
		typedef std::shared_ptr<Compiler> CompilerPtr;
		class AContext;
		typedef std::shared_ptr<AContext> AContextPtr;
		class IContext;
		typedef std::shared_ptr<IContext> IContextPtr;		
		class ASpielanweisung;
		typedef std::shared_ptr<ASpielanweisung> ASpielanweisungPtr;
		class AModification;
		typedef std::shared_ptr<AModification> AModificationPtr;		
	}
	namespace lua {
		class ALuaScript;
		typedef std::shared_ptr<ALuaScript> ALuaScriptPtr;
		class LuaMidiContext;
		typedef std::shared_ptr<LuaMidiContext> LuaMidiContextPtr;			
	}
	class Document;
	typedef std::shared_ptr<Document> DocumentPtr;
	typedef std::weak_ptr<Document> DocumentWPtr;
	typedef std::shared_ptr<const Document> ConstDocumentPtr;
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

	class Werckmeister;
}

namespace funk {
    class UdpSender;
	typedef std::shared_ptr<UdpSender> UdpSenderPtr;
}

#endif
