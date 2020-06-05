CONTAINER GVmograph_falloff
{
	NAME		GVmograph_falloff;

	INCLUDE GVbase;
	INCLUDE Ofalloff_panel;

	GROUP		ID_GVPORTS
	{
		MATRIX	GV_MG_FALLOFF_MATRIX
		{
			INPORT;
			STATICPORT;
			CREATEPORT;
		}
		VECTOR	GV_MG_FALLOFF_POSITION
		{
			INPORT;
			STATICPORT;
			CREATEPORT;
		}
		REAL	GV_MG_FALLOFF_VALUE
		{
			OUTPORT;
			STATICPORT;
			CREATEPORT;
		}
	}
}
