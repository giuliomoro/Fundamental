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

void Bela_userSettings(BelaInitSettings *settings)
{
	settings->uniformSampleRate = 1;
	settings->interleave = 1;
	settings->analogOutputsPersist = 0;
}

bool setup(BelaContext *context, void *userData)
{
	bool ret = RackSetup(context->audioFrames, context->audioOutChannels + context->analogOutChannels, context->audioInChannels + context->analogInChannels);
	if(!ret)
		return false;
	if(context->audioFrames != gRackIo->audioFrames)
		return false;
	return true;
}

void render(BelaContext *context, void *userData)
{
	if(1)
	for(unsigned int n = 0; n < context->audioFrames; ++n)
	{
		for(unsigned int channel = 0; channel < context->audioInChannels; ++channel)
		{
			gRackIo->audioIn[n * gRackIo->audioInChannels + channel] = audioRead(context, n, channel);
		}

		for(unsigned int channel = 0; channel < context->analogInChannels; ++channel)
		{
			gRackIo->audioIn[n * gRackIo->audioInChannels + channel + context->audioInChannels] = analogRead(context, n, channel);
		}
	}
	for(unsigned int n = 0; n < context->audioFrames; ++n)
	{
		RackRender();
	}
	for(unsigned int n = 0; n < context->audioFrames; ++n)
	{
		for(unsigned int channel = 0; channel < context->audioOutChannels; ++channel)
		{
			audioWrite(context, n, channel, gRackIo->audioOut[n * gRackIo->audioOutChannels + channel]);
		}

		for(unsigned int channel = 0; channel < context->analogOutChannels; ++channel)
		{
			analogWriteOnce(context, n, channel, gRackIo->audioOut[n * gRackIo->audioOutChannels + channel + context->audioOutChannels]);
		}
	}
}

void cleanup(BelaContext *context, void *userData)
{
	RackCleanup();
}

