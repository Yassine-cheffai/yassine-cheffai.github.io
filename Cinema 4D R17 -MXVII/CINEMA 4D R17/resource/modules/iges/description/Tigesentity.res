CONTAINER Tigesentity
{
	NAME Tigesentity;
	INCLUDE Texpression;

	GROUP ID_TAGPROPERTIES
	{
		STATICTEXT IGESENTITY_DESCRIPTION { ANIM OFF; }

		SEPARATOR { LINE; }
		GROUP
		{
			LAYOUTGROUP; COLUMNS 2;
		    GROUP
		    {
                LONG IGESENTITY_NUMBER { }
                LONG IGESENTITY_TYPE { }
                LONG IGESENTITY_FORMTYPE { }
                STRING IGESENTITY_LABEL { }
                LONG IGESENTITY_SUBSCRIPT { }

                LONG IGESENTITY_STATUS1 { }
                LONG IGESENTITY_STATUS2 { }
                LONG IGESENTITY_STATUS3 { }
                LONG IGESENTITY_STATUS4 { }

                LONG IGESENTITY_LEVEL { }
                LONG IGESENTITY_LINEWEIGHT { }
                LONG IGESENTITY_LINEFONT { }
                LONG IGESENTITY_COLOR { }
            }

		    GROUP
		    {
                LONG IGESENTITY_NRSTRUCTURE { }
                LONG IGESENTITY_NRLINEFONT { }
                LONG IGESENTITY_NRLEVEL { }
                LONG IGESENTITY_NRVIEW { }
                LONG IGESENTITY_NRMATRIX { }
                LONG IGESENTITY_NRLABEL { }
                LONG IGESENTITY_NRCOLOR { }

                LONG IGESENTITY_PNUMBER { }
                LONG IGESENTITY_PCOUNT { }
                LONG IGESENTITY_RESERVED1 { }
                LONG IGESENTITY_RESERVED2 { }
            }
		}
	}
}
