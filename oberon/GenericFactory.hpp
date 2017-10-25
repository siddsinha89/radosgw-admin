#ifndef OBERON_GENERICFACTORY_HPP
#define OBERON_GENERICFACTORY_HPP

#include <map>
#include <vector>
#include <functional>
#include <stdexcept>
#include <sstream>

namespace oberon {

//**********************************************************************************************************************
  /** A generic factory that allows a user to create objects based off configured index associations.
   *
   * @todo might want an unregister method.
   */
  template <typename T_FactoryProduct,
            typename T_IndexValue,
            typename T_CreatorMethod = std::function<T_FactoryProduct()> >
  class GenericFactory
  {
  public: // interface
    /** Construct and return a product of the specified type
     *
     * @throws if there is no creator method registered for the index.
     */
    template <typename ...Args>
    T_FactoryProduct get(const T_IndexValue& index, Args... args) const
    {
      if ( creatorIndex_.find(index) != creatorIndex_.end() )
      {
        return creatorIndex_.at(index)(args...);
      }
      else
      {
        std::ostringstream errorMessage;
        errorMessage << "Creator method for index: " << index << " has not been registered with the factory";
        throw std::runtime_error(errorMessage.str());
      }
    }

    /** Register a creator method for the given value
     *
     * @throws if a method has already been registered for the index.
     */
    void registerCreator(const T_IndexValue& index, const T_CreatorMethod& creatorMethod)
    {
      if ( creatorIndex_.find(index) == creatorIndex_.end() )
      {
        creatorIndex_.insert( std::make_pair(index, creatorMethod) );
      }
      else
      {
        std::ostringstream errorMessage;
        errorMessage << "Creator method already specified for index: " << index;
        throw std::runtime_error(errorMessage.str());
      }
    }

    /** Get a collection containing all registered indices in no particular order
     */
    std::vector<T_IndexValue> registeredIndexes() const
    {
      std::vector<T_IndexValue> indices;
      for (auto pair : creatorIndex_)
      {
        indices.push_back(pair.first);
      }

      return indices;
    }

    /** Check whether a given index value has an associated registration
     */
    bool hasRegistration(const T_IndexValue& index) const
    {
      return creatorIndex_.count(index) > 0;
    }

    /** Remove the registration for the given index
     *
     * @note: may want to throw if the index does not have a registration
     */
    void removeRegistration(const T_IndexValue& index)
    {
      creatorIndex_.erase(index);
    }

  private: // types
    typedef std::map<T_IndexValue, T_CreatorMethod> CreatorIndex;

  private: // data
    CreatorIndex creatorIndex_;

  }; // class

//**********************************************************************************************************************

} // namespace

#endif // OBERON_GENERICFACTORY_HPP
