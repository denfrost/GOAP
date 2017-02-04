#	include "GOAP/TaskCallback.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskCallback::TaskCallback( const CallbackProviderPtr & _provider )
        : Task( TASK_EVENT_RUN )
		, m_provider( _provider )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskCallback::~TaskCallback()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskCallback::_onRun()
	{
		bool skip = this->isSkip();

		m_provider->onCallback( this, skip );

		return false;
	}	
	//////////////////////////////////////////////////////////////////////////
	void TaskCallback::onCallback( bool _skip )
	{
		this->complete( true, _skip );
	}
}