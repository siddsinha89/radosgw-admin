#include "OptionPrinter.hpp"

#include "boost/algorithm/string/regex.hpp"
#include "boost/range/algorithm/sort.hpp"

namespace
{
  namespace po = boost::program_options;
  typedef std::vector<boost::shared_ptr<po::option_description > > Options;

} // namespace

namespace oberon {

//----------------------------------------------------------------------------------------------------------------------
  OptionPrinter::OptionPrinter(po::options_description desc, po::positional_options_description positionalDesc)
  { 
    Options allOptions = desc.options();
    for (Options::iterator it = allOptions.begin(); it != allOptions.end(); ++it)
    {
      CustomOptionDescription currOption(*it, positionalDesc);

      addOption(currOption);

    } // foreach option


  }

//----------------------------------------------------------------------------------------------------------------------
  void OptionPrinter::addOption(const CustomOptionDescription& optionDesc)
  {
    optionDesc.positionalInfo_ ? positionalOptions_.push_back(optionDesc) : options_.push_back(optionDesc);

  }

//----------------------------------------------------------------------------------------------------------------------
  std::string OptionPrinter::usage()
  {
    std::stringstream usageDesc;
    /** simple flags that have a short version
     */
    bool firstShortOption = true;
    for (std::vector<CustomOptionDescription>::iterator it = options_.begin();
         it != options_.end();
         ++it)
    {
      if ( it->hasShort_ && ! it->hasArgument_ && ! it->required_ )
      {
        if (firstShortOption)
        {
          usageDesc << "[-";
          firstShortOption = false;
        }

        usageDesc << it->optionDisplayName_[1];
      }

    }
    if ( ! firstShortOption )
    {
      usageDesc << "] ";
    }

    /** simple flags that DO NOT have a short version
     */
    for (std::vector<CustomOptionDescription>::iterator it = options_.begin();
         it != options_.end();
         ++it)
    {
      if ( ! it->hasShort_ && ! it->hasArgument_ && ! it->required_ )
      {
        usageDesc << "[" << it->optionDisplayName_ << "] ";
      }

    }

    /** options with arguments
     */
    for (std::vector<CustomOptionDescription>::iterator it = options_.begin();
         it != options_.end();
         ++it)
    {
      if ( it->hasArgument_ && ! it->required_ )
      {
        usageDesc << "[" << it->optionDisplayName_ << " ARG] ";
      }

    }

    /** required options with arguments
     */
    for (std::vector<CustomOptionDescription>::iterator it = options_.begin();
         it != options_.end();
         ++it)
    {
      if ( it->hasArgument_ && it->required_ )
      {
        usageDesc << it->optionDisplayName_ << " ARG ";
      }

    }

    /** positional options
     */
    for (auto value : boost::range::sort(positionalOptions_,
                                         [](const CustomOptionDescription& lhs, const CustomOptionDescription& rhs)
                                         {
                                           return lhs.positionalInfo_->position() < rhs.positionalInfo_->position();
                                         }) )
    {
      if ( value.required_ )
      {
        usageDesc << value.optionDisplayName_ << (value.positionalInfo_->unlimitedInstances() ? "... " : " ");
      }
      else
      {
        usageDesc << "[" << value.optionDisplayName_ << (value.positionalInfo_->unlimitedInstances() ? "...] " : "] ");
      }
    }

    return usageDesc.str();

  }

//----------------------------------------------------------------------------------------------------------------------
  std::string OptionPrinter::positionalOptionDetails()
  {
    bool firstOption = true;
    std::stringstream output;

    for (auto value : boost::range::sort(positionalOptions_,
                                         [](const CustomOptionDescription& lhs, const CustomOptionDescription& rhs)
                                         {
                                           return lhs.positionalInfo_->position() < rhs.positionalInfo_->position();
                                         }) )
    {
      if ( ! firstOption )
      {
        output << std::endl;
      }
      output << value.getOptionUsageString();
      firstOption = false;
    }

    return output.str();
  }

//----------------------------------------------------------------------------------------------------------------------
  std::string OptionPrinter::optionDetails()
  {
    std::stringstream output;
    for (std::vector<CustomOptionDescription>::iterator it = options_.begin();
         it != options_.end();
         ++it)
    {
      output << it->getOptionUsageString() << std::endl;

    }

    return output.str();
  }

//----------------------------------------------------------------------------------------------------------------------
  void OptionPrinter::printStandardAppDesc(const std::string& appName,
                                           std::ostream& out,
                                           po::options_description desc,
                                           po::positional_options_description positionalDesc)
  {
    OptionPrinter optionPrinter(desc, positionalDesc);

    out << "USAGE: " << appName << " " << optionPrinter.usage() << std::endl
        << std::endl
        << "-- Option Descriptions --" << std::endl
        << std::endl
        << "Positional arguments:" << std::endl
        << optionPrinter.positionalOptionDetails()
        << std::endl
        << "Option Arguments: " << std::endl
        << optionPrinter.optionDetails()
        << std::endl;
  }

//----------------------------------------------------------------------------------------------------------------------
  void OptionPrinter::printSingleLineUsageDescription(const std::string& appName,
                                                      std::ostream& out,
                                                      po::options_description desc,
                                                      po::positional_options_description positionalDesc)
  {
    OptionPrinter optionPrinter(desc, positionalDesc);

    out << "USAGE: " << appName << " " << optionPrinter.usage();
  }

//----------------------------------------------------------------------------------------------------------------------
  void OptionPrinter::formatRequiredOptionError(po::required_option& error)
  {
    std::string currOptionName = error.get_option_name();
    boost::algorithm::erase_regex(currOptionName, boost::regex("^-+"));
    error.set_option_name(currOptionName);

  }

//----------------------------------------------------------------------------------------------------------------------

} // namespace
