#pragma once

class IEROption;
class IEROptionGroup;
class IEROptionItem;
class IEROptionItemCheck;
class IEROptionItemRadio;
class IEROptionItemText;
class IEROptionItemSelect;

class IEROption
{
public:
	virtual ~IEROption() = 0 {}

public:
	virtual bool addOptionGroup(IEROptionGroup *) = 0;
	virtual bool addOptionItem(IEROptionItem *) = 0;
};

class IEROptionGroup
{
public:
	virtual ~IEROptionGroup() = 0 {}

public:
	virtual bool addOptionItem(IEROptionItem *) = 0;
};

class IEROptionItem
{
public:
	virtual ~IEROptionItem() = 0 {}

public:
	virtual const wchar_t *getName() = 0;
};

class IEROptionItemCheck
{
public:
	virtual ~IEROptionItemCheck() = 0 {}
};
