#include "ExceptionTranslater.hpp"

namespace
{
  namespace po = boost::program_options;

} // namespace


namespace oberon {

//----------------------------------------------------------------------------------------------------------------------
  boost::optional<CustomOptionDescription> locateOption(const std::string& optionName,
                                                        const po::options_description& options)
  {
    for ( auto option : options.options() )
    {
      CustomOptionDescription parsedOption(option);
      if ( parsedOption.requiredExceptionName_ == optionName )
      {
        return parsedOption;
      }
    }

    return boost::none;
  }

//----------------------------------------------------------------------------------------------------------------------

} // namespace
