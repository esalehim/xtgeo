/*
 ******************************************************************************
 *
 * NAME:
 *    cube_get_randomline.c
 *
 * AUTHOR(S):
 *    Jan C. Rivenaes
 *
 * DESCRIPTION:
 *    Given X Y Z vectors, return a a randomline array
 *
 * ARGUMENTS:
 *    x, y           i     Arrays coords XY
 *    zmin, zmax     i     Vertical range
 *    zsampling      i     vertical sampling increment
 *    xinc..rot_deg  i     Cube geometry description
 *    yflip          i     If the cube is flipped in Y (1 or -1)
 *    nx ny nz       i     Cube dimensions
 *    p_val_v        i     3D cube values
 *    value          o     Randomline array
 *    option         i     0: nearest value, 1: interpolate
 *    debug          i     Debug level
 *
 * RETURNS:
 *    Array length, -1 if fail
 *
 * TODO/ISSUES/BUGS:
 *
 * LICENCE:
 *    Statoil property
 ******************************************************************************
 */


#include "libxtg.h"
#include "libxtg_.h"

int cube_get_randomline(
                        double *xvec,
                        long nxvec,
                        double *yvec,
                        long nyvec,
                        double zmin,
                        double zmax,
                        int nzsam,
                        double xori,
                        double xinc,
                        double yori,
                        double yinc,
                        double zori,
                        double zinc,
                        double rot_deg,
                        int yflip,
                        int nx,
                        int ny,
                        int nz,
                        float *p_val_v,
                        long ncube,
                        double *values,
                        long nvalues,
                        int option,
                        int debug
                        )
{
    /* locals */
    char sbn[24] = "cube_get_randomline";
    int  ib, ic, izc, ier;
    float val, zsam, xc, yc, zc;

    xtgverbose(debug);

    if (debug > 2) xtg_speak(sbn, 3, "Entering routine %s", sbn);

    zsam = (zmax - zmin) / (nzsam - 1);

    ib = 0;
    for (ic = 0; ic < nxvec; ic++) {
        xc = xvec[ic];
        yc = yvec[ic];

        for (izc = 0; izc < nzsam; izc++) {

            zc = zmin + izc * zsam;

            ier = cube_value_xyz_cell(xc, yc, zc, xori, xinc, yori, yinc,
                                      zori, zinc, rot_deg, yflip,
                                      nx, ny, nz, p_val_v, &val, 0,
                                      debug);

            if (ier == 0) values[ib] = val;
            if (ier != 0) values[ib] = UNDEF;

            ib++;
        }
    }

    if (nvalues != ib) {
        xtg_error(sbn, "Bug probably in %s", sbn);
        return -1;
    }

    return EXIT_SUCCESS;

}
