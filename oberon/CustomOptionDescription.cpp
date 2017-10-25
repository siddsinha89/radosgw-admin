
#include "CustomOptionDescription.hpp"

#include "boost/algorithm/string/erase.hpp"

#include <iomanip>
#include <cstdint>

namespace
{
  const uint32_t LONG_NON_PREPENDED_IF_EXIST_ELSE_PREPENDED_SHORT = 0;
  const uint32_t LONG_PREPENDED_IF_EXIST_ELSE_PREPENDED_SHORT = 1;
  const uint32_t SHORT_PREPENDED_IF_EXIST_ELSE_LONG = 4;

  const uint32_t SHORT_OPTION_STRING_LENGTH = 2; // -x
  const uint32_t ADEQUATE_WIDTH_FOR_OPTION_NAME = 20;

  const bool HAS_ARGUMENT = true;
  const bool DOES_NOT_HAVE_ARGUMENT = false;

  namespace po = boost::program_options;

} // namespace

namespace oberon {

//----------------------------------------------------------------------------------------------------------------------
  CustomOptionDescription::CustomOptionDescription(boost::shared_ptr<po::option_description> option,
                                                   const po::positional_options_description& positionalDesc) :
    required_(false),
    hasShort_(false),
    hasArgument_(false)
  {
    if ( (option->canonical_display_name(SHORT_PREPENDED_IF_EXIST_ELSE_LONG).size() == SHORT_OPTION_STRING_LENGTH ) )
    {
      hasShort_ = true;
      optionID_ = option->canonical_display_name(SHORT_PREPENDED_IF_EXIST_ELSE_LONG);
      optionDisplayName_ = option->canonical_display_name(SHORT_PREPENDED_IF_EXIST_ELSE_LONG);
    }
    else
    {
      hasShort_ = false;
      optionID_ = option->canonical_display_name(LONG_NON_PREPENDED_IF_EXIST_ELSE_PREPENDED_SHORT);
      optionDisplayName_ = option->canonical_display_name(LONG_PREPENDED_IF_EXIST_ELSE_PREPENDED_SHORT);
    }

    requiredExceptionName_ = option->canonical_display_name(po::command_line_style::allow_long);

    boost::shared_ptr<const po::value_semantic> semantic = option->semantic();
    required_ = semantic->is_required();
    hasArgument_ = semantic->max_tokens() > 0 ? HAS_ARGUMENT : DOES_NOT_HAVE_ARGUMENT;

    optionDescription_ = option->description();
    optionFormatName_ = option->format_name();

    determinePositionalInfo(positionalDesc);

  }

//----------------------------------------------------------------------------------------------------------------------
  void CustomOptionDescription::determinePositionalInfo(const po::positional_options_description& positionalDesc)
  {
    if ( positionalInfo_ = getPositionalInfo(positionalDesc, optionID_) )
    {
      boost::algorithm::erase_all(optionDisplayName_, "-");
    }

  }

//----------------------------------------------------------------------------------------------------------------------
  std::string CustomOptionDescription::getOptionUsageString()
  {
    std::stringstream usageString;
    if ( positionalInfo_ )
    {
      usageString << "\t" << std::setw(ADEQUATE_WIDTH_FOR_OPTION_NAME) << std::left << optionDisplayName_ << "\t" << optionDescription_;
    }
    else
    {
      usageString << "\t" << std::setw(ADEQUATE_WIDTH_FOR_OPTION_NAME) << std::left << optionFormatName_ << "\t" << optionDescription_;
    }

    return usageString.str();

  }

//----------------------------------------------------------------------------------------------------------------------

} // namespace
