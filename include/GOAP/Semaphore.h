/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Config.h"

#   include "GOAP/Event.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class Semaphore
        : public Factorable
    {
    public:
        Semaphore( int32_t _value );
        ~Semaphore();

    public:
        void setValue( int32_t _value );
        int32_t getValue() const;
        
    public:
        void subtractValue( int32_t _value );
        void addValue( int32_t _value );
        bool equalValue( int32_t _value ) const;
        bool lessValue( int32_t _value ) const;

    public:
        const EventProviderPtr & addObserver( const EventProviderPtr & _event );
        void removeObserver( const EventProviderPtr & _event );

    protected:
        EventPtr m_event;
        int32_t m_value;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Semaphore> SemaphorePtr;
    //////////////////////////////////////////////////////////////////////////
}