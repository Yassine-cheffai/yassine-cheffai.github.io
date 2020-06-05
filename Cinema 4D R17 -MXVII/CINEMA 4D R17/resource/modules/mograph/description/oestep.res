CONTAINER Oestep
{
	NAME		Oestep;

	INCLUDE Obaseeffector;

	GROUP		ID_MG_BASEEFFECTOR_GROUPEFFECTOR
	{
		SPLINE	MGSTEPEFFECTOR_SPLINE
		{
			SHOWGRID_H;
			SHOWGRID_V;

			GRIDSIZE_H 8;
			GRIDSIZE_V 8;

			HAS_PRESET_BTN;

			MINSIZE_H 120;
			MINSIZE_V 90;

			EDIT_H;
			EDIT_V;

			LABELS_H;
			LABELS_V;

			HAS_ROUND_SLIDER;

			X_MIN 0;
			X_MAX 1;

			Y_MIN 0;
			Y_MAX 1;

			X_STEPS 1;
			Y_STEPS 1;
		}
		LONG	MGSTEPEFFECTOR_GAP
		{
			MIN 0;
		}
	}
}
