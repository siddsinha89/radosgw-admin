#ifndef CREATE_HPP
#define CREATE_HPP

#include "oberon/Subcommand.hpp"
#include "oberon/OptionCollection.hpp"

namespace basic
{
  class Create : public oberon::Subcommand
  {
  public: // interface
    Create(oberon::OptionCollection sharedOptions) :
      oberon::Subcommand("create", "create a user from the provided string", sharedOptions)
    {
      /** **/
    }

    boost::program_options::options_description uniqueOptions(bool includeHidden, bool enableRestrictions) const
    {
      /** Several options for specifying user info while creation.
       *  See how nasty this would be without subcommand separation?
       */
      boost::program_options::options_description returnOptions;
      returnOptions.add_options()
        ("uuid,u", "Include lower case characters in the selection for random replacement")
        ("display-name,d", "Include upper case characters in the selection for random replacement")
        ("email,e", "Include numbers in the selection for random replacement");

      return returnOptions;
    }

    boost::program_options::positional_options_description positionalOptions() const
    {
      boost::program_options::positional_options_description positional;
      positional.add("uuid-String", 1);
      return positional;
    }

    /** This is a new addition yet to be implemented
     */
  //  void checkOptionConsistency(boost::program_options::variables_map vm) const;

  }; // class

//**********************************************************************************************************************

} // namespace

#endif // CREATE_HPP
