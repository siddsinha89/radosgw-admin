#ifndef RAD_OPTIONCOLLECTION_HPP
#define RAD_OPTIONCOLLECTION_HPP

#include "GenericFactory.hpp"

#include "boost/program_options.hpp"

#include <vector>
#include <string>
#include <functional>

namespace oberon {

//**********************************************************************************************************************
  class OptionCollection
  {
  public: // interface

    OptionCollection() = default; // allow default construction

    /** Get a number of options
     *
     * @throws if any of the options are not part of the collection
     */
    boost::program_options::options_description getOptions(const std::vector<std::string>& optionNames,
                                                           bool enableRestrictions=true) const;
    OptionCollection getSubsetOfOptions(const std::vector<std::string>& optionNames,
                                        bool enableRestrictions=true) const;

    boost::program_options::options_description allOptions(bool enableRestrictions=true) const;

    /** @name arg adding methods
     *
     * Both methods use '\0' as a sentinel value indicating no shorthand option set
     */
    //@{
      template <typename T_OptionArgType>
      void addArgOption(const std::string& name,
                        const std::string& optionDescription,
                        bool required=false,
                        char shortHandOption='\0');

      void addBasicOption(const std::string& name,
                          const std::string& optionDescription,
                          bool required=false,
                          char shortHandOption='\0');
    //@}

    size_t size() const;
    bool exists(const std::string& name) const;

  private: // types
    typedef boost::shared_ptr<boost::program_options::option_description>                 OptionPtr;
    typedef GenericFactory<OptionPtr, std::string, std::function<OptionPtr(bool)> >       OptionFactory;

    typedef std::function<boost::program_options::value_semantic*(bool)>                  ValueSemanticCreator;

  private: // methods
    /** Private method for registering factory methods, contains complexity so the public interface doesn't have to.
     *
     * @param creator: functor that returns a new value_semantic*, Note: this is needed in order to ensure that each
     *                 factory constructed option gets new'd semantic values, if they don't they are deleted and
     *                 explosions occur.
     */
    void registerOptionCreator(const std::string& name,
                               const std::string& optionDescription,
                               char shortHandOption,
                               ValueSemanticCreator valueSemanticCreator);

    /** A convenient private constructor to allow member functions to return an OptionCollection
     */
    OptionCollection(OptionFactory internalFactory, std::vector<std::string> optionsToInclude);

  private: // data
    OptionFactory optionFactory_;

  }; // class

//**********************************************************************************************************************

} // namespace

namespace oberon {

  template <typename T_OptionArgType>
  void OptionCollection::addArgOption(const std::string& name,
                                      const std::string& optionDescription,
                                      bool required,
                                      char shortHandOption)
  {
    auto creatorMethod = [=](bool enforceRequirement)
    {
      if ( required && enforceRequirement)
      {
        return boost::program_options::value<T_OptionArgType>()->required();
      }
      else
      {
        return boost::program_options::value<T_OptionArgType>();
      }

    }; // lambda

    registerOptionCreator(name,
                          optionDescription,
                          shortHandOption,
                          creatorMethod);
  }

} // namespace

#endif // RAD_OPTIONCOLLECTION_HPP
