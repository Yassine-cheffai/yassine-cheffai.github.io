CONTAINER Tcaweight
{
	NAME Tcaweight;
	INCLUDE Tbase;

	GROUP ID_TAGPROPERTIES
	{
		SCALE_V; 
		
		GROUP
		{
			GROUP
			{
				COLUMNS 2;

				BUTTON ID_CA_WEIGHT_TAG_SET { SCALE_H; }
				BUTTON ID_CA_WEIGHT_TAG_RESET { SCALE_H; }
			}
			
			REAL ID_CA_WEIGHT_TAG_TOTALWEIGHT { UNIT PERCENT; MIN 0.0; MAX 100.0; CUSTOMGUI REALSLIDER; }
		}

		GROUP ID_CA_WEIGHT_TAG_JOINTS_GROUP
		{
			SCALE_V; DEFAULT 1;

			GROUP
			{
				SCALE_V;
				
				ITEMTREE ID_CA_WEIGHT_TAG_JOINTS_LIST
				{
					SCALE_V;
					DRAGDROP;
					ICON;
					MULTIPLE;
					ACCEPT { Obase; };
					FOLDERS;
				}
			}
			
			GROUP
			{	
				LINK ID_CA_WEIGHT_TAG_JOINT_MAP { ACCEPT { Tbase; } }
			}
		}		
	}
}
