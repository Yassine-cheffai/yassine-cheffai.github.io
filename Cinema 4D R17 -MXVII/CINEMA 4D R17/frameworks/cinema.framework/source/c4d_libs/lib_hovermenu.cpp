#include "lib_hovermenu.h"

static HoverMenuLib *lib_CloudObject = nullptr;

static HoverMenuLib *CheckLib(Int32 offset)
{
	return (HoverMenuLib*)CheckLib(C4D_HOVERMENU_LIBRARY_ID, offset, (C4DLibrary**)&lib_CloudObject);
}

#define HoverCall(b)       HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, b)); \
	if (!lib || !lib->b) return; \
	(((iHoverMenu*)this)->*lib->b)

#define HoverCallR(a, b)     HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, b)); \
	if (!lib || !lib->b) return a; \
	return (((iHoverMenu*)this)->*lib->b)

#define HoverCallREntry(b)     HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, b)); \
	if (!lib || !lib->b) return nullptr; \
	return (HoverMenuEntry*)(((iHoverMenu*)this)->*lib->b)

#define HoverEntryCall(b)       HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, b)); \
	if (!lib || !lib->b) return; \
	(((iHoverMenuEntry*)this)->*lib->b)

#define HoverEntryCallR(a, b)     HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, b)); \
	if (!lib || !lib->b) return a; \
	return (((iHoverMenuEntry*)this)->*lib->b)

HoverMenuEntry::HoverMenuEntry()
{
	HoverEntryCall(eConstructor1)();
}

HoverMenuEntry::HoverMenuEntry(Int32 id)
{
	HoverEntryCall(eConstructor2)(id);
}

HoverMenuEntry::HoverMenuEntry(Int32 id, const String &name, MENUFLAGS flags /*= MENUFLAGS_0*/)
{
	HoverEntryCall(eConstructor3)(id, name, flags);
}

HoverMenuEntry::HoverMenuEntry(const String &name, MENUFLAGS flags /*= MENUFLAGS_0*/)
{
	HoverEntryCall(eConstructor4)(name, flags);
}

HoverMenuEntry::HoverMenuEntry(Int32 id, const String &name, const HoverMenuEntry &entry, MENUFLAGS flags /*= MENUFLAGS_0*/)
{
	HoverEntryCall(eConstructor7)(id, name, entry, flags);
}

HoverMenuEntry::HoverMenuEntry(const String &name, const HoverMenuEntry &entry)
{
	HoverEntryCall(eConstructor8)(name, entry);
}

HoverMenuEntry::HoverMenuEntry(const BaseContainer &bc)
{
	HoverEntryCall(eConstructor9)(bc);
}

HoverMenuEntry::HoverMenuEntry(const HoverMenuEntry &m)
{
	HoverEntryCall(eConstructor10)(m);
}

void HoverMenuEntry::Flush()
{
	HoverEntryCall(eFlush)();
}

void HoverMenuEntry::SetFlags(MENUFLAGS flags)
{
	HoverEntryCall(eSetFlags)(flags);
}

void HoverMenuEntry::SetName(const String &name)
{
	HoverEntryCall(eSetName)(name);
}

void HoverMenuEntry::SetId(Int32 id)
{
	HoverEntryCall(eSetId)(id);
}

Bool HoverMenuEntry::Append(const HoverMenuEntry &p)
{
	HoverEntryCallR(false, eAppend)(p);
}

void HoverMenuEntry::RemoveChild(const String &name)
{
	HoverEntryCall(eRemoveChildA)(name);
}

void HoverMenuEntry::RemoveChild(Int32 id)
{
	HoverEntryCall(eRemoveChildB)(id);
}

void HoverMenuEntry::RemoveChild(HoverMenuEntry *child)
{
	HoverEntryCall(eRemoveChildC)(child);
}

Bool HoverMenuEntry::Insert(const HoverMenuEntry &entry, HoverMenuEntry *location)
{
	HoverEntryCallR(false, eInsertA)(entry, location);
}

Bool HoverMenuEntry::Insert(const HoverMenuEntry &entry, Int index)
{
	HoverEntryCallR(false, eInsertB)(entry, index);
}

Int HoverMenuEntry::GetChildCount()
{
	HoverEntryCallR(0, eGetChildCount)();
}

const HoverMenuEntry& HoverMenuEntry::GetChild(Int index)
{
	HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, eGetChild));
	return (HoverMenuEntry&)(((iHoverMenuEntry*)this)->*lib->eGetChild)(index);
}

Bool HoverMenuEntry::IsSubmenu()
{
	HoverEntryCallR(false, eIsSubmenu)();
}

HoverMenuEntry& HoverMenuEntry::Concatenate(const HoverMenuEntry &p)
{
	HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, eConcatenate));
	if (!lib || !lib->eConcatenate) return *this;
	return (HoverMenuEntry&)(((iHoverMenuEntry*)this)->*lib->eConcatenate)(p);
}

void HoverMenuEntry::Construct1(Int32 id, const String &name, const std::function<Bool(Int32)> &func, MENUFLAGS flags)
{
	HoverEntryCall(eConstructor5)(id, name, func, flags);
}

void HoverMenuEntry::Construct2(const String &name, const std::function<Bool(Int32)> &func, MENUFLAGS flags)
{
	HoverEntryCall(eConstructor6)(name, func, flags);
}

void HoverMenuEntry::iSetFunction(const std::function<Bool(Int32)> &func)
{
	HoverEntryCall(eSetFunction)(func);
}

//////////////////////////////////////////////////////////////////////////

void HoverMenu::Flush()
{
	HoverCall(hmFlush)();
}

Bool HoverMenu::Init(const HoverMenuEntry &menu, HOVERMENU_DELAY delay /*= HOVERMENU_DELAY_LONG*/)
{
	HoverCallR(false, hmInit)(menu, delay);
}

Bool HoverMenu::Append(const HoverMenuEntry &menu)
{
	HoverCallR(false, hmAppend)(menu);
}

HoverMenuEntry* HoverMenu::GetRootEntry()
{
	HoverCallREntry(hmGetRootEntry)();
}

HoverMenuEntry* HoverMenu::Find(Int32 id)
{
	HoverCallREntry(hmFindA)(id);
}

HoverMenuEntry* HoverMenu::Find(const String &name)
{
	HoverCallREntry(hmFindB)(name);
}

void HoverMenu::RemoveEntry(Int32 id)
{
	HoverCall(hmRemoveEntryA)(id);
}

void HoverMenu::RemoveEntry(const String &name)
{
	HoverCall(hmRemoveEntryB)(name);
}

void HoverMenu::SetDelay(HOVERMENU_DELAY delay)
{
	HoverCall(hmSetDelay)(delay);
}

void HoverMenu::SetName(const String &name)
{
	HoverCall(hmSetName)(name);
}

Bool HoverMenu::CheckMenu(HOVERKEY key /*= HOVERKEY_LEFT*/)
{
	HoverCallR(false, hmCheckMenu)(key);
}

Int32 HoverMenu::Show()
{
	HoverCallR(0, hmShow)();
}

Bool HoverMenu::IsInStaticHover()
{
	HoverCallR(false, hmIsInStaticHover)();
}

void HoverMenu::Disable()
{
	HoverCall(hmDisable)();
}

void HoverMenu::Enable()
{
	HoverCall(hmEnable)();
}

HoverMenu* HoverMenu::Alloc()
{
	HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, hmAlloc));
	if (!lib || !lib->hmAlloc) return nullptr;
	return (HoverMenu*)lib->hmAlloc();
}

void HoverMenu::Free(HoverMenu *&h)
{
	HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, hmFree));
	if (!lib || !lib->hmFree) return;
	lib->hmFree((iHoverMenu*&)h);
}

void HoverMenu::iSetFunction(const std::function<Bool(Int32)> &func)
{
	HoverCall(hmSetFunction)(func);
}

//////////////////////////////////////////////////////////////////////////

Bool GetHoverMenuFlashState()
{
	HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, gGetHoverMenuFlashState));
	if (!lib || !lib->gGetHoverMenuFlashState) return false;
	return lib->gGetHoverMenuFlashState();
}

HOVERMENURESULT CheckHoverMenu(HOVERKEY key)
{
	HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, gCheckHoverMenu));
	if (!lib || !lib->gCheckHoverMenu) return HOVERMENURESULT_CONTINUE;
	return lib->gCheckHoverMenu(key);
}

HoverMenu *GetActiveHoverMenu()
{
	HoverMenuLib *lib = CheckLib(LIBOFFSET(HoverMenuLib, gGetActiveHoverMenu));
	if (!lib || !lib->gGetActiveHoverMenu) return nullptr;
	return (HoverMenu*)lib->gGetActiveHoverMenu();
}
