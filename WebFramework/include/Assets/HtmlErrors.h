#pragma once

#include <string_view>

namespace framework::asset
{
	constexpr std::string_view getBadRequestError() noexcept;

	constexpr std::string_view getForbiddenError() noexcept;

	constexpr std::string_view getNotFoundError() noexcept;

	constexpr std::string_view getInternalServerError() noexcept;

	constexpr std::string_view getBadGatewayError() noexcept;
}

namespace framework::asset
{
	inline constexpr std::string_view getBadRequestError() noexcept
	{
		return R"(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>400 Bad Request</title>
</head>

<body>

<h1>Bad Request</h1>

</body>

</html>
)";
	}

	inline constexpr std::string_view getForbiddenError() noexcept
	{
		return R"(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>403 Forbidden</title>
</head>

<body>

<h1>Forbidden</h1>

</body>

</html>
)";
	}

	inline constexpr std::string_view getNotFoundError() noexcept
	{
		return R"(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>404 Not Found</title>
</head>

<body>

<h1>Page Not Found</h1>

</body>

</html>
)";
	}

	inline constexpr std::string_view getInternalServerError() noexcept
	{
		return R"(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>500 Internal Server Error</title>
</head>

<body>

    <h1>Internal Server Error</h1>

</body>

</html>
)";
	}

	inline constexpr std::string_view getBadGatewayError() noexcept
	{
		return R"(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>502 Bad Gateway</title>
</head>

<body>

    <h1>Bad Gateway</h1>

</body>

</html>
)";
	}
}
