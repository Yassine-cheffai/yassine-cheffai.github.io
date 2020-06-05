#include "c4d_help.h"
#include "operatingsystem.h"
#include "basearray.h"
#include "c4d_general.h"

static maxon::BaseArray<PluginHelpDelegate> g_helpDelegates;
static PluginHelpDelegate g_helpBrowser = nullptr;

static Bool DispatchHelpRequest(const String& opType, const String& baseType, const String& group, const String& property)
{
	for (Int i = 0; i < g_helpDelegates.GetCount(); i++)
	{
		if (g_helpDelegates[i](opType, baseType, group, property))
			return true;
	}

	return g_helpBrowser(opType, baseType, group, property);
}

void FreeHelpDelegates()
{
	g_helpDelegates.Reset();
}

Bool RegisterPluginHelpDelegate(Int32 pluginId, PluginHelpDelegate delegate)
{
	if (GetC4DVersion() < 15043)
	{
		// C4D help support for pre 15.043 versions. Using this code path will not work with R16.
		if (g_helpDelegates.Append(delegate) == nullptr)
			return false;

		if (g_helpBrowser == nullptr)
		{
			g_helpBrowser = C4DOS.Ge->CallHelpBrowser;
			C4DOS.Ge->CallHelpBrowser = DispatchHelpRequest;
		}
		return true;
	}

	// Register delegate in the help system.
	return C4DOS.Ge->RegisterPluginHelpDelegate(pluginId, delegate);
}

void OpenHelpBrowser(const String& opType, const String& baseType, const String& group, const String& property)
{
	if (GetC4DVersion() < 15043)
	{
		// C4D help support for pre 15.043 versions. Using this code path will not work with R16.
		C4DOS.Ge->CallHelpBrowser(opType, baseType, group, property);
		return;
	}

	// Open the help browser for the indicated topic.
	C4DOS.Ge->OpenHelpBrowser(opType, baseType, group, property);
}
