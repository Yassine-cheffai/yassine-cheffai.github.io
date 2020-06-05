CONTAINER CMstandard
{
	NAME CMstandard;
	INCLUDE Omappedcurve;

	GROUP ID_MAPPEDCURVEPROPERTIES
	{
 		BOOL MAPPEDCURVE_SEGMENTED {}

		LINK MAPPEDCURVE_SURFACE
    { 
      ACCEPT { 1015548; } 
    }

		LINK MAPPEDCURVE_ORIGIN
    { 
      ACCEPT { 1015549; } 
    }
	}
}
