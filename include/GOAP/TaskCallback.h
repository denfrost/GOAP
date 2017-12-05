/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Task.h"

#	include "GOAP/CallbackProvider.h"

namespace GOAP
{
    class TaskCallback
        : public Task
        , public CallbackObserver
    {
    public:
        explicit TaskCallback( const CallbackProviderPtr & _provider );
        ~TaskCallback() override;

    public:
        bool _onRun() override;
        void _onFinally() override;

    protected:
        void onCallback( bool _skip ) override;

    protected:
        CallbackProviderPtr m_provider;
    };
}