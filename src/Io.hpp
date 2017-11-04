#pragma once

struct rackIo
{
	float* audioIn;
	float* audioOut;
	unsigned int audioFrames;
	unsigned int audioInChannels;
	unsigned int audioOutChannels;
	unsigned int currentFrame;
};

extern struct rackIo* gRackIo;

