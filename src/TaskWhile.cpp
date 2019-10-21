/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskWhile.h"
#include "GOAP/Source.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskWhile::TaskWhile( const WhileProviderPtr & _provider )
        : m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskWhile::~TaskWhile()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskWhile::_onRun( NodeInterface * _task )
    {
        SourcePtr source = _task->makeSource();

        bool injecting = m_provider->onWhile( source );

        if( injecting == true )
        {
            source->addWhileProvider( m_provider );

            const SourceProviderInterfacePtr & provider = source->getSourceProvider();

            if( _task->injectSource( provider ) == false )
            {
                Helper::throw_exception( "TaskWhile invalid inject source" );
            }
        }

        m_provider = nullptr;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskWhile::_onFastSkip()
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskWhile::_onFinalize()
    {
        m_provider = nullptr;
    }
}
