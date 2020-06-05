enum
{
	_DEFINE_CORE_MESSAGE_
};

enum
{
	// IMPORTANT: maximum event number is 2000 (see GeEventsDoIt)
	// priority groups start at each multiple of 50

	MESSAGE_PAINTER_NEWAKTUBITMAP		= 650,
	MESSAGE_PAINTER_BITMAPCHANGED		= 651,

	MESSAGE_PAINTER_NEWAKTUBRUSH		= 700,
	MESSAGE_PAINTER_BRUSHCHANGED		= 701,

	MESSAGE_PAINTER_NEWAKTUCOLOR		= 750,
	MESSAGE_PAINTER_COLORCHANGED		= 751,

	MESSAGE_PAINTER_NEWAKTUPOLY			= 800,
	MESSAGE_PAINTER_POLYCHANGED			= 801,

	MESSAGE_PAINTER_TOOLCHANGED			= 850,
	MESSAGE_PAINTER_UNDOCHANGED			= 900,

	MESSAGE_PAINTER_INPUTDEVCHANGED	= 950
};

enum
{
	VIEW_SINGLE         = 0,
	VIEW_2V             = 1,
	VIEW_2H             = 2,
	VIEW_2SPLIT_TOP     = 3,
	VIEW_2SPLIT_BOTTOM  = 4,
	VIEW_2SPLIT_LEFT    = 5,
	VIEW_2SPLIT_RIGHT   = 6,
	VIEW_4T             = 7,
	VIEW_3SPLIT_TOP     = 8,
	VIEW_3SPLIT_BOTTOM  = 9,
	VIEW_3SPLIT_LEFT    = 10,
	VIEW_3SPLIT_RIGHT   = 11,
	VIEW_4H             = 12,
	VIEW_4V             = 13,

	VIEW_DUMMY
};

enum
{
	COREMSG_PAINTER											= 'CMpa',	// container request to painter core
	COREMSG_AMBER												= 'amb6',
	COREMSG_CINEMA											= 'CMci',	// container request to C4D core

	COREMSG_SETDATA											= 'setd', // modeling tools

	// data container
	CM_DISABLED													= 'disb',
	CM_TYPE_BUTTON											= 'bttn',
	CM_TYPE_STRING											= 'strg',
		CM_STRING													= 'strg',
	CM_TYPE_INT													= 'vint',
	CM_TYPE_FLOAT												= 'vflt',
		CM_VALUE_VAL											= 'valu',	// necessary
		CM_VALUE_MIN											= 'mini',	// unnecessary
		CM_VALUE_MAX											= 'maxi',	// unnecessary
		CM_VALUE_MIN2											= 'min2', // for second range of slider with ints
		CM_VALUE_MAX2											= 'max2', // for second range of slider with ints
		CM_VALUE_STEP											= 'step',	// unnecessary
		CM_VALUE_FORMAT										= 'frmt',	// unnecessary
			FORMAT_FLOAT											= 'frea',
			FORMAT_INT											= 'flng',
			FORMAT_PERCENT									= 'fpct',
			FORMAT_DEGREE										= 'fdgr',
			FORMAT_METER										= 'fmet',
			FORMAT_FRAMES										= 'ffrm',
			FORMAT_SECONDS									= 'fsec',
			FORMAT_SMPTE										= 'fsmp',
		CM_VALUE_FPS											= 'ffps',	// for FORMAT_FRAMES, FORMAT_SECONDS, FORMAT_SMPTE
		CM_VALUE_QUADSCALE								= 'quad',	// quadscale of the slider
		CM_VALUE_TRISTATE									= 'tris',	// 0 == off, 1 == enabled/even values 2 == enabled/different values
	CM_TYPE_DATA												= 'vdat',

	COREMSG_SYSTEM											= 'CMsy',	// container request to C4D OS
		COREMSG_SYSTEM_GETINPUTDEVICEDATA = 'Sgdd', // input device data
			CSD_INPUTDEV_NAME								= 'idna', // input device data
			CSD_INPUTDEV_ID									= 'idid', // input device data
			CSD_INPUTDEV_SUPPORT						= 'idsp',	// ...

		// request bitmap data
		COREMSG_PAINTER_GETBITMAPDATA 		= 'Pgbd', // get data of requested layer
		COREMSG_PAINTER_SETBITMAPDATA		  = 'Psbd', // set data of requested layer
			PGD_BITMAP_MARKER								= 'bmid',	// set/get bitmap marker; set only: 0 == active
			PGD_BITMAP_NAME									= 'name', // set/get layer name (string)
			PGD_BITMAP_TYPE									= 'type',	// get type of bitmap (enum)
				PGD_BITMAPTYPE_GROUP					= 'grou',	// paint layer group
				PGD_BITMAPTYPE_PIXELLAYER			= 'pixe',	// paint layer
			PGD_LAYER_STRENGTH							= 'stre', // set/get coverage (real)
			PGD_LAYER_BLENDING							= 'blnd', // set/get blending (enum)
				PGD_BLEND_NORMAL							= 'nrml',
				PGD_BLEND_DISSOLVE							= 'disv',
				PGD_BLEND_DIFFERENCE					= 'diff',
				PGD_BLEND_LIGHTEN							= 'ligt',
				PGD_BLEND_DARKEN							= 'dark',
				PGD_BLEND_MULTIPLY						= 'mult',
				PGD_BLEND_SCREEN							= 'scrn',
				PGD_BLEND_ADD									= 'add_',
				PGD_BLEND_EXCLUSION						= 'excl',
				PGD_BLEND_ERASE								= 'eras',
				PGD_BLEND_REPLACEVOXEL				= 'repl',
			PGD_LAYER_MODE									= 'mode', // set/get mode (enum)
				PGD_MODE_RGB									= 'rgb_',
				PGD_MODE_RGBA									= 'rgba',
				PGD_MODE_GREY8								= 'gry8',
				PGD_MODE_GREY8A								= 'gr8a',
				PGD_MODE_CMYK									= 'cmyk',
				PGD_MODE_CMYKA								= 'cmka',
			PGD_LAYER_SHOW									= 'show', // set/get layer visibility
			PGD_LAYER_XOFFSET								= 'xoff', // set/get x offset
			PGD_LAYER_YOFFSET								= 'yoff', // set/get y offset
			PGD_BITMAP_WIDTH								= 'widt', // get width
			PGD_BITMAP_HEIGHT								= 'heig', // get height
			PGD_MEMORY_USAGE								= 'memo',	// get memory usage

		// request data of current tool
		COREMSG_PAINTER_GETTOOL						= 'Pgtd', // get tool mode data (container)
		COREMSG_PAINTER_SETTOOL						= 'Pstd', // set tool mode data (container)
			PGD_TOOL_MODE										= 'bmmd',	// set/get tool mode
				PGD_TOOLMODE_DRAWBRUSH				= 'tdrw',	// draw with brush
				PGD_TOOLMODE_DODGE						= 'dodg',	// dodge tool
				PGD_TOOLMODE_BURN							= 'burn',	// burn tool
				PGD_TOOLMODE_SPONGE						= 'spng',	// sponge tool
				PGD_TOOLMODE_BLUR							= 'blur', // blur tool
				PGD_TOOLMODE_SHARPEN					= 'shar', // sharpen tool
				PGD_TOOLMODE_HSV							= 'thsv', // coloration tool
				PGD_TOOLMODE_SMEAR						= 'smea',	// smear tool
				PGD_TOOLMODE_FILLBITMAP				= 'fill', // fill bitmap
				PGD_TOOLMODE_MAGICWAND				= 'magc', // fill bitmap
				PGD_TOOLMODE_PICKUP						= 'pick', // pick color
				PGD_TOOLMODE_MOVESPLINE				= 'spmv',	// move spline
				PGD_TOOLMODE_EDITSPLINE				= 'sped', // edit spline
				PGD_TOOLMODE_MOVELAYER				= 'lymv',	// move layer
				PGD_TOOLMODE_DRAWLINE					= 'line',	// draw line
				PGD_TOOLMODE_DRAWCIRCLE				= 'crcl',	// draw circle
				PGD_TOOLMODE_DRAWRECTANGLE		= 'rect', // draw rectangle
				PGD_TOOLMODE_DRAWTEXT					= 'text', // text tool
				PGD_TOOLMODE_TRANSFORMBITMAP	= 'trns', // distort bitmap
				PGD_TOOLMODE_UVEDIT						= 'uved', // uvedit mode
				PGD_TOOLMODE_SELECTIONTOOLS		= 'sele',	// draw circle
				PGD_TOOLMODE_SELECTCOLORRANGE	= 'selc',	// select colorrange
				PGD_TOOLMODE_MAGNIFY					= 'magn', // magnify
				PGD_TOOLMODE_CLONE            = 'clon', // clone tool
				PGD_TOOLMODE_GRADIENT					= 'grad',	// gradient tool

		// for all tools
		PGD_TOOLACTIVE										= 'Pact', // tool is active

		COREMSG_PAINTER_GETBRUSHDATA			= 'Pgbr',	// get brush data
		COREMSG_PAINTER_SETBRUSHDATA			= 'Psbr',	// set brush data
			PGD_BRUSH_MARKER								= 'mark',	// brush marker
			PGD_BRUSH_NAME									= 'name',	// brush name
			PGD_BRUSH_PRESSURE							= 'pres', // brush pressure (real)
			PGD_BRUSH_PRESSURE_MODIFY				= 'mprs',	// pressure control
				PGD_MODIFY_NONE								= 'none', // no control
				PGD_MODIFY_PENPRESSURE				= 'pprs', // pen pressure
				PGD_MODIFY_PENTILT						= 'ptlt', // pen tilt
				PGD_MODIFY_PENDIRECTION				= 'pdir', // pen direction
				PGD_MODIFY_DRAWDIRECTION			= 'ddir', // draw direction
				PGD_MODIFY_DRAWRANDOM					= 'drnd', // randomness
				PGD_MODIFY_DRAWWHEEL					= 'dwhl', // mousewheel
				PGD_MODIFY_DRAWDISTANCE				= 'ddst', // distance
			PGD_BRUSH_SIZE									= 'size', // brush size (real 1..500)
			PGD_BRUSH_SIZE_MODIFY						= 'msiz', // brush size control
			PGD_BRUSH_HARD									= 'hard', // brush hardness (real 0..1)
			PGD_BRUSH_HARD_MODIFY						= 'mhar', // brush hardness control
			PGD_BRUSH_DISTANCE							= 'dist', // brush distance (real 1..oo)
			PGD_BRUSH_DISTANCE_MODIFY				= 'mdis', // brush distance control
			PGD_BRUSH_DISTANCE_ONOFF				= 'mdon', // distance on/off

		COREMSG_PAINTER_GETFILLDATA				= 'Pgfl',	// get fill data
		COREMSG_PAINTER_SETFILLDATA				= 'Psfl',	// set fill data
			PGD_FILL_TOLERANCE							= 'tole', // color tolerance (real 0..1)
			PGD_FILL_AA											= 'anti', // antialiasing (bool)
			PGD_FILL_MERGELAYER							= 'merg',	// merge layer (bool)

			PGD_CLONE_RELATIVE							= 'rela',	// (bool) clone relative
			PGD_CLONE_SCALE									= 'cscl',	// (real) clone scale
			PGD_CLONE_ROT										= 'crot',	// (real) clone rotation
			PGD_BLUR_RADIUS									= 'brad',	// (real) blur radius
			PGD_SHARPEN_STRENGTH						= 'stre',	// (real) sharpen strength

		COREMSG_PAINTER_GETMAGICDATA			= 'Pgma',	// get fill data
		COREMSG_PAINTER_SETMAGICDATA			= 'Psma',	// set fill data
			PGD_MAGIC_MODE									= 'mamo', // mode
				PGD_MAGIC_NEWSELECT						= 'senw', // create new selection
				PGD_MAGIC_NEWINVERSESELECT		= 'seiv', // create invert selection
				PGD_MAGIC_ADDSELECT						= 'sead', // add to selection
				PGD_MAGIC_SUBSELECT						= 'sesb', // subtract from selection

		COREMSG_PAINTER_GETLINEDATA				= 'Pgli',	// get line data
		COREMSG_PAINTER_SETLINEDATA				= 'Psli',	// set line data
			PGD_LINE_WIDTH									= 'liwd', // line thickness (real)
			PGD_LINE_BEGIN									= 'libg',	// start form
			PGD_LINE_END										= 'lied',	// end form
				PGD_CAPS_NONE									= 'none',
				PGD_CAPS_OUT_CIRCLE						= 'ocrc',
				PGD_CAPS_IN_CIRCLE						= 'icrc',
				PGD_CAPS_OUT_SPICE						= 'ospc',
				PGD_CAPS_IN_SPICE							= 'ispc',
				PGD_CAPS_OUT_BEVEL						= 'obvl',
				PGD_CAPS_IN_BEVEL							= 'ibvl',
				PGD_CAPS_ARROW								= 'arrw',
			PGD_POLY_DRAWMODE								= 'pldr',	// draw mode (filled/outline)
				PGD_DRAWMODE_FILLED						= 'dmfl', // filled
				PGD_DRAWMODE_OUTLINED					= 'dmol', // outline
				PGD_DRAWMODE_PATH							= 'dmpt', // path

		COREMSG_PAINTER_GETGRADIENTDATA		= 'Pgri',	// get line data
		COREMSG_PAINTER_SETGRADIENTDATA		= 'Psri',	// set line data
			PGD_GRADIENT_COL								= 'grco', // gradient color
			PGD_GRADIENT_TYPE								= 'grty', // gradient type
				PGD_GRADIENT_TYPE_LINEAR			= 'line',
				PGD_GRADIENT_TYPE_RADIAL			= 'radi',
				PGD_GRADIENT_TYPE_CIRCLE			= 'grty',
				PGD_GRADIENT_TYPE_REFLECTIVE	= 'refl',
				PGD_GRADIENT_TYPE_ROUTE				= 'rout',
			PGD_GRADIENT_DITHER							= 'grdt', // gradient type
			PGD_GRADIENT_ALPHA							= 'gral', // gradient type
			PGD_GRADIENT_INVERT							= 'grin',
			PGD_GRADIENT_BLENDMODE					= 'grbm',
			PGD_GRADIENT_BLENDSTRENGTH			= 'grbs',
			PGD_GRADIENT_NOISEVARIATION			= 'grnv',
			PGD_GRADIENT_NOISESCALE					= 'grns',
			PGD_GRADIENT_PREVIEW						= 'grpr', // gradient type
			PGD_GRADIENT_ADDPRESET					= 'grAd', // gradient type

		COREMSG_PAINTER_GETPOLYFILLDATA		= 'Pgpf',	// get polyfill data
		COREMSG_PAINTER_SETPOLYFILLDATA		= 'Pspf',	// set polyfill data
			PGD_POLYFILL_BORDER							= 'pfbd', // soft edge
			PGD_POLYFILL_ANTIALIAS					= 'anti', // antialiasing
			PGD_POLYFILL_BRUSHEDITOR				= 'bred',
			PGD_POLYFILL_BRUSHEDITORTAB			= 'bftb',

		COREMSG_PAINTER_GETCIRCLEDATA			= 'Pgcr',	// get circle data
		COREMSG_PAINTER_SETCIRCLEDATA			= 'Pscr',	// set circle data
			PGD_CIRCLE_CIRCLEMODE						= 'crmd',
				PGD_CIRCLEMODE_DIAMETER				= 'cdia',
				PGD_CIRCLEMODE_RADIUS					= 'crad',
			PGD_CIRCLE_ASPECTRATIO					= 'casp',	// aspect ratio circle

		COREMSG_PAINTER_GETPICKUPDATA			= 'Pgpd',	// get circle data
		COREMSG_PAINTER_SETPICKUPDATA			= 'Pspd',	// set circle data
			PGD_PICKUP_MODE									= 'mode', // mode
				PICKUP_MODE_SOLIDCOLOR				= 'soli', // mode
				PICKUP_MODE_TEXTURECLONE			= 'clon', // mode
				PICKUP_MODE_TEXTUREBRUSH			= 'brus', // mode
			PGD_PICKUP_SOLID_RANGE					= 'Srad', // sample radius
			PGD_PICKUP_SOLID_ENABLESAMPLING	= 'Sran', // enable sampling

		COREMSG_PAINTER_GETSPECIALBRUSH		= 'Pgpd',	// get special brush
		COREMSG_PAINTER_SETSPECIALBRUSH		= 'Pspd',	// set special brush
			PGD_SPECIALBRUSH_MODE								= 'Pdod',
				PGD_SPECIALBRUSH_MODE_SHADOW				= 1,
				PGD_SPECIALBRUSH_MODE_MID						= 2,
				PGD_SPECIALBRUSH_MODE_HIGH					= 3,
				PGD_SPECIALBRUSH_MODE_SAT						= 4,
				PGD_SPECIALBRUSH_MODE_DESAT					= 5,
			PGD_SPECIALBRUSH_STRENGTH						= 'Pstr',

	COREMSG_DUMMY
};
