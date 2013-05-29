/**
 * Provides a class that manages properties for some class (e.g. IEntity). By
 * providing a PropertyManager class, other classes can easily add a flexible
 * property management system if desired.
 *
 * @file include/GQE/Entity/classes/PropertyManager.hpp
 * @author Jacob Dix
 * @date 20120615 - Initial Release
 * @date 20120622 - Fix warning and error comments
 * @date 20120630 - Fix documentation of various methods
 */
#ifndef PROPERTY_MANAGER_HPP_INCLUDED
#define PROPERTY_MANAGER_HPP_INCLUDED

#include <map>
#include <typeinfo>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <GQE/Core/interfaces/IProperty.hpp>
#include <GQE/Core/interfaces/TProperty.hpp>
#include <GQE/Core/Core_types.hpp>

namespace GQE
{
/// Provides the PropertyManager class for managing IProperty classes
class GQE_API PropertyManager
{
public:
    /**
     * PropertyManager default constructor
     */
    PropertyManager();

    /**
     * PropertyManager deconstructor
     */
    virtual ~PropertyManager();

    /**
     * HasID returns true if thePropertyID specified exists in this
     * PropertyManager.
     * @param[in] thePropertyID to lookup in this PropertyManager
     * @return true if thePropertyID exists, false otherwise
     */
    bool hasID(const typePropertyID thePropertyID) const;

    /**
     * Get returns the property as type with the ID of thePropertyID.
     * @param[in] thePropertyID is the ID of the property to return.
     * @return the value stored in the found propery in the form of TYPE. If no
     * Property was found it returns the default value the type constructor.
     */
    template<class TYPE>
    TYPE get(const typePropertyID thePropertyID)
    {
        if(mList.find(thePropertyID) != mList.end())
        {
            if(mList.at(thePropertyID)->getType()->name() == typeid(TYPE).name())
                return static_cast<TProperty<TYPE>*>(mList[thePropertyID])->getValue();
        }
        else
        {
            WLOG() << "PropertyManager:get() returning blank property("
                   << thePropertyID << ") type" << std::endl;
        }
        TYPE anReturn=TYPE();
        return anReturn;
    }
    /**
     * @brief getPointer
     * @tparam TYPE
     * @param thePropertyID
     * @return a pointer to the property value
     */
    template<class TYPE>
    TYPE* getPointer(const typePropertyID thePropertyID)
    {
        TYPE* anReturn = NULL;
        if(mList.at(thePropertyID)->getType()->name() == typeid(TYPE).name())
            anReturn = static_cast<TProperty<TYPE>*>(mList[thePropertyID])->getPointer();
        else
        {
            WLOG() << "PropertyManager:getPointer() returning a NULL pointer("
                   << thePropertyID << ") type" << std::endl;
        }
        return anReturn;
    }
    /**
    * GetProperty returns the property as TProperty<TYPE> with the ID of thePropertyID.
    * @param[in] thePropertyID is the ID of the property to return.
    * @return A reference to a TProperty<TYPE> object.
    */
    template<class TYPE>
    TProperty<TYPE>& getProperty(const typePropertyID thePropertyID)
    {
        if(mList.find(thePropertyID) != mList.end())
        {
            if(mList.at(thePropertyID)->getType()->name() == typeid(TYPE).name())
                return static_cast<TProperty<TYPE>&>(mList[thePropertyID]);
        }
        else
        {
            WLOG() << "PropertyManager:get() returning blank property("
                   << thePropertyID << ") type" << std::endl;
        }
        TYPE anReturn=TYPE();
        return anReturn;
    }
    /**
     * Set sets the property with the ID of thePropertyID to theValue.
     * @param[in] thePropertyID is the ID of the property to set.
     * @param[in] theValue is the value to set.
     */
    template<class TYPE>
    void set(const typePropertyID thePropertyID, TYPE theValue)
    {
        if(mList.find(thePropertyID) != mList.end())
        {
            if(mList.at(thePropertyID)->getType()->name() == typeid(TYPE).name())
            {
                static_cast<TProperty<TYPE>*>(mList[thePropertyID])->setValue(theValue);
            }
        }
        else
        {
            ELOG() << "PropertyManager:set() unable to find property("
                   << thePropertyID << ")" << std::endl;
        }
    }

    /**
     * Add creates a Property and addes it to this PropertyManager.
     * @param[in] thePropertyID is the ID of the property to create.
     * @param[in] theValue is the inital value to set.
     */
    template<class TYPE>
    void add(const typePropertyID thePropertyID, TYPE theValue)
    {
        // Only add the property if it doesn't already exist
        if(mList.find(thePropertyID) == mList.end())
        {
            TProperty<TYPE>* anProperty=new(std::nothrow) TProperty<TYPE>(thePropertyID);
            anProperty->setValue(theValue);
            mList[anProperty->getID()]=anProperty;
        }
    }

    /**
     * Add gets a premade Property and adds it to this PropertyManager.
     * @param[in] theProperty is a pointer to a pre exisiting property.
     */
    void add(IProperty* theProperty);

    /**
     * @brief remove 
     * @param typePropertyID
     */
    void remove(typePropertyID);

    /**
     * Clone is responsible for making a clone of each property in the
     * PropertyManager provided.
     * @param[in] thePropertyManager to clone into ourselves
     */
    void clone(const PropertyManager& thePropertyManager);
protected:

private:
    // Variables
    ///////////////////////////////////////////////////////////////////////////
    /// A map of all Properties available for this PropertyManager class
    std::map<const typePropertyID, IProperty*> mList;
}; // PropertyManager class
} // namespace GQE

#endif

/**
 * @class GQE::PropertyManager
 * @ingroup Entity
 * The PropertyManager class is responsible for providing management for
 * IProperty based classes used for an IEntity object or some other class.
 *
 * Copyright (c) 2010-2012 Jacob Dix
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
