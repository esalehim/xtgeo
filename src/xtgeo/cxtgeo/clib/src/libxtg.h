/*
 * ----------------------------------------------------------------------------
 * Python stuff SWIG (examples...; see cxtgeo.i):
 * ----------------------------------------------------------------------------
 * int    *swig_int_out_p1,         // Value of output pointers
 * double *swig_np_dbl_aout_v1,     // *p_xx_v to update argout for numpy
 * long   n_swig_np_dbl_aout_v1,    // length of nmpy array
 *
 * char    *swig_bnd_char_10k,      // bounded characters up to 10000
 *
 */

#include <stdio.h>


#define PI  3.14159265358979323846
#define PIHALF  1.57079632679489661923

#define FLOATEPS 1.0E-05
#define VERYLARGEFLOAT 10E30;
#define VERYSMALLFLOAT -10E30;

/* better? */
#define VERYLARGEPOSITIVE 10E30;
#define VERYLARGENEGATIVE -10E30;

/*
 * ----------------------------------------------------------------------------
 * Maps etc. Undef values set to very high values. Map values > UNDEF_MAP_LIMIT
 * are undefined
 * ----------------------------------------------------------------------------
 */

/* general limits in XTGeo are recoemmended for all XTGeo data types! */
#define UNDEF               10E32
#define UNDEF_LIMIT         9.9E32
#define UNDEF_INT           2000000000
#define UNDEF_INT_LIMIT     1999999999


/* specific list */
#define UNDEF_MAP              10E32
#define UNDEF_INT_MAP          2000000000
#define UNDEF_MAP_LIMIT        9.9E32
#define UNDEF_INT_MAP_LIMIT    1999999999
#define UNDEF_MAP_STORM        -999.00
#define UNDEF_MAP_IRAP         9999900.0000  /* Irap ascii */
#define UNDEF_MAP_IRAPB        1E30          /* Irap binary */
#define UNDEF_MAP_ZMAP         -99999.0      /* ZMAP binary (but can vary) */
#define UNDEF_MAP_IRAPB_LIMIT  0.99E30
#define UNDEF_CUBE_RMS         -9999.00
#define UNDEF_POINT            10E32
#define UNDEF_POINT_LIMIT      9.9E32
#define UNDEF_POINT_RMS        -999.0000
#define UNDEF_POINT_IRAP       999.0000
#define LAST_POINT             -10E30
#define LAST_POINT_LIMIT       -9.9E30


#define MAXPSTACK 5000
#define ROFFSTRLEN 100
#define ECLNAMELEN 9
#define ECLTYPELEN 5
#define ECLINTEHEADLEN 240
#define ECLDOUBHEADLEN 160
#define ECLLOGIHEADLEN 80

#define  ECLNUMBLOCKLEN 1000
#define  ECLCHABLOCKLEN 105

#define UNDEF_ROFFBYTE 255
#define UNDEF_ROFFINT -999
#define UNDEF_ROFFFLOAT -999.0

/* for eclipse ascii export e.g. GRDECL */
#define UNDEF_ECLINT 0
#define UNDEF_ECLFLOAT 0


/*
 *=============================================================================
 * GENERAL XTGEO
 *=============================================================================
 */

int xtgverbose(int);

int xtg_silent (int value);
char *xtg_verbose_file(char *filename);

int xtg_speak(
	      char *,
	      int,
	      char *,
	      ...
	      );

int xtg_warn(
	      char *,
	      int,
	      char *,
	      ...
	      );

int xtg_error(
	      char *,
	      char *,
	      ...
	      );

int xtg_shout(
	      char *,
	      char *,
	      ...
	      );

FILE *xtg_fopen(const char *filename, const char *mode);

int xtg_fclose(FILE *fhandle);

/*
 *=============================================================================
 * GENERAL FUNCTIONS
 *=============================================================================
 */

double x_interp_map_nodes (
                           double *x_v,
                           double *y_v,
                           double *z_v,
                           double x,
                           double y,
                           int method,
                           int debug
                           );

long x_ijk2ib (
	      int i,
	      int j,
	      int k,
	      int nx,
	      int ny,
	      int nz,
	      int ia_start
	      );

long x_ijk2ic (
	      int i,
	      int j,
	      int k,
	      int nx,
	      int ny,
	      int nz,
	      int ia_start
	      );

void x_ib2ijk (
	       long  ib,
	       int   *i,
	       int   *j,
	       int   *k,
	       int   nx,
	       int   ny,
	       int   nz,
	       int   ia_start
	       );

void x_ic2ijk (
	       long  ic,
	       int   *i,
	       int   *j,
	       int   *k,
	       int   nx,
	       int   ny,
	       int   nz,
	       int   ia_start
	       );

int x_vector_info2 (
		    double  x1,
		    double  x2,
		    double  y1,
		    double  y2,
		    double  *vlen,
		    double  *xangle_radian,
		    double  *xangle_degrees,
		    int     option,
		    int     debug
		    );


int x_conv_int2float (
		      int n,
		      int *v1,
		      float *v2,
		      int debug
		      );

int x_conv_int2double (
		       int n,
		       int *v1,
		       double *v2,
		       int debug
		       );

int x_conv_double2float (
			 int n,
			 double *v1,
			 float *v2,
			 int debug
			 );

double x_rotation_conv (
			double  ain,
			int     aimode,
			int     mode,
			int     option,
			int     debug
			);

/*
 * ============================================================================
 * MAPS. Note map_* is the older routines, that will not support rotation,
 * while surf_* will support rotation
 * ============================================================================
 */

void map_create (
		 int    mx,
		 int    my,
		 double xori,
		 double xstep,
		 double yori,
		 double ystep,
		 double rotation,
		 double *p_zval_v,
		 double value,
		 int    option,
		 int    debug
		 );

void map_import_irap_ascii (
			    int *ndef,
			    int *ndefsum,
			    int *nx,
			    int *ny,
			    double *xstep,
			    double *ystep,
			    double *xmin,
			    double *xmax,
			    double *ymin,
			    double *ymax,
			    double *zmin,
			    double *zmax,
			    double *p_map_v,
			    char  *file,
			    int *ierr,
			    int debug
			    );

void map_import_surfer_ascii (
			      int *ndef,
			      int *ndefsum,
			      int *nx,
			      int *ny,
			      double *xstep,
			      double *ystep,
			      double *xmin,
			      double *xmax,
			      double *ymin,
			      double *ymax,
			      double *zmin,
			      double *zmax,
			      double *p_map_v,
			      char  *file,
			      int *ierr,
			      int debug
			      );

void map_import_storm_binary (
			      int   *ndef,
			      int   *ndefsum,
			      int   *nx,
			      int   *ny,
			      double *xstep,
			      double *ystep,
			      double *xmin,
			      double *xmax,
			      double *ymin,
			      double *ymax,
			      double *zmin,
			      double *zmax,
			      double *p_map_v,
			      char  *file,
			      int   *ierr,
			      int   debug
			      );

/* the swg* names are for eventual typemaps signatures in the cxtgeo.i
 * file to SWIG */
int surf_import_irap_bin(
			 char   *filename,
			 int    mode,
			 int    *swig_int_out_p1,       // *p_mx
			 int    *swig_int_out_p2,       // *p_my
			 long   *swig_lon_out_p1,       // *p_ndef
			 double *swig_dbl_out_p1,       // *p_xori
			 double *swig_dbl_out_p2,       // *p_yori
			 double *swig_dbl_out_p3,       // *p_xinc
			 double *swig_dbl_out_p4,       // *p_yinc
			 double *swig_dbl_out_p5,       // *p_rot
			 double *swig_np_dbl_aout_v1,   // *p_map_v
			 long   n_swig_np_dbl_aout_v1,  // nmap
			 int    option,
			 int    debug
			 );


int surf_import_irap_ascii(
                           char   *filename,
                           int    mode,
                           int    *swig_int_out_p1,       // *nx
                           int    *swig_int_out_p2,       // *ny
                           long   *swig_lon_out_p1,       // *ndef
                           double *swig_dbl_out_p1,       // *xori
                           double *swig_dbl_out_p2,       // *yori
                           double *swig_dbl_out_p3,       // *xinc
                           double *swig_dbl_out_p4,       // *yinc
                           double *swig_dbl_out_p5,       // *rot
                           double *swig_np_dbl_aout_v1,   // *p_map_v
                           long   n_swig_np_dbl_aout_v1,  // nmap
                           int    option,
                           int    debug
                           );

int surf_import_ijxyz(
                      char *file,
                      int mode,
                      int *swig_int_out_p1,         // *nx
                      int *swig_int_out_p2,         // *ny
                      long *swig_lon_out_p1,        // *ndef,
                      double *swig_dbl_out_p1,      // *xori,
                      double *swig_dbl_out_p2,      // *yori,
                      double *swig_dbl_out_p3,      // *xinc,
                      double *swig_dbl_out_p4,      // *yinc,
                      double *swig_dbl_out_p5,      // *rot,
                      int *swig_np_int_aout_v1,     // *ilines,
                      long n_swig_np_int_aout_v1,   // nilines or ncol,
                      int *swig_np_int_aout_v2,     // *xlines,
                      long n_swig_np_int_aout_v2,   // nxlines or nrow,
                      double *swig_np_dbl_aout_v1,  // *p_map_v
                      long n_swig_np_dbl_aout_v1,   // nmap = nrow*ncol
                      int *swig_int_out_p3,         // yflip
                      int option,
                      int debug
                      );


void map_import_arcflt (
		       int   *ndef,
		       int   *ndefsum,
		       int   *nx,
		       int   *ny,
		       double *xstep,
		       double *ystep,
		       double *xmin,
		       double *xmax,
		       double *ymin,
		       double *ymax,
		       double *zmin,
		       double *zmax,
		       double *p_map_v,
		       char  *file1,
		       char  *file2,
		       int   *ierr,
		       int   debug
		       );


void map_export_storm_binary (
			      int nx,
			      int ny,
			      double xstep,
			      double ystep,
			      double xmin,
			      double xmax,
			      double ymin,
			      double ymax,
			      double *p_map_v,
			      char  *file,
			      int   debug
			      );
int surf_export_irap_bin(
			 char   *filename,
			 int    mx,
			 int    my,
			 double xori,
			 double yori,
			 double xinc,
			 double yinc,
			 double rot,
			 double *swig_np_dbl_in_v1,     // *p_map_v,
                         long   n_swig_np_dbl_in_v1,    // mxy
			 int    option,
			 int    debug
			 );

int surf_export_irap_ascii(
                           char   *filename,
                           int    mx,
                           int    my,
                           double xori,
                           double yori,
                           double xinc,
                           double yinc,
                           double rot,
                           double *swig_np_dbl_in_v1,     // *p_map_v
                           long   n_swig_np_dbl_in_v1,    // mxy
                           double zmin,
                           double zmax,
                           int    option,
                           int    debug
                           );

int surf_export_zmap_ascii(
                           char *filename,
                           int mx,
                           int my,
                           double xori,
                           double yori,
                           double xinc,
                           double yinc,
                           double *swig_np_dbl_in_v1,     // *p_map_v
                           long   n_swig_np_dbl_in_v1,    // mxy
                           double zmin,
                           double zmax,
                           int option,
                           int debug
                           );

int surf_export_storm_bin(
                          char *filename,
                          int mx,
                          int my,
                          double xori,
                          double yori,
                          double xinc,
                          double yinc,
                          double *swig_np_dbl_in_v1,     // *p_map_v
                          long   n_swig_np_dbl_in_v1,    // mxy
                          double zmin,
                          double zmax,
                          int option,
                          int debug
                          );

int surf_export_ijxyz(
                      char *filename,
                      int mx,
                      int my,
                      double xori,
                      double yori,
                      double xinc,
                      double yinc,
                      double rot,
                      int yflip,
                      int *swig_np_int_in_v1,      // ilines
                      long n_swig_np_int_in_v1,    // ncol
                      int *swig_np_int_in_v2,      // xlines
                      long n_swig_np_int_in_v2,    // nrow
                      double *swig_np_dbl_in_v1,     // *p_map_v
                      long   n_swig_np_dbl_in_v1,    // mxy
                      int option,
                      int debug
                      );

int surf_swapaxes (
                   int *nx,
                   int *ny,
                   int *yflip,
                   double xori,
                   double *xinc,
                   double yori,
                   double *yinc,
                   double *rotation,
                   double *swig_np_dbl_inplace_v1,   // *p_map_v
                   long n_swig_np_dbl_inplace_v1,   // nval, total nx*ny*nz
                   int option,
                   int debug
                   );

int surf_zminmax(
                 int nx,
                 int ny,
                 double *p_map_v,
                 double *zmin,
                 double *zmax,
                 int debug
                 );

void map_export_arcflt (
			int nx,
			int ny,
			double xstep,
			double ystep,
			double xmin,
			double xmax,
			double ymin,
			double ymax,
			double *p_map_v,
			char  *file1,
			char  *file2,
			int   debug
			);

void map_export_surfer_ascii (
			      int nx,
			      int ny,
			      double xstep,
			      double ystep,
			      double xmin,
			      double xmax,
			      double ymin,
			      double ymax,
			      double *p_map_v,
			      char  *file,
			      int   debug
			      );

void map_export_ev_ascii (
			  int nx,
			  int ny,
			  double xstep,
			  double ystep,
			  double xmin,
			  double xmax,
			  double ymin,
			  double ymax,
			  double *p_map_v,
			  char  *file,
			  int   debug
			  );


void map_set_value (
		    int nx,
		    int ny,
		    double *p_zval_v,
		    double value,
		    int debug
		    );


void map_operation_value (
			  int mode,
			  int nx,
			  int ny,
			  double *p_zval_v,
			  double value,
			  double value2,
			  double value3,
			  int debug
			  );


int map_median_filter (
		       double *map_in_v,
		       int nx,
		       int ny,
		       int nradius,
		       int mode
		       );



void map_pol_mask(
		  int nx,
		  int ny,
		  double xstep,
		  double ystep,
		  double xmin,
		  double ymin,
		  double *p_map_v,
		  double *p_xp_v,
		  double *p_yp_v,
		  int   np,
		  int   debug
		  );


void map_chk_point_between(
			   double x,
			   double y,
			   double z,

			   int nx1,
			   int ny1,
			   double xstep1,
			   double ystep1,
			   double xmin1,
			   double ymin1,
			   double *p_map1_v,

			   int nx2,
			   int ny2,
			   double xstep2,
			   double ystep2,
			   double xmin2,
			   double ymin2,
			   double *p_map2_v,

			   int   *outside,
			   double *zdiff,

			   int debug
			   );

int map_get_corners_xy(
		       double x,
		       double y,
		       int    nx,
		       int    ny,
		       double xstep,
		       double ystep,
		       double xmin,
		       double ymin,
		       double *p_map_v,
		       int    debug
		       );

int surf_xyz_from_ij(
		     int    i,
		     int    j,
		     double *swig_dbl_out_p1,  //  *x
		     double *swig_dbl_out_p2,  //  *y
		     double *swig_dbl_out_p3,  //  *z
		     double xori,
		     double xinc,
		     double yori,
		     double yinc,
		     int    nx,
		     int    ny,
                     int    yflip,
		     double rot_deg,
                     double *swig_np_dbl_in_v1,     // *p_map_v,
                     long   n_swig_np_dbl_in_v1,    // nn
		     int    flag,
		     int    debug
		     );

int surf_xyori_from_ij(
                       int i,
                       int j,
                       double x,
                       double y,
                       double *xori,
                       double xinc,
                       double *yori,
                       double yinc,
                       int nx,
                       int ny,
                       int yflip,
                       double rot_deg,
                       int flag,
                       int debug
                       );

double surf_get_z_from_ij(
                          int ic,
                          int jc,
                          double x,
                          double y,
                          int nx,
                          int ny,
                          double xinc,
                          double yinc,
                          double xori,
                          double yori,
                          double *p_map_v,
                          int debug
                          );


double surf_get_z_from_xy(
			  double x,
			  double y,
			  int    nx,
			  int    ny,
			  double xori,
			  double yori,
			  double xinc,
			  double yinc,
                          int    yflip,
			  double rot_deg,
                          double *swig_np_dbl_in_v1,    // *p_map_v
                          long   n_swig_np_dbl_in_v1,   // nn
			  int    debug
			  );

int surf_get_zv_from_xyv(
                         double *swig_np_dbl_in_v1,       // *xv
                         long   n_swig_np_dbl_in_v1,
                         double *swig_np_dbl_in_v2,       // *yv
                         long   n_swig_np_dbl_in_v2,
                         double *swig_np_dbl_inplace_v1,  // *zv
                         long   n_swig_np_dbl_inplace_v1,
                         int    nx,
                         int    ny,
                         double xori,
                         double yori,
                         double xinc,
                         double yinc,
                         int    yflip,
                         double rot_deg,
                         double *swig_np_dbl_in_v3,       // *p_map_v,
                         long   n_swig_np_dbl_in_v3,
                         int    debug
                         );

int surf_xy_as_values(
		      double xori,
		      double xinc,
		      double yori,
		      double yinc,
		      int    nx,
		      int    ny,
		      double rot_deg,
                      double *swig_np_dbl_aout_v1,  // *p_x_v
                      long   n_swig_np_dbl_aout_v1, // nn1
                      double *swig_np_dbl_aout_v2,  // *p_y_v
                      long   n_swig_np_dbl_aout_v2, // nn2
		      int    flag,
		      int    debug
		      );

int surf_slice_grd3d (int mcol,
                      int mrow,
                      double xori,
                      double xinc,
                      double yori,
                      double yinc,
                      double rotation,
                      int yflip,
                      double *swig_np_dbl_in_v1,   // *p_zslice_v
                      long n_swig_np_dbl_in_v1,
                      double *swig_np_dbl_aout_v1, // *p_map_v to update argout
                      long n_swig_np_dbl_aout_v1,
                      int ncol,
                      int nrow,
                      int nlay,
                      double *p_coord_v,
                      double *p_zcorn_v,
                      int *p_actnum_v,
                      double *p_prop_v,
                      int buffer,
                      int option,
                      int debug
                      );
int surf_resample(
                  int    nx1,
                  int    ny1,
                  double xori1,
                  double xinc1,
                  double yori1,
                  double yinc1,
                  int    yflip1,
                  double rota1,
                  double *swig_np_dbl_in_v1,  //*mapv1, IN only
                  long   n_swig_np_dbl_in_v1,
                  int    nx2,
                  int    ny2,
                  double xori2,
                  double xinc2,
                  double yori2,
                  double yinc2,
                  int    yflip2,
                  double rota2,
                  double *swig_np_dbl_inplace_v1,  // *mapv2 INPLACE
                  long   n_swig_np_dbl_inplace_v1,
                  int    option,
                  int    debug
                  );


int surf_get_dist_values(
			 double xori,
			 double xinc,
			 double yori,
			 double yinc,
			 int    nx,
			 int    ny,
			 double rot_deg,
			 double x0,
			 double y0,
			 double azimuth,
			 double *swig_np_dbl_inplace_v1,  // *p_map_v INPLACE
                         long   n_swig_np_dbl_inplace_v1,
			 int    flag,
			 int    debug
			 );

double map_get_z_from_xy(
			double x,
			double y,

			int nx,
			int ny,
			double xstep,
			double ystep,
			double xmin,
			double ymin,
			double *p_map_v,

			int debug
			);

double map_get_z_from_ij(
                         int ic,
                         int jc,
                         double x,
                         double y,
                         int nx,
                         int ny,
                         double xinc,
                         double yinc,
                         double xori,
                         double yori,
                         double *p_map_v,
                         int debug
                         );

void map_simple_stats (
		       double *p_map_in_v,
		       int nx,
		       int ny,
		       double *zmin,
		       double *zmax,
		       double *mean,
		       int   *ndef,
		       int   *sign,
		       int   debug
		       );


void map_tilt (
	       int   nx,
	       int   ny,
	       double dx,
	       double dy,
	       double xstart,
	       double ystart,
	       double *p_map_v,
	       double angle,
	       double azimuth,
	       int   ierror,
	       int   debug
	       );

int map_interp_holes(int mx, int my, double *p_zval_v, int option,
		     int debug);


void map_operation_map (
			int   nx,
			int   ny,
			double *p_map1_v,
			double *p_map2_v,
			int   iop,
			double factor,
			int   debug
			);


void map_reduce_byminmax (
			  int mode,
			  int nx,
			  int ny,
			  double xori,
			  double yori,
			  double xinc,
			  double yinc,
			  double *p_zval_v,
			  double newxmin,
			  double newxmax,
			  double newymin,
			  double newymax,
			  int *nnx,
			  int *nny,
			  int *nnxsh,
			  int *nnysh,
			  double *nxori,
			  double *nyori,
			  double *p_newzval_v,
			  int debug
			  );


void map_merge_map (
			  int   nx,
			  int   ny,
			  double xmin,
			  double ymin,
			  double xinc,
			  double yinc,
			  double *p_map1_v,
			  double *p_map2_v,
			  double *p_xp_v,
			  double *p_yp_v,
			  int   np,
			  int   pflag,
			  int   debug
			  );



void map_merge_red_map (
			int nx1,
			int ny1,
			double xori1,
			double yori1,
			double xinc1,
			double yinc1,
			double *p_zval1_v,
			int nx2,
			int ny2,
			double xori2,
			double yori2,
			double xinc2,
			double yinc2,
			double *p_zval2_v,
			int debug
			);

void map_wiener_from_grd3d (
			    double z,
			    int nx,
			    int ny,
			    int nz,
			    double *p_coord_v,
			    double *p_zcorn_v,
			    char  *ptype,
			    int   *p_int_v,
			    double *p_double_v,
			    int mx,
			    int my,
			    double xmin,
			    double xstep,
			    double ymin,
			    double ystep,
			    double *p_map_v,
			    int   debug
			    );

int surf_slice_cube(
                    int    ncx,
                    int    ncy,
                    int    ncz,
                    double cxori,
                    double cxinc,
                    double cyori,
                    double cyinc,
                    double czori,
                    double czinc,
                    double crotation,
                    int    yflip,
                    float  *swig_np_flt_in_v1,  // *p_cubeval_v
                    long   n_swig_np_flt_in_v1,
                    int    mx,
                    int    my,
                    double xori,
                    double xinc,
                    double yori,
                    double yinc,
                    int mapflip,
                    double mrotation,
                    double *swig_np_dbl_in_v1,   // *p_zslice_v
                    long   n_swig_np_dbl_in_v1,
                    double *swig_np_dbl_aout_v1, // *p_map_v to update argout
                    long   n_swig_np_dbl_aout_v1,
                    int    option1,
                    int    option2,
                    int    debug
                    );

int surf_slice_cube_window(
                           int ncx,
                           int ncy,
                           int ncz,
                           double cxori,
                           double cxinc,
                           double cyori,
                           double cyinc,
                           double czori,
                           double czinc,
                           double crotation,
                           int yflip,
                           float  *swig_np_flt_in_v1,  // *p_cubeval_v
                           long n_swig_np_flt_in_v1,   // ncube
                           int mx,
                           int my,
                           double xori,
                           double xinc,
                           double yori,
                           double yinc,
                           int mapflip,
                           double mrotation,
                           double *swig_np_dbl_in_v1,   // *p_map_v
                           long n_swig_np_dbl_in_v1,    // nmap
                           double zincr,
                           int nzincr,
                           double *swig_np_dbl_aout_v1, // *p_attrs_v: update
                           long n_swig_np_dbl_aout_v1,  //nattrsmap
                           int nattr,
                           int option1,
                           int option2,
                           int debug
                           );


void map_slice_cube (
		     int    ncx,
		     int    ncy,
		     int    ncz,
		     double cxmin,
		     double cxinc,
		     double cymin,
		     double cyinc,
		     double czmin,
		     double czinc,
		     float  *p_cubeval_v,
		     double crotation,
                     int    yflip,
                     int    mx,
		     int    my,
		     double xmin,
		     double xstep,
		     double ymin,
		     double ystep,
		     double *p_zslice_v,
		     double *p_map_v,
		     int    option,
		     int    debug
		     );


void map_slice_grd3d (
		      int   nx,
		      int   ny,
		      int   nz,
		      double *p_coord_v,
		      double *p_zcorn_v,
		      int   *p_actnum_v,
		      int   mx,
		      int   my,
		      double xmin,
		      double xstep,
		      double ymin,
		      double ystep,
		      double *p_zval_v,
		      int   *p_ib_v,
		      int   option,
		      int   debug
		      );

void map_sample_grd3d_prop (
			    int   nx,
			    int   ny,
			    int   nz,
			    int   type,
			    int   *p_iprop_v,
			    double *p_fprop_v,
			    int   mx,
			    int   my,
			    double *p_zval_v,
			    int   *p_ib_v,
			    int   debug
			    );

void map_sample_grd3d_lay (
			   int    nx,
			   int    ny,
			   int    nz,
			   double *p_coord_v,
			   double *p_zcorn_v,
			   int    *p_actnum_v,
			   int    klayer,
			   int    mx,
			   int    my,
			   double xori,
			   double xstep,
			   double yori,
			   double ystep,
			   double *p_zval_v,
			   int    option,
			   int    debug
			   );

void surf_sample_grd3d_lay (
			   int    nx,
			   int    ny,
			   int    nz,
			   double *p_coord_v,
			   double *p_zcorn_v,
			   int    *p_actnum_v,
			   int    klayer,
			   int    mx,
			   int    my,
			   double xori,
			   double xstep,
			   double yori,
			   double ystep,
			   double rotation,
			   double *p_zval_v,
			   int    option,
			   int    debug
			   );

/*
 *=============================================================================
 * POLYGON/POINTS
 *=============================================================================
 */

void pox_import_rms (
		     double *p_xp_v,
		     double *p_yp_v,
		     double *p_zp_v,
		     char   *file,
		     int    debug
		     );


int pox_import_irap (
		     double *p_xp_v,
		     double *p_yp_v,
		     double *p_zp_v,
		     int    npoints,
		     char   *file,
		     int    debug
		     );



void pox_export_irap_ascii (
			    double *p_xp_v,
			    double *p_yp_v,
			    double *p_zp_v,
			    char   *file,
			    int    npoints,
			    int    iflag,
			    int    debug
			    );



double pox_zsum (
		 double *p_z1_v,
		 int    np1,
		 int    debug
		 );


int pox_copy_pox (
		  int   np,
		  double *p_x1_v,
		  double *p_y1_v,
		  double *p_z1_v,
		  double *p_x2_v,
		  double *p_y2_v,
		  double *p_z2_v,
		  int   debug
		  );

int pox_operation_scalar (
			  double *p_x1_v,
			  double *p_y1_v,
			  double *p_z1_v,
			  int    np1,
			  double value,
			  int    iop,
			  int    debug
			  );

int pox_operation_pox (
		       double *p_x1_v,
		       double *p_y1_v,
		       double *p_z1_v,
		       int   np1,
		       double *p_x2_v,
		       double *p_y2_v,
		       double *p_z2_v,
		       int   np2,
		       int   iop,
		       double tol,
		       int   debug
		       );



void pol_import_irap (
		      int    i1,
		      int    i2,
		      double  *p_xp_v,
		      double  *p_yp_v,
		      double  *p_zp_v,
		      char   *file,
		      int    debug
		      );


int pol_chk_point_inside(
			 double x,
			 double y,
			 double *p_xp_v,
			 double *p_yp_v,
			 int    np,
			 int    debug
			 );

int polys_chk_point_inside(
			   double x,
			   double y,
			   double *p_xp_v,
			   double *p_yp_v,
			   int    np1,
			   int    np2,
			   int    debug
			   );

int pol_close(
	       int     np,
	       double *p_x_v,
	       double *p_y_v,
	       double *p_z_v,
	       double dist,
	       int    option,
	       int    debug
	      );

int pol_set_entry (
		   int    i,
		   double x,
		   double y,
		   double z,
		   int    npmax,
		   double *p_x_v,
		   double *p_y_v,
		   double *p_z_v,
		   int    option,
		   int    debug
		   );

int pol_geometrics(int nlen, double *xv, double *yv, double *zv,
                   double *hlenv,
                   int debug);

int pol_info(int nlen,
             double *xv,
             double *yv,
             double *zv,
             double *xmin,
             double *xmax,
             double *ymin,
             double *ymax,
             int *closed,
             int debug
             );

int pol_refine (
		int    np,
		int    npmax,
		double *p_x_v,
		double *p_y_v,
		double *p_z_v,
		double dist,
		int    option,
		int    debug
		);

int pol_extend (
		int    np,
		double *p_x_v,
		double *p_y_v,
		double *p_z_v,
		double dist,
		int    mode,
		double xang,
		int    option,  /* 0: look in 3D, 1: look in 2d XY */
		int    debug
		);

int pol_resample(int nlen, double *xv, double *yv, double *zv,
                 double smpl, int next, int nbuf, int *nolen,
                 double *xov, double *yov, double *zov, double *hlen,
                 int option, int debug);

/*
 *=============================================================================
 * CUBE (REGULAR 3D)
 *=============================================================================
 */
/* sucu_* is common for surf and cube: */
int sucu_ij_from_xy(
                    int   *i,
                    int   *j,
                    double *rx,
                    double *ry,
                    double x,
                    double y,
                    double xori,
                    double xinc,
                    double yori,
                    double yinc,
                    int    nx,
                    int    ny,
                    int yflip,
                    double rot_azi_deg,
                    int    flag,
                    int    debug
                    );


void cube_scan_segy_hdr (
			 char  *file,
			 int   *gn_bitsheader,
			 int   *gn_formatcode,
			 float *gf_segyformat,
			 int   *gn_samplespertrace,
			 int   *gn_measuresystem,
			 int   option,
                         char  *outfile,
			 int   debug
			 );

int cube_import_storm (
                       int nx,
                       int ny,
                       int nz,
                       char *file,
                       int lstart,
                       float *swig_np_flt_aout_v1,   // *p_cube_v
                       long n_swig_np_flt_aout_v1,    // nxyz
                       int option,
                       int debug
                       );

void cube_import_segy (
		       char     *file,
		       int      hbitoffset,
		       int      formatcode,
		       float    segyformat,
		       int      samplespertrace,
		       int      *nx,
		       int      *ny,
		       int      *nz,
		       float    *p_val_v,
		       double   *xori,
		       double   *xinc,
		       double   *yori,
		       double   *yinc,
		       double   *zori,
		       double   *zinc,
		       double   *rotation,
		       int      *yflip,
		       int      *zflip,
		       double   *minval,
		       double   *maxval,
		       int      optscan,
		       int      option,
                       char     *outfile,
		       int      debug
		       );

void cube_import_rmsregular (
                             int   iline,
                             int   *ndef,
                             int   *ndefsum,
                             int   nx,
                             int   ny,
                             int   nz,
                             float *val_v,
                             double *vmin,
                             double *vmax,
                             char  *file,
                             int   *ierr,
                             int   debug
                             );

int cube_export_segy (
                      char *sfile,
                      int nx,
                      int ny,
                      int nz,
                      float *swig_np_flt_in_v1,   // cube_v
                      long n_swig_np_flt_in_v1,   // n total nx*ny*nz
                      double xori,
                      double xinc,
                      double yori,
                      double yinc,
                      double zori,
                      double zinc,
                      double rotation,
                      int yflip,
                      int zflip,
                      int *ilinesp,
                      int *xlinesp,
                      int *tracidp,
                      int option,
                      int debug
                      );

int cube_export_rmsregular (
                            int   nx,
                            int   ny,
                            int   nz,
                            double xmin,
                            double ymin,
                            double zmin,
                            double xinc,
                            double yinc,
                            double zinc,
                            double rotation,
                            int yflip,
                            float *swig_np_flt_in_v1,   // *val_v
                            long n_swig_np_flt_in_v1,   // n total nx*ny*nz
                            char  *file,
                            int   debug
                            );
int cube_swapaxes (
                   int *nx,
                   int *ny,
                   int nz,
                   int *yflip,
                   double xori,
                   double *xinc,
                   double yori,
                   double *yinc,
                   double *rotation,
                   float *swig_np_flt_inplace_v1,   // *p_val_v
                   long n_swig_np_flt_inplace_v1,   // nval, total nx*ny*nz
                   int *swig_np_int_inplace_v1,     // *p_traceid_v
                   long n_swig_np_int_inplace_v1,   // nval for traceid, nx*ny
                   int option,
                   int debug
                   );

int cube_conv_from_grd3d(
			 int nx,
			 int ny,
			 int nz,
			 double *p_coord_v,
			 double *p_zcorn_v,
			 int   *p_actnum_v,
			 double *p_prop_v,
			 double *xori,
			 double *xinc,
			 double *yori,
			 double *yinc,
			 double *zori,
			 double *zinc,
			 double *rotation,
			 float  *p_cube_val,
			 double *cube_val_min,
			 double *cube_val_max,
			 int   option,
			 int   debug
			 );

int cube_coord_val_ijk(
                       int   i,
                       int   j,
                       int   k,
                       int   nx,
                       int   ny,
                       int   nz,
                       double xori,
                       double xinc,
                       double yori,
                       double yinc,
                       double zori,
                       double zinc,
                       double rot_deg,
                       int yflip,
                       float *p_val_v,
                       double *x,
                       double *y,
                       double *z,
                       float *value,
                       int option,
                       int   debug
                       );

int cube_xy_from_ij(
		    int    i,
		    int    j,
		    double *x,
		    double *y,
		    double xori,
		    double xinc,
		    double yori,
		    double yinc,
		    int    nx,
		    int    ny,
                    int yflip,
		    double rot_azi_deg,
		    int    flag,
		    int    debug
		    );

int cube_ijk_from_xyz(
		      int    *i,
		      int    *j,
		      int    *k,
		      double *rx,
		      double *ry,
		      double *rz,
		      double x,
		      double y,
		      double z,
		      double xori,
		      double xinc,
		      double yori,
		      double yinc,
		      double zori,
		      double zinc,
		      int    nx,
		      int    ny,
		      int    nz,
		      double rot_deg,
                      int    yflip,
                      int    flag,
		      int    debug
		      );

int cube_value_ijk(
                   int    i,
                   int    j,
                   int    k,
                   int    nx,
                   int    ny,
                   int    nz,
                   float *p_val_v,
                   float *value,
                   int    debug
                   );

int cube_value_xyz_cell(
                        double x,
                        double y,
                        double z,
                        double xori,
                        double xinc,
                        double yori,
                        double yinc,
                        double zori,
                        double zinc,
                        double rot_deg,
                        int   yflip,
                        int   nx,
                        int   ny,
                        int   nz,
                        float *p_val_v,
                        float *value,
                        int option,
                        int   debug
                        );

int cube_value_xyz_interp(
                          double x,
                          double y,
                          double z,
                          double xori,
                          double xinc,
                          double yori,
                          double yinc,
                          double zori,
                          double zinc,
                          double rot_deg,
                          int   yflip,
                          int   nx,
                          int   ny,
                          int   nz,
                          float *p_val_v,
                          float *value,
                          int option,
                          int   debug
                          );


int cube_vertical_val_list(
			   int    i,
			   int    j,
			   int    nx,
			   int    ny,
			   int    nz,
			   float *p_val_v,
			   float *p_vertical_v,
			   int    debug
			   );

int cube_resample_cube(
                       int ncx1,
                       int ncy1,
                       int ncz1,
                       double cxori1,
                       double cxinc1,
                       double cyori1,
                       double cyinc1,
                       double czori1,
                       double czinc1,
                       double crotation1,
                       int yflip1,
                       float *swig_np_flt_inplace_v1,  // *p_cubeval1_v,
                       long n_swig_np_flt_inplace_v1,  // ncube1,
                       int ncx2,
                       int ncy2,
                       int ncz2,
                       double cxori2,
                       double cxinc2,
                       double cyori2,
                       double cyinc2,
                       double czori2,
                       double czinc2,
                       double crotation2,
                       int yflip2,
                       float *swig_np_flt_in_v1,    // *p_cubeval2_v,
                       long n_swig_np_flt_in_v1,    // ncube2,
                       int option1,
                       int option2,
                       float ovalue,
                       int debug
                       );


/*
 *=============================================================================
 * GRID (3D) CORNERPOINTS
 *=============================================================================
 */

/* last generation import roff binary from here --> */
long grd3d_scan_roffbinary (FILE *fc,
                            int *swig_int_out_p1,    // *swap,
                            char *swig_bnd_char_1m,  // *tags,
                            int *rectypes,
                            long *reclengths,
                            long *recstarts,
                            long maxkw,
                            int debug);

void grd3d_conv_roxapi_grid (
                             int nx,
                             int ny,
                             int nz,
                             long nxyz,
                             int *cact,
                             double *crds,
                             double *p_coord_v,
                             double *p_zcorn_v,
                             int *p_actnum_v,
                             int debug);


int grd3d_imp_roffbin_arr (FILE *fc,
                           int swap,
                           int nx,
                           int ny,
                           int nz,
                           long bytepos,
                           int dtype,
                           float *swig_np_flt_inplace_v1,  // *farray
                           long n_swig_np_flt_inplace_v1,  // *nfarray
                           int *swig_np_int_inplace_v1,    // *iarray
                           long n_swig_np_int_inplace_v1,  // *niarray
                           int debug);

int grd3d_imp_roffbin_data (FILE *fc,
                            int swap,
                            int dtype,
                            long bytepos,
                            int *pidata,
                            float *pfdata,
                            int debug);

/* --> last generation import roff binary to here */

void grd3d_import_roff_grid (
			    int     *num_act,
			    int     *num_subgrds,
			    double  *p_coord_v,
			    double  *p_zcorn_v,
			    int     *p_actnum_v,
			    int     *p_subgrd_v,
			    int     nnsub,
			    char    *filename,
			    int     debug
			    );

int grd3d_imp_prop_roffbin (
			    char    *filename,
			    int     scanmode,
			    int     *p_type,
			    int     *p_nx,
			    int     *p_ny,
			    int     *p_nz,
			    int     *p_ncodes,
			    char    *prop_name,
			    int     *p_int_v,
			    double  *p_double_v,
			    char    *swig_bnd_char_10k,  // p_codenames_v,
			    int     *p_codevalues_v,
			    int     option,
			    int     debug
			    );

/* old version */
void grd3d_import_roff_prop (
			      int     nx,
			      int     ny,
			      int     nz,
			      char    *prop_name,
			      int     *p_int_v,
			      double  *p_dfloat_v,
			      char    **codenames,
			      int     *codevalues,
			      char    *filename,
			      int     debug
			     );

void grd3d_export_roff_grid (
			    int     mode,
			    int     nx,
			    int     ny,
			    int     nz,
			    int     num_subgrds,
			    int     isubgrd_to_export,
			    double   xoffset,
			    double   yoffset,
			    double   zoffset,
			    double  *p_coord_v,
			    double  *p_zcorn_v,
			    int     *p_actnum_v,
			    int     *p_subgrd_v,
			    char    *filename,
			    int     debug
			    );


void grd3d_export_roff_end (
			    int     mode,
			    char    *filename,
			    int     debug
			    );

void grd3d_scan_roff_bingrid (
			      int     *nx,
			      int     *ny,
			      int     *nz,
			      int     *nsubs,
			      char    *filename,
			      int     debug
			      );

int grd3d_scan_roff_binpar (
			    char    *parname,
			    char    *filename,
			    int     *ndcodes,
			    int     debug
			    );


void grd3d_export_roff_pstart (
			      int     mode,
			      int     nx,
			      int     ny,
			      int     nz,
			      char    *filename,
			      int     debug
			      );


void grd3d_export_roff_prop (
			      int     mode,
			      int     nx,
			      int     ny,
			      int     nz,
			      int     num_subgrds,
			      int     isubgrd_to_export,
			      int     *p_subgrd_v,
			      char    *pname,
			      char    *ptype,
			      int     *p_int_v,
			      double  *p_double_v,
			      int     ncodes,
			      char    *codenames,
			      int     *codevalues,
			      char    *filename,
			      int     debug
			      );

/* use in Perl version (codenames as double pointer) */
void grd3d_export_roff_prop_old (int mode,
                                 int     nx,
                                 int     ny,
                                 int     nz,
                                 int     num_subgrds,
                                 int     isubgrd_to_export,
                                 int     *p_subgrd_v,
                                 char    *pname,
                                 char    *ptype,
                                 int     *p_int_v,
                                 double  *p_double_v,
                                 int     ncodes,
                                 char    **codenames,
                                 int     *codevalues,
                                 char    *filename,
                                 int     debug
                                 );

long grd3d_scan_eclbinary (
                           FILE *fc,
                           char *swig_bnd_char_1m,  // *keywords,
                           int *rectype,
                           long *reclengths,
                           long *recstarts,
                           long maxkw,
                           int debug
                           );

int grd3d_read_eclrecord (
                          FILE   *fc,
                          long   recstart,
                          int    rectype,
                          int    *swig_np_int_inplace_v1,
                          long   n_swig_np_int_inplace_v1,
                          float  *swig_np_flt_inplace_v1,
                          long   n_swig_np_flt_inplace_v1,
                          double *swig_np_dbl_inplace_v1,
                          long   n_swig_np_dbl_inplace_v1,
                          int    debug
                          );

void grd3d_zcorn_convert (
                          int nx,
                          int ny,
                          int nz,
                          float *zcorn,
                          double *p_zcorn_v,
                          int option,
                          int debug
                          );

int grd3d_ecl_tsteps (
                      FILE *fc,
                      int *seqnums,
                      int *day,
                      int *mon,
                      int *year,
                      int nmax,
                      int debug
                      );

void grd3d_import_ecl_egrid (
			     int    mode,
			     int    nx,
			     int    ny,
			     int    nz,
			     int    *num_active,
			     double *coord_v,
			     double *p_zcorn_v,
			     int    *actnum_v,
			     char   *filename,
			     int    debug
			     );
/* new version */
int grd3d_imp_ecl_egrid (
                         FILE *fc,
                         int nx,
                         int ny,
                         int nz,
                         long bpos_mapaxes,
                         long bpos_coord,
                         long bpos_zcorn,
                         long bpos_actnum,
                         double *p_coord_v,
                         double *p_zcorn_v,
                         int *actnum_v,
                         int debug
                         );


void grd3d_scan_ecl_grid_hd (
			     int   ftype,
			     int   *nx,
			     int   *ny,
			     int   *nz,
			     char  *filename,
			     int   debug
			     );

void grd3d_scan_ecl_egrid_hd (
			      int   ftype,
			      int   *nx,
			      int   *ny,
			      int   *nz,
			      char  *filename,
			      int   debug
			      );

void grd3d_scan_ecl_init_hd (
			     int   ftype,
			     int   *nx,
			     int   *ny,
			     int   *nz,
			     char  *filename,
			     int   debug
			     );

void grd3d_import_ecl_grid (
			    int    ftype,
			    int    nxyz,
			    int    *num_active,
			    double *coord_v,
			    double *p_zcorn_v,
			    int    *actnum_v,
			    char   *filename,
			    int    debug
			    );


void grd3d_import_ecl_prop (
			    int    ftype,
			    int    nxyz,
			    int    *p_actnum_v,
			    int    nklist,
			    char   *klist,
			    int    ndates,
			    int    *day,
			    int    *month,
			    int    *year,
			    char   *filename,
			    double *dvector_v,
			    int    *nktype,
			    int    *norder,
			    int    *dsuccess,
			    int    debug
			    );

void grd3d_list_ecl_kw (
			int    ftype,
			char   *filename,
			int    debug
			);


void grd3d_strip_adouble (
			  int     nxyz,
			  int     counter,
			  double  *v1,
			  double  *v2,
			  int     debug
			  );

void grd3d_strip_afloat (
			 int     nxyz,
			 int     counter,
			 double  *v1,
			 float   *v2,
			 int     debug
			 );

void grd3d_strip_anint (
			 int     nxyz,
			 int     counter,
			 double  *v1,
			 int     *v2,
			 int     debug
			 );

/*
void grd3d_import_ecl_prop (
			     int    ftype,
			     int    ntstep,
			     int    nxyz,
			     int    *nactive,
			     char   *keywords,
			     char   *filename,
			     int    *ivector_v,
			     double  *fvector_v,
			     double *dvector_v,
			     char   **cvector_v,
			     int    *lvector_v,
			     int    debug
			     );
*/

void grd3d_export_ecl_grid (
			    int     nx,
			    int     ny,
			    int     nz,
			    double   *p_coord_v,
			    double   *p_zcorn_v,
			    int     *p_actnum_v,
			    char    *filename,
			    int     mode,
			    int     debug
			    );


void grd3d_export_ecl_pstart (
			      int     mode,
			      int     nx,
			      int     ny,
			      int     nz,
			      int     *p_actnum_v,
			      char    *filename,
			      int     debug
			      );

void grd3d_export_ecl_prop (
			     int     mode,
			     int     nx,
			     int     ny,
			     int     nz,
			     char    *cname,
			     char    *ctype,
			     int     *int_v,
			     double   *float_v,
			     double  *double_v,
			     char    **string_v,
			     int     *logi_v,
			     int     *p_actnum_v,
			     char    *filename,
			     int     debug
			     );

void grd3d_import_grdecl (
			  int     nx,
			  int     ny,
			  int     nz,
			  double  *p_coord_v,
			  double  *p_zcorn_v,
			  int     *p_actnum_v,
			  int     *nact,
			  char    *filename,
			  int     debug
			  );

void grd3d_export_grdecl (
			  int     nx,
			  int     ny,
			  int     nz,
			  double  *p_coord_v,
			  double  *p_zcorn_v,
			  int     *p_actnum_v,
			  char    *filename,
			  int     debug
			  );

void grd3d_export_grdeclprop (
			      int     nx,
			      int     ny,
			      int     nz,
			      int     formatspec,
			      char    *propname,
			      double   *p_fprop_v,
			      char    *filename,
			      int     filemode,
			      int     debug
			      );
void grd3d_import_grdeclpar (
                             int     nx,
			     int     ny,
			     int     nz,
			     char    *prop_name,
			     double   *p_float_v,
			     char    *filename,
			     int     debug
			     );
void grd3d_export_flgrs(
                        int nx,
			int ny,
			int nz,
			double *p_coord_v,
			double *p_zcorn_v,
			int   *p_actnum_v,
			double flimit,
			char  *file,
			int   mdiv,
			int   kthrough,
			int   append,
			int   additonal,
			int   debug
                        );

int grd3d_copy(
               int ncol,
               int nrow,
               int nlay,
               double *p_coord1_v,
               double *p_zcorn1_v,
               int *p_actnum1_v,
               double *p_coord2_v,
               double *p_zcorn2_v,
               int *p_actnum2_v,
               int iflag,
               int debug);


int grd3d_ijwindow_from_poly(
                             int np,
                             double *p_xp_v,
                             double *p_yp_v,
                             int nx,
                             int ny,
                             int nz,
                             int klayer,
                             double *p_coord_v,
                             double *p_zcorn_v,
                             int *p_actnum_v,
                             int *i1,
                             int *i2,
                             int *j1,
                             int *j2,
                             double extra,
                             int option,
                             int debug
                             );


void grd3d_add_zcells (
		       int    nx,
		       int    ny,
		       int    nz,
		       int    nadd,
		       double thickness,
		       double *p_zcorn1_v,
		       int    *p_actnum1_v,
		       double *p_zcornnew_v,
		       int    *p_actnumnew_v,
		       int    *p_numnew_act,
		       int    option,
		       int    debug
		       );



int grd3d_crop_geometry (
                         int nx,
                         int ny,
                         int nz,
                         double *p_coord1_v,
                         double *p_zcorn1_v,
                         int *p_actnum1_v,
                         double *p_coord2_v,
                         double *p_zcorn2_v,
                         int *p_actnum2_v,
                         int ic1, int ic2, int jc1, int jc2, int kc1, int kc2,
                         int *nactive,
                         int iflag,
                         int debug
                         );

int grd3d_reduce_by_zon (
			 int    zmin,
			 int    zmax,
			 int    nx,
			 int    ny,
			 int    nz1,
			 int    *nz2,
			 double *p_zcorn1_v,
			 double *p_zcorn2_v,
			 int    *p_actnum1_v,
			 int    *p_actnum2_v,
			 int    *p_zon1_v,
			 int    *p_zon2_v,
			 int    *nactive,
			 int    *ibmappermin,
			 int    *ibmappermax,
			 int    iflag,
			 int    debug
			 );

int grd3d_reduce_onelayer (
			   int    nx,
			   int    ny,
			   int    nz,
			   double *p_zcorn1_v,
			   double *p_zcorn2_v,
			   int    *p_actnum1_v,
			   int    *p_actnum2_v,
			   int    *nactive,
			   int    iflag,
			   int    debug
			   );


void grd3d_fault_marks(
		       int nx,
		       int ny,
		       int nz,
		       double *p_coord_v,
		       double *p_zcorn_v,
		       int   *p_actnum_v,
		       int   *p_fmark_v,
		       double flimit,
		       int   debug
		       );


int grd3d_pol_ftrace(
		     int    nx,
		     int    ny,
		     int    nz,
		     double  *p_coord_v,
		     double  *p_zcorn_v,
		     int    *p_actnum_v,
		     double  *p_fprop_v,
		     double  *p_fprop2_v,
		     int    klayer,
		     int    nelem,
		     int    *fid,
		     double  *sfr,
		     double  *dir,
		     int    *ext,
		     int    *pri,
		     double  *xw1,
		     double  *xw2,
		     double  *yw1,
		     double  *yw2,
		     int    *np,
		     double  *p_xp_v,
		     double  *p_yp_v,
		     int    maxradius,
		     int    ntracemax,
		     int    option,
		     int    debug
		     );


int grd3d_cell_faultthrows(
			   int   i,
			   int   j,
			   int   z,
			   int   nx,
			   int   ny,
			   int   nz,
			   double *p_coord_v,
			   double *p_zcorn_v,
			   int   *p_actnum_v,
			   double throw[],
			   int   option,
			   int   debug
			   );

int grd3d_count_prop_int(
			 int   nxyz,
			 int   *p_nprop_v,
			 int   *p_actnum_v,
			 int   nval,
			 int   debug
			 );

int grd3d_refine_vert(
                      int nx,
                      int ny,
                      int nz,
                      double *p_coord_v,
                      double *p_zcorn_v,
                      int *p_actnum_v,
                      int nzref,
                      double *p_zcornhyb_v,
                      int *p_actnumhyb_v,
                      int *p_num_act,
                      int *rfac,
                      int option,
                      int debug
                      );

void grd3d_convert_hybrid (
			   int   nx,
			   int   ny,
			   int   nz,
			   double *p_coord_v,
			   double *p_zcorn_v,
			   int   *p_actnum_v,
			   int   nzhyb,
			   double *p_zcornhyb_v,
			   int   *p_actnumhyb_v,
			   int   *p_num_act,
			   double toplevel,
			   double botlevel,
			   int   ndiv,
			   int   debug
			   );

void grd3d_convert_hybrid2(
			   int   nx,
			   int   ny,
			   int   nz,
			   double *p_coord_v,
			   double *p_zcorn_v,
			   int   *p_actnum_v,
			   int   nzhyb,
			   double *p_zcornhyb_v,
			   int   *p_actnumhyb_v,
			   int   *p_num_act,
			   double toplevel,
			   double botlevel,
			   int   ndiv,
			   double *p_region_v,
			   int   region,
			   int   debug
			   );


int grd3d_ijk_range_by_prop (
                             int    nx,
                             int    ny,
                             int    nz,
                             int    p1,
                             int    p2,
                             int    *p_iprop_v,
                             int    *p_actnum_v,
                             int    *imin,
                             int    *imax,
                             int    *jmin,
                             int    *jmax,
                             int    *kmin,
                             int    *kmax,
                             int    iflag,
                             int    debug
                             );

void grd3d_merge_grids (
			int   nx,
			int   ny,
			int   nz1,
			double *p_coord1_v,
			double *p_zcorn1_v,
			int   *p_actnum1_v,
			int   nz2,
			double *p_zcorn2_v,
			int   *p_actnum2_v,
			int   nznew,
			double *p_zcornnew_v,
			int   *p_actnumnew_v,
			int   *p_numnew_act,
			int   option,
			int   debug
			);


void grd3d_make_z_consistent (
			      int    nx,
			      int    ny,
			      int    nz,
			      double *p_zcorn_v,
			      int    *p_actnum_v,
			      double  zsep,
			      int    debug
			      );

void grd3d_flip_depth (
		       int   nx,
		       int   ny,
		       int   nz,
		       double *p_coord_v,
		       double *p_zcorn_v,
		       int   debug
		       );

int grd3d_translate (
		     int   nx,
		     int   ny,
		     int   nz,
		     int   xflip,
		     int   yflip,
		     int   zflip,
		     double xshift,
		     double yshift,
		     double zshift,
		     double *p_coord_v,
		     double *p_zcorn_v,
		     int   debug
		     );

void grd3d_set_dz_cell (
			int   i,
			int   j,
			int   k,
			int   nx,
			int   ny,
			int   nz,
			double *p_zcorn_v,
			int   *p_actnum_v,
			double zsep,
			int   debug
			);


int grd3d_point_in_cell(
			int ibstart,
			int  kzonly,
			double x,
			double y,
			double z,
			int nx,
			int ny,
			int nz,
			double *p_coor_v,
			double *p_zcorn_v,
			int   *p_actnum_v,
			int   maxrad,
			int   sflag,
			int   *nradsearch,
			int   option,
			int   debug
			);


void grd3d_collapse_inact (
			   int   nx,
			   int   ny,
			   int   nz,
			   double *p_zcorn_v,
			   int   *p_actnum_v,
			   int   debug
			   );


void grd3d_midpoint (
		     int     i,
		     int     j,
		     int     k,
		     int     nx,
		     int     ny,
		     int     nz,
		     double  *p_coord_v,
		     double  *p_zcorn_v,
		     double  *x,
		     double  *y,
		     double  *z,
		     int     debug
		     );

void grd3d_cellpoint (
		      int     i,
		      int     j,
		      int     k,
		      int     nx,
		      int     ny,
		      int     nz,
		      int     ftype,
		      double   *p_coord_v,
		      double   *p_zcorn_v,
		      double   *x,
		      double   *y,
		      double   *z,
		      int     debug
		      );



void grd3d_make_active(
		       int     i1,
		       int     i2,
		       int     j1,
		       int     j2,
		       int     k1,
		       int     k2,
		       int     nx,
		       int     ny,
		       int     nz,
		       int     *p_actnum_v,
		       int     debug
		       );

// python version:
int grd3d_inact_outside_pol(
                            double *swig_np_dbl_in_v1,    // polygons X
                            long   n_swig_np_dbl_in_v1,   // N
                            double *swig_np_dbl_in_v2,    // polygons Y
                            long   n_swig_np_dbl_in_v2,   // N
                            int    nx,
                            int    ny,
                            int    nz,
                            double *p_coord_v,
                            double *p_zcorn_v,
                            int    *p_actnum_v,  // *actnum, just use C ref
                            int    k1,
                            int    k2,
                            int    force_close,
                            int    option,
                            int    debug
                            );

// perl version:
void grd3d_inact_outs_pol(
			  int     np,
			  double   *p_xp_v,
			  double   *p_yp_v,
			  int     nx,
			  int     ny,
			  int     nz,
			  double   *p_coord_v,
			  double   *p_zcorn_v,
			  int     *p_actnum_v,
			  int     *p_subgrd_v,
			  int     isub,
			  int     nsub,
			  int     option,
			  int     debug
			  );

int grd3d_geometrics(
		     int     nx,
		     int     ny,
		     int     nz,
		     double   *p_coord_v,
		     double   *p_zcorn_v,
		     int     *p_actnum_v,
		     double   *xori,
		     double   *yori,
		     double   *zori,
		     double   *xmin,
		     double   *xmax,
		     double   *ymin,
		     double   *ymax,
		     double   *zmin,
		     double   *zmax,
		     double   *rotation,
		     double   *dx,
		     double   *dy,
		     double   *dz,
		     int     option1,
		     int     option2,
		     int     debug
		     );

void grd3d_set_prop_by_pol(
			   int    np,
			   double  *p_xp_v,
			   double  *p_yp_v,
			   int    nx,
			   int    ny,
			   int    nz,
			   double  *p_coord_v,
			   double  *p_zcorn_v,
			   int    *p_actnum_v,
			   double  *p_prop_v,
			   double  value,
			   double  ronly,
			   int    i1,
			   int    i2,
			   int    j1,
			   int    j2,
			   int    k1,
			   int    k2,
			   int    option,
			   int    debug
			   );

void grd3d_set_prop_in_pol(
			   int    np,
			   double  *p_xp_v,
			   double  *p_yp_v,
			   int    nx,
			   int    ny,
			   int    nz,
			   double  *p_coord_v,
			   double  *p_zcorn_v,
			   int    *p_actnum_v,
			   int    *p_prop_v,
			   double  value,
			   int    i1,
			   int    i2,
			   int    j1,
			   int    j2,
			   int    k1,
			   int    k2,
			   int    option,
			   int    debug
			   );

void grd3d_split_prop_by_pol(
			     int    np,
			     double  *p_xp_v,
			     double  *p_yp_v,
			     int    nx,
			     int    ny,
			     int    nz,
			     double  *p_coord_v,
			     double  *p_zcorn_v,
			     int    *p_actnum_v,
			     int    *p_prop_v,
			     double  *p_propfilter_v,
			     double  filterval,
			     int    i1,
			     int    i2,
			     int    j1,
			     int    j2,
			     int    k1,
			     int    k2,
			     int    option,
			     int    debug
			     );

void grd3d_inact_by_dz(
		       int nx,
		       int ny,
		       int nz,
		       double *p_zcorn_v,
		       int   *p_actnum_v,
		       double threshold,
		       int   flip,
		       int   debug
		       );

void grd3d_copy_prop_int(
			 long   nxyz,
			 int   *p_input_v,
			 int   *p_output_v,
			 int   debug
			 );

void grd3d_copy_prop_float(
			   long   nxyz,
			   float *p_input_v,
			   float *p_output_v,
			   int   debug
			   );

void grd3d_copy_prop_doflo(
			   long   nxyz,
			   double *p_input_v,
			   float *p_output_v,
			   int   debug
			   );

void grd3d_copy_prop_double(
			   long   nxyz,
			   double *p_input_v,
			   double *p_output_v,
			   int   debug
			   );

void grd3d_transfer_prop_int(
			     int   nx,
			     int   ny,
			     int   nz1,
			     int   nz2,
			     int   k1min,
			     int   k1max,
			     int   k2min,
			     int   k2max,
			     int   *p_input_v,
			     int   *p_output_v,
			     int   option,
			     int   debug
			     );

void grd3d_transfer_prop_dbl(
			     int    nx,
			     int    ny,
			     int    nz1,
			     int    nz2,
			     int    k1min,
			     int    k1max,
			     int    k2min,
			     int    k2max,
			     double *p_input_v,
			     double *p_output_v,
			     int    option,
			     int    debug
			     );


void grd3d_ifthen_prop_intint(
			      int   nx,
			      int   ny,
			      int   nz,
			      int   i1,
			      int   i2,
			      int   j1,
			      int   j2,
			      int   k1,
			      int   k2,
			      int   *p_other_v,
			      int   *p_this_v,
			      int   *p_array,
			      int   alen,
			      int   newvalue,
			      int   elsevalue,
			      int   rmin,
			      int   rmax,
			      int   debug
			      );

double grd3d_frac_prop_within_ii(
			       int   nxyz,
			       int   *p_other_v,
			       int   *p_this_v,
			       int   *p_oarray,
			       int   oalen,
			       int   *p_tarray,
			       int   talen,
			       int   debug
				);

void grd3d_remap_prop_g2g(
			   int   nx1,
			   int   ny1,
			   int   nz1,
			   int   nx2,
			   int   ny2,
			   int   nz2,
			   int   isub,
			   int   num_subgrds,
			   int   *p_subgrd_v,
			   char  *ptype,
			   int   *p_int1_v,
			   double *p_double1_v,
			   int   *p_int2_v,
			   double *p_double2_v,
			   int   debug
			   );

int grd3d_count_active(
		       int     nx,
		       int     ny,
		       int     nz,
		       int    *p_actnum_v,
		       int debug
		       );


void grd3d_calc_dz(
		   int     nx,
		   int     ny,
		   int     nz,
		   double  *p_zcorn_v,
		   int     *p_actnum_v,
		   double   *p_dz_v,
		   int     flip,
		   int     option,
		   int     debug
		   );

int grd3d_calc_dxdy(
                    int      nx,
                    int      ny,
                    int      nz,
                    double   *p_coord_v,
                    double   *p_zcorn_v,
                    int      *p_actnum_v,
                    double   *dx,
                    double   *dy,
                    int      option1,
                    int      option2,
                    int      debug
                    );

void grd3d_calc_xyz(
		    int      nx,
		    int      ny,
		    int      nz,
		    double   *p_coord_v,
		    double   *p_zcorn_v,
		    int      *p_actnum_v,
		    double   *p_x_v,
		    double   *p_y_v,
		    double   *p_z_v,
		    int      option,
		    int      debug
		    );


void grd3d_calc_z(
		  int nx,
		  int ny,
		  int nz,
		  double *p_zcorn_v,
		  double *p_z_v,
		  int debug
		  );

int grd3d_prop_infill1_int(
			   int nx,
			   int ny,
			   int nz,
			   int i1,
			   int i2,
			   int j1,
			   int j2,
			   int k1,
			   int k2,
			   double *p_coord_v,
			   double *p_zcorn_v,
			   int   *p_actnum_v,
			   int   *p_xxx_v,
			   int   value,
			   int   debug
			   );

void grd3d_calc_cell_dip(
			 int nx,
			 int ny,
			 int nz,
			 double *p_coord_v,
			 double *p_zcorn_v,
			 double *p_dip_v,
			 int   debug
			 );

void grd3d_interp_prop_verti(
			     int    nx,
			     int    ny,
			     int    nz,
			     double *p_zcorn_v,
			     int    *p_actnum_v,
			     double  *p_xxx_v,
			     double  bgval,
			     double  dzmax,
			     int    option,
			     int    debug
			     );

void grd3d_interp_prop_vertii(
			      int    nx,
			      int    ny,
			      int    nz,
			      int    i1,
			      int    i2,
			      int    j1,
			      int    j2,
			      double *p_zcorn_v,
			      int    *p_actnum_v,
			      int    *p_xxx_v,
			      int    bgval,
			      double  dzmax,
			      int    option,
			      int    debug
			      );

int grd3d_getcell_by_prop(
			  int      nx,
			  int      ny,
			  int      nz,
			  int      i1,
			  int      i2,
			  int      j1,
			  int      j2,
			  int      k1,
			  int      k2,
			  double   *p_coord_v,
			  double   *p_zcorn_v,
			  int      *p_actnum_v,
			  double    *p_xxx_v,
			  double    pmin,
			  double    pmax,
			  int      criteria,
			  int      option,
			  int      debug
			  );


void grd3d_calc_sum_dz(
		       int    nx,
		       int    ny,
		       int    nz,
		       double *p_zcorn_v,
		       int    *p_actnum_v,
		       double  *p_sumdz_v,
		       int    flip,
		       int    debug
		       );


void grd3d_calc_abase(
		      int option,
		      int nx,
		      int ny,
		      int nz,
		      double *p_dz_v,
		      double *p_abase_v,
		      int   flip,
		      int   debug
		      );


void grd3d_print_cellinfo (
			   int   i1,
			   int   i2,
			   int   j1,
			   int   j2,
			   int   k1,
			   int   k2,
			   int   nx,
			   int   ny,
			   int   nz,
			   double *p_coord_v,
			   double *p_zcorn_v,
			   int   *actnum_v,
			   int   debug
			   );

void grd3d_corners (
		    int     i,
		    int     j,
		    int     k,
		    int     nx,
		    int     ny,
		    int     nz,
		    double  *p_coord_v,
		    double  *p_zcorn_v,
		    double  corners[],
		    int     debug
		    );

double grd3d_zminmax(
                     int i,
                     int j,
                     int k,
                     int nx,
                     int ny,
                     int nz,
                     double *p_zcorn_v,
                     int option,
                     int debug
                     );

void grd3d_get_all_corners(
                           int nx,
                           int ny,
                           int nz,
                           double *p_coord_v,
                           double *p_zcorn_v,
                           int *p_actnum_v,
                           double* x1,
                           double* y1,
                           double* z1,
                           double* x2,
                           double* y2,
                           double* z2,
                           double* x3,
                           double* y3,
                           double* z3,
                           double* x4,
                           double* y4,
                           double* z4,
                           double* x5,
                           double* y5,
                           double* z5,
                           double* x6,
                           double* y6,
                           double* z6,
                           double* x7,
                           double* y7,
                           double* z7,
                           double* x8,
                           double* y8,
                           double* z8,
                           int option,
                           int debug
                           );

void grd3d_set_propval_int(
			   int     nxyz,
			   int     *p_prop_v,
			   int     value,
			   int     debug
			   );

void grd3d_set_propval_float(
			     int     nxyz,
			     float   *p_prop_v,
			     float   value,
			     int     debug
			     );

void grd3d_set_propval_double(
			     int     nxyz,
			     double   *p_prop_v,
			     double   value,
			     int     debug
			     );


void grd3d_pzcorns (
		    int     i,
		    int     j,
		    int     k,
		    int     nx,
		    int     ny,
		    int     nz,
		    double   *p_coord_v,
		    double   *p_zcorn_v,
		    double   *p,
		    int     *flag,
		    int     debug
		    );


int grd3d_rpt_zlog_vs_zon (
			   int   nx,
			   int   ny,
			   int   nz,
			   double *p_coord_v,
			   double *p_zcorn_v,
			   int   *p_actnum_v,
			   int   *p_zon_v,
			   int   nval,
			   double *p_utme_v,
			   double *p_utmn_v,
			   double *p_tvds_v,
			   int   *p_zlog_v,
			   int   zlmin,
			   int   zlmax,
			   double *p_zcorn_onelay_v,
			   int    *p_actnum_onelay_v,
			   double *results,
			   int   iflag,
			   int   debug
			   );

void grd3d_adj_z_from_map (
			   int nx,
			   int ny,
			   int nz,
			   double *p_coord_v,
			   double *p_zcorn_v,
			   int *p_actnum_v,
			   int mx,
			   int my,
			   double xmin,
			   double xstep,
			   double ymin,
			   double ystep,
			   double *p_grd2d_v,
			   double zconst,
			   int   iflag,
			   int   flip,
			   int   debug
			   );

void grd3d_adj_z_from_mapv2 (
			     int nx,
			     int ny,
			     int nz,
			     double *p_coord_v,
			     double *p_zcorn_v,
			     int *p_actnum_v,
			     int mx,
			     int my,
			     double xmin,
			     double xstep,
			     double ymin,
			     double ystep,
			     double *p_map_v,
			     int   iflag,
			     int   flip,
			     int   debug
			     );
void grd3d_adj_z_from_mapv3 (
			     int nx,
			     int ny,
			     int nz,
			     double *p_coord_v,
			     double *p_zcorn_v,
			     int *p_actnum_v,
			     int mx,
			     int my,
			     double xmin,
			     double xstep,
			     double ymin,
			     double ystep,
			     double *p_map_v,
			     int   iflag,
			     int   flip,
			     int   debug
			     );


void grd3d_diff_zcorn (
		       int nx,
		       int ny,
		       int nz,
		       double *p_coord1_v,
		       double *p_zcorn1_v,
		       int   *p_actnum1_v,
		       double *p_dz1_v,
		       double *p_dz2_v,
		       double *p_dz3_v,
		       double *p_dz4_v,
		       double *p_dz5_v,
		       double *p_dz6_v,
		       double *p_dz7_v,
		       double *p_dz8_v,
		       double *p_dzavg_v,
		       double *p_coord2_v,
		       double *p_zcorn2_v,
		       int   *p_actnum2_v,
		       int   debug
		       );


void grd3d_adj_dzcorn (
		       int nx,
		       int ny,
		       int nz,
		       double *p_zcorn_v,
		       double *p_dz1_v,
		       double *p_dz2_v,
		       double *p_dz3_v,
		       double *p_dz4_v,
		       double *p_dz5_v,
		       double *p_dz6_v,
		       double *p_dz7_v,
		       double *p_dz8_v,
		       int   debug
		       );


int grd3d_set_active(
		     int   nx,
		     int   ny,
		     int   nz,
		     int   i1,
		     int   i2,
		     int   j1,
		     int   j2,
		     int   k1,
		     int   k2,
		     int   *p_actnum_v,
		     int   debug
		     );

void grd3d_zdominance_int(
			  int   nx,
			  int   ny,
			  int   nz,
			  double *p_zcorn_v,
			  int   *p_actnum_v,
			  int   *p_xxx_v,
			  int   option,
			  int   debug
			  );


int well_geometrics (
                     int np,
                     double *xv,
                     double *yv,
                     double *zv,
                     double *md,
                     double *incl,
                     int option,
                     int debug
                     );