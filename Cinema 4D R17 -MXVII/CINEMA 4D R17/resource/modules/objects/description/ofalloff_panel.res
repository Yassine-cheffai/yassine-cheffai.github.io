CONTAINER Ofalloff_panel
{
	NAME	Ofalloff_panel;

	GROUP FALLOFF_GROUPFALLOFF
	{
		DEFAULT 0;

		LONG	FALLOFF_MODE
		{
			CYCLE
			{

			}
			SCALE_H;
		}
		GROUP
		{
			COLUMNS 2;

			BOOL	FALLOFF_INVERT {}
			BOOL	FALLOFF_VISIBLE {}
		}
	}
}
