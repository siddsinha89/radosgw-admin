#include "SubcommandCLI.hpp"

#include "OptionPrinter.hpp"
#include "ExceptionTranslater.hpp"

#include <algorithm>

#include <cassert>


namespace
{
  namespace po = boost::program_options;

} // namespace


namespace oberon {

//----------------------------------------------------------------------------------------------------------------------
  SubcommandCLI::SubcommandCLI(const std::string& applicationName,
                               const std::string& applicationDesc,
                               SubcommandCollection subcommands,
                               const boost::program_options::options_description& mainOptions) :
    applicationSubcommands_(subcommands),
    globalAppOptions_( mainOptions ),
    applicationDescription_(applicationDesc),
    applicationName_(applicationName)
  {
    globalAppOptions_.add_options()
      ("subcommand", po::value<std::string>(), "The subcommand to execute, see below:");

    subcommandOptions_.add_options()
      ("additional-positional", po::value<std::vector<std::string> >(), "hidden option for dev only");

    subcommandPositionalOptions_.add("subcommand", 1);
    subcommandPositionalOptions_.add("additional-positional", -1);

    visiblePositionalOptions_.add("subcommand", 1);

  }

//----------------------------------------------------------------------------------------------------------------------
  SubcommandCLI::ParseOutput SubcommandCLI::parseCommandLine(int argc, char** argv)
  {
    po::options_description all;
    all.add(applicationSubcommands_.allUniqueOptions()).add(globalAppOptions_).add(subcommandOptions_);

    auto parsingCode = [&]() -> ParseOutput
    {
      po::parsed_options parsed = po::command_line_parser(argc, argv)
                                  .options(all)
                                  .positional(subcommandPositionalOptions_)
                                  .run();
      po::variables_map vm;
      po::store(parsed, vm);

      po::notify(vm);


      if ( ! vm.count("subcommand") )
      {
        return ParseOutput(vm);
      }

      std::vector<std::string> subcommandOptions = prepareCommandLine(parsed.options,
                                                                      vm.count("additional-positional")
                                                                        ? vm["additional-positional"].as<std::vector<std::string> >()
                                                                          : std::vector<std::string>());

      std::string selectedSubcommand = vm["subcommand"].as<std::string>();

      if ( ! applicationSubcommands_.exists(selectedSubcommand) )
      {
        throw CommandLineParsingError("Subcommand used: " + selectedSubcommand + ", is not valid for this application");
      }

      return ParseOutput( SubcommandParse{ applicationSubcommands_.getSubcommand(selectedSubcommand),
                                           subcommandOptions} );

    }; // lambda

    return executeAndTranslateExceptions<ParseOutput>(parsingCode, all, subcommandPositionalOptions_);

  }

//----------------------------------------------------------------------------------------------------------------------
  void SubcommandCLI::displayHelp(boost::optional<std::vector<std::string> > topics, std::ostream& out)
  {
    if ( topics )
    {
      for (auto topic : topics.get())
      {
        if ( applicationSubcommands_.exists(topic) )
        {
          out << applicationSubcommands_.getSubcommand(topic)->usageDescription() << std::endl
              << std::endl;
        }
        else
        {
          out << "Topic requested: " << topic << ", does not exist and no help can be displayed" << std::endl
              << std::endl;
        }

      } // for

    }
    else
    {
      out << applicationUsage() << std::endl;
    }
  }

//----------------------------------------------------------------------------------------------------------------------
  void SubcommandCLI::displayParsingError(CommandLineParsingError& error, std::ostream& out, std::ostream& errorOut)
  {
    if ( boost::optional<std::string> triggeringSubcommandName = error.subcommandName() )
    {
      assert( applicationSubcommands_.exists(triggeringSubcommandName.get())
              && "Parsing error should only ever contain the name of a subcommand that exists in the system");
      out << applicationSubcommands_.getSubcommand(triggeringSubcommandName.get())->usageDescription();
    }
    else
    {
      out << applicationUsage() << std::endl;
    }

    errorOut << std::endl
             << "ERROR: Parsing command line options failed " << std::endl
             << error.what() << std::endl
             << std::endl;
  }

//----------------------------------------------------------------------------------------------------------------------
  std::string SubcommandCLI::applicationUsage()
  {
    OptionPrinter printer(globalAppOptions_, visiblePositionalOptions_);
    std::ostringstream applicationDesc;
    applicationDesc << applicationDescription_ << std::endl
                    << "USAGE: " << applicationName_ << " " << printer.usage()
                    << std::endl
                    << std::endl
                    << printer.optionDetails() << std::endl
                    << printer.positionalOptionDetails() << std::endl;

    for (auto subcommandName : applicationSubcommands_.allSubcommandKeys())
    {
      applicationDesc << "\t\t\t\t\t\t" << subcommandName << std::endl;
    }

    return applicationDesc.str();
  }

//----------------------------------------------------------------------------------------------------------------------
  std::vector<std::string> SubcommandCLI::prepareCommandLine(const std::vector<boost::program_options::option>& options,
                                                             const std::vector<std::string>& posOptions)
  {
    std::vector<std::string> preparedCmdLine;
    if ( posOptions.size() > 0 )
    {
      preparedCmdLine = posOptions;
    }
    for (auto option : options)
    {
      if ( option.position_key == -1 )
      {
        std::for_each( std::begin(option.original_tokens),
                       std::end(option.original_tokens),
                       [&](const std::string& str) { preparedCmdLine.push_back(str); } );
      }
    }

    return preparedCmdLine;

  }

//----------------------------------------------------------------------------------------------------------------------

} // namespace
