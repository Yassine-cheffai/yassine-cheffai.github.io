/***************************************************************************
 *  Copyright (C) 2006 MAXON Computer GmbH                                 *
 *                                                                         *
 *  These coded instructions, statements, and computer programs contain    *
 *  unpublished proprietary information of MAXON Computer GmbH,            *
 *  and are protected by copyright law. They  may  not be disclosed        *
 *  to third parties or copied or duplicated in any form, in whole or      *
 *  in part, without the prior written consent of MAXON Computer GmbH.     *
 *                                                                         *
 ***************************************************************************/

/* For point P (we are passed both the current and shader space
 * coordinates), gather illumination from the light sources and
 * compute the smoke density at that point.
 */


void
smokedensity (point Pcur;
	      output color Lscatter; output float smoke)
{
    Lscatter = 0;
    illuminance ("foglight", Pcur) {
    	extern color Cl;
  	Lscatter += Cl;
    }
    
    smoke = 1;
}


/* Return a component-by-component exp() of a color */
color colorexp (color C)
{
    return color (exp(comp(C,0)), exp(comp(C,1)), exp(comp(C,2)));
}


/* 0 means no fog, 1 means only fog */
float fogdensity (float dist; float fogstrength; float fogdistance)
{
    float ret = 0;
    if (fogstrength < 1e-6) ret = 0;
    else if (dist <= 1e-6) ret = 0;
    else if (dist >= fogdistance) ret = 1;
    else if (fogdistance <= 1e-6) ret = 1;
    else ret = (dist / fogdistance) * fogstrength;
    return ret;
} 


volume
c4dvolume(float stepsize = 0;
          float integstart = 0;
          float integend = 10000;
          color fogcolor = color "rgb" (1, 1, 1);
          float fogstrength = 0;
          float fogdistance = 10000;
		  output varying color __atmosphere = color( 0, 0, 0 );
		  output varying float __atmosphere_mul = 1;
) {
    point Worigin = P - I;    /* Origin of volume ray */
    point origin = transform ("shader", Worigin);
	
    /*
    ** Volumetric Integration Step
    ** Only used if stepsize is > 0.00001 and integstart and integend
    ** do not have the same value!
    */
    color Cv = 0, Ov = 0;   /* color & opacity of volume that we accumulate */
    if ( stepsize > 0.00001 && abs(integstart - integend) > 0.00001 ) {
        float lightdensity   = 1;
        float opacdensity    = 0;
        color scatter        = color(1, 1, 1);
        float dtau, last_dtau;
        color li, last_li;

#ifdef AIR
        
        vector dir = normalize(I);
        point Pstart = (P - I) + dir * integstart;
        point Pend   = (P - I) + dir * integend;
        Cv = 2 * foglight(Pstart, Pend, stepsize, lightdensity, opacdensity, Ov);
#else
        /* Integrate forwards from the start point */
        float d = integstart + random() * stepsize;
        vector IN = normalize (vtransform ("shader", I));
        vector WIN = vtransform ("shader", "current", IN);

        /* Calculate a reasonable step size */
        float end = min (length (I), integend) - 0.0001;
        float ss = min (stepsize, end - d);
        
        /* Get the in-scattered light and the local smoke density for the
        ** beginning of the ray 
        */
        smokedensity (Worigin + d*WIN /*, origin + d*IN, ss*/, last_li, last_dtau);

        while (d <= end) {
            /* Take a step and get the local scattered light and smoke density */
            ss = clamp (ss, 0.005, end - d);
            d += ss;
            smokedensity (Worigin + d*WIN, li, dtau);

            /* Find the blocking and light scattering contribution of 
            ** the portion of the volume covered by this step.
            */
            float tau      = opacdensity  * ss * 0.5 * (dtau + last_dtau);
            color lighttau = lightdensity * ss * 0.5 * (li * dtau + last_li * last_dtau);
            /* lighttau /= 2000; */

            /* Composite with exponential extinction of background light */
            /*float fog = fogdensity(d, fogstrength, fogdistance);*/
            Cv += (1 - Ov) * lighttau;
            Ov += (1 - Ov) * (1 - colorexp( -tau * scatter ));
            last_dtau = dtau;
            last_li   = li;
        }
#endif
    }
    
    /* Ci & Oi are the color and opacity of the background element.
     * Now Cv is the light contributed by the volume itself, and Ov is the
     * opacity of the volume, i.e. (1-Ov)*Ci is the light from the background
     * which makes it through the volume.  So just composite!
     */
    __atmosphere_mul = 1 - fogdensity(length(I), fogstrength, fogdistance);
	__atmosphere = (1 - __atmosphere_mul) * fogcolor + Cv;
    Ci = __atmosphere_mul * Ci + __atmosphere;
    Oi = Oi;
}