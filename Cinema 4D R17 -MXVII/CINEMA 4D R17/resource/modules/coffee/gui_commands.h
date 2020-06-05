enum
{
	DIALOG_PIN = 1,						// LONG flags
	DIALOG_CHECKBOX,					// LONG id, LONG flags, String *name
	DIALOG_STATICTEXT,				// LONG id, LONG flags, String *name, LONG borderstyle
	DIALOG_BUTTON,						// LONG id, LONG flags, String *name
	DIALOG_ARROWBUTTON,				// LONG id, LONG flags, LONG arrowtype
	DIALOG_EDITTEXT,					// LONG id, LONG flags
	DIALOG_EDITNUMBER,				// LONG id, LONG flags
	DIALOG_EDITNUMBERUD,			// LONG id, LONG flags
	DIALOG_EDITSLIDER,				// LONG id, LONG flags
	DIALOG_SLIDER,						// LONG id, LONG flags
	DIALOG_COLORFIELD,				// LONG id, LONG flags
	DIALOG_COLORCHOOSER,			// LONG id, LONG flags
	DIALOG_USERAREA,					// LONG id, LONG flags
	DIALOG_RADIOGROUP,				// LONG id, LONG flags
	DIALOG_COMBOBOX,					// LONG id, LONG flags
	DIALOG_POPUPBUTTON,				// LONG id, LONG flags
	DIALOG_CHILD,							// LONG id, LONG subid, String *child);
	DIALOG_FREECHILDREN,			// LONG id
	DIALOG_DLGGROUP,					// LONG flags
	DIALOG_SETTITLE,					// String *name
	DIALOG_GROUPSPACE,				// spacex,spacey
	DIALOG_GROUPBORDER,				// borderstyle
	DIALOG_GROUPBORDERSIZE,	  // left, top, right, bottom
	DIALOG_SETIDS,						// left, top, right, bottom
	DIALOG_LAYOUTCHANGED,			// relayout dialog
	DIALOG_ACTIVATE,					// activate gadget
	DIALOG_ADDSUBMENU,				// add submenu, text
	DIALOG_ENDSUBMENU,				// add submenu, text
	DIALOG_ADDMENUCMD,				// add menutext, text, cmdid
	DIALOG_FLUSHMENU,					// delete all menu entries
	DIALOG_INIT,							// create new layout
	DIALOG_CHECKNUMBERS,			// range check of all dialog elements
	DELME_DIALOG_SETGROUP,					// set group as insert group
	DIALOG_FLUSHGROUP,				// flush all elements of this group and set insert point to this group
	DIALOG_SETMENU,						// set and display menu in manager
	DIALOG_SCREEN2LOCALX,			// Screen2Local coordinates
	DIALOG_SCREEN2LOCALY,			// Screen2Local coordinates
	DIALOG_ADDMENUSTR,				// add menutext, text, id
	DIALOG_RADIOBUTTON,				// LONG id, LONG flags, String *name
	DIALOG_ADDMENUSEP,				// add menu separator
	DIALOG_SEPARATOR,					// add separator
	DIALOG_MULTILINEEDITTEXT, // add multiline editfield
	DIALOG_INITMENUSTR,				// enable/disable/check/uncheck menustrings
	DIALOG_RADIOTEXT,					// radio text like in material manager
	DIALOG_MENURESOURCE,			// private for painter
	DIALOG_LISTVIEW,					// LONG id, LONG flags
	DIALOG_SUBDIALOG,					// LONG id, LONG flags
	DIALOG_CHECKCLOSE,				// CheckCloseMessage for dialog
	DIALOG_GETTRISTATE,				// get TriState-flag of the specified gadget
	DIALOG_SDK,								// LONG id, LONG flags, String *name
	DIALOG_SCROLLGROUP,				// create ScrollGroup
	DIALOG_ISOPEN,						// returns TRUE if the dialog is open
	DIALOG_REMOVEGADGET,			// removes an element from the layout
	DIALOG_MENUGROUPBEGIN,
	DIALOG_NOMENUBAR,					// removes the menubar
	DIALOG_SAVEWEIGHTS,				// store the weights of a group
	DIALOG_LOADWEIGHTS,				// restore the weights of a group
	DIALOG_EDITSHORTCUT,			// editfield to enter shortcuts
	DIALOG_ISVISIBLE,					// returns TRUE if the dialog is visible (e.g. FALSE if tabbed behind)
	DIALOG_HIDEELEMENT,				// TRUE - hides the element
	// R12 news
	DIALOG_COMBOBUTTON,				// ComboButton

	DIALOG_ // Dummy ...
};
