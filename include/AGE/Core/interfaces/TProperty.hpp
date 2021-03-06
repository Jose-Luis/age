/**
 * Provides a system for storing custom property values.
 *
 * @file include/AGE/Core/interfaces/TProperty.hpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#ifndef TPROPERTY_HPP_INCLUDED
#define TPROPERTY_HPP_INCLUDED

#include <typeinfo>
#include <AGE/Core/interfaces/IProperty.hpp>
#include <AGE/Core/Core_types.hpp>

namespace AGE
{
  /// The Template version of the IProperty class for custom property values
  template<class TYPE=Uint32>
    class TProperty : public IProperty
  {
    public:
      /**
       * TProperty default constructor
       * @param[in] thePropertyID to use for this property
       */
      TProperty(const Id thePropertyID) :
        IProperty(typeid(TYPE).name(), thePropertyID)
      {
      }

      /**
       * GetValue will return the property value
       * @return the property value
       */
      TYPE getValue()
      {
        return mValue;
      }
      /**
       * @brief getPointer 
       * @return 
       */
      TYPE* getPointer()
      {
        return &mValue;
      }
      /**
       * SetValue will set the property value to the value
       * provided.
       */
      void setValue(TYPE& theValue)
      {
        // Use assignment operation to transfer theValue contents over
        mValue = theValue;
      }

      /**
       * Update will be called giving each property a chance to change itself
       * during the Update phase of the GameLoop (see IEntity::UpdateFixed)
       */
      void update()
      {
      }

      /**
       * MakeClone is responsible for creating a clone of this IProperty
       * derived class and returning it as part of the Prototype and Instance
       * system. The value of the Property will also be copied into the clone.
       * @return pointer to the IProperty derived class clone that was created
       */
      IProperty* makeClone()
      {
        TProperty<TYPE>* anProperty = new(std::nothrow) TProperty<TYPE>(getID());

        // Make sure new didn't fail before setting the value for this property
        if(NULL != anProperty)
        {
          anProperty->setValue(mValue);
        }

        // Return cloned anProperty or NULL if none was created
        return anProperty;
      }
    private:
      TYPE mValue;
  }; // class TProperty
} // namespace AGE
#endif // TPROPERTY_HPP_INCLUDED

/**
 * @class AGE::TProperty
 * @ingroup Entity
 * The TProperty class is a templatized version of the IProperty class that
 * makes it easy to create your own IProperty derived classes using a custom
 * value.
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

