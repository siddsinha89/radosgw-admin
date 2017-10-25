#ifndef RAD_SUBCOMMANDCOLLECTION_HPP
#define RAD_SUBCOMMANDCOLLECTION_HPP

#include "OptionCollection.hpp"
#include "Subcommand.hpp"
#include "GenericFactory.hpp"

#include "boost/program_options.hpp"

#include <memory>
#include <string>
#include <functional>

namespace oberon
{
//**********************************************************************************************************************
  /** A factory for subcommands with a number of convenience methods for extraction and integrated support for help display.
   */
  class SubcommandCollection
  {
  public: // types
    typedef std::unique_ptr<Subcommand> SubcommandPtr;
    typedef std::function<SubcommandPtr()> SubcommandCreator;

  public: // interface
    SubcommandCollection();

    /** Optional method that prevents additional subcommands from being registered and inserts a generic help subcommand.
     *
     * This should be called when all regular registrations have been completed, it's primary purpose is to add a
     * help subcommand that can provide information on the other registered subcommands and once this has been done
     * it doesn't make sense to allow further registrations.
     */
    void finaliseRegistrations();

    /** @throws std::runtime_error: if requested subcommand is already registered or registrations are closed */
    void add(const std::string& key, SubcommandCreator creatorMethod);

    /** @throws std::runtime_error: if requested subcommand is not registered */
    SubcommandPtr getSubcommand(const std::string& subcommandName);

    std::vector<SubcommandPtr> allSubcommands();

    bool exists(const std::string& subcommandName);
    std::vector<std::string> allSubcommandKeys();

    boost::program_options::options_description allUniqueOptions();

  private: // data
    GenericFactory<SubcommandPtr, std::string> subcommandFactory_;
    bool registrationsFinalised_;

  }; // class

//**********************************************************************************************************************

} // namespace

#endif // RAD_SUBCOMMANDCOLLECTION_HPP
