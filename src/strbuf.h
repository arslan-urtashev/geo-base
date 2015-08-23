#pragma once

namespace troll {

struct strbuf_t {
public:
	strbuf_t(char const *str = nullptr, size_t count = 0)
		: str(str)
		, count(count)
	{
	}

	char const *data() const
	{
		return str;
	}

	size_t length() const
	{
		return count;
	}

private:
	char const *str;
	size_t count;
};

}
