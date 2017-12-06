/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Semaphore.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Semaphore::Semaphore( int32_t _value )
        : m_value( _value )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Semaphore::~Semaphore()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Semaphore::setValue( int32_t _value )
    {
        m_value = _value;
    }
    //////////////////////////////////////////////////////////////////////////
    int32_t Semaphore::getValue() const
    {
        return m_value;
    }
    //////////////////////////////////////////////////////////////////////////
    void Semaphore::subtractValue( int32_t _value )
    {
        m_value -= _value;

        m_event->call();
    }
    //////////////////////////////////////////////////////////////////////////
    void Semaphore::addValue( int32_t _value )
    {
        m_value += _value;

        m_event->call();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Semaphore::equalValue( int32_t _value ) const
    {
        return m_value == _value;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Semaphore::lessValue( int32_t _value ) const
    {
        return m_value < _value;
    }
    //////////////////////////////////////////////////////////////////////////
    const EventProviderPtr & Semaphore::addObserver( const EventProviderPtr & _event )
    {
        m_event->addObserver( _event );

        return _event;
    }
    //////////////////////////////////////////////////////////////////////////
    void Semaphore::removeObserver( const EventProviderPtr & _event )
    {
        m_event->removeObserver( _event );
    }
    //////////////////////////////////////////////////////////////////////////
}