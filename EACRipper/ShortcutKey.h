#pragma once

#include <cinttypes>

#include <map>

#include "Singleton.h"
#include "Window.h"

namespace EACRipper
{
	class ShortcutKey : public Singleton<ShortcutKey>
	{
	public:
		typedef enum _Modifier
		{
			None,
			Ctrl,
			Alt,
			Ctrl_Alt,
			Ctrl_Shift,
			Alt_Shift,
			Ctrl_Shift_Alt
		} Modifier;

		typedef struct _Key
		{
			Modifier modifier;
			uint32_t key;

			bool operator <(const struct _Key &rhs) const
			{
				return modifier < rhs.modifier || (modifier == rhs.modifier && key < rhs.key);
			}
		} Key;

		private:
		typedef struct _ConfigureData
		{
			typedef struct _ConfigureField
			{
			} ConfigureField;

			uint32_t size;
			ConfigureField f[1];
		} ConfigureData;

	private:
		typedef struct _Command
		{
			bool canExtended;
			bool canRepeated;
			uint32_t id;
		} Command;

	private:
		std::map<std::pair<Key, const Window *>, Command> keyMap;

	private:
		ShortcutKey();
		~ShortcutKey();

	public:
		Modifier getModifier(bool, bool, bool) const;
		bool addShortcut(const Window *, uint32_t, Key, bool, bool);
		bool modifyShortcut(const Window *, uint32_t, Key, bool, bool);
		bool removeShortcut(const Window *, uint32_t);
		uint32_t processShortcut(const Window *, Key, bool, bool) const;
		bool processKeydownMessage(const Window *, WPARAM, LPARAM) const;

		friend class Singleton<ShortcutKey>;
	};
}
