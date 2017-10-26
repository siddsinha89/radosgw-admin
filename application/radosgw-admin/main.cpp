#include "Delete.hpp"
#include "Create.hpp"
#include "Info.hpp"

#include "oberon/Subcommand.hpp"
#include "oberon/SubcommandCollection.hpp"
#include "oberon/SubcommandCLI.hpp"
#include "oberon/Utils.hpp"

#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"

#include <iostream>

namespace
{
  namespace po = boost::program_options;

  const int SUCCESS = 0;
  const int FAILURE = 1;

  const std::string APPLICATION_USAGE = "Application that is alternative to radosgw-admin";

} // namespace


int main(int argc, char** argv)
{
  oberon::OptionCollection sharedOptions;
  sharedOptions.addArgOption<std::string>("uuid-String", "String for the uuid of the user", true);

  oberon::SubcommandCollection subcommands;
  subcommands.add( "delete",    [=]() { return std::unique_ptr<basic::Delete>( new basic::Delete(sharedOptions) ); } );
  subcommands.add( "create", [=]() { return std::unique_ptr<basic::Create>( new basic::Create(sharedOptions) ); } );
  subcommands.add( "info", [=]() { return std::unique_ptr<basic::Info>( new basic::Info(sharedOptions) ); } );
  subcommands.finaliseRegistrations();

  /** Application level options are being added now as well, just an ultra basic version option
   */
  po::options_description mainDesc;
  mainDesc.add_options()
      ("version", "Display the version information for the application");

  oberon::SubcommandCLI subcommandApp = oberon::SubcommandCLI("radosgw-admin",
                                                              APPLICATION_USAGE,
                                                              subcommands,
                                                              mainDesc); // pass the app level options
 
  std::vector<std::string> v;
  try
  {
    oberon::SubcommandCLI::ParseOutput parseOutput = subcommandApp.parseCommandLine(argc, argv);
    po::variables_map parsedVars = parseOutput.vm();

    if ( boost::optional<std::string> subcommandNameOptional = parseOutput.subcommandUsed() )
    {
      std::string subcommandName = subcommandNameOptional.get();

      if ( subcommandName == "create" ) // processing for the user create subcommand
      {
        std::string u_str = parsedVars["uuid-String"].as<std::string>();

	v.push_back(u_str);

	std::cout << "User with uuid " << u_str << " created" << std::endl;
      }
      else if ( subcommandName == "delete" ) // processing for the user delete subcommand
      {
        std::string u_str = parsedVars["uuid-String"].as<std::string>();
	
	auto itr = std::find(v.begin(), v.end(), u_str);
	if (itr != v.end()) 
		v.erase(itr);

	std::cout << "User with uuid " << u_str << " deleted" << std::endl;
      }
      else if ( subcommandName == "info" ) // processing for the user info subcommand
      {
	for (auto i = v.begin(); i != v.end(); ++i)
    	std::cout << *i << ' ';
      }	
      else if ( subcommandName == "help" )
      {
        subcommandApp.displayHelp(parsedVars.count("topic") ?
                                     parsedVars["topic"].as< std::vector<std::string> >() :
                                        std::vector<std::string>(),
                                  std::cout);
      }
      else
      {
        assert(false && "Unrecognised subcommand used and it somehow got through parsing");
      }
    }
    else
    {
      if ( parsedVars.count("version") ) // act on the version option
      {
        std::cout << "Application Version: 0.1.0 - demo" << std::endl;
      }
      else
      {
        subcommandApp.displayHelp(boost::none, std::cout);
      }
    }

  }
  catch(oberon::CommandLineParsingError& e)
  {
    subcommandApp.displayParsingError(e, std::cout, std::cerr);
    return FAILURE;

  }

  return SUCCESS;

} // main

//----------------------------------------------------------------------------------------------------------------------
