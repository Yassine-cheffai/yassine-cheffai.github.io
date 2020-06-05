CONTAINER OP_Rolling
{
	NAME OP_Rolling;
	INCLUDE GVbase;

	GROUP ID_GVPROPERTIES
	{
	}

	GROUP ID_GVPORTS
	{
		LONG				ROLL_TYPE				{ INPORT; EDITPORT;
																	CYCLE
																	{
																			ROLL_TYPE_ROLL;
																			ROLL_TYPE_TRAVEL;
																		}
																}	

		REAL				ROLL_LAZINESS		{ INPORT; EDITPORT; MIN 0.0; STEP 0.01;}
		VECTOR			ROLL_AXIS					{ INPORT; EDITPORT; }
		LONG				ROLL_SOURCE    	{ INPORT; EDITPORT; 
																		CYCLE 
																		{ 
																			ROLL_SOURCE_X; 
																			ROLL_SOURCE_Y;
																			ROLL_SOURCE_Z;
																		}
																	}
		BOOL				ROLL_INVERT			{ INPORT; EDITPORT; }
		REAL				ROLL_STRETCH			{ INPORT; EDITPORT; UNIT PERCENT; STEP 0.1;}
		
		BOOL      	IN_ROLL_ON				{ INPORT; PORTONLY; }
		BASETIME   	IN_ROLL_ATIME			{ INPORT; PORTONLY; }
		TP_PARTICLE	IN_ROLL_PARTICLE	{ INPORT; PORTONLY; STATICPORT; NEEDCONNECTION; CREATEPORT; }
	}
}
