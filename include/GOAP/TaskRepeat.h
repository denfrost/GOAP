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
    typedef IntrusivePtr<class WhileProvider> WhileProviderPtr;
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskRepeat
        : public Task
    {
        GOAP_DECLARE_VISITABLE( Task );

    public:
        TaskRepeat( const WhileProviderPtr & _providerRepeat, const SourcePtr & _until );
        ~TaskRepeat() override;

    public:
        bool _onRun() override;
        void _onFinalize() override;

    protected:
        WhileProviderPtr m_providerRepeat;
        SourcePtr m_sourceUntil;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskRepeat> TaskRepeatPtr;
    //////////////////////////////////////////////////////////////////////////
}