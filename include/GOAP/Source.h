/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Config.h"

#	include "GOAP/FunctionProvider.h"
#	include "GOAP/CallbackProvider.h"
#	include "GOAP/ScopeProvider.h"
#	include "GOAP/IfProvider.h"
#	include "GOAP/SwitchProvider.h"
#	include "GOAP/GuardProvider.h"

namespace GOAP
{
    typedef IntrusivePtr<class Factory> FactoryPtr;
    typedef IntrusivePtr<class Task> TaskPtr;
    typedef IntrusivePtr<class Chain> ChainPtr;
    typedef IntrusivePtr<class Source> SourcePtr;
    typedef IntrusivePtr<class Transcriptor> TranscriptorPtr;

    typedef Vector<SourcePtr> TVectorSources;
    typedef Vector<TranscriptorPtr> TVectorTranscriptor;

    struct IfSource
    {
        SourcePtr source_true;
        SourcePtr source_false;
    };

    class Source
        : public Factorable
    {
    public:
        Source( const FactoryPtr & _fatory );
        ~Source();

    public:
        const FactoryPtr & getFactory() const;

    public:
        void setSkip( bool _skip );
        bool isSkip() const;
        
    public:
        void addTask( const TaskPtr & _task );
        void addTask( const TypeId & _type, const Params & _params );

    public:
        TVectorSources & addParallel( size_t _count );
        TVectorSources & addRace( size_t _count );
        SourcePtr addFork();
        void addDeadLock();

        template<class F>
        void addFunction( F _f )
        {
            FunctionProviderPtr provider = new FunctionProviderT<F>( _f );

            this->addFunctionProvider( provider );
        }

        template<class F>
        void addCallback( F _f )
        {
            CallbackProviderPtr provider = new CallbackProviderT<F>( _f );

            this->addCallbackProvider( provider );
        }

        template<class F>
        void addScope( F _f )
        {
            ScopeProviderPtr provider = new ScopeProviderT<F>( _f );

            this->addScopeProvider( provider );
        }

        template<class F>
        IfSource addIf( F _f )
        {
            IfProviderPtr provider = new IfProviderT<F>( _f );

            IfSource desc = this->addIfProvider( provider );

            return desc;
        }

        template<class F>
        TVectorSources & addSwitch( size_t _count, F _f )
        {
            SwitchProviderPtr provider = new SwitchProviderT<F>( _f );

            TVectorSources & sources = this->addSwitchProvider( provider, _count );

            return sources;
        }

        template<class F>
        SourcePtr addRepeat( F _f )
        {
            ScopeProviderPtr provider = new ScopeProviderT<F>( _f );

            SourcePtr source = this->addRepeatProvider( provider );

            return source;
        }

        template<class FB, class FE>
        SourcePtr addGuard( FB _begin, FE _end )
        {
            GuardProviderPtr begin_provider = new GuardProviderT<FB>( _begin );
            GuardProviderPtr end_provider = new GuardProviderT<FE>( _end );

            SourcePtr source = this->addGuardProvider( begin_provider, end_provider );
        }

    public:
        void addFunctionProvider( const FunctionProviderPtr & _provider );
        void addCallbackProvider( const CallbackProviderPtr & _provider );
        void addScopeProvider( const ScopeProviderPtr & _provider );
        IfSource addIfProvider( const IfProviderPtr & _provider );
        SourcePtr addRepeatProvider( const ScopeProviderPtr & _provider );
        TVectorSources & addSwitchProvider( const SwitchProviderPtr & _provider, size_t _count );
        SourcePtr addGuardProvider( const GuardProviderPtr & _begin, const GuardProviderPtr & _end );

    public:
        TaskPtr parse( const ChainPtr & _chain, const TaskPtr & _task );

    protected:
        virtual SourcePtr _provideSource();

    protected:
        FactoryPtr m_factory;

        TVectorTranscriptor m_transcriptors;

        bool m_skip;
    };

    typedef IntrusivePtr<Source> SourcePtr;
}