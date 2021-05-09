#include <boost/spirit/include/qi.hpp>
#include <sheet/AliasPitchDef.h>
#include <fm/common.hpp>

namespace sheet {
    namespace compiler {
        namespace _impl {
            typedef fm::String::const_iterator Iterator;
            void initExtendedPitches(boost::spirit::qi::rule<Iterator, AliasPitchDef(), boost::spirit::ascii::space_type>&);
        }
    }
}