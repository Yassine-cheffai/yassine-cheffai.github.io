CONTAINER Prefsmaterial
{
	NAME Prefsmaterial;
	GROUP PREF_MM_MAIN_GROUP
	{
	
		DEFAULT 1;
		COLUMNS 2;
		
		
		LONG PREF_MM_SIZE
		{
			ANIM OFF;

			CYCLE
			{
				PREF_MM_SIZE_SMALL;
				PREF_MM_SIZE_MED;
				PREF_MM_SIZE_LARGE;
				PREF_MM_SIZE_HUGE;
	
			}
			
		}
		STATICTEXT{}
		
		LONG PREF_MM_SHSCENE
		{
			ANIM OFF;
			CYCLE
			{
	
			}
		}
		STATICTEXT{}
		
		LONG PREF_MM_MATSCENE
		{
			ANIM OFF;
			CYCLE
			{
	
			}
		}
		STATICTEXT{}
		
		LONG PREF_MM_FRATE {ANIM OFF; MIN 1; MAX 100;}
		STATICTEXT{}
		LONG PREF_MM_PREV {ANIM OFF; MIN 0;}
		STATICTEXT{}
		
	}
}
