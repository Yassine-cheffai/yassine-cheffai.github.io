#ifndef LIB_HOVERMENU_H__
#define LIB_HOVERMENU_H__

#include <functional>
#include "ge_sys_math.h"
#include "c4d_string.h"
#include "c4d_library.h"
#ifndef USE_API_MAXON
	#include "c4d_misc/datastructures/basearray.h"
#else
	#include "maxon/basearray.h"
#endif

/// @addtogroup group_hovermenulibrary HoverMenu Library
/// @ingroup group_library
/// @{
/// @since R17

/// HoverMenu library ID.
#define C4D_HOVERMENU_LIBRARY_ID		440000198

//class MenuConstruct;
class HoverMenu;

#define MENUCOMMAND			"CMD"

/// @addtogroup HOVERMENU_DELAY
/// @{
enum HOVERMENU_DELAY
{
	HOVERMENU_DELAY_LONG				=	1500,			///< A long delay of one and a half seconds before the HoverMenu opens.
	HOVERMENU_DELAY_SHORT				=	300				///< A short delay of @em 300ms before the HoverMenu opens.
};
/// @}

#define HOVERMENU_MOUSETRAVEL			10			///< The minimum distance the mouse must travel in pixels before a hover menu will not appear.

/// @addtogroup MENUFLAGS
/// @{
enum MENUFLAGS
{
	MENUFLAGS_0					=	0,						///< The state of the MenuEntry is Active.
	MENUFLAGS_DISABLED	=	(1 << 0),			///< The state of the MenuEntry is Disabled.
	MENUFLAGS_CHECKED		=	(1 << 1)			///< The state of the MenuEntry is Checked.
} ENUM_END_FLAGS(MENUFLAGS);
/// @}

/// @addtogroup HOVERKEY
/// @{
enum HOVERKEY
{
	HOVERKEY_LEFT			=	0,							///< The Left Mouse button is being held down.
	HOVERKEY_MIDDLE		=	1,							///< The Middle Mouse button is being held down.
	HOVERKEY_RIGHT		=	2								///< The Right Mouse button is being held down.
} ENUM_END_LIST(HOVERKEY);
/// @}

/// @addtogroup HOVERMENURESULT
/// @{
enum HOVERMENURESULT
{
	HOVERMENURESULT_CONTINUE	=	0,			///< Continue checking, the menu has not been used yet.
	HOVERMENURESULT_END				=	1,			///< The menu was used.
	HOVERMENURESULT_NOMENU		=	-1			///< There is no hover menu, skip testing.
} ENUM_END_LIST(HOVERMENURESULT);
/// @}

//----------------------------------------------------------------------------------------
/// Represents a single entry in the menu or a submenu with children.  Can hold the entire menu structure.
//----------------------------------------------------------------------------------------
class HoverMenuEntry
{
private:
	maxon::BaseArray<HoverMenuEntry>	_children;
	Int32	_id;
	String	_name;
	MENUFLAGS	_flags;
	std::function<Bool(Int32)>	_func;

	HoverMenuEntry &operator =(const HoverMenuEntry &p);

	void Construct1(Int32 id, const String &name, const std::function<Bool(Int32)> &func, MENUFLAGS flags);
	void Construct2(const String &name, const std::function<Bool(Int32)> &func, MENUFLAGS flags);
	void iSetFunction(const std::function<Bool(Int32)> &func);
public:
	//----------------------------------------------------------------------------------------
	/// Constructor to generate a simple separator entry.
	//----------------------------------------------------------------------------------------
	HoverMenuEntry();

	//----------------------------------------------------------------------------------------
	/// Constructor for a command entry, just pass the plugin ID.
	/// @param[in] id									The CommandData plugin ID for the entry.
	//----------------------------------------------------------------------------------------
	HoverMenuEntry(Int32 id);

	//----------------------------------------------------------------------------------------
	/// Constructor for simple ID and name entry.
	/// @param[in] id									The ID for the entry.
	/// @param[in] name								The name for the entry.
	/// @param[in] flags							The checked or disabled flags for the entry: @enumerateEnum{MENUFLAGS}
	//----------------------------------------------------------------------------------------
	HoverMenuEntry(Int32 id, const String &name, MENUFLAGS flags = MENUFLAGS_0);

	//----------------------------------------------------------------------------------------
	/// Constructor for simple menu entry based on the name. The ID is automatically generated.
	/// @param[in] name								The name for the entry.
	/// @param[in] flags							The checked or disabled flags for the entry: @enumerateEnum{MENUFLAGS}
	//----------------------------------------------------------------------------------------
	HoverMenuEntry(const String &name, MENUFLAGS flags = MENUFLAGS_0);

	//----------------------------------------------------------------------------------------
	/// Constructor for a standard menu entry.
	/// @param[in] id									The ID for the entry.
	/// @param[in] name								The name for the entry.
	/// @param[in] func								The function to be called should the menu entry be selected.
	/// @param[in] flags							The checked or disabled flags for the entry: @enumerateEnum{MENUFLAGS}
	//----------------------------------------------------------------------------------------
	template<typename FUNC> HoverMenuEntry(Int32 id, const String &name, const FUNC &func, MENUFLAGS flags = MENUFLAGS_0)
	{
		Construct1(id, name, func, flags);
	}

	//----------------------------------------------------------------------------------------
	/// Constructor for a standard menu entry. The ID is automatically generated.
	/// @param[in] name								The name for the entry.
	/// @param[in] func								The function to be called should the menu entry be selected.
	/// @param[in] flags							The checked or disabled flags for the entry: @enumerateEnum{MENUFLAGS}
	//----------------------------------------------------------------------------------------
	template<typename FUNC> HoverMenuEntry(const String &name, const FUNC &func, MENUFLAGS flags = MENUFLAGS_0)
	{
		Construct2(name, func, flags);
	}

	//----------------------------------------------------------------------------------------
	/// Constructor to generate a submenu.
	/// @param[in] id									The ID for the submenu.
	/// @param[in] name								The name for the submenu.
	/// @param[in] entry							The submenu entry.
	/// @param[in] flags							The checked or disabled flags for the submenu entry: @enumerateEnum{MENUFLAGS}
	//----------------------------------------------------------------------------------------
	HoverMenuEntry(Int32 id, const String &name, const HoverMenuEntry &entry, MENUFLAGS flags = MENUFLAGS_0);

	//----------------------------------------------------------------------------------------
	/// Constructor to generate a submenu with a single name.
	/// @param[in] name								TThe name for the submenu.
	/// @param[in] entry							The submenu entry.
	//----------------------------------------------------------------------------------------
	HoverMenuEntry(const String &name, const HoverMenuEntry &entry);

	//----------------------------------------------------------------------------------------
	/// Constructor using a BaseContainer.
	/// @param[in] bc									The container setup.
	//----------------------------------------------------------------------------------------
	HoverMenuEntry(const BaseContainer &bc);

	//----------------------------------------------------------------------------------------
	/// Copy constructor.
	/// @param[in] m									The menu entry to copy from.
	//----------------------------------------------------------------------------------------
	HoverMenuEntry(const HoverMenuEntry &m);

	/// Destructor
	~HoverMenuEntry() { Flush(); }

	//----------------------------------------------------------------------------------------
	/// Resets the contents of the menu entry to their default (blank) values.
	//----------------------------------------------------------------------------------------
	void Flush();

	//----------------------------------------------------------------------------------------
	/// Sets the function that the lambda will use.
	/// @param[in] func								The function (lambda) to invoke on the function being called.
	//----------------------------------------------------------------------------------------
	template <typename FUNC> void SetFunction(const FUNC &func)
	{
		iSetFunction(func);
	}

	//----------------------------------------------------------------------------------------
	/// Sets the flags for the entry (how it is displayed, checked, enabled state etc
	/// @param[in,out] flags					The menu flags: @enumerateEnum{MENUFLAGS}
	//----------------------------------------------------------------------------------------
	void SetFlags(MENUFLAGS flags);

	//----------------------------------------------------------------------------------------
	/// Sets the name (the visible string) for the menu entry.
	/// @param[in] name								The name to use.
	//----------------------------------------------------------------------------------------
	void SetName(const String &name);

	//----------------------------------------------------------------------------------------
	/// Changes the ID for the menu entry.
	/// @param[in] id									The new ID for the menu entry.
	//----------------------------------------------------------------------------------------
	void SetId(Int32 id);

	//----------------------------------------------------------------------------------------
	/// Adds a new menu entry as a child of the entry, turning the entry into a submenu.
	/// @param[in] p									The menu entry to add as a child of the entry.
	/// @return												@trueIfOtherwiseFalse{successful}
	//----------------------------------------------------------------------------------------
	Bool Append(const HoverMenuEntry &p);

	//----------------------------------------------------------------------------------------
	/// Removes a child entry (from a submenu) from the entry finding it by name.
	/// @note This searches recursively through all submenus under the entry.
	/// @param[in] name								The name to search for among the submenu entries.
	//----------------------------------------------------------------------------------------
	void RemoveChild(const String &name);

	//----------------------------------------------------------------------------------------
	/// Removes a child (submenu entry) from the entry from its ID.
	/// @note This searches recursively through all submenus under the entry.
	/// @param[in] id									The ID of the submenu entry to find and remove.
	//----------------------------------------------------------------------------------------
	void RemoveChild(Int32 id);

	//----------------------------------------------------------------------------------------
	/// Removes a specific submenu entry from under the entry.
	/// @note This searches recursively through all submenus under the entry.
	/// @param[in] child							The specific entry by pointer to remove.
	//----------------------------------------------------------------------------------------
	void RemoveChild(HoverMenuEntry *child);

	//----------------------------------------------------------------------------------------
	/// Inserts a submenu @p entry before a passed @p location pointer entry.
	/// @param[in] entry							The entry to add to the menu.
	/// @param[in] location						A pointer to the entry location to insert before.
	/// @return												@trueIfOtherwiseFalse{successful}
	//----------------------------------------------------------------------------------------
	Bool Insert(const HoverMenuEntry &entry, HoverMenuEntry *location);

	//----------------------------------------------------------------------------------------
	/// Inserts a submenu @p entry at a specific location under the entry by the passed @p index.
	/// @param[in] entry							The entry to add to the menu.
	/// @param[in] index							The index that the entry should be added at.
	/// @return												@trueIfOtherwiseFalse{successful}
	//----------------------------------------------------------------------------------------
	Bool Insert(const HoverMenuEntry &entry, Int index);

	//----------------------------------------------------------------------------------------
	/// Returns the number of submenu entries that belong to the submenu.
	/// @return												The number of submenu entries under the entry.
	//----------------------------------------------------------------------------------------
	Int GetChildCount();

	//----------------------------------------------------------------------------------------
	/// Gets (to modify) a specific submenu entry.
	/// @param[in] index							The index of the submenu entry to retrieve.
	/// @return												A reference to the submenu entry.
	//----------------------------------------------------------------------------------------
	const HoverMenuEntry &GetChild(Int index);

	//----------------------------------------------------------------------------------------
	/// Checks if the entry is a submenu or not.
	/// @return												@trueIfOtherwiseFalse{the entry is a submenu and has children}
	//----------------------------------------------------------------------------------------
	Bool IsSubmenu();

	//----------------------------------------------------------------------------------------
	/// Concatenates entries. Allows to join menu entries together into a single larger entry.
	/// @param[in] p									The next menu entry to join in.
	/// @return												The compound menu entry.
	//----------------------------------------------------------------------------------------
	HoverMenuEntry& Concatenate(const HoverMenuEntry &p);

	//----------------------------------------------------------------------------------------
	/// Concatenate operator. Allows to join menu entries together into a single larger entry.
	/// @param[in] p									The next menu entry to join in.
	/// @return												The compound menu entry.
	//----------------------------------------------------------------------------------------
	HoverMenuEntry& operator, (const HoverMenuEntry &p)
	{
		return Concatenate(p);
	}
};

//----------------------------------------------------------------------------------------
/// Shows a popup menu after a specified time when hovering a same location.
//----------------------------------------------------------------------------------------
class HoverMenu
{
private:
	void iSetFunction(const std::function<Bool(Int32)> &func);
public:
	//----------------------------------------------------------------------------------------
	/// Flushes the contents of the HoverMenu completely.
	//----------------------------------------------------------------------------------------
	void Flush();

	//----------------------------------------------------------------------------------------
	/// Initializes the HoverMenu with a passed Menu described by the HoverMenuEntry system.
	/// @param[in] menu								The entry for the menu.
	/// @param[in] delay							The delay before opening the menu: @enumerateEnum{HOVERMENU_DELAY}
	/// @return												@trueIfOtherwiseFalse{successful}
	//----------------------------------------------------------------------------------------
	Bool Init(const HoverMenuEntry &menu, HOVERMENU_DELAY delay = HOVERMENU_DELAY_LONG);

	//----------------------------------------------------------------------------------------
	/// Sets the global lambda function to be used on the various menu entries.
	/// @param[in] func								The function (lambda Bool(Int32)) used to evaluate what happens on menu selection.
	//----------------------------------------------------------------------------------------
	template <typename FUNC> void SetFunction(const FUNC& func)
	{
		iSetFunction(func);
	}

	//----------------------------------------------------------------------------------------
	/// Adds a new entry to the menu.
	/// @param[in] menu								The new menu entry.
	/// @return												@trueIfOtherwiseFalse{successful}
	//----------------------------------------------------------------------------------------
	Bool Append(const HoverMenuEntry &menu);

	//----------------------------------------------------------------------------------------
	/// Get the base level entry in the menu.
	/// @return												The root HoverMenuEntry that makes up the whole menu. @theOwnsPointed{HoverMenu, HoverMenuEntry}
	//----------------------------------------------------------------------------------------
	HoverMenuEntry* GetRootEntry();

	//----------------------------------------------------------------------------------------
	/// Finds and returns a menu entry in the current menu from its ID.
	/// @param[in] id									The ID to search for within the menu (recursively).
	/// @return												The HoverMenuEntry if found, or @c nullptr if it does not exist within the menu. @theOwnsPointed{HoverMenu, HoverMenuEntry}
	//----------------------------------------------------------------------------------------
	HoverMenuEntry* Find(Int32 id);

	//----------------------------------------------------------------------------------------
	/// Finds and returns a menu entry in the current menu from its name.
	/// @param[in] name								The name to search for within the menu (recursively).
	/// @return												The HoverMenuEntry if found, or @c nullptr if it does not exist within the menu. @theOwnsPointed{HoverMenu, HoverMenuEntry}
	//----------------------------------------------------------------------------------------
	HoverMenuEntry* Find(const String &name);

	//----------------------------------------------------------------------------------------
	/// Removes an entry from the menu from its ID.
	/// @param[in] id									The ID of the entry to remove.
	//----------------------------------------------------------------------------------------
	void RemoveEntry(Int32 id);

	//----------------------------------------------------------------------------------------
	/// Removes an entry from the menu from its name.
	/// @param[in] name								The name of the entry to remove.
	//----------------------------------------------------------------------------------------
	void RemoveEntry(const String &name);

	//----------------------------------------------------------------------------------------
	/// Changes the delay before the menu will popup when hovering.
	/// @param[in] delay							The delay in @em ms before the menu will open when the mouse hovers in a single location with the buttons held down: @enumerateEnum{HOVERMENU_DELAY}
	//----------------------------------------------------------------------------------------
	void SetDelay(HOVERMENU_DELAY delay);

	//----------------------------------------------------------------------------------------
	/// Gives the global menu a name.
	/// @param[in] name								The name (string) to use for the menu.
	//----------------------------------------------------------------------------------------
	void SetName(const String &name);

	//----------------------------------------------------------------------------------------
	/// Checks the menu, forcing it to update.
	/// @note Should be called during a polling session.
	/// @param[in] key								The mouse button being held down, right button will invoke the menu instantly: @enumerateEnum{HOVERKEY}
	/// @return												@trueOtherwiseFalse{the menu is opened}
	//----------------------------------------------------------------------------------------
	Bool CheckMenu(HOVERKEY key = HOVERKEY_LEFT);

	//----------------------------------------------------------------------------------------
	/// Forces to show the menu at the current mouse location, disables the menu for this mouse drag after its use (enable must be manually called to reinvoke it).
	/// @return												The ID of the menu entry picked, @em 0 if no entry was picked or the menu could not be shown.
	//----------------------------------------------------------------------------------------
	Int32 Show();

	//----------------------------------------------------------------------------------------
	/// Checks to see if the hover menu may still show, i.e. mouse has not moved during this drag and the delay has not timed out
	/// @return												@trueOtherwiseFalse{the mouse is still hovering}
	//----------------------------------------------------------------------------------------
	Bool IsInStaticHover();

	//----------------------------------------------------------------------------------------
	/// Disables the menu from the current or next mouse drag.
	//----------------------------------------------------------------------------------------
	void Disable();

	//----------------------------------------------------------------------------------------
	/// Enables the menu for the next mouse drag.
	//----------------------------------------------------------------------------------------
	void Enable();

	/// @name Alloc/Free
	/// @{

	//----------------------------------------------------------------------------------------
	/// @allocatesA{hover menu}
	/// @return												@allocReturn{hover menu}
	//----------------------------------------------------------------------------------------
	static HoverMenu* Alloc();

	//----------------------------------------------------------------------------------------
	/// @destructsAlloc{hover menus}
	/// @param[in] h									@theToDestruct{hover menu}
	//----------------------------------------------------------------------------------------
	static void Free(HoverMenu *&h);

	/// @}
};

//----------------------------------------------------------------------------------------
/// Gets the flash state for the HoverMenu.
/// @note This is used to e.g. highlight an element the cursor is hovering while the mouse is down before the menu is brought up to warn the user of this.
/// @return												@c false if the highlight element should be disabled, @c true if it should be enabled.
//----------------------------------------------------------------------------------------
Bool GetHoverMenuFlashState();

//----------------------------------------------------------------------------------------
/// Updates the hover menu state. Should be polled during long mouse routines to update the menu and bring it up or not.
/// @param[in] key								The mouse key being polled: @enumerateEnum{HOVERKEY}
/// @return												The hovermenu result: break out of the loop if the result is not @ref HOVERMENURESULT_CONTINUE. @enumerateEnum{HOVERMENURESULT}
//----------------------------------------------------------------------------------------
HOVERMENURESULT CheckHoverMenu(HOVERKEY key = HOVERKEY_LEFT);

//----------------------------------------------------------------------------------------
/// Gets the active (topmost) HoverMenu for modification, or just to see which one it is.
/// @return												The currently active hover menu if any are available at all.
//----------------------------------------------------------------------------------------
HoverMenu *GetActiveHoverMenu();

/// @}

/// @cond IGNORE

// INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF
// INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF
// INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF

class iHoverMenuEntry;
class iHoverMenu;

struct HoverMenuLib : public C4DLibrary
{
	void						(iHoverMenuEntry::*eConstructor1)();
	void						(iHoverMenuEntry::*eConstructor2)(Int32 id);
	void						(iHoverMenuEntry::*eConstructor3)(Int32 id, const String &name, MENUFLAGS flags);
	void						(iHoverMenuEntry::*eConstructor4)(const String &name, MENUFLAGS flags);
	void						(iHoverMenuEntry::*eConstructor5)(Int32 id, const String &name, const std::function<Bool(Int32)> &func, MENUFLAGS flags);
	void						(iHoverMenuEntry::*eConstructor6)(const String &name, const std::function<Bool(Int32)> &func, MENUFLAGS flags);
	void						(iHoverMenuEntry::*eConstructor7)(Int32 id, const String &name, const HoverMenuEntry &entry, MENUFLAGS flags);
	void						(iHoverMenuEntry::*eConstructor8)(const String &name, const HoverMenuEntry &entry);
	void						(iHoverMenuEntry::*eConstructor9)(const BaseContainer &bc);
	void						(iHoverMenuEntry::*eConstructor10)(const HoverMenuEntry &m);
	void						(iHoverMenuEntry::*eFlush)();
	void						(iHoverMenuEntry::*eSetFunction)(const std::function<Bool(Int32)> &func);
	void						(iHoverMenuEntry::*eSetFlags)(MENUFLAGS flags);
	void						(iHoverMenuEntry::*eSetName)(const String &name);
	void						(iHoverMenuEntry::*eSetId)(Int32 id);
	Bool						(iHoverMenuEntry::*eAppend)(const HoverMenuEntry &p);
	void						(iHoverMenuEntry::*eRemoveChildA)(const String &name);
	void						(iHoverMenuEntry::*eRemoveChildB)(Int32 id);
	void						(iHoverMenuEntry::*eRemoveChildC)(HoverMenuEntry *child);
	Bool						(iHoverMenuEntry::*eInsertA)(const HoverMenuEntry &entry, HoverMenuEntry *location);
	Bool						(iHoverMenuEntry::*eInsertB)(const HoverMenuEntry &entry, Int index);
	Int							(iHoverMenuEntry::*eGetChildCount)();
	const iHoverMenuEntry &(iHoverMenuEntry::*eGetChild)(Int index);
	Bool						(iHoverMenuEntry::*eIsSubmenu)();
	iHoverMenuEntry&	(iHoverMenuEntry::*eConcatenate)(const HoverMenuEntry &p);

	void						(iHoverMenu::*hmFlush)();
	Bool						(iHoverMenu::*hmInit)(const HoverMenuEntry &menu, HOVERMENU_DELAY delay);
	void						(iHoverMenu::*hmSetFunction)(const std::function<Bool(Int32)>& func);
	Bool						(iHoverMenu::*hmAppend)(const HoverMenuEntry &menu);
	iHoverMenuEntry*	(iHoverMenu::*hmGetRootEntry)();
	iHoverMenuEntry*	(iHoverMenu::*hmFindA)(Int32 id);
	iHoverMenuEntry*	(iHoverMenu::*hmFindB)(const String &name);
	void						(iHoverMenu::*hmRemoveEntryA)(Int32 id);
	void						(iHoverMenu::*hmRemoveEntryB)(const String &name);
	void						(iHoverMenu::*hmSetDelay)(HOVERMENU_DELAY delay);
	void						(iHoverMenu::*hmSetName)(const String &name);
	Bool						(iHoverMenu::*hmCheckMenu)(HOVERKEY key);
	Int32						(iHoverMenu::*hmShow)();
	Bool						(iHoverMenu::*hmIsInStaticHover)();
	void						(iHoverMenu::*hmDisable)();
	void						(iHoverMenu::*hmEnable)();

	iHoverMenu*			(*hmAlloc)();
	void						(*hmFree)(iHoverMenu *&h);

	Bool						(*gGetHoverMenuFlashState)();
	HOVERMENURESULT	(*gCheckHoverMenu)(HOVERKEY key);
	iHoverMenu*			(*gGetActiveHoverMenu)();
};

// INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF
// INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF
// INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF -- INTERNAL STUFF

/// @endcond

#endif // LIB_HOVERMENU_H__
