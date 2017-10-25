#include "Utils.hpp"

namespace oberon {

//----------------------------------------------------------------------------------------------------------------------
  CommandLineParsingError::CommandLineParsingError(const std::string& whatMessage,
                                                   const boost::optional<std::string> triggeringSubcommand) :
    std::runtime_error(whatMessage),
    subcommandName_(triggeringSubcommand)
  {

  }

//----------------------------------------------------------------------------------------------------------------------
  /** unsigned being used for variables here to match the internal style used by boost.Program_options.
   */
  boost::optional<PositionalInfo> getPositionalInfo(const boost::program_options::positional_options_description& positionalOptions,
                                                    const std::string& optionName)
  {
    /** If there are unlimited positional options can be specified (which happens often) then need to work out how many
     *  unique types there are so we don't search up to max() redundant times
     */

    unsigned count = 0;
    bool unlimitedOptionInPositionals = positionalOptions.max_total_count() == std::numeric_limits<unsigned>::max();
    if ( unlimitedOptionInPositionals )
    {
      std::string lastName = positionalOptions.name_for_position( std::numeric_limits<unsigned>::max() - 1);
      for ( /* */ ; lastName != positionalOptions.name_for_position(count); ++count)
      {
        // the loop itself increments count which is all that is desired
      }
      ++count; // need to add this here as the loop doesn't increment to the right value
    }
    else
    {
      count = positionalOptions.max_total_count();
    }

    unsigned instanceCount = 0;
    for ( unsigned position=0; position < count; ++position)
    {
      std::string positionName = positionalOptions.name_for_position(position);
      if (optionName == positionName)
      {
        ++instanceCount;
        const bool THIS_IS_THE_LAST_OPTION = (position + 1 == count);
        if ( unlimitedOptionInPositionals && THIS_IS_THE_LAST_OPTION )
        {
          return PositionalInfo(position, std::numeric_limits<unsigned>::max());
        }
        else if (THIS_IS_THE_LAST_OPTION)
        {
          return PositionalInfo(position, instanceCount);
        }
      }
      else if (optionName != positionName && instanceCount > 0)
      {
        return PositionalInfo(position-1, instanceCount);
      }

    }

    return boost::none;

  }

//----------------------------------------------------------------------------------------------------------------------

} // namespace
