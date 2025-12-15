#include "ChunksExecutor.h"

#include "TextGenerator.h"

void ChunksExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.sendChunks<TextGenerator>(response, "Some information here");
}

void ChunksExecutor::doPost(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.sendFileChunks<TextGenerator>(response, "file.txt", "Some information here");
}

DEFINE_EXECUTOR(ChunksExecutor)
