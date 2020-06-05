CONTAINER OP_Alignment
{
	NAME OP_Alignment;
	INCLUDE GVbase;

	GROUP ID_GVPROPERTIES
	{
	}

	GROUP ID_GVPORTS
	{
		REAL				ALIGN_LAZINESS		{ INPORT; EDITPORT; MIN 0.0; STEP 0.01;}
		LONG				ALIGN_SOURCE    	{ INPORT; EDITPORT; 
																		CYCLE 
																		{ 
																			ALIGN_SOURCE_X; 
																			ALIGN_SOURCE_Y;
																			ALIGN_SOURCE_Z;
																		}
																	}
		BOOL				ALIGN_INVERT			{ INPORT; EDITPORT; }
		REAL				ALIGN_XROT				{ INPORT; EDITPORT; UNIT DEGREE; STEP 1.0; }
		REAL				ALIGN_YROT				{ INPORT; EDITPORT; UNIT DEGREE; STEP 1.0; }
		REAL				ALIGN_ZROT				{ INPORT; EDITPORT; UNIT DEGREE; STEP 1.0; }
		LONG				ALIGN_TYPE				{ INPORT; EDITPORT;
																		CYCLE
																		{
																			ALIGN_TYPE_NONE;
																			ALIGN_TYPE_RANDOM;
																			ALIGN_TYPE_TRAVEL;
																			ALIGN_TYPE_X;
																			ALIGN_TYPE_Y;
																			ALIGN_TYPE_Z;
																			ALIGN_TYPE_USERDIR;
																			ALIGN_TYPE_USERPOS;
																		}
																	}	
		VECTOR			ALIGN_AXIS				{ INPORT; EDITPORT; }
		REAL				ALIGN_VAR					{ INPORT; EDITPORT; UNIT DEGREE; MIN 0.0; MAX 180.0; STEP 1.0;}
		BOOL      	IN_ALIGN_ON				{ INPORT; PORTONLY; }
		BASETIME   	IN_ALIGN_ATIME		{ INPORT; PORTONLY; }
		TP_PARTICLE	IN_ALIGN_PARTICLE	{ INPORT; PORTONLY; STATICPORT; NEEDCONNECTION; CREATEPORT; }
	}
}
