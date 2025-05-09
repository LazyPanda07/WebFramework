#include "ChunksExecutor.h"

#include "TextGenerator.h"

void ChunksExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.sendChunks<TextGenerator>(response, "Some information here");
}

void ChunksExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.sendFileChunks<TextGenerator>(response, "file.txt", "Some information here");
}

DECLARE_EXECUTOR(ChunksExecutor)
