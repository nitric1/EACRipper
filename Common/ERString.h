#pragma once

class ERString
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~ERString() = 0 {}

public:
	virtual bool assign(const wchar_t *);
	virtual bool assign(ERString *);
	virtual bool append(const wchar_t *);
	virtual bool append(ERString *);
	virtual ERString *substr(size_t, size_t);
	virtual bool erase(size_t, size_t);

	virtual size_t getLength();
};
