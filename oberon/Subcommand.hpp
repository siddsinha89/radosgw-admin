#ifndef OBERON_SUBCOMMAND_HPP
#define OBERON_SUBCOMMAND_HPP

#include "OptionCollection.hpp"

#include "boost/program_options.hpp"

#include <vector>
#include <string>


namespace oberon
{
//**********************************************************************************************************************
  /** Interface that represents a distinct command with an arbitrary number of options associated with it
   *
   * The interface for a subcommand is focussed on allowing a client to retrieve options based on different criteria so
   * they can be used selectively for parsing a command line, displaying usage information or for other purposes.
   * Derived classes
   *
   * Shared:  Because it is assumed that a subcommand will be used alongside others and that they may share some options
   *          in common.
   * Unique:  Following the same reasoning above these are the options that are unique to this subcommand only
   * Visible: These are options that are considered public, can be shown to a user of the application.
   */
  class Subcommand
  {
  public: // interface

    virtual ~Subcommand() {}

    /** Constructor
     *
     * @param: text representation of the subcommand, should match text used to select subcommand in a command line
     * @param: text describing the purpose of the subcommand at a high level
     */
    Subcommand(const std::string& name,
               const std::string& usageDescription,
               const OptionCollection& sharedOptions=OptionCollection());

    /** Return options that should appear in help text for the subcommand
     */
    boost::program_options::options_description visibleOptions(bool includeDescText=true) const;

    /** Return all options for the subcommand, used when parsing a command line for this subcommand
     */
    boost::program_options::options_description allOptions(bool includeDescText=true) const;

    std::string name() const { return name_; }

  public: // virtual interface

    /** Return options that are not unique to this particular subcommand
     *
     * Options here are present in other subcommands used in the same subcommand set as this one
     */
    virtual boost::program_options::options_description sharedOptions(bool enableRestrictions=true) const;

    /** Return options that are specific to this command
     *
     * @param includeHidden: include options that are hidden by default.
     * @param enableRestrictions: return options with required or other modifiers.
     *
     * @note: use getOptionValue to optionally enable restrictions based on the parameter
     */
    virtual boost::program_options::options_description uniqueOptions(bool includeHidden=true,
                                                                      bool enableRestrictions=true) const = 0;

    /** Return any positional options for the subcommand
     *
     * @note: this implementation returns an empty positional description, derived class should override this if they
     *        need positional options.
     */
    virtual boost::program_options::positional_options_description positionalOptions() const;


    /** Parse the options and give back a map of the parsed variables
     *
     * @throws CommandLineParsingError: if there was an error parsing the command line
     */
    virtual boost::program_options::variables_map parseCommandLine(const std::vector<std::string>& commandLine) const;


    /** Print the standard formatted usage description for this subcommand.
     */
    virtual std::string usageDescription() const;


    /** Optional method to verify that the variables map is consistent with the options for this subcommand
     *
     * The implementation of this method does nothing and should should only be implemented by derived classes if they
     * need to check options based on their own custom rules (see below).
     *
     * Unfortunately the program options parser isn't designed to catch some meta level dependencies between options
     * and so this method exists to ensure that the parsed values are consistent.
     * e.g. if there is a --target and --all option for a subcommand; one or the other must be set, but not both at the
     * same time. Neither can be made required and there is no built in way of describing the relationship between them.
     *
     * @throws CommandLineParsingError: if the options are found to be inconsistent.
     */
    virtual void checkOptionConsistency(boost::program_options::variables_map vm) const;


  protected: // statics
    /** Utility for allowing retrieval of required options based on bool switch
     *
     * Used when optionally applying restrictions to an option based upon a bool flag
     */
    template <typename T_OptionValueType>
    static boost::program_options::typed_value<T_OptionValueType>* getOptionValue(bool required=false)
    {
      boost::program_options::typed_value<T_OptionValueType>* val = boost::program_options::value<T_OptionValueType>();
      if ( required )
      {
        val->required();
      }

      return val;
    }

  protected: // data
    std::string name_;
    std::string commandUsageDescription_;
    OptionCollection sharedOptions_;

  }; // class

//**********************************************************************************************************************


} // namespace


#endif // OBERON_SUBCOMMAND_HPP
