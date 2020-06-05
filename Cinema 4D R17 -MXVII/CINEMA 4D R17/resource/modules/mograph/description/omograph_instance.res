CONTAINER Omograph_instance
{
	NAME		Omograph_instance;
	INCLUDE Obase;
	INCLUDE Obasemogen;

	GROUP		ID_OBJECTPROPERTIES
	{
		GROUP
		{
			COLUMNS 1;
			LINK	MGINSTANCER_LINK
			{
				ACCEPT { Obase; }
			}
			LONG	MGINSTANCER_DEPTH
			{
				MIN 1;
				MAXSLIDER 100;
				CUSTOMGUI LONGSLIDER;
			}
		}
	}
}
