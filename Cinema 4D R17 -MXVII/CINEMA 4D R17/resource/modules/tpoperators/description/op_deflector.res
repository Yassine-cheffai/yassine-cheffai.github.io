CONTAINER OP_Deflector
{
	NAME OP_Deflector;
	INCLUDE GVbase;

	GROUP ID_GVPROPERTIES
	{
		//BOOL	UD_EVENT{ }
		LONG	UD_SEED { MIN 0; }
	}

	GROUP ID_GVPORTS
	{
		LINK				UD_OBJECT					{ INPORT; EDITPORT; ACCEPT { Obase; } }		
		LONG				UD_DTYPE					{ INPORT; EDITPORT; 
																		CYCLE 
																		{ 
																			UD_DTYPE_BOX; 
																			UD_DTYPE_SPHERE;
																			UD_DTYPE_OBJECT;
																			UD_DTYPE_GEOM;
																		}
																	}
		REAL				UD_XSIZE					{ INPORT; EDITPORT; MIN 0.0; }
		REAL				UD_YSIZE					{ INPORT; EDITPORT; MIN 0.0; }
		REAL				UD_ZSIZE					{ INPORT; EDITPORT; MIN 0.0; }
		REAL				UD_RADIUS					{ INPORT; EDITPORT; MIN 0.0; }
		LONG				UD_CTYPE					{ INPORT; EDITPORT; 
																		CYCLE 
																		{ 
																			UD_CTYPE_FRONT; 
																			UD_CTYPE_BACK;
																			UD_CTYPE_TWO;
																		}
																	}
		
		LONG				UD_OTYPE					{ INPORT; EDITPORT; 
																		CYCLE 
																		{ 
																			UD_OTYPE_NONE; 
																			UD_OTYPE_PSIZE_TRAVEL;
																			UD_OTYPE_PSIZE_X;
																			UD_OTYPE_PSIZE_Y;
																			UD_OTYPE_PSIZE_Z;
																			UD_OTYPE_VALUE_TRAVEL;
																			UD_OTYPE_VALUE_X;
																			UD_OTYPE_VALUE_Y;
																			UD_OTYPE_VALUE_Z;
																		}
																	}
		REAL				UD_OFFSET					{ INPORT; EDITPORT; }
		REAL				UD_OFFSETVAR			{ INPORT; EDITPORT; UNIT PERCENT; MIN 0.0; MAX 100.0; STEP 1.0; }


		
		REAL				UD_BOUNCE					{ INPORT; EDITPORT; UNIT PERCENT; MIN 0.0; STEP 1.0; }
		REAL				UD_BOUNCEVAR			{ INPORT; EDITPORT; UNIT PERCENT; MIN 0.0; MAX 100.0; STEP 1.0; }
		REAL				UD_SURFACE				{ INPORT; EDITPORT; UNIT PERCENT; MIN 0.0; MAX 100.0; STEP 1.0; }
		REAL				UD_ENERGY					{ INPORT; EDITPORT; UNIT PERCENT; MIN 0.0; STEP 1.0; }
		REAL				UD_FRICTION				{ INPORT; EDITPORT; UNIT PERCENT; MIN 0.0; MAX 100.0; STEP 1.0; }
		REAL				UD_SPIN						{ INPORT; EDITPORT; UNIT PERCENT; MIN 0.0; STEP 1.0; }
		REAL				UD_CHAOS					{ INPORT; EDITPORT; UNIT PERCENT; MIN 0.0; MAX 100.0; STEP 1.0; }
		REAL				UD_VELINHERI			{ INPORT; EDITPORT; UNIT PERCENT; STEP 1.0; }

		BOOL				UD_EVENT					{ INPORT; EDITPORT; }

		
		TP_PARTICLE	IN_UD_PARTICLE	{ INPORT; PORTONLY; STATICPORT; NEEDCONNECTION; CREATEPORT;}
		BOOL      	IN_UD_ON				{ INPORT; PORTONLY; }
		BASETIME   	IN_UD_ATIME			{ INPORT; PORTONLY; }

		BOOL				OUT_UD_EVENT						{ OUTPORT; PORTONLY; ITERATOR; }
		VECTOR			OUT_UD_EVENT_POSITION		{ OUTPORT; PORTONLY; ITERATOR; }
		NORMAL			OUT_UD_EVENT_NORMAL			{ OUTPORT; PORTONLY; ITERATOR; }
		NORMAL			OUT_UD_EVENT_REFLECTION	{ OUTPORT; PORTONLY; ITERATOR; }
	}
}
