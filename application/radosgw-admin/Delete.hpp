#ifndef DELETE_HPP
#define DELETE_HPP

#include "oberon/Subcommand.hpp"
#include "oberon/OptionCollection.hpp"

namespace basic
{
  class Remove : public oberon::Subcommand
  {
  public: // interface
    Remove(oberon::OptionCollection sharedOptions) :
      oberon::Subcommand("delete", "delete user with uuid from the provided string", sharedOptions)
    {
      /** **/
    }

    boost::program_options::options_description uniqueOptions(bool includeHidden, bool enableRestrictions) const
    {
      boost::program_options::options_description returnOptions;
      returnOptions.add_options()
          ("pid,p", "Thid is the uuid of the user to be deleted");

      return returnOptions;
    }

    boost::program_options::positional_options_description positionalOptions() const
    {
      /** Now that we have share positional options it is necessary to specify where they should appear in the command
       *  line for this subcommand. Note here that you as the programmer 'know' the names of the shared options that
       *  will be present for this subcommand. This in not the best but works just fine in practice.
       */
      boost::program_options::positional_options_description positional;
      positional.add("uuid-String", 1);
      return positional;
    }

  }; // class

//**********************************************************************************************************************

} // namespace

#endif // DELETE_HPP
