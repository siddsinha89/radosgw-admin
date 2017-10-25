#ifndef OBERON_SUBCOMMANDCLI_HPP
#define OBERON_SUBCOMMANDCLI_HPP

#include "SubcommandCollection.hpp"
#include "Utils.hpp"

#include "boost/program_options.hpp"
#include "boost/optional.hpp"

#include <string>
#include <vector>
#include <memory>
#include <ostream>


namespace oberon
{
//**********************************************************************************************************************
  /** Top level class used for creating a subcommand command line interface
   *
   * This object is configured with all of the desired subcommands and other information and provides a convenient
   * interface for parsing the command line options
   */
  class SubcommandCLI
  {
  public: // types
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    struct SubcommandParse
    {
      boost::shared_ptr<Subcommand> subcommand_;
      std::vector<std::string> processedCommandLine_;

      boost::program_options::variables_map doSubcommandParse() { return subcommand_->parseCommandLine(processedCommandLine_); }

    }; // struct
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /** Parse result, with either a subcommand used or not, allowing a client to extract relevant information
     */
    class ParseOutput
    {
    public: // interface
      ParseOutput(const boost::program_options::variables_map& vm) : vm_(vm) {}
      ParseOutput(const SubcommandParse& parse) : subcommandParse_(parse) {}

      /** @throws CommandLineParsingError: if there were errors while parsing the command line */
      boost::program_options::variables_map vm()    { return subcommandParse_ ? subcommandParse_->doSubcommandParse() : vm_.get(); }

      /** @note: the subcommand name returned here is guaranteed to exist in the system */
      boost::optional<std::string> subcommandUsed() { return subcommandParse_ ?
                                                               boost::optional<std::string>(subcommandParse_->subcommand_->name()) :
                                                                  boost::none; }

    private: // data
      boost::optional<SubcommandParse> subcommandParse_;
      boost::optional<boost::program_options::variables_map> vm_;

    }; // class
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  public: // interface
    /** Constructor
     *
     * @pre subcommands: registrations must be finalised.
     * @param mainOptions: include this if your application can be used without a subcommand, i.e. app --version or
     *                     something similar.
     */
    SubcommandCLI(const std::string& applicationName,
                  const std::string& applicationDesc,
                  SubcommandCollection subcommands,
                  const boost::program_options::options_description& mainOptions=boost::program_options::options_description());


    /** Does a global parse of the command line parameters to ensure validity and prep for subcommand parsing
     *
     * This is the first step of parsing, it ensures that all the options used in the command line exist and are valid,
     * and it returns all relevant information to the user.
     *
     * If a subcommand is not present in the command line then it is up to the client to parse the options for
     * themselves. The client should have access to all the relevant information to do this as they provide the main
     * options in the constructor if applicable.
     *
     * @returns: the result of parsing the supplied command line arguments, see ParseOutput for details.
     * @throws CommandLineParsingError: if the subcommand specified in the command line does not exist in the system.
     * @throws CommandLineParsingError: if command line is non conformant
     */
    ParseOutput parseCommandLine(int argc, char** argv);


    /** Writes help for the indicated subcommands or the application usage if no topics specified to the ostream provided.
     */
    void displayHelp(boost::optional<std::vector<std::string> > topics, std::ostream& out);


    /** helper function to avoid duplication when displaying parsing errors to the app user
     *
     * Prints the usage for the subcommand used (or the app if one wasn't used) and a formatted error message.
     */
    void displayParsingError(CommandLineParsingError& error, std::ostream& out, std::ostream& errorOut);


    /** Return a string of the top level help for the application
     *
     * Includes example command line usage and a list of available subcommands
     */
    std::string applicationUsage();


    SubcommandCollection subcommands() { return applicationSubcommands_; }


  private: // methods
    std::vector<std::string> prepareCommandLine(const std::vector<boost::program_options::option>& options,
                                                const std::vector<std::string>& posOptions);

  private: // data
    SubcommandCollection applicationSubcommands_;
    boost::program_options::options_description globalAppOptions_;
    boost::program_options::options_description subcommandOptions_;
    boost::program_options::positional_options_description subcommandPositionalOptions_;
    boost::program_options::positional_options_description visiblePositionalOptions_;

    std::string applicationDescription_;
    std::string applicationName_;

  }; // class

//**********************************************************************************************************************

} // namespace

#endif // OBERON_SUBCOMMANDCLI_HPP
