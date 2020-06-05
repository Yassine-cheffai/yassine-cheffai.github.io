/***************************************************************************
 *  Copyright (C) 2008 MAXON Computer GmbH                                 *
 *                                                                         *
 *  These coded instructions, statements, and computer programs contain    *
 *  unpublished proprietary information of MAXON Computer GmbH,            *
 *  and are protected by copyright law. They  may  not be disclosed        *
 *  to third parties or copied or duplicated in any form, in whole or      *
 *  in part, without the prior written consent of MAXON Computer GmbH.     *
 *                                                                         *
 ***************************************************************************/

#ifndef C4DLIGHT_H
#define C4DLIGHT_H

float shadowPoint(vector Lrel;
		string    sfpx;
		string    sfnx;
		string    sfpy;
		string    sfny;
		string    sfpz;
		string    sfnz;
		float samples;
		float blur;
		float bias)
{
	float attenuation = 0.0;
		float Lx, Ly, Lz, AbsLx, AbsLy, AbsLz;

	extern point Ps;

				Lx = xcomp(Lrel);
				AbsLx = abs(Lx);
				Ly = ycomp(Lrel);
				AbsLy = abs(Ly);
				Lz = zcomp(Lrel);
				AbsLz = abs(Lz);

				if((AbsLx > AbsLy) && (AbsLx > AbsLz)) {
						if((Lx > 0.0)&&(sfpx != ""))
		attenuation = shadow( sfpx, Ps, "samples", samples,
																			 "blur", blur, "bias", bias );
						else if (sfnx != "")
		attenuation = shadow( sfnx, Ps, "samples", samples,
																			 "blur", blur, "bias", bias );
	}
				else if((AbsLy > AbsLx) && (AbsLy > AbsLz)) {
						if((Ly > 0.0)&&(sfpy != ""))
		attenuation = shadow( sfpy, Ps, "samples", samples,
																			 "blur", blur, "bias", bias );
						else if (sfny != "")
		attenuation = shadow( sfny, Ps, "samples", samples,
																			 "blur", blur, "bias", bias );
	}
				else if((AbsLz > AbsLy) && (AbsLz > AbsLx)) {
						if((Lz > 0.0)&&(sfpz != ""))
		attenuation = shadow( sfpz, Ps, "samples", samples,
																			 "blur", blur, "bias", bias );
						else if (sfnz != "")
		attenuation = shadow( sfnz, Ps, "samples", samples,
																			 "blur", blur, "bias", bias );
	}
	return attenuation;
}

#endif // #ifndef C4DLIGHT_H
