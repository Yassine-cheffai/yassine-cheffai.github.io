CONTAINER MGgridarray
{
	NAME		MGgridarray;
	INCLUDE Obase;
	INCLUDE otransform_panel;

	GROUP		ID_OBJECTPROPERTIES
	{
		GROUP
		{
			COLUMNS 1;

			VECTOR	MG_GRID_RESOLUTION
			{
				MIN 1;
				MAX 2000000;
				STEP 1;
				OPEN;
			}
			VECTOR	MG_GRID_SIZE
			{
				UNIT	METER;
				OPEN;
			}
			GROUP
			{
				COLUMNS 2;
				LONG	MG_GRID_TYPE
				{
					CYCLE
					{
						MG_GRID_TYPE_CUBIC;
						MG_GRID_TYPE_SPHERE;
						MG_GRID_TYPE_TUBE;
					}
				}
				REAL	MG_GRID_INSIDE
				{
					UNIT	PERCENT;
					MIN 0;
					MAX 100;
				}
			}
		}
	}
	GROUP ID_MG_TRANSFORM_GROUPTRANSFORM
	{
		LONG	MG_GRID_UVAXIS
		{
			CYCLE
			{
				MG_GRID_UVAXIS_XP;
				MG_GRID_UVAXIS_XN;
				MG_GRID_UVAXIS_YP;
				MG_GRID_UVAXIS_YN;
				MG_GRID_UVAXIS_ZP;
				MG_GRID_UVAXIS_ZN;
			}
		}
	}
}
