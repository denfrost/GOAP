/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Task.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    typedef IntrusivePtr<class IfProvider> IfProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskIf
        : public Task
    {
        GOAP_DECLARE_VISITABLE( Task );

    public:
        TaskIf( const IfProviderPtr & _provider, const SourceInterfacePtr & _sourceTrue, const SourceInterfacePtr & _sourceFalse );
        ~TaskIf() override;

    public:
        bool _onRun( NodeInterface * _task ) override;
        void _onFinally() override;

    protected:
        IfProviderPtr m_provider;

        SourceInterfacePtr m_sourceTrue;
        SourceInterfacePtr m_sourceFalse;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskIf> TaskIfPtr;
    //////////////////////////////////////////////////////////////////////////
}