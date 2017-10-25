#ifndef OBERON_UTILS_HPP
#define OBERON_UTILS_HPP

#include "boost/program_options.hpp"
#include "boost/optional.hpp"
#include "boost/function.hpp"

#include <cstdint>

namespace oberon
{
//**********************************************************************************************************************
  /** Exception type that specifies an error during parsing of program options
   *
   * Optionally contains the name of the triggering subcommand if one was specified on the command line.
   */
  class CommandLineParsingError : public std::runtime_error
  {
  public: // interface
    CommandLineParsingError(const std::string& whatMessage, boost::optional<std::string> triggeringSubcommand=boost::none);

    virtual ~CommandLineParsingError() throw() {} // required by runtime_error inheritance

    boost::optional<std::string> subcommandName() { return subcommandName_; }

  private: // data
    boost::optional<std::string> subcommandName_;

  }; // class

//**********************************************************************************************************************
  class PositionalInfo
  {
  public: // methods
    PositionalInfo(uint32_t position, unsigned numInstances) :
      position_(position), numInstances_(numInstances)
    { /* */ }

    unsigned position()            const { return position_; }
    unsigned numInstancesAllowed() const { return numInstances_; }
    bool unlimitedInstances()      const { return numInstances_ == std::numeric_limits<unsigned>::max(); }

  private: // data
    unsigned position_;
    unsigned numInstances_;

  }; // struct

//**********************************************************************************************************************

  /** convenience method to allow easy inspection and collection of information regarding positionality of a given option
   */
  boost::optional<PositionalInfo> getPositionalInfo(const boost::program_options::positional_options_description& positionalOptions,
                                                    const std::string& optionName);

} // namespace

#endif // OBERON_UTILS_HPP

