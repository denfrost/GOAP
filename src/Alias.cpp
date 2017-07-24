/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/Alias.h"

#   include "GOAP/Source.h"
#   include "GOAP/Chain.h"

#   include "GOAP/Exception.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
    bool Alias::_onRun()
    {
        ChainPtr chain = m_chain;

        GOAP::SourcePtr source = new GOAP::Source();

        bool skiped = this->isSkip();
        source->setSkip( skiped );

        this->_onGenerate( source );

        if( this->injectSource( source ) == false )
        {
            Helper::throw_exception( "Alias invalid inject source" );
        }

        return true;
    }
}