#include "Subcommand.hpp"

#include "OptionPrinter.hpp"
#include "ExceptionTranslater.hpp"


namespace
{
  namespace po = boost::program_options;

} // namespace


namespace oberon {

//----------------------------------------------------------------------------------------------------------------------
  Subcommand::Subcommand(const std::string& name, const std::string& usageDescription, const OptionCollection& sharedOptions) :
    name_(name),
    commandUsageDescription_(usageDescription),
    sharedOptions_(sharedOptions)
  {

  }

//----------------------------------------------------------------------------------------------------------------------
  po::options_description Subcommand::visibleOptions(bool includeDescText) const
  {
    po::options_description visible(includeDescText ? commandUsageDescription_ : "");
    visible.add( uniqueOptions(false, true)).add( sharedOptions() );

    return visible;

  }

//----------------------------------------------------------------------------------------------------------------------
  po::options_description Subcommand::allOptions(bool includeDescText) const
  {
    po::options_description all(commandUsageDescription_);
    all.add( uniqueOptions(true, true) ).add( sharedOptions() );

    return all;

  }

//----------------------------------------------------------------------------------------------------------------------
  po::options_description Subcommand::sharedOptions(bool enableRestrictions) const
  {
    return sharedOptions_.allOptions(enableRestrictions);
  }

//----------------------------------------------------------------------------------------------------------------------
  po::positional_options_description Subcommand::positionalOptions() const
  {
    return po::positional_options_description();
  }

//----------------------------------------------------------------------------------------------------------------------
  po::variables_map Subcommand::parseCommandLine(const std::vector<std::string>& commandLine) const
  {
    /** Beware here; even though the command_line_parser takes const&'s for options, it stores the reference (?!?)
      * so if you pass in a temporary things get undefined real fast. Hence the reason I'm using local objects instead
      * of the direct return values of the member functions */
    po::options_description allOptionsLocal = allOptions();
    po::positional_options_description positionalLocal = positionalOptions();
    po::variables_map vm;

    /** Using a lambda to call the correct overload of store and generally get this done in a one line call. I tried
     *  to wrap run() and store() individually but I was getting a segfault in the e.what() of the exception when either
     *  failed. I suspect this was due to some internal boost.Program_options shenanigans but didn't spend the time to
     *  find out as I think this solution is better anyway.
     */
    auto wrapParseAndStore = [&]()
    {
      po::store(po::command_line_parser(commandLine).options( allOptionsLocal ).positional( positionalLocal ).run(), vm);
    };
    executeAndTranslateExceptions<void>( wrapParseAndStore, allOptionsLocal, positionalLocal, this->name() );
    executeAndTranslateExceptions<void>( std::bind(&po::notify, vm), allOptionsLocal, positionalLocal, this->name());

    checkOptionConsistency(vm);

    return vm;

  }

//----------------------------------------------------------------------------------------------------------------------
  std::string Subcommand::usageDescription() const
  {
    std::ostringstream description;
    OptionPrinter allPrinter(allOptions(), positionalOptions());
    OptionPrinter uniquePrinter(uniqueOptions(), positionalOptions());
    OptionPrinter sharedPrinter(sharedOptions(), positionalOptions());

    description << "Subcommand: " << name_ << std::endl
                << std::endl
                << commandUsageDescription_ << std::endl
                << "USAGE: " << name_ << " " << allPrinter.usage()
                << std::endl
                << std::endl
                << allPrinter.positionalOptionDetails() << std::endl
                << std::endl
                << uniquePrinter.optionDetails() << std::endl
                << sharedPrinter.optionDetails();


    return description.str();
  }

//----------------------------------------------------------------------------------------------------------------------
  void Subcommand::checkOptionConsistency(po::variables_map vm) const
  {
    // as specified in the header, this
    return;
  }

//----------------------------------------------------------------------------------------------------------------------

} // namespace
