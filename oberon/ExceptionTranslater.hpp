#ifndef OBERON_EXCEPTIONTRANSLATER_HPP
#define OBERON_EXCEPTIONTRANSLATER_HPP

#include "CustomOptionDescription.hpp"

#include "boost/program_options.hpp"
#include "boost/optional.hpp"
#include "boost/function.hpp"

#include <cstdint>
#include <memory>

namespace oberon
{
  /** Finds the option in the description with the specified name
   *
   * @throws std::runtime_error: if an option with the specified name could not be found.
   */
  boost::optional<CustomOptionDescription> locateOption(const std::string& optionName,
                                                        const boost::program_options::options_description& options);

  /** Convenience function to wrap a method, catch any PO exceptions and translate them to something more useful
   *
   * The purpose of this method is to execute arbitrary code and catch any boost.Program_options errors that are thrown.
   * Using the information supplied the error is then translated into something more readable, relevant and specific.
   *
   * @throws CommandLineParsingError: if function being executed throws a PO error
   */
  template <typename T_CodeReturnType>
  T_CodeReturnType executeAndTranslateExceptions(boost::function<T_CodeReturnType()> codeToExecute,
                                                 const boost::program_options::options_description& options,
                                                 const boost::program_options::positional_options_description& positionalOptions,
                                                 boost::optional<std::string> subcommandName=boost::none)
  {
    try
    {
      return codeToExecute();

    }
    catch (boost::program_options::required_option& e)
    {
      boost::optional<CustomOptionDescription> correspondingOptionForException = locateOption(e.get_option_name(), options);
      assert(correspondingOptionForException && "If we didn't get a match then the matching of the exception name to "
                                                "option was not set up correctly");
      correspondingOptionForException->determinePositionalInfo(positionalOptions);
      std::string optionType = (correspondingOptionForException->positionalInfo_ ? "positional " : " ");
      std::string commandSource = (subcommandName ? "Subcommand '" + subcommandName.get() + "'": "Application");
      throw CommandLineParsingError(commandSource + " used without required " + optionType + "option: "
                                    + correspondingOptionForException->optionDisplayName_ + ".",
                                    subcommandName);



    }
    catch (boost::program_options::unknown_option& e)
    {
      throw CommandLineParsingError("Unknown option " + e.get_option_name() + " specified", subcommandName);
    }
    catch (boost::program_options::too_many_positional_options_error&)
    {
      throw CommandLineParsingError("Too many positional options specified", subcommandName);
    }
    catch (boost::program_options::ambiguous_option& e)
    {
      throw CommandLineParsingError(std::string(e.what()) + "\nNOTE: there is a current limitation where two subcommands"
                                                            " cannot contain the same option name or shorthand unless"
                                                            " they are specifically shared duplicate options.",
                                    subcommandName);
    }
    catch (boost::program_options::invalid_option_value& e)
    {
      throw CommandLineParsingError(std::string(e.what()) + ", likely the provided value can't be parsed into the "
                                                            "desired type for the argument",
                                    subcommandName);
    }
    catch (boost::program_options::error& e)
    {
      throw CommandLineParsingError(e.what(), subcommandName);
    }

  }

} // namespace

#endif // OBERON_EXCEPTIONTRANSLATER_HPP
