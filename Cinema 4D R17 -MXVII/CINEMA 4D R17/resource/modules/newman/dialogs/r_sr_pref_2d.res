// C4D-DialogResource

DIALOG R_SR_PREF_2D
{
  NAME T1;

  GROUP
	{
		COLUMNS 1;

		GROUP
		{
			COLUMNS 2;
			STATICTEXT { NAME TF; } FILENAME IDC_2D_PATHNAME { SIZE 560,0; SAVE; } 
		}

		GROUP
		{
			COLUMNS 3;
			STATICTEXT { NAME T3; }
			GROUP
			{
				COLUMNS 2;

				COMBOBOX IDC_2D_MODE
				{
					ALIGN_LEFT;
					CHILDS
					{
						0,T4;
						1,T5;
						2,T6;
					}
				}
			}

			CHECKBOX IDC_2D_PLAY { NAME T7; }
		}
	}


  GROUP
	{
		COLUMNS 2;
		BUTTON IDC_CALCULATE { NAME TC; }
	}
}
