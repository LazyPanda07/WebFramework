#pragma once

#include <fstream>
#include <filesystem>

#include <functional>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <memory>
#include <variant>
#include <optional>

#include <mutex>
#include <shared_mutex>
#include <thread>

#include <vector>
#include <unordered_map>
#include <array>
#include <queue>
#include <map>

#include <chrono>
#include <ctime>

#include <openssl/ssl.h>

#include "WebFrameworkConstants.h"

#include "JSONBuilder.h"
#include "JSONParser.h"
#include "JSONUtility.h"

#include "ThreadPool.h"
#include "SingletonThreadPool.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "HTTPNetwork.h"
#include "HTTPSNetwork.h"

#include "Log.h"

#include "SHA256.h"

#include "INIParser.h"

#include "BaseTCPServer.h"

#include "BaseIOSocketStream.h"

#include <Windows.h>
