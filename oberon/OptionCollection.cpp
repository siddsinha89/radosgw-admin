#include "OptionCollection.hpp"

#include "boost/make_shared.hpp"

namespace
{
  namespace po = boost::program_options;

  const std::string NO_VALUE_KEY = "no_value";
  const std::string STRING_VALUE_KEY = "string";
} // namespace

namespace oberon {

//----------------------------------------------------------------------------------------------------------------------
  po::options_description OptionCollection::getOptions(const std::vector<std::string>& optionNames,
                                                       bool enableRestrictions) const
  {
    po::options_description desc;
    for (auto name : optionNames)
    {
      if ( optionFactory_.hasRegistration(name) )
      {
        desc.add( optionFactory_.get(name, enableRestrictions) );
      }
      else
      {
        throw std::runtime_error("Requested option: " + name + " is not a member of the option collection");
      }
    }

    return desc;
  }

//----------------------------------------------------------------------------------------------------------------------
  OptionCollection OptionCollection::getSubsetOfOptions(const std::vector<std::string>& optionNames,
                                                        bool enableRestrictions) const
  {
    return OptionCollection(optionFactory_, optionNames);
  }

//----------------------------------------------------------------------------------------------------------------------
  po::options_description OptionCollection::allOptions(bool enableRestrictions) const
  {
    po::options_description allOptions;
    for ( auto index : optionFactory_.registeredIndexes() )
    {
      allOptions.add( optionFactory_.get(index, enableRestrictions) );
    }

    return allOptions;
  }

//----------------------------------------------------------------------------------------------------------------------
  void OptionCollection::addBasicOption(const std::string& name,
                                        const std::string& optionDescription,
                                        bool required,
                                        char shortHandOption)
  {
    registerOptionCreator(name,
                          optionDescription,
                          shortHandOption,
                          [](bool enforceRequirement){ return new po::untyped_value(true); } ); // ignore requires as it
                                                                                                // makes no sense for a
                                                                                                // pure option
  }

//----------------------------------------------------------------------------------------------------------------------
  size_t OptionCollection::size() const
  {
    return optionFactory_.registeredIndexes().size();
  }

//----------------------------------------------------------------------------------------------------------------------
  bool OptionCollection::exists(const std::string& name) const
  {
    return optionFactory_.hasRegistration(name);
  }

//----------------------------------------------------------------------------------------------------------------------
  void OptionCollection::registerOptionCreator(const std::string& name,
                                               const std::string& optionDescription,
                                               char shortHandOption,
                                               ValueSemanticCreator valueSemanticCreator)
  {
    std::string optionSpec = shortHandOption != '\0' ? name + "," + shortHandOption : name;

    optionFactory_.registerCreator(name,
      [=](bool enforceRequires)
      {
        return boost::make_shared<po::option_description>(optionSpec.c_str(),
                                                          valueSemanticCreator(enforceRequires),
                                                          optionDescription.c_str());
      }
    );
  }

//----------------------------------------------------------------------------------------------------------------------
  OptionCollection::OptionCollection(OptionCollection::OptionFactory internalFactory,
                                     std::vector<std::string> optionsToInclude)
  {
    optionFactory_ = internalFactory;
    for (auto registration : optionFactory_.registeredIndexes())
    {
      if ( std::find(optionsToInclude.begin(), optionsToInclude.end(), registration) == optionsToInclude.end() )
      {
        optionFactory_.removeRegistration(registration);
      }

    }

  }

//----------------------------------------------------------------------------------------------------------------------

} // namespace
