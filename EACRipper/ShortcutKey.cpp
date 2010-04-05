#include "Defaults.h"

#include "ShortcutKey.h"

using namespace std;

namespace EACRipper
{
	ShortcutKey::~ShortcutKey()
	{
	}

	ShortcutKey::Modifier ShortcutKey::getModifier(bool ctrl, bool alt, bool shift) const
	{
		if(ctrl && shift && alt)
			return Ctrl_Shift_Alt;
		else if(ctrl && shift)
			return Ctrl_Shift;
		else if(ctrl && alt)
			return Ctrl_Alt;
		else if(ctrl)
			return Ctrl;
		else if(alt && shift)
			return Alt_Shift;
		else if(alt)
			return Alt;
		return None;
	}

	bool ShortcutKey::addShortcut(const WindowBase *window, uint32_t id, Key key, bool canExtended, bool canRepeated)
	{
		Command cmd;
		cmd.canExtended = canExtended;
		cmd.canRepeated = canRepeated;
		cmd.id = id;

		return keyMap.insert(make_pair(make_pair(key, window), cmd)).second;
	}

	bool ShortcutKey::modifyShortcut(const WindowBase *window, uint32_t id, Key key, bool canExtended, bool canRepeated)
	{
		if(!removeShortcut(window, id))
			return false;
		return addShortcut(window, id, key, canExtended, canRepeated);
	}

	bool ShortcutKey::removeShortcut(const WindowBase *window, uint32_t id)
	{
		for(map<pair<Key, const WindowBase *>, Command>::iterator it = keyMap.begin();
			it != keyMap.end();
			++ it)
		{
			if(it->first.second == window && it->second.id == id)
			{
				keyMap.erase(it);
				return true;
			}
		}

		return false;
	}

	uint32_t ShortcutKey::processShortcut(const WindowBase *window, Key key, bool extended, bool repeated) const
	{
		map<pair<Key, const WindowBase *>, Command>::const_iterator it = keyMap.find(make_pair(key, window));
		if(it == keyMap.end())
			return 0;

		if((extended && !it->second.canExtended) || (repeated && !it->second.canRepeated))
			return 0;

		return it->second.id;
	}

	bool ShortcutKey::processKeydownMessage(const WindowBase *window, WPARAM wParam, LPARAM lParam) const
	{
		bool ctrl, alt, shift;
		ctrl = (GetKeyState(VK_CONTROL) < 0);
		alt = (GetKeyState(VK_MENU) < 0);
		shift = (GetKeyState(VK_SHIFT) < 0);

		Key key;
		key.modifier = getModifier(ctrl, alt, shift);
		key.key = static_cast<uint32_t>(wParam);

		uint32_t id = processShortcut(window, key, ((lParam & 0x01000000) == 0x01000000), ((lParam & 0x0000FFFF) > 0));
		if(id == 0)
			return false;

		return PostMessageW(window->getWindow(), WM_COMMAND, id, 0) != FALSE;
	}
}
