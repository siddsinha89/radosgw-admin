#ifndef RAD_CUSTOMOPTIONDESCRIPTION_HPP
#define RAD_CUSTOMOPTIONDESCRIPTION_HPP

#include "Utils.hpp"

#include "boost/program_options.hpp"
#include "boost/optional.hpp"

#include <string>

namespace oberon {

//**********************************************************************************************************************
  /** Class that wraps a boost.Program_options option and presents convenient data
   *
   * Extracting relevant information about a po::option_description is pretty complicated, this class exists to hide
   * that complexity.
   */
  class CustomOptionDescription
  {
  public: // interface
    CustomOptionDescription(boost::shared_ptr<boost::program_options::option_description> option,
                            const boost::program_options::positional_options_description& positionalDesc=boost::program_options::positional_options_description());

    /** Check if this option is a member of the provided positional options, setting the public flag if so
     *
     * sets the appropriate flag and also modifies the display name to be appropriate for a positional option.
     */
    void determinePositionalInfo(const boost::program_options::positional_options_description& positionalDesc);

    std::string getOptionUsageString();

  public: // data
    std::string optionID_;
    std::string optionDisplayName_;
    std::string optionDescription_;
    std::string optionFormatName_;
    std::string requiredExceptionName_; // name that is used in a required_option exception

    bool required_;
    bool hasShort_;
    bool hasArgument_;
    boost::optional<PositionalInfo> positionalInfo_;

  private: // methods
    void init(boost::shared_ptr<boost::program_options::option_description> option);

  }; // class

//**********************************************************************************************************************

} // namespace

#endif // RAD_CUSTOMOPTIONDESCRIPTION_HPP
