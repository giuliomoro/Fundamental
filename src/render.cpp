/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

The platform for ultra-low latency audio and sensor processing

http://bela.io

A project of the Augmented Instruments Laboratory within the
Centre for Digital Music at Queen Mary University of London.
http://www.eecs.qmul.ac.uk/~andrewm

(c) 2016 Augmented Instruments Laboratory: Andrew McPherson,
	Astrid Bin, Liam Donovan, Christian Heinrichs, Robert Jack,
	Giulio Moro, Laurel Pardue, Victor Zappi. All rights reserved.

The Bela software is distributed under the GNU Lesser General Public License
(LGPL 3.0), available here: https://www.gnu.org/licenses/lgpl-3.0.txt
*/

#include <Bela.h>
#include "RackWrapper.h"
#include "Io.hpp"
#include <string.h>

bool setup(BelaContext *context, void *userData)
{
	bool ret = RackSetup(context->audioFrames, 2, 2);
	if(!ret)
		return false;
	if(context->audioFrames != gRackIo->audioFrames)
		return false;
	if(context->audioOutChannels != gRackIo->audioOutChannels)
		return false;
	return true;
}

void render(BelaContext *context, void *userData)
{
	memcpy((void*)gRackIo->audioIn, (void*)context->audioIn, sizeof(context->audioIn[0]) * context->audioFrames * context->audioInChannels);
	for(unsigned int n = 0; n < context->audioFrames; ++n)
	{
		RackRender();
	}
	memcpy((void*)context->audioOut, (void*)gRackIo->audioOut, sizeof(context->audioOut[0]) * context->audioFrames * context->audioOutChannels);
}

void cleanup(BelaContext *context, void *userData)
{
	RackCleanup();
}

