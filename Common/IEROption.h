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
	virtual const wchar_t *name() = 0;
	virtual const wchar_t *type() { return L"Item"; }
	virtual const wchar_t *value() = 0;
	virtual const wchar_t *label() = 0;
	virtual const wchar_t *description() = 0;
};

class IEROptionItemCheck : public IEROptionItem
{
public:
	virtual ~IEROptionItemCheck() = 0 {}

public:
	virtual const wchar_t *getType() { return L"Check"; }
};

class IEROptionItemRadio : public IEROptionItem
{
public:
	virtual ~IEROptionItemRadio() = 0 {}

public:
	virtual const wchar_t *getType() { return L"Radio"; }
};

class IEROptionItemText : public IEROptionItem
{
public:
	virtual ~IEROptionItemText() = 0 {}

public:
	virtual const wchar_t *getType() { return L"Text"; }
};

class IEROptionItemSelect : public IEROptionItem
{
public:
	virtual ~IEROptionItemSelect() = 0 {}

public:
	virtual const wchar_t *getType() { return L"Select"; }
};
