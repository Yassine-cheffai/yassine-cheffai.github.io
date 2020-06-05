CONTAINER Oinstance
{
	NAME Oinstance;
	INCLUDE Obase;

	GROUP ID_OBJECTPROPERTIES
	{
		LINK INSTANCEOBJECT_LINK 
		{
			ACCEPT
			{
				Obase;
			}
		}
		BOOL INSTANCEOBJECT_RENDERINSTANCE { }
	}
}
