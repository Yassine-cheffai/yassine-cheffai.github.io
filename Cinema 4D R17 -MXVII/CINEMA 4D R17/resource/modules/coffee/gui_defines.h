enum
{
	GUI_H_
};

enum
{
	FONT_DEFAULT		= 0,
	FONT_STANDARD		= 1,
	FONT_BOLD				= 2,
	FONT_MONOSPACED	= 3
};

// COFFEE qualifiers
enum
{
	QSHIFT 		= 1,
	QCTRL  		= 2,
	QALT   		= 4,
	QALT2  		= 8,
	QQUAL_

};

// COFFEE keys
enum
{
	KEY_MLEFT			= 0xF000,
	KEY_MRIGHT		= 0xF001,
	KEY_MMIDDLE		= 0xF002,
	KEY_MX1				= 0xF003,
	KEY_MX2				= 0xF004,

	// modifier keys
	KEY_SHIFT			= 0xF010,
	KEY_CONTROL		= 0xF011,
	KEY_ALT				= 0xF012,
	KEY_CAPSLOCK	= 0xF013,
	KEY_MODIFIERS = 0xF014,
	KEY_COMMAND		= 0xF015,

	KEY_BACKSPACE	= 0xF108,																		// backspace key
	KEY_TAB				= 0xF109,																		// tab key
	KEY_ENTER			= 0xF10D,																		// enter key
	KEY_ESC				= 0xF11B,																		// escape key
	KEY_SPACE			= 0xF120,																		// spacebar key
	KEY_DELETE		= 0xF17F,																		// delete key

	KEY_UP				= 0xF180,																		// up arrow key
	KEY_DOWN			= 0xF181,																		// down arrow key
	KEY_LEFT			= 0xF182,																		// left arrow key
	KEY_RIGHT			= 0xF183,																		// right arrow key
	KEY_PGUP			= 0xF184,																		// page up key
	KEY_PGDOWN		= 0xF185,																		// page down key
	KEY_HOME			= 0xF186,																		// home key
	KEY_END				= 0xF187,																		// end key
	KEY_INSERT		= 0xF188,																		// insert key

	// function keys
	KEY_F1				= 0xF1A0,
	KEY_F2				= 0xF1A1,
	KEY_F3				= 0xF1A2,
	KEY_F4				= 0xF1A3,
	KEY_F5				= 0xF1A4,
	KEY_F6				= 0xF1A5,
	KEY_F7				= 0xF1A6,
	KEY_F8				= 0xF1A7,
	KEY_F9				= 0xF1A8,
	KEY_F10				= 0xF1A9,
	KEY_F11				= 0xF1AA,
	KEY_F12				= 0xF1AB,
	KEY_F13				= 0xF1AC,
	KEY_F14				= 0xF1AD,
	KEY_F15				= 0xF1AE,
	KEY_F16				= 0xF1AF,
	KEY_F17				= 0xF1B0,
	KEY_F18				= 0xF1B1,
	KEY_F19				= 0xF1B2,
	KEY_F20				= 0xF1B3,
	KEY_F21				= 0xF1B4,
	KEY_F22				= 0xF1B5,
	KEY_F23				= 0xF1B6,
	KEY_F24				= 0xF1B7,
	KEY_F25				= 0xF1B8,
	KEY_F26				= 0xF1B9,
	KEY_F27				= 0xF1BA,
	KEY_F28				= 0xF1BB,
	KEY_F29				= 0xF1BC,
	KEY_F30				= 0xF1BD,
	KEY_F31				= 0xF1BE,
	KEY_F32				= 0xF1BF
};
enum
{
	BfBLACK			= 0,
	BfWHITE			= 1,
	BfLTGRAY		= 2,
	BfLTRED			= 3,
	BfLTGREEN		= 4,
	BfLTBLUE		= 5,
	BfLTYELLOW	= 6,
	BfGRAY			= 7,
	BfRED				= 8,
	BfGREEN			= 9,
	BfBLUE			= 10,
	BfYELLOW		= 11,
	BfDGRAY			= 12,
	BfDRED			= 13,
	BfDGREEN		= 14,
	BfDBLUE			= 15,
	BfDYELLOW		= 16,
	BfDWHITE		= 17,
	BfORANGE		= 18,
	BfBROWN			= 19,
	BfPURPLE		= 20
};

enum
{
	// bitmap modes
	BMP_NORMAL		   				= 0,
	BMP_NORMALSCALED 				= 1,
	BMP_EMBOSSED	  				= 3,
	BMP_ALLOWALPHA					= 256, // bitmask
	BMP_APPLY_COLORPROFILE	= 512,
	BMP_DIMIMAGE						= 1024,
	BMP_MIRROR_H						= 2048, // force bitmap mirror (even in non-r2l layouts)
	BMP_MIRROR_H_FORBID			= 4096,	// forbid automatic mirroring (in r2l layouts)
	BMP_MIRROR_V						= 8192, // force bitmap mirror
	BMP_MIRROR_V_FORBID			= 16384
};

// flags for layout - bf_flags
enum
{
	BFV_GRIDGROUP_EQUALCOLS 				= 1,		// each column has the same width
	BFV_GRIDGROUP_EQUALROWS 				= 2,		// each row has the same height
	BFV_CMD_EQUALCOLUMNS						= 2048, // same column width
	BFV_BORDERGROUP_CHECKBOX				= 4096, // checkbox in title
	BFV_BORDERGROUP_FOLD						= 8192, // fold symbol in title of a bordergroup
	BFV_BORDERGROUP_FOLD_OPEN				= 16384, // fold symbol in title of a bordergroup group is opened, otherwise closed
	BFV_BORDERGROUP_FOLD2						= 32768, // foldable group, but NO switchgadget
	BFV_GRIDGROUP_ALLOW_WEIGHTS			= 65536, // allow the user to move the weights
	BFV_GRIDGROUP_FORBID_MIRROR			= 131072, // dont mirror the layout of this group

	BFV_DIALOG_REMOVEABLE   				= 4,		// dialog is removeable
	BFV_DIALOG_BAR_VERT     				= 8,		// dialog has a vertical dialogbar
	BFV_DIALOG_NOBUTTONS						= 16,		// no button bar

	//BFV_GRIDGROUP_LAYOUTER				= 256,	// mark a layouting frame
	BFV_LAYOUTGROUP_PALETTEOUTLINES = 512,
	BFV_IGNORE_FOCUS                = 1024,

	BFV_TABGROUP_RELOADDIALOG       = 2048, // hack
	BFV_LAYOUTGROUP_NODROP					= 4096,
	BFV_LAYOUTGROUP_NODROP2					= 8192,

	BFx_NOEQUAL			= 64,

	BFV_
};

enum //CommandResourceObj
{
	RESOURCE_MENU				= 'rmnu',	// GetCResource();
	RESOURCE_DIALOG			= 'rdlg',	// GetCResource();
	RESOURCE_CONTAINER  = 'rcnt'  // GetCResource();
};

// menu resource object
enum
{
	MENURESOURCE_SUBMENU			= 1,
	MENURESOURCE_SEPERATOR		= 2,
	MENURESOURCE_COMMAND			= 3,
	MENURESOURCE_SUBTITLE			= 4,
	MENURESOURCE_STRING				= 5,
	MENURESOURCE_MENURESOURCE = 6
};

// border types
enum
{
	BORDER_NONE								= 0,
	BORDER_THIN_IN						= 1,
	BORDER_THIN_OUT						= 2,
	BORDER_IN									= 3,
	BORDER_OUT								= 4,
	BORDER_GROUP_IN						= 5,
	BORDER_GROUP_OUT					= 6,
	BORDER_OUT2								= 7,
	BORDER_OUT3								= 8,
	BORDER_BLACK							= 9,
	BORDER_ACTIVE_1						= 10,
	BORDER_ACTIVE_2						= 11,
	BORDER_GROUP_TOP					= 12,
	BORDER_ACTIVE_3						= 13,
	BORDER_ACTIVE_4						= 14,
	BORDER_ROUND							= 15,
	BORDER_SCHEME_EDIT				= 16,
	BORDER_SCHEME_EDIT_NUMERIC = 17,
	BORDER_OUT3l							= 18,
	BORDER_OUT3r							= 19,

	BORDER_MASK								= 0x1FFFFFFF,
	BORDER_WITH_TITLE_MONO		= 0x20000000,
	BORDER_WITH_TITLE_BOLD		= 0x40000000,
	BORDER_WITH_TITLE					= 0x80000000
};

// dialog resource object
enum
{
	DR_NAME														= 'name',
	DR_CHILD													= 'chld',
	DR_ID															= 'rsid',
		DRTYPE_DIALOG										= 'rdlg',

		DRTYPE_SCROLLGROUP							= 'sgrp',
			DR_SCROLL_V										= 'scrv',
			DR_SCROLL_H										= 'scrh',
			DR_SCROLL_BORDERIN						= 'scrb',
			DR_SCROLL_STATUSBAR 		      = 'scrs',
			DR_SCROLL_AUTO_H							= 'scah',
			DR_SCROLL_AUTO_V							= 'scav',
			DR_SCROLL_NOBLIT							= 'scnb',
			DR_SCROLL_LEFT								= 'scle',

		DRTYPE_TAB											= 'dtab',
			DR_TAB_SELECTBY								= 'tbsl',
				TAB_TABS										= 0,
				TAB_NOSELECT								= 1,
				TAB_CYCLE										= 2,
				TAB_RADIO										= 3,
				TAB_VLTABS									= 4,
				TAB_VRTABS									= 5,

		DRTYPE_RADIOGROUP								= 'rgrp', // radio group

		DRTYPE_DLGGROUP									= 'dlgg',
			DR_DLGGROUP_OK								= 1,
			DR_DLGGROUP_CANCEL						= 2,

		DRTYPE_GROUP										= 'grup',
			//DR_CHILD
			//DR_NAME
			//DR_ID
			DR_COLUMNS										= 'cols', // number of columns, either DR_COLUMNS or DR_ROWS
			DR_ROWS												= 'rows', // number of rows
			DR_EQUALROWS									= 'eqrw',
			DR_EQUALCOLS									= 'eqcl',
			DR_BORDERSTYLE								= 'bdst', // type of the border
				//BORDER_STYLES ...
			DR_ALLOW_WEIGHTS							= 'alwg',	// allow weights
			DR_BORDERGROUP_CHECKBOX				= 'bgch', // title checkbox

			DR_BORDERSIZE_L								= 'bszl', // space around the group
			DR_BORDERSIZE_R								= 'bszr', // space around the group
			DR_BORDERSIZE_T								= 'bszt', // space around the group
			DR_BORDERSIZE_B								= 'bszb', // space around the group
			DR_SPACE_X										= 'spcx', // space between elements
			DR_SPACE_Y										= 'spcy', // space between elements
			DR_ALIGNMENT									= 'algn', // alignment
				// binfo->pos baseframe vertical and horizontal orientation
				BFV_CENTER									= 0,
				BFV_TOP											= 1, // (1<<0),
				BFV_BOTTOM									= 2, // (1<<1),
				BFV_FIT											= 3, // (BFV_BOTTOM|BFV_TOP),
				BFV_SCALE										= 4, // (1<<2),
				BFV_SCALEFIT								= 7, // (BFV_SCALE|BFV_FIT),
					BFV_MASK									= 3,

				BFH_CENTER									= 0,
				BFH_LEFT										= 8,  // 1<<3
				BFH_RIGHT										= 16, // 1<<4
				BFH_FIT											= 24,
				BFH_SCALE										= 32, // 1<<5
				BFH_SCALEFIT								= 56,
					BFH_MASK									= 24,

		DRTYPE_WINDOWPIN								= 'wpin',
		DRTYPE_SEPARATOR								= 'wsep',

		DRTYPE_OLDWIN										= 'oldw',
			DROLDWIN_SIZE_W								= 'sizw',	// default dialog size (w,h) in dlus
			DROLDWIN_SIZE_H								= 'sizh',	// default dialog size (w,h) in dlus

			DROLDWIN_EDITSHORTCUT					= 'edsh',

			DROLDWIN_EDITTEXT							= 'edit',
			DROLDWIN_MULTILINETEXT				= 'edmt',
				DR_MULTILINE_MONOSPACED			= 1,
				DR_MULTILINE_SYNTAXCOLOR		= 2,
				DR_MULTILINE_STATUSBAR    	= 4,
				DR_MULTILINE_HIGHLIGHTLINE 	= 8,
				DR_MULTILINE_READONLY				= 16,
				//DR_MULTILINE_RESERVED			= 32,
				//DR_MULTILINE_RESERVED			= 64,
				DR_MULTILINE_PYTHON					= 128,
				DR_MULTILINE_WORDWRAP				= 256,

			DROLDWIN_EDITNUMBERARROWS 		= 'numa',
			DROLDWIN_EDITNUMBER						= 'numb',
			DROLDWIN_COMBOBOX							= 'cmbo',
			DROLDWIN_COMBOBUTTON					= 'Cmbo',
			DROLDWIN_POPUPBUTTON					= 'popb',
			DROLDWIN_CHECKBOX							= 'chck',
			DROLDWIN_STATICTEXT						= 'text',
			DROLDWIN_SLIDER								= 'sldr',
			DROLDWIN_SLIDERNUM						= 'slnm',
			DROLDWIN_GROUPBOX							= 'ogrp',
			DROLDWIN_BUTTON								= 'bttn',
			DROLDWIN_OWNERDRAW						= 'ownr',
			DROLDWIN_COLORFIELD						= 'cfld',
				DR_COLORFIELD_NO_BRIGHTNESS 					= 1,
				DR_COLORFIELD_NO_COLOR								= 2,
				DR_COLORFIELD_BODYPAINT								= 4,	// uses the BP style
				DR_COLORFIELD_ICC_BASEDOC							= 8,	// use iccprofile of the current basedocument
				DR_COLORFIELD_ICC_BPTEX								= 16,	// uses iccprofile of the current bp texture
				DR_COLORFIELD_NO_MODE_BUTTONS 				= 32, // Hides the color mode buttons
				DR_COLORFIELD_NO_COLORWHEEL 					= 64, //
				DR_COLORFIELD_NO_SPECTRUM							= 128,
				DR_COLORFIELD_NO_PICTURE							= 256,
				DR_COLORFIELD_NO_RGB									= 512,
				DR_COLORFIELD_NO_HSV									= 1024,
				DR_COLORFIELD_NO_KELVIN								= 2048,
				DR_COLORFIELD_NO_MIXER								= 4096,
				DR_COLORFIELD_NO_SWATCHES							= 8192,
				DR_COLORFIELD_NO_SCREENPICKER 				= 16384,
				DR_COLORFIELD_ENABLE_COLORWHEEL 			= 32768,
				DR_COLORFIELD_ENABLE_SPECTRUM					= 65536,
				DR_COLORFIELD_ENABLE_PICTURE					= 131072,
				DR_COLORFIELD_ENABLE_RGB							= 262144,
				DR_COLORFIELD_ENABLE_HSV							= 524288,
				DR_COLORFIELD_ENABLE_KELVIN						= 1048576,
				DR_COLORFIELD_ENABLE_MIXER						= 2097152,
				DR_COLORFIELD_ENABLE_SWATCHES					= 4194304,
				DR_COLORFIELD_RGB_HIDE_HEX						= 8388608,

			DROLDWIN_RADIOGADGET					= 'rgad',
			DROLDWIN_SPECIALRADIO					= 'sbtn',
				// for all gadget types
				// DR_ID										= 'rsid',
				// DR_CHILD									= 'chld',
				// DR_NAME									= 'name',
				DROLDWIN_POS_X							= 'posx',	// position (x,y,w,h) in dlus for the object
				DROLDWIN_POS_Y							= 'posy',	// position (x,y,w,h) in dlus for the object
				DROLDWIN_POS_W							= 'posw',	// position (x,y,w,h) in dlus for the object
				DROLDWIN_POS_H							= 'posh',	// position (x,y,w,h) in dlus for the object
			DROLDWIN_ARROWBUTTON          = 'arrg',
				DR_ARROWTYPE								= 'arrt',
					ARROW_LEFT 								= 1,
					ARROW_RIGHT								= 2,
					ARROW_UP 									= 3,
					ARROW_DOWN 								= 4,
					ARROW_SMALL_LEFT 					= 5,
					ARROW_SMALL_RIGHT					= 6,
					ARROW_SMALL_UP 						= 7,
					ARROW_SMALL_DOWN 					= 8,
			DROLDWIN_LISTVIEW							= 'list',
			DROLDWIN_SUBDIALOG						= 'subd',
			DROLDWIN_SDK									= 'sdk8',

	DIALOGRESOURCE_
};

// flags for command gadget
enum
{
	CMD_POPUP_RIGHT		= 1,
	CMD_POPUP_BELOW		= 2,
	CMD_PIC					  = 4,
	CMD_TOGGLE				= 8,			// can be toggled
	CMD_TEXT					= 16,			// text
	CMD_SHORTCUT			= 32,			// shortcut
	CMD_ARROW					= 64,			// arrows for menu
	CMD_VERT					= 128,		// vertical alignment of text and icon
	CMD_BUTTONLIKE		= 256,		// button (TRUE) or menu entry (FALSE)
	CMD_MENU					= 512,		// menu entry (is a menu entry and no command entry)
	CMD_CYCLE					= 1024,		// stays toggled
	CMD_EDITPALETTE		= 4096,		// command manager flag (button shows up in palette manager)
	CMD_SMALLICONS		= 8192,		// small icons (text height)
	CMD_SHOWLOGO			= 16384,	// show app icon in icon bar

	CMD_VALUE							= 0x3FFFFFFF,
	CMD_ENABLED						= 0x40000000,

	CMD_
};

// flags for popups
enum
{
	POPUP_ABOVE									= 1,		// sdk: open to this direction relative to mouse
	POPUP_BELOW									= 2,		// sdk: open to this direction relative to mouse
	POPUP_CENTERVERT						= 3,		// sdk: open to this direction relative to mouse

	POPUP_LEFT									= 4,		// sdk: open to this direction relative to mouse
	POPUP_RIGHT									= 8,		// sdk: open to this direction relative to mouse
	POPUP_CENTERHORIZ						= 12,		// sdk: open to this direction relative to mouse

	POPUP_ADJUSTWIDTH						= 16,		// only for internal usage
	POPUP_ADJUSTHEIGHT					= 32,		// only for internal usage

	POPUP_EXECUTECOMMANDS				= 64,		// sdk: execute command immediatly

	POPUP_ALLOWUNDOCK						= 128,	// allow to undock popupmenu
	POPUP_ALLOWUNDOCK_REC				= 256,	// allow to undock popupmenu for children

	POPUP_
};

// flags for coffee manager window
enum
{
	SCROLLGROUP_VERT									= 4,
	SCROLLGROUP_HORIZ									= 8,
	SCROLLGROUP_NOBLIT								= 16,
	SCROLLGROUP_LEFT									= 32,
	SCROLLGROUP_BORDERIN							= 64,
	SCROLLGROUP_STATUSBAR							= 128,
	SCROLLGROUP_AUTOHORIZ							= 256,
	SCROLLGROUP_AUTOVERT							= 512,
	SCROLLGROUP_NOSCROLLER						= 1024,
	SCROLLGROUP_NOVGAP								= 2048,
	SCROLLGROUP_STATUSBAR_EXT_GROUP		= 4096,

	ID_SCROLLGROUP_STATUSBAR_EXTLEFT_GROUP	= 200000239,
	ID_SCROLLGROUP_STATUSBAR_EXTRIGHT_GROUP	= 200000240
};

// modes for MouseSelectAreas function
enum
{
	MOUSESELECT_NOCROSSCURSOR	= 0x40000000, //(1<<30)
	MOUSESELECT_NOFILL				= 0x20000000,	//(1<<29)

	MOUSESELECT_RECTANGLE			= 1,
	MOUSESELECT_POLYGON				= 2,
	MOUSESELECT_FREE					= 3,
	MOUSESELECT_OUTLINE_RECT	= 0x20000001,
	MOUSESELECT_OUTLINE_POLY	= 0x20000002,
	MOUSESELECT_OUTLINE_FREE	= 0x20000003
};

enum
{
	MANAGER_NONE,

	MANAGER_SCROLLER_HORIZ,
	MANAGER_SCROLLER_VERT,
	MANAGER_CLOSEBUTTON,
	MANAGER_SIZEABLE,
	MANAGER_TITLE,
	MANAGER_ID,
	MANAGER_MENUID,
	MANAGER_NOBLITBIT,
	MANAGER_WANTOWNSTATUSBAR,
	MANAGER_SPECIALUNLOCKMOUSEDOWN,

	MANAGER_DUMMY
};

// for color chooser
enum
{
	NO_BRIGHTNESS = 1,
	NO_COLOR      = 2,
	DUMMY_
};

// focus definitions of COFFEE frames
enum
{
	FOCUS_OFF					= 0,	// has no focus
	FOCUS_ON					= 1,	// has focus and window is active
	FOCUS_INACT				= 2,	// has focus but window is inactive
	FOCUS_HIDDEN			= 3,	// focus is hidden through ENTER
	FOCUS_INPROGRESS	= 4,
	FOCUS_
};

enum
{
	BFM_INIT							= 'bINI',

	BFM_TIMER_MESSAGE   	= 10020,

	BFM_CALCSIZE					= 'bCAS',

	BFM_CHECKCLOSE				= 'chkc',	// message to physical window when user closes window
		BFM_CHECKCLOSE_LAYOUTSWITCH = 'ccll',
	BFM_ASKCLOSE					= 'askc',	// send within dialog to check if dialog can be closed
	BFM_CLOSE							= 'clse', // directly before BFM_DESTROY and after BFM_ASKCLOSE
	BFM_DESTROY						= 'dsty',	// force closing

	BFM_CLOSEWINDOW				= 'clos',	// parent message for window closing

	BFM_ASK_TABSWITCH			= 'stab', // ask for tab switching

	BFM_VISIBLE_ON				= 'visT', // message about visibility change
	BFM_VISIBLE_OFF				= 'visF', // message about visibility change

	BFM_PARENT_TITLECHNG	= 'pren', // title change parent notify
	BFM_RENAMEWINDOW			= 'wren',	// rename window message

	BFM_GETACTIVETABTITLE = 'tact', // ask for active tab title

	BFM_GETCURSORINFO			= 'cinf',	// ask for mousepointdata (returns container)
		// BFM_DRAG_SCREENX - screenx
		// BFM_DRAG_SCREENY - screeny
		// resultcontainer
		RESULT_CURSOR									= 1,		// cursortype
		RESULT_BUBBLEHELP							= 2,		// bubblehelptext
		RESULT_SUPPRESSBUBBLE					= 3,
		RESULT_CURSOR_FORCE_HIDE			= 4,
		RESULT_CURSOR_NO_STEREO_HIDE	= 5,
		RESULT_HELP1									= 10,
		RESULT_HELP2									= 11,
		RESULT_HELP3									= 12,
		RESULT_HELP4									= 13,
		RESULT_BUBBLEHELP_TITLE				= 20,		// bubblehelptext title (printed in bold for the bubblehelp. in the statusbar it's not visible)

	BFM_CURSORINFO_REMOVE = 'cirm', // cursorinfo removed

	BFM_GETINFO						= 'bInf',

	BFM_DRAW							= 'bDRA',
		BFM_DRAW_LEFT						= 1,			// only redraw this area
		BFM_DRAW_TOP						= 2,			// only redraw this area
		BFM_DRAW_RIGHT					= 3,			// only redraw this area
		BFM_DRAW_BOTTOM					= 4,			// only redraw this area
		BFM_DRAW_HASRECT				= 5,			// flag for a existing redraw rectangle
		BFM_DRAW_OGL						= 6,			// flag for a existing redraw rectangle
		BFM_DRAW_REASON					= 7,			// message which started the redraw
		BFM_DRAW_IGNORE_UPDATED	= 8,			// flag which tells the redraw not to clear the dirty flag (cfUpdateFrame), BUGFIX #16161

	BFM_GET_SHOW_AREA			= 'sget',
	BFM_SHOW_AREA					= 'sare', // scrolls the given rectangle so that it becomes visible (ScrollArea)
		//BFM_DRAW_LEFT
		//BFM_DRAW_TOP
		//BFM_DRAW_RIGHT
		//BFM_DRAW_BOTTOM

	BFM_SCROLLGROUP_SCROLLED	= 'scrs',
	SCROLLGROUP_HEADER	= 0xdeadbeee,

	BFM_TITLECHNG					= 'bTIC',

	BFM_ENABLE						= 'bEna',	// enables the COFFEE frame
	BFM_DISABLE						= 'bDis',	// disables the COFFEE frame

	BFM_MINCHNG						= 'bMIC',

	BFM_MAXCHNG						= 'bMAC',

	BFM_VALUECHNG					= 'bVAC',

	BFM_IDCHNG						= 'bIDC',

	BFM_GOTFOCUS					= 'bGFC',	// item got the focus
	BFM_LOSTFOCUS					= 'bLFC',	// item lost the focus

	BFM_SETFOCUS					= 'bSFC',	// internal message
	BFM_SETFIRSTFOCUS			= 'bSFF',	// help message focus-keyboard control

	BFM_MOVEFOCUSNEXT			= 'bFNX',	// help message focus-keyboard control
	BFM_MOVEFOCUSPREV			= 'bFPR',	// help message focus-keyboard control

	BFM_SETLASTFOCUS			= 'bSLF',	// help message focus-keyboard control

	BFM_SHOW_FRAME				= 'sFrm', // brings frame to front, returns TRUE if found
		BFM_SHOW_ID					= 1,			// show this id
		BFM_MANAGER_ID			= 2,			// show this id

	BFM_CHILD_REMOVED			= 'bRem',
		BFM_CHILD_ID				= 1,

	BFM_ACTIVATE_WINDOW		= 'wact',

	BFM_INPUT											= 'bIPN',
		BFM_INPUT_QUALIFIER					= 'ipqa',	// qualifier
		BFM_INPUT_MODIFIERS					= 'ipmo',	// all modifier states
		BFM_INPUT_DEVICE						= 'ipdv', // device
			BFM_INPUT_MOUSE						= 'mous', // mouse
			BFM_INPUT_KEYBOARD				= 'keyb', // keyboard input
				BFM_INPUT_ASC						= 'kasc', // unicode input from keyboard
			BFM_INPUT_KEYBOARD_SLCT		= 'keys', // select/deselect/delete previous keyboard input
				BFM_INPUT_SLCT_CMD			= 'tslc',
				BFM_INPUT_SLCT_PARAM		= 'tspr',

		BFM_INPUT_CHANNEL						= 'ipca', // channel
				BFM_INPUT_MOUSELEFT			= 1,
				BFM_INPUT_MOUSERIGHT		= 2,
				BFM_INPUT_MOUSEMIDDLE		= 3,
				BFM_INPUT_MOUSEX1				= 5,
				BFM_INPUT_MOUSEX2				= 6,
				BFM_INPUT_MOUSEWHEEL		= 100,		// windows mouse wheel message
				BFM_INPUT_MOUSEMOVE			= 101,		// windows mouse wheel message
			BFM_INPUT_VALUE						= 'ipva',	// LONG: channel value (e.g. pressure)
			BFM_INPUT_VALUE_REAL			= 'ipvA',	// REAL: Value des Channels (z.B Pressure)
			BFM_INPUT_X								= 'ipvx',	// REAL: channel x value
			BFM_INPUT_Y								= 'ipvy',	// REAL: channel y value
			BFM_INPUT_Z								= 'ipvz',	// REAL: channel z value
			BFM_INPUT_TILT						= 'itlt',	// REAL: pen tilt
			BFM_INPUT_ORIENTATION			= 'irot',	// REAL: pen rotation, now called orientation
			BFM_INPUT_FINGERWHEEL			= 'ifng',	// REAL:finger wheel
			BFM_INPUT_P_ROTATION			= 'prot',	// REAL: pen rotation (around own axis)
			BFM_INPUT_HSCROLL					= 'ihsc',	// REAL: amount of horizontal scrolling (touch pad, mouse ball)
			BFM_INPUT_VSCROLL					= 'ivsc',	// REAL: amount of vertical scrolling (touch pad, mouse wheel/ball)

		BFM_INPUT_DOUBLECLICK 			= 'ipdb', // double click (bool)
		INPUT_DBLCLK								= 0x8000,

	BFM_SIZED											= 'bISI',

	BFM_ACTION										= 'bACT',
		BFM_ACTION_ID								= 'meid',
		BFM_ACTION_VALUE						= 'meva',
		BFM_ACTION_INDRAG						= 'medr',	// slider in dragging mode (unfinished) (bool)
		BFM_ACTION_STRCHG						= 'mest',	// string in textfield changed (bool)
		BFM_ACTION_VALCHG						= 'vchg',	// NumberEdit/SliderChg (bool)
		BFM_ACTION_ESC							= 'aesc',	// action escaped
		BFM_ACTION_RESET						= 'ares',	// action escaped
		BFM_ACTION_UPDATE						= 'updt',	// update without verify

	BFM_COLORCHOOSER_PARENTMESSAGE	= 'colP',
	BFM_COLORCHOOSER								= 'colC',
		BFM_COLORCHOOSER_SYSTEM					= 1, // Not used anymore (legacy)
		BFM_COLORCHOOSER_RGB_RANGE			= 2,
		BFM_COLORCHOOSER_H_RANGE				= 3, // Not used anymore (legacy)
		BFM_COLORCHOOSER_SV_RANGE				= 4, // Not used anymore (legacy)
		BFM_COLORCHOOSER_SYSTEMMESSAGE	= 5,	// sends a parent message "BFM_COLORCHOOSER_PARENTMESSAGE" if settings change
		BFM_COLORCHOOSER_QUICKSTORE			= 6, // Not used anymore (legacy)
		BFM_COLORCHOOSER_MIXINGPANEL		= 7, // Not used anymore (legacy)

	BFM_GET_FOCUS_RECTANGLE = 'gfRc',
		BFM_GET_FOCUS_RECTANGLE_X = 0,
		BFM_GET_FOCUS_RECTANGLE_Y = 1,
		BFM_GET_FOCUS_RECTANGLE_W = 2,
		BFM_GET_FOCUS_RECTANGLE_H = 3,

	BFM_ACTIVE_CHG								= 'bACG',	// activation changed
		BFM_ACTIVE									= 'actv',	// flag, if window is active

	BFM_DRAGSTART									= 10001,
	BFM_DRAGRECEIVE								= 10003,
	BFM_DRAGEND										= 10004,
	BFM_DRAGAUTOSCROLL						= 10005,
		BFM_DRAG_TYPE								= 1,

			DRAGTYPE_FILES						= 1,			// data pointer is a string with filename
			DRAGTYPE_ICON							= 4,			//
			DRAGTYPE_MANAGER					= 5,			// destination drag for COFFEE manager
			DRAGTYPE_COMMAND					= 6,			// destination drag for command
			DRAGTYPE_CMDPALETTE				= 7,
			DRAGTYPE_TEXT							= 8,			/// Drag a block of text as a string

			DRAGTYPE_OBJECT						= 100,
			DRAGTYPE_TAG							= 101,
			DRAGTYPE_DESCID						= 113,

			DRAGTYPE_MATERIAL					= 200,
			DRAGTYPE_ATOMARRAY				= 201,
			DRAGTYPE_FILENAME_IMAGE   = 202,
			DRAGTYPE_RGB							= 203,
			DRAGTYPE_FILENAME_SCENE		= 204,
			DRAGTYPE_FILENAME_OTHER		= 205,

			DRAGTYPE_BROWSER_SCENE    = 401,
			DRAGTYPE_BROWSER_MATERIAL = 402,
			DRAGTYPE_BROWSER_SOUND		= 403,
			DRAGTYPE_BROWSER_FCV			= 404,
			DRAGTYPE_BROWSER_COFFEE		= 405,
			DRAGTYPE_A_LINE						= 45000,
			DRAGTYPE_A_VDRAG					= 45001,

		BFM_DRAG_DATA								= 2,
		BFM_DRAG_SCREENX						= 3,
		BFM_DRAG_SCREENY						= 4,
		BFM_DRAG_FINISHED						= 5,
		BFM_DRAG_PRIVATE						= 6,
		BFM_DRAG_LOST								= 7,
		BFM_DRAG_TYPE_NEW						= 8,
		BFM_DRAG_DATA_NEW						= 9,
		BFM_DRAG_ESC								= 10,

		AUTOSCROLL_UP								= 1,	// result of BFM_DRAGAUTOSCROLL
		AUTOSCROLL_DOWN							=	2,	// result of BFM_DRAGAUTOSCROLL
		AUTOSCROLL_LEFT							= 3,	// result of BFM_DRAGAUTOSCROLL
		AUTOSCROLL_RIGHT						=	4,	// result of BFM_DRAGAUTOSCROLL
		AUTOSCROLL_LEFT_UP					= 7,	// result of BFM_DRAGAUTOSCROLL
		AUTOSCROLL_LEFT_DOWN				= 8,	// result of BFM_DRAGAUTOSCROLL
		AUTOSCROLL_RIGHT_UP					=	9,	// result of BFM_DRAGAUTOSCROLL
		AUTOSCROLL_RIGHT_DOWN				=	10,	// result of BFM_DRAGAUTOSCROLL

		HANDLEMOUSEDRAG_PRIVATEFRAME					= 1,
		HANDLEMOUSEDRAG_PRIVATEAUTOSCROLL			= 2,

	// Tilo changed
	BFM_GETVALUE									= 'gVAL',	// get Value

	BFM_VALUEADD									= 'bVAD',
	BFM_VALUESUB									= 'bVSB',

	BFM_INITVALUES								= 'bVIN',

	BFM_ADJUSTSIZE								= 'fADJ',	// message to adjust the object in the given range l.d.
		BFM_ADJUSTSIZE_LEFT					= 1,
		BFM_ADJUSTSIZE_TOP					= 2,
		BFM_ADJUSTSIZE_WIDTH				= 3,
		BFM_ADJUSTSIZE_HEIGHT				= 4,

	// parent messages
	//	BFM_GETDISABLED						= 'bDIS',	// ask if parent is disabled

	BFM_LAYOUT_CHANGED_INTERNAL = 'iLAY',
	BFM_LAYOUT_CHANGED						= 'fLAY',	// layout change message to parent

	BFM_FULLSCREENMODE_ON					= 'ful1',
	BFM_FULLSCREENMODE_OFF				= 'ful0',

	BFM_ISACTIVE									= 'iact', // to ask active view...

	BFM_SETSTATUSBAR 							= 'stat',
		BFM_STATUSBAR_PROGRESSON				= 1,			// bool
		BFM_STATUSBAR_TXT								= 2,			// string
		BFM_STATUSBAR_PROGRESS					= 3,			// real 0.0...1.0
		BFM_STATUSBAR_PROGRESSSPIN			= 4,			// ...
		BFM_STATUSBAR_HELP        			= 5,
		BFM_STATUSBAR_PROGRESSFULLSIZE	= 6,			// use full-sized progress bar
		BFM_STATUSBAR_TINT_COLOR   			= 7,			// LONG colorid, Vector rgbvalue 0...1
		BFM_STATUSBAR_NETTINT_COLOR   	= 8,			// LONG colorid, Vector rgbvalue 0...1
		BFM_STATUSBAR_NETPROGRESSON			= 9,			// bool
		BFM_STATUSBAR_NETTXT						= 10,			// string
		BFM_STATUSBAR_NETPROGRESS				= 11,			// real 0.0...1.0
		BFM_STATUSBAR_NETPROGRESSSPIN		= 12,			// ...
		BFM_STATUSBAR_NETICON						= 13,			// bool

	BFM_MARKFOCUS									= 'bMFC',	// reverts focus message ... for internal use only

	BFM_CLEARFOCUS								= 'bCFC',	// clear focus of all children

	BFM_SETACTIVE									= 'bSAC',	// set active dialog bar
		BFM_SETACTIVE_DATA					= 'bSAC',	// set active dialog bar

	BFM_REMOVE_DIALOG							= 'bREM', // remove the dialog from current group
		BFM_DESTINATION_GROUP 			= 'gdst', // destination for dialog, NULL means own window

	BFM_SCROLLAREA								= 10010, 	// scroll client area
		BFM_SCROLLX									= 1, 			// scroll direction
		BFM_SCROLLY									= 2, 			// scroll direction

	BFM_GETVISIBLE_XOFF						= 10011,	// internal usage
	BFM_GETVISIBLE_YOFF						= 10012,	// internal usage
	BFM_GETVISIBLE_WIDTH					= 10013,	// internal usage
	BFM_GETVISIBLE_HEIGHT					= 10014,	// internal usage

	// scroller
	SCR_VISIBLEOBJS								= 'sVIS',	// visible scroll elements

	BFM_SETVIEWPORTORIGIN 				= 'cORG',
		BFM_SETVIEWPORTORIGIN_X			= 1,
		BFM_SETVIEWPORTORIGIN_Y			= 2,
	BFM_SETVIEWPORTSIZE						= 'cSIZ',

	BFM_MENU_SET									= 10000,
		BFM_MENU_ON									= 1,
		BFM_MENU_OFF								= 2,
		BFM_MENU_CHECK							= 4,
		BFM_MENU_UNCHECK						= 8,

	BFM_DRAWUSERITEM							= 'usrd',
		BFM_DRAWUSERITEM_ID					= 1,

	// C4D message
	BFM_SYNC_MESSAGE							= 'sync',
	BFM_CORE_MESSAGE							= 'MciM',
		BFM_CORE_ID									= 'MciI',	// C4D message ID
		BFM_CORE_UNIQUEID						= 'Muid',	// time stamp
		BFM_CORE_PAR1								= 'Mci1',	// parameter 1
		BFM_CORE_PAR2								= 'Mci2',	// parameter 2
		BFM_CORE_SPECIALCOREID 			= 'scid', // special manager ID for sync message

	BFM_TESTONLY									= 'test', // do nothing
	BFM_SPECIALSTRING							= 'spst', // special thing
	BFM_SPECIALGETSTRING					= 'gtst', // special thing
	BFM_SPECIALSETRANGE						= 'srng', // special hack
	BFM_SPECIALMODE								= 'srnm', // special hack
	BFM_SETSPECIALMULTI						= 'SSSS', // set passed setting to True (for MULTILINEEDIT)
	BFM_SETSPECIALMULTID					= 'SSSD', // set passed setting to False (for MULTILINEEDIT)

	BFM_SPECIALSETPOPUPMENU				= 'spop', // special container for popup button menudata

	BFM_POPUPNOTIFY								= 'popn', // notify of popupbuttons before the menu opens

	BFM_INTERACTSTART							= 'inta',	// interact start notify
	BFM_INTERACTEND								= 'inte',	// interact end notify

	BFM_CLEARFOCUS_SPECIAL				= 'bCFS', // private: clear focus special

	BFM_CORE_UPDATECOMMANDS				= 'updc', // forces update of all command buttons

	BF_INSERT_LAST								= 2147483647,

	BFM_MENUFINDER								= 'find',
	BFM_SET_MSG_BITMASK						= 'bitm',
		BITMASK_CORE_MESSAGE				= 1,
		BITMASK_SYNC_MESSAGE				= 2,

	BFM_SETMAINTITLE 							= 'mtit',

	BFM_GETVIEWPANELDATA					= 'vpdt',

	BFM_GETVIEWPANEL_PRIVATEDATA	= 'vppd',
	BFM_SETVIEWPANELLAYOUT 				= 'vpsl',
		BFM_VPD_PANELID							= 'vpid',
		BFM_VPD_LAYOUTTYPE					= 'vplt',
		BFM_VPD_MAXIMIZED						= 'vpmx',
		BFM_VPD_PRIVATEDATA  		 		= 'vpdt',

	BFM_STORE_WEIGHTS							= 'stwg',
	BFM_MARKFORCELAYOUT						= 'mkfl',

	BFM_REDRAW_EDITMODE 					= 'dwed',
	BFM_RELOAD_MENUS							= 'ldmn',
	BFM_OPTIMIZE									= 'opti',
	BFM_CORE_UPDATEACTIVECOMMANDS = 'upda',

	BFM_UPDATE_REGION 						= 'uprg',	// parent message -> update region
	BFM_GUIPREFSCHANGED						= 'gpch',	// message for changed gui prefs
	BFM_COMMANDSCHANGED						= 'cmch',	// Message for command-changed

	BFM_LAYOUT_GETDATA						= 'layg',	// get layoutdata from manager
	BFM_LAYOUT_SETDATA						= 'lays',	// set layoutdata in manager

	BFM_GETCUSTOMGUILAYOUTDATA		= 'layG', // get am layout data

	BFM_SCROLL_VISIBLE_PARENT			= 'ScPa',

	BFM_WEIGHTS_CHANGED						= 'wChg',

	BFM_GETPARENT_MANAGER_ID			= 'gpid',

	BFM_BG_CLOSE									= 'bgcl',		// an inactive window in the background has been closed

	BFM_EDITFIELD_GETCURSORPOS		= 'getc',
	BFM_EDITFIELD_GETBLOCKSTART		= 'getb',
	BFM_EDITFIELD_SETCURSORPOS		= 'setc',

	BFM_FADE											= 'fade',
	BFM_FADE_REMOVEALL						= 'FADE',

	BFM_EDITFIELD_STOREUNDO				= 'stru',	//Store the undo container for multiline edit text locally
	BFM_EDITFIELD_RESTOREUNDO			=	'rstu',	//Restore the undo container for multiline edit text from the local storage
	BFM_EDITFIELD_FLUSHUNDO				= 'flun',	//Kill the undo stack for the multiline edit text
	BFM_EDITFIELD_GETUNDOSTATS		=	'unst',	//Get the undo statistics for the multiline edit text
		BFM_EDITFIELD_UNDOSTAT_COUNT			=	1,		//The undo stack size Int32
		BFM_EDITFIELD_UNDOSTAT_UNDOLEVEL	=	2,		//The current undo level Int32

	BFM_REQUIRESRESULT						= 'reqr', //Set to true in the passed container for GeDialog::SendMessage to return a value from the message

	BFM_DUMMY 										= 0 			// dummy
};

enum
{
	GUI_DIALOG_MINIMIZE = 90,
	GUI_DIALOG_RELEASE  = 91,
	GUI_DIALOG_CLOSE		= 92
};

