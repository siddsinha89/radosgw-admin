#ifndef INFO_HPP
#define INFO_HPP

#include "oberon/Subcommand.hpp"
#include "oberon/OptionCollection.hpp"

namespace basic
{
  class Info : public oberon::Subcommand
  {
  public: // interface
    Info(oberon::OptionCollection sharedOptions) :
      oberon::Subcommand("info", "list user with uuid from the provided string", sharedOptions)
    {
      /** **/
    }

    boost::program_options::options_description uniqueOptions(bool includeHidden, bool enableRestrictions) const
    {
      boost::program_options::options_description returnOptions;
      returnOptions.add_options()
        ("gid,g", "This is gid of user to be displayed");

      return returnOptions;
    }

    boost::program_options::positional_options_description positionalOptions() const
    {
      boost::program_options::positional_options_description positional;
      //positional.add("targetString", 1);
      positional.add("uuid-String", 1);
      return positional;
    }

    /** This is a new addition yet to be implemented
     */
  //  void checkOptionConsistency(boost::program_options::variables_map vm) const;

  }; // class

//**********************************************************************************************************************

} // namespace

#endif // INFO_HPP
