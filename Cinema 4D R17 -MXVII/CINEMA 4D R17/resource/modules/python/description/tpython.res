CONTAINER Tpython
{
	NAME Tpython;
	INCLUDE Texpression;
	
	GROUP Obaselist
	{
		GROUP
		{
			BOOL TPYTHON_FRAME { }
		}
	}
	
	GROUP ID_TAGPROPERTIES
	{	
		SCALE_V;
		BOOL TPYTHON_RESET { }
		
		STRING TPYTHON_CODE
		{
		  CUSTOMGUI MULTISTRING;
		  PYTHON;
		  SCALE_V;
		}
		
		BUTTON TPYTHON_OPENEDITOR { }
	}
}
