/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TaskInterface.h"

namespace GOAP
{
    class TaskRaceNeck
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskRaceNeck();
        ~TaskRaceNeck() override;

    protected:
        bool _onCheckRun( const NodeInterface * _node ) const override;
        bool _onCheckSkip( const NodeInterface * _node ) const override;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskRaceNeck> TaskRaceNeckPtr;
    //////////////////////////////////////////////////////////////////////////
}