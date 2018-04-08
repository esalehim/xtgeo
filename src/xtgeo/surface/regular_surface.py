# -*- coding: utf-8 -*-
"""
Module/class for regular surfaces with XTGeo. Regular surfaces have a
constant distance in between nodes (xinc, yinc), and this simplifies
computations a lot. A regular surface is defined by an origin (xori, yori)
in UTM, a number of columns (along X axis, if no rotation), a number of
rows (along Y axis if no rotation), and increment (distance between nodes).
The map itself is an array of values.
Rotation is allowed and is measured in degrees, anticlock from X axis.

Note that an instance of a regular surface can be made directly with::

 import xtgeo
 mysurf = xtgeo.surface_from_file('some_name.gri')

or::

 mysurf = xtgeo.surface_from_roxar('some_rms_project', 'TopX', 'DepthSurface')

"""

from __future__ import print_function, absolute_import

import os
import os.path
import sys
import math
from types import FunctionType
import warnings

import numpy as np
import numpy.ma as ma

from xtgeo.common import XTGeoDialog
from xtgeo.plot import Map
from xtgeo.xyz import Points
from xtgeo.common.constants import UNDEF, UNDEF_LIMIT
from xtgeo.common.constants import VERYLARGENEGATIVE, VERYLARGEPOSITIVE

from xtgeo.surface import _regsurf_import
from xtgeo.surface import _regsurf_export
from xtgeo.surface import _regsurf_cube
from xtgeo.surface import _regsurf_roxapi
from xtgeo.surface import _regsurf_gridding
from xtgeo.surface import _regsurf_oper

xtg = XTGeoDialog()
logger = xtg.functionlogger(__name__)


# =============================================================================
# METHODS as wrappers to class init + import


def surface_from_file(mfile, fformat='guess'):
    """Make an instance of a RegularSurface directly from file import.

    Args:
        mfile (str): Name of file
        fformat (str): See :meth:`RegularSurface.from_file`

    Example::

        import xtgeo
        mysurf = xtgeo.surface_from_file('some_name.gri')
    """

    obj = RegularSurface()

    obj.from_file(mfile, fformat=fformat)

    return obj


def surface_from_roxar(project, name, category, stype='horizons',
                       realisation=0):
    """This makes an instance of a RegularSurface directly from roxar input.

    For arguments, see :meth:`RegularSurface.from_roxar`.

    Example::

        # inside RMS:
        import xtgeo
        mysurf = xtgeo.surface_from_roxar(project, 'TopEtive', 'DepthSurface')

    """

    obj = RegularSurface()

    obj.from_roxar(project, name, category, stype=stype,
                   realisation=realisation)

    return obj


# =============================================================================
# RegularSurface class:

class RegularSurface(object):
    """Class for a regular surface in the XTGeo framework.

    The regular surface instance is usually initiated by
    import from file, but can also be made from scratch.
    The values can as default be accessed by the user as a 2D masked numpy
    (ncol, nrow) float64 array, but also other representations or views are
    possible (e.g. as 1D ordinary numpy).

    For construction:

    Args:
        ncol: Integer for number of X direction columns
        nrow: Integer for number of Y direction rows
        xori: X (East) origon coordinate
        yori: Y (North) origin coordinate
        xinc: X increment
        yinc: Y increment
        rotation: rotation in degrees, anticlock from X axis between 0, 360
        values: 2D (masked) numpy array of shape (ncol, nrow), C order

    Examples:

        The instance can be made either from file or by a spesification::

            >>> x1 = RegularSurface('somefilename')  # assume Irap binary
            >>> x2 = RegularSurface('somefilename', fformat='irap_ascii')
            >>> x3 = RegularSurface().from_file('somefilename',
                                                 fformat='irap_binary')
            >>> x4 = RegularSurface()
            >>> x4.from_file('somefilename', fformat='irap_binary')
            >>> x5 = RegularSurface(ncol=20, nrow=10, xori=2000.0, yori=2000.0,
                                    rotation=0.0, xinc=25.0, yinc=25.0,
                                    values=np.zeros((20,10)))

        Initiate a class and import::

          from xtgeo.surface import RegularSurface
          x = RegularSurface()
          x.from_file('some.irap', fformat='irap_binary')

    """

    def __init__(self, *args, **kwargs):
        """Initiate a RegularSurface instance."""

        clsname = '{}.{}'.format(type(self).__module__, type(self).__name__)
        logger.info(clsname)

        self._undef = UNDEF
        self._undef_limit = UNDEF_LIMIT
        self._masked = False
        self._filesrc = None     # Name of original input file

        # assume so far:
        self._yflip = 1

        if args:
            # make instance from file import
            mfile = args[0]
            fformat = kwargs.get('fformat', 'guess')
            self.from_file(mfile, fformat=fformat)

        else:
            # make instance by kw spesification
            self._xori = kwargs.get('xori', 0.0)
            self._yori = kwargs.get('yori', 0.0)
            if 'nx' in kwargs:
                self._ncol = kwargs.get('nx', 5)  # backward compatibility
                self._nrow = kwargs.get('ny', 3)  # backward compatibility
            else:
                self._ncol = kwargs.get('ncol', 5)
                self._nrow = kwargs.get('nrow', 3)
            self._xinc = kwargs.get('xinc', 25.0)
            self._yinc = kwargs.get('yinc', 25.0)
            self._rotation = kwargs.get('rotation', 0.0)

            values = kwargs.get('values', None)

            if values is None:
                values = np.array([[1, 6, 11],
                                   [2, 7, 12],
                                   [3, 8, 1e33],
                                   [4, 9, 14],
                                   [5, 10, 15]],
                                  dtype=np.float64, order='C')
                # make it masked
                values = ma.masked_greater(values, UNDEF_LIMIT)
                self._masked = True
                self._values = values
            else:
                self._values = self.ensure_correct_values(self.ncol,
                                                          self.nrow, values)

        # _nsurfaces += 1

        if self._values is not None:
            logger.debug('Shape of value: and values')
            logger.debug('\n{}'.format(self._values.shape))
            logger.debug('\n{}'.format(repr(self._values)))

        logger.debug('Ran __init__ method for RegularSurface object')

    def __repr__(self):
        avg = self.values.mean()
        dsc = ('{0.__class__} (ncol={0.ncol!r}, '
               'nrow={0.nrow!r}, original file: {0._filesrc}), '
               'average {1} ID=<{2}>'.format(self, avg, id(self)))
        return dsc

    def __str__(self):
        avg = self.values.mean()
        dsc = ('{0.__class__.__name__} (ncol={0.ncol!r}, '
               'nrow={0.nrow!r}, original file: {0._filesrc}), '
               'average {1:.4f}'.format(self, avg))
        return dsc

    # =========================================================================
    # Class and static methods
    # =========================================================================

    @classmethod
    def methods(cls):
        """Returns the names of the methods in the class.

        >>> print(RegularSurface.methods())
        """
        return [x for x, y in cls.__dict__.items() if type(y) == FunctionType]

    @staticmethod
    def ensure_correct_values(ncol, nrow, values):
        """Ensures that values is a 2D masked numpy (ncol, nrol), C order.

        Args:
            ncol (int): Number of columns.
            nrow (int): Number of rows.
            values (array or scalar): Values to process.

        Return:
            values (MaskedArray): Array on correct format.

        Example::

            vals = np.ones((nx*ny))  # a 1D numpy array in C order by default

            # secure that the values are masked, in correct format and shape:
            mymap.values = mymap.ensure_correct_values(nc, nr, vals)
        """
        if np.isscalar(values):
            vals = ma.zeros((ncol, nrow), order='C', dtype=np.float64)
            values = vals + float(values)

        if not isinstance(values, ma.MaskedArray):
            values = ma.array(values, order='C')

        if not values.shape == (ncol, nrow):
            try:
                values = ma.reshape(values, (ncol, nrow), order='C')
            except ValueError as emsg:
                xtg.error('Cannot reshape array: {}'.format(emsg))
                raise

        # replace any undef or nan with mask
        values = ma.masked_greater(values, UNDEF_LIMIT)
        values = ma.masked_invalid(values)

        if not values.flags.c_contiguous:
            mask = ma.getmaskarray(values)
            mask = np.asanyarray(mask, order='C')
            values = np.asanyarray(values, order='C')
            values = ma.array(values, mask=mask, order='C')

        return values

    # =========================================================================
    # Properties
    # =========================================================================

    @property
    def ncol(self):
        """The NCOL (NX or N-Idir) number, as property (read only)."""
        return self._ncol

    @property
    def nrow(self):
        """The NROW (NY or N-Jdir) number, as property (read only)."""
        return self._nrow

    @property
    def nx(self):
        """The NX (or N-Idir) number, as property (deprecated, use ncol)."""
        logger.warning('Deprecated; use ncol instead')
        return self._ncol

    @property
    def ny(self):
        """The NY (or N-Jdir) number, as property (deprecated, use nrow)."""
        logger.warning('Deprecated; use nrow instead')
        return self._nrow

    @property
    def rotation(self):
        """The rotation, anticlock from X axis, in degrees [0..360>."""
        return self._rotation

    @rotation.setter
    def rotation(self, rota):
        if rota >= 0 and rota < 360:
            self._rotation = rota
        else:
            raise ValueError('Rotation must be in interval [0, 360>')

    @property
    def xinc(self):
        """The X increment (or I dir increment)."""
        return self._xinc

    @property
    def yinc(self):
        """The Y increment (or I dir increment)."""
        return self._yinc

    @property
    def yflip(self):
        """The Y flip (handedness) indicator 1, or -1 (read only).

        The value 1 (default) means a left-handed system if depth values are
        positive downwards. Not sure if -1 ever happens.
        """
        return self._yflip

    @property
    def xori(self):
        """The X coordinate origin of the map."""
        return self._xori

    @xori.setter
    def xori(self, xnew):
        self._xori = xnew

    @property
    def yori(self):
        """The Y coordinate origin of the map."""
        return self._yori

    @yori.setter
    def yori(self, ynew):
        self._yori = ynew

    @property
    def xmin(self):
        """The minimim X coordinate (read only)."""
        corners = self.get_map_xycorners()

        xmin = VERYLARGEPOSITIVE
        for c in corners:
            if c[0] < xmin:
                xmin = c[0]
        return xmin

    @property
    def xmax(self):
        """The maximum X coordinate (read only)."""
        corners = self.get_map_xycorners()

        xmax = VERYLARGENEGATIVE
        for c in corners:
            if c[0] > xmax:
                xmax = c[0]
        return xmax

    @property
    def ymin(self):
        """The minimim Y coordinate (read only)."""
        corners = self.get_map_xycorners()

        ymin = VERYLARGEPOSITIVE
        for c in corners:
            if c[1] < ymin:
                ymin = c[1]
        return ymin

    @property
    def ymax(self):
        """The maximum Y xoordinate (read only)."""
        corners = self.get_map_xycorners()

        ymax = VERYLARGENEGATIVE
        for c in corners:
            if c[1] > ymax:
                ymax = c[1]
        return ymax

    @property
    def values(self):
        """The map values, as 2D masked numpy (float64), shape (ncol, nrow)."""

        if not self._values.flags.c_contiguous:
            logger.warning('Not C order in numpy')

        return self._values

    @values.setter
    def values(self, values):
        logger.debug('Enter method...')

        values = self.ensure_correct_values(self.ncol, self.nrow, values)

        self._values = values

        logger.debug('Values shape: {}'.format(self._values.shape))
        logger.debug('Flags: {}'.format(self._values.flags))

    @property
    def values1d(self):
        """(Read only) Map values, as 1D numpy masked, normally a numpy
        view(?).

        Example::

            map = RegularSurface('myfile.gri')
            values = map.values1d
        """
        return self.get_values1d(asmasked=True)

    @property
    def npvalues1d(self):
        """(Read only) Map values, as 1D numpy (not masked), undef as np.nan.

        In most cases this will be a copy of the values.

        Example::

            map = RegularSurface('myfile.gri')
            values = map.npvalues1d
            mean = np.nanmean(values)
            values[values <= 0] = np.nan
        """
        return self.get_values1d(asmasked=False, fill_value=np.nan)

    @property
    def undef(self):
        """Returns or set the undef value, to be used e.g. when in the
        get_zval method."""
        return self._undef

    @undef.setter
    def undef(self, undef):
        self._undef = undef

    @property
    def undef_limit(self):
        """Returns or set the undef_limit value, to be used when in the
        get_zval method."""
        return self._undef_limit

    @undef_limit.setter
    def undef_limit(self, undef_limit):
        self._undef_limit = undef_limit

# =============================================================================
# Import and export
# =============================================================================

    def from_file(self, mfile, fformat='guess'):
        """Import surface (regular map) from file.

        Note that the 'guess' option will look at the file extesions, where
        "gri" will irap_binary and "fgr" assume Irap Ascii

        Args:
            mfile (str): Name of file
            fformat (str): File format, guess/irap_binary/irap_ascii
                is currently supported.


        Returns:
            Object instance, optionally.

        Example:
            Here the from_file method is used to initiate the object
            directly::

            >>> mymapobject = RegularSurface().from_file('myfile.x')


        """

        self._values = None

        if os.path.isfile(mfile):
            pass
        else:
            logger.critical('Not OK file')
            raise os.error

        froot, fext = os.path.splitext(mfile)
        if fformat is None or fformat == 'guess':
            if len(fext) == 0:
                msg = ('Stop: fformat is "guess" but file '
                       'extension is missing for {}'.format(froot))
                raise ValueError(msg)
            else:
                fformat = fext.lower().replace('.', '')

        if fformat in ['irap_binary', 'gri', 'bin', 'irapbin']:
            _regsurf_import.import_irap_binary(self, mfile)
        elif fformat in ['irap_ascii', 'fgr', 'asc', 'irapasc']:
            _regsurf_import.import_irap_ascii(self, mfile)
        else:
            raise ValueError('Invalid file format: {}'.format(fformat))

        return self

    def to_file(self, mfile, fformat='irap_binary'):
        """Export a surface (map) to file.

        Note, for zmap_ascii and storm_binary an unrotation will be done
        automatically. The sampling will be somewhat finer than the
        original map in order to prevent aliasing. See :func:`unrotate`.

        Args:
            mfile (str): Name of file
            fformat (str): File format, irap_binary/irap_ascii/zmap_ascii/
                storm_binary. Default is irap_binary.

        Example::

            >>> x = RegularSurface()
            >>> x.from_file('myfile.x', fformat = 'irap_ascii')
            >>> x.values = x.values + 300
            >>> x.to_file('myfile2.x', fformat = 'irap_ascii')

        """

        logger.debug('Enter method...')
        logger.info('Export to file...')
        if fformat == 'irap_ascii':
            _regsurf_export.export_irap_ascii(self, mfile)
        elif fformat == 'irap_binary':
            _regsurf_export.export_irap_binary(self, mfile)
        elif fformat == 'zmap_ascii':
            _regsurf_export.export_zmap_ascii(self, mfile)
        elif fformat == 'storm_binary':
            _regsurf_export.export_storm_binary(self, mfile)
        else:
            logger.critical('Invalid file format')

    def from_roxar(self, project, name, category, stype='horizons',
                   realisation=0):
        """Load a surface from a Roxar RMS project.

        The import from the RMS project can be done either within the project
        or outside the project.

        Note that a shortform to::

          import xtgeo
          mysurf = xtgeo.surface.RegularSurface()
          mysurf.from_roxar(project, 'TopAare', 'DepthSurface')

        is::

          import xtgeo
          mysurf = xtgeo.surface_from_roxar(project, 'TopAare', 'DepthSurface')

        Note also that horizon/zone name and category must exists in advance,
        otherwise an Exception will be raised.

        Args:
            project (str or special): Name of project (as folder) if
                outside RMS, og just use the magic project word if within RMS.
            name (str): Name of surface/map
            category (str): For horizons/zones only: for example 'DS_extracted'
            stype (str): RMS folder type, 'horizons' (default) or 'zones'
            realisation (int): Realisation number, default is 0

        Returns:
            Object instance updated

        Raises:
            ValueError: Various types of invalid inputs.

        Example:
            Here the from_roxar method is used to initiate the object
            directly::

            >>> mymap = RegularSurface()
            >>> mymap.from_roxar(project, 'TopAare', 'DepthSurface')

        """

        stype = stype.lower()
        valid_stypes = ['horizons', 'zones']

        if stype not in valid_stypes:
            raise ValueError('Invalid stype, only {} stypes is supported.'
                             .format(valid_stypes))

        _regsurf_roxapi.import_horizon_roxapi(
            self, project, name, category, stype, realisation)

    def to_roxar(self, project, name, category, stype='horizons',
                 realisation=0):
        """Store (export) a regular surface to a Roxar RMS project.

        The export to the RMS project can be done either within the project
        or outside the project. The storing is done to the Horizons or the
        Zones folder in RMS.

        Note that horizon or zone name and category must exists in advance,
        otherwise an Exception will be raised.

        Args:
            project (str or special): Name of project (as folder) if
                outside RMS, og just use the magic project word if within RMS.
            name (str): Name of surface/map
            category (str): For horizons/zones only: e.g. 'DS_extracted'.
            stype (str): RMS folder type, 'horizons' (default) or 'zones'
            realisation (int): Realisation number, default is 0

        Raises:
            ValueError: If name or category does not exist in the project

        Example:
            Here the from_roxar method is used to initiate the object
            directly::

              import xtgeo
              topupperreek = xtgeo.surface_from_roxar(project, 'TopUpperReek',
                                                    'DS_extracted')
              topupperreek.values += 200

              # export to file:
              topupperreek.to_file('topupperreek.gri')

              # store in project
              topupperreek.to_roxar(project, 'TopUpperReek', 'DS_something')

        """

        stype = stype.lower()
        valid_stypes = ['horizons', 'zones']

        if stype in valid_stypes and name is None or category is None:
            logger.error('Need to spesify name and category for '
                         'horizon')
        elif stype not in valid_stypes:
            raise ValueError('Only {} stype is supported per now'
                             .format(valid_stypes))

        _regsurf_roxapi.export_horizon_roxapi(
            self, project, name, category, stype, realisation)

    def copy(self):
        """Copy a xtgeo.surface.RegularSurface object to another instance::

            >>> mymapcopy = mymap.copy()

        """
        logger.debug('Copy object instance...')
        logger.debug(self._values)
        logger.debug(self._values.flags)
        logger.debug(id(self._values))

        xsurf = RegularSurface(ncol=self.ncol, nrow=self.nrow, xinc=self.xinc,
                               yinc=self.yinc, xori=self.xori, yori=self.yori,
                               rotation=self.rotation,
                               values=self.values.copy())

        logger.debug('New array + flags + ID')
        return xsurf

    def get_values1d(self, order='C', asmasked=False, fill_value=UNDEF):
        """Get an an 1D, numpy or masked array of the map values.

        Args:
            order (str): Flatteting is in C (default) or F order
            asmasked (bool): If true, as MaskedArray, other as standard
                numpy ndarray with undef as np.nan or fill_value
            fill_value (str): Relevent only if asmasked is False, this
                will be the value of undef entries
        """

        val = self.values
        # now reshape
        if order != 'C':
            val = np.asanyarray(val, order=order)  # ma subclass conserved

        if not asmasked:
            val = ma.filled(val, fill_value=fill_value)

        return val.ravel(order='K')

    def set_values1d(self, val, order='C'):
        """Update the values attribute based on a 1D input, multiple options.

        If values are np.nan or values are > self.undef_limit, they will be
        masked.

        Args:
            order (str): Input is C (default) or F order
        """

        if order == 'F':
            val = np.copy(val, order='C')

        val = val.reshape((self.ncol, self.nrow))

        if not isinstance(val, ma.MaskedArray):
            val = ma.array(val)

        val = ma.masked_greater(val, self.undef_limit)
        val = ma.masked_invalid(val)

        self.values = val

    def get_zval(self):
        """Get an an 1D, numpy array, F order of the map values (not masked).

        Note that undef values are very large numbers (see undef property).
        Also, this will reorder a 2D values array to column fastest, i.e.
        get stuff into Fortran order.

        This routine exists for historical reasons and prefer get_values1d
        instead (C order).
        """
        zval = self.get_values1d(order='F', asmasked=False,
                                 fill_value=self.undef)

        return zval

    def set_zval(self, vals):
        """Set a 1D (unmasked) numpy array (kept for historical reasons).

        The numpy array must be in Fortran order (i columns (ncol) fastest).

        This routine exists for historical reasons and prefer set_values1d
        instead (C order).
        """
        self.set_values1d(vals, order='F')

    def get_rotation(self):
        """Returns the surface roation, in degrees, from X, anti-clock."""
        return self._rotation

    def get_nx(self):
        """ Same as ncol (nx) (for backward compatibility) """
        return self._ncol

    def get_ny(self):
        """ Same as nrow (ny) (for backward compatibility) """
        return self._nrow

    def get_xori(self):
        """ Same as xori (for backward compatibility) """
        return self._xori

    def get_yori(self):
        """ Same as yori (for backward compatibility) """
        return self._yori

    def get_xinc(self):
        """ Same as xinc (for backward compatibility) """
        return self._xinc

    def get_yinc(self):
        """ Same as yinc (for backward compatibility) """
        return self._yinc

    def similarity_index(self, other):
        """Report the degree of similarity between two maps, by comparing mean.

        The method computes the average per surface, and the similarity
        is difference in mean divided on mean of self. I.e. values close
        to 0.0 mean small difference.

        Args:
            other (surface object): The other surface to compare with

        """
        ovalues = other.values
        svalues = self.values

        diff = math.pow(svalues.mean() - ovalues.mean(), 2)
        diff = math.sqrt(diff)

        try:
            diff = diff / svalues.mean()
        except ZeroDivisionError:
            diff = -999

        return diff

    def compare_topology(self, other):
        """Check that two object has the same topology, i.e. map definitions.

        Map definitions such as origin, dimensions, number of defined cells...

        Args:
            other (surface object): The other surface to compare with

        Returns:
            True of same topology, False if not
        """
        if (self.ncol != other.ncol or self.nrow != other.nrow or
                self.xori != other.xori or self.yori != other.yori or
                self.xinc != other.xinc or self.yinc != other.yinc or
                self.rotation != other.rotation):

            return False

        # check that masks are equal
        mas1 = ma.getmaskarray(self.values)
        mas2 = ma.getmaskarray(other.values)
        if not np.array_equal(mas1, mas2):
            return False

        logger.debug('Surfaces have same topology')
        return True

    def get_map_xycorners(self):
        """Get the X and Y coordinates of the map corners.

        Returns a tuple on the form
        ((x0, y0), (x1, y1), (x2, y2), (x3, y3)) where
        (if unrotated and normal flip) 0 is the lower left
        corner, 1 is the right, 2 is the upper left, 3 is the upper right.
        """

        rot1 = self._rotation * math.pi / 180
        rot2 = rot1 + (math.pi / 2.0)

        x0 = self._xori
        y0 = self._yori

        x1 = self._xori + (self.ncol - 1) * math.cos(rot1) * self._xinc
        y1 = self._yori + (self.ncol - 1) * math.sin(rot1) * self._xinc

        x2 = self._xori + (self.nrow - 1) * math.cos(rot2) * self._yinc
        y2 = self._yori + (self.nrow - 1) * math.sin(rot2) * self._yinc

        x3 = x2 + (self.ncol - 1) * math.cos(rot1) * self._xinc
        y3 = y2 + (self.ncol - 1) * math.sin(rot1) * self._xinc

        return ((x0, y0), (x1, y1), (x2, y2), (x3, y3))

    def get_value_from_xy(self, point=(0.0, 0.0)):
        """Return the map value given a X Y point.

        Args:
            point (float tuple): Position of X and Y coordinate
        Returns:
            The map value (interpolated). None if XY is outside defined map

        Example::
            mvalue = map.get_value_from_xy(point=(539291.12, 6788228.2))

        """
        zcoord = _regsurf_oper.get_value_from_xy(self, point=point)

        return zcoord

    def get_xy_value_from_ij(self, iloc, jloc, zvalues=None):
        """Returns x, y, z(value) from i j location.

        Args:
            iloc (int): I (col) location (base is 1)
            jloc (int): J (row) location (base is 1)
            zvalues (ndarray). If this is used in a loop it is wise
                to precompute the numpy surface once in the caller,
                and submit the numpy array (use surf.get_values1d()).

        Returns:
            The z value at location iloc, jloc, None if undefined cell.
        """

        xval, yval, value = _regsurf_oper.get_xy_value_from_ij(
            self, iloc, jloc, zvalues=zvalues)

        return xval, yval, value

    def get_xy_values(self):
        """Return coordinates for X and Y as numpy 2D masked arrays."""

        xvals, yvals = _regsurf_oper.get_xy_values(self)

        return xvals, yvals

    def get_xyz_values(self):
        """Return coordinates for X Y and Z (values) as numpy 2D masked
        arrays."""

        xcoord, ycoord = self.get_xy_values()

        return xcoord, ycoord, self.values

    def get_xy_value_lists(self, lformat='webportal', xyfmt=None,
                           valuefmt=None):
        """Returns two lists for coordinates (x, y) and values.

        For lformat = 'webportal' (default):

        The lists are returned as xylist and valuelist, where xylist
        is on the format:

            [(x1, y1), (x2, y2) ...] (a list of x, y tuples)

        and valuelist is one the format

            [v1, v2, ...]

        Inactive cells will be ignored.

        Args:
            lformat (string): List return format ('webportal' is default,
                other options later)
            xyfmt (string): The formatter for xy numbers, e.g. '12.2f'
                (default None). Note no checks on valid input.
            valuefmt (string): The formatter for values e.g. '8.4f'
                (default None). Note no checks on valid input.

        Returns:
            xylist, valuelist

        Example:

            >>> mymap = RegularSurface('somefile.gri')
            >>> xylist, valuelist = mymap.get_xy_value_lists(valuefmt='6.2f')
        """

        xylist = []
        valuelist = []

        zvalues = self.get_values1d()

        for j in range(self.nrow):
            for i in range(self.ncol):
                xc, yc, vc = self.get_xy_value_from_ij(i + 1, j + 1,
                                                       zvalues=zvalues)

                if vc is not None:
                    if xyfmt is not None:
                        xc = float('{:{width}}'.format(xc, width=xyfmt))
                        yc = float('{:{width}}'.format(yc, width=xyfmt))
                    if valuefmt is not None:
                        vc = float('{:{width}}'.format(vc, width=valuefmt))
                    valuelist.append(vc)
                    xylist.append((xc, yc))

        return xylist, valuelist

    # =========================================================================
    # Interacion with points
    # =========================================================================

    def gridding(self, points, method='linear', coarsen=1):
        """Grid a surface from points.

        Args:
            points(Points): XTGeo Points instance.
            method (str): Gridding method option: linear / cubic / nearest
            coarsen (int): Coarsen factor, to speed up gridding, but will
                give poorer result.

        Example::

            mypoints = Points('pfile.poi')
            mysurf = RegularSurface('top.gri')

            # update the surface by gridding the points
            mysurf.gridding(mypoints)

        Raises:
            RuntimeError: If not possible to grid for some reason
            ValueError: If invalid input

        """

        if not isinstance(points, Points):
            raise ValueError('Argument not a Points '
                             'instance')

        logger.info('Do gridding...')

        _regsurf_gridding.points_gridding(self, points, coarsen=coarsen,
                                          method=method)

    # =========================================================================
    # Interacion with other surface
    # =========================================================================

    def resample(self, other):
        """Resample a surface from another surface instance.

        Note that there may be some 'loss' of nodes at the edges of the
        updated map, as only the 'inside' nodes in the updated map
        versus the input map are applied.

        Args:
            other(RegularSurface): Surface to resample from
        """

        if not isinstance(other, RegularSurface):
            raise ValueError('Argument not a RegularSurface '
                             'instance')

        logger.info('Do resampling...')

        _regsurf_oper.resample(self, other)

    # =========================================================================
    # Change a surface more fundamentally
    # =========================================================================

    def unrotate(self, factor=2):
        """Unrotete a map instance, and this will also change nrow, ncol,
        xinc, etc.

        The default sampling (factor=2) makes a finer gridding in order to
        avoid artifacts, and this default can be used in most cases.

        If a finer grid is wanted, increase the factor. Theoretically the
        new increment for factor=N is between :math:`\\frac{1}{N}` and
        :math:`\\frac{1}{N}\\sqrt{2}` of the original increment,
        dependent on the rotation of the original surface.

        A factor < 1 is not recommended, and a warning will be issued.

        """

        if factor < 1:
            warnings.warn('In unrotate(): A factor < 1 is not recommended!',
                          UserWarning)

        xlen = self.xmax - self.xmin
        ylen = self.ymax - self.ymin
        ncol = self.ncol * factor
        nrow = self.nrow * factor
        xinc = xlen / (ncol - 1)  # node based, not cell center based
        yinc = ylen / (nrow - 1)
        vals = ma.zeros((ncol, nrow), order='C')

        nonrot = RegularSurface(xori=self.xmin,
                                yori=self.ymin,
                                xinc=xinc, yinc=yinc,
                                ncol=ncol, nrow=nrow,
                                values=vals)
        nonrot.resample(self)

        self._values = nonrot.values
        self._nrow = nonrot.nrow
        self._ncol = nonrot.ncol
        self._rotation = nonrot.rotation
        self._xori = nonrot.xori
        self._yori = nonrot.yori
        self._xinc = nonrot.xinc
        self._yinc = nonrot.yinc
        self._cvalues = None

    # =========================================================================
    # Interacion with a cube
    # =========================================================================

    def slice_cube(self, cube, zsurf=None, sampling='nearest', mask=True):
        """Slice the cube and update the instance surface to sampled cube
        values.

        Args:
            cube (object): Instance of a Cube()
            zsurf (surface object): Instance of a depth (or time) map, which
                is the depth or time map (or...) that is used a slicer.
                If None, then the surface instance itself is used a slice
                criteria. Note that zsurf must have same map defs as the
                surface instance.
            sampling (str): 'nearest' for nearest node (default), or
                'trilinear' for trilinear interpolation.
            mask (bool): If True (default), then the map values outside
                the cube will be undef.

        Example::

            cube = Cube('some.segy')
            surf = RegularSurface('s.gri')
            # update surf to sample cube values:
            surf.slice_cube(cube)

        Raises:
            Exception if maps have different definitions (topology)
            RuntimeWarning if number of sampled nodes is less than 10%
        """

        ier = _regsurf_cube.slice_cube(self, cube, zsurf=zsurf,
                                       sampling=sampling, mask=mask)

        if ier == -4:
            raise RuntimeWarning('Number of sampled nodes < 10\%')
        elif ier == -5:
            raise RuntimeWarning('No nodes sampled: map is outside cube?')

    def slice_cube_window(self, cube, zsurf=None, other=None,
                          other_position='below', sampling='nearest',
                          mask=True, zrange=None, ndiv=None, attribute='max'):
        """Slice the cube within a vertical window and get the statistical
        attrubute.

        The statistical attribute can be min, max etc. Attributes are:

        * 'max' for maximum

        * 'min' for minimum

        * 'rms' for root mean square

        * 'mean' for expected value

        * 'var' for variance

        Args:
            cube (Cube): Instance of a Cube()
            zsurf (RegularSurface): Instance of a depth (or time) map, which
                is the depth or time map (or...) that is used a slicer.
                If None, then the surface instance itself is used a slice
                criteria. Note that zsurf must have same map defs as the
                surface instance.
            other (RegularSurface): Instance of other surface if window is
                between surfaces instead of a static window. The zrange
                input is then not applied.
            sampling (str): 'nearest' for nearest node (default), or
                'trilinear' for trilinear interpolation.
            mask (bool): If True (default), then the map values outside
                the cube will be undef.
            zrange (float): The one-sided range of the window, e.g. 10
                (10 is default) units (e.g. meters if in depth mode).
                The full window is +- zrange. If other surface is present,
                zrange is computed based on that.
            ndiv (int): Number of intervals for sampling within zrange. None
                means 'auto' sampling, using 0.5 of cube Z increment as basis.
            attribute (str): The requested attribute, e.g. 'max' value

        Example::

            cube = Cube('some.segy')
            surf = RegularSurface('s.gri')
            # update surf to sample cube values:
            surf.slice_cube_window(cube, attribute='min', zrange=15.0)

        Raises:
            Exception if maps have different definitions (topology)
            ValueError if attribute is invalid.
        """

        if other is None and zrange is None:
            zrange = 10

        _regsurf_cube.slice_cube_window(self, cube, zsurf=zsurf, other=other,
                                        other_position=other_position,
                                        sampling=sampling, mask=mask,
                                        zrange=zrange, ndiv=ndiv,
                                        attribute=attribute)

    # =========================================================================
    # Special methods
    # =========================================================================

    def get_fence(self, xyfence):
        """
        Sample the surface along X and Y positions (numpy arrays) and get Z.

        Note the result is a masked numpy (2D) with rows masked

        Args:
            xyfence (np): A 2D numpy array with shape (N, 3) where columns
            are (X, Y, Z). The Z will be updated to the map.
        """

        xyfence = _regsurf_oper.get_fence(self, xyfence)

        return xyfence

    def hc_thickness_from_3dprops(self, xprop=None, yprop=None,
                                  hcpfzprop=None, zoneprop=None,
                                  zone_minmax=None, dzprop=None,
                                  zone_avg=False, coarsen=1,
                                  mask_outside=False):
        """Make a thickness weighted HC thickness map.

        Make a HC thickness map based on numpy arrays of properties
        from a 3D grid. The numpy arrays here shall be ndarray,
        not masked numpies (MaskedArray).

        Note that the input hcpfzprop is hydrocarbon fraction multiplied
        with thickness, which can be achieved by e.g.:
        cpfz = dz*poro*ntg*shc or by hcpfz = dz*hcpv/vbulk

        Args:
            xprop (ndarray): 3D numpy array of X coordinates
            yprop (ndarray): 3D numpy array of Y coordinates
            hcpfzprop (ndarray): 3D numpy array of HC fraction multiplied
                with DZ per cell.
            zoneprop (ndarray): 3D numpy array indicating zonation
                property, where 1 is the lowest (0 values can be used to
                exclude parts of the grid)
            dzprop (ndarray): 3D numpy array holding DZ thickness. Will
                be applied in weighting if zone_avg is active.
            zone_minmax (tuple): (optional) 2 element list indicating start
                and stop zonation (both start and end spec are included)
            zone_avg (bool): A zone averaging is done prior to map gridding.
                This may speed up the process a lot, but result will be less
                precise. Default is False.
            coarsen (int): Select every N'th X Y point in the gridding. Will
                speed up process, but less precise result. Default=1
            mask_outside (bool): Will mask the result map undef where sum of DZ
                is zero. Default is False as it costs some extra CPU.
        Returns:
            True if operation went OK (but check result!), False if not
        """
        subname = sys._getframe().f_code.co_name

        for i, myprop in enumerate([xprop, yprop, hcpfzprop, zoneprop]):
            if isinstance(myprop, ma.MaskedArray):
                raise ValueError('Property input {} with avg {} to {} is a '
                                 'masked array, not a plain numpy ndarray'
                                 .format(i, myprop.mean(), subname))

        status = _regsurf_gridding.avgsum_from_3dprops_gridding(
            self,
            summing=True,
            xprop=xprop,
            yprop=yprop,
            mprop=hcpfzprop,
            dzprop=dzprop,
            zoneprop=zoneprop,
            zone_minmax=zone_minmax,
            zone_avg=zone_avg,
            coarsen=coarsen,
            mask_outside=mask_outside)

        if status is False:
            raise RuntimeError('Failure from hc thickness calculation')

    def avg_from_3dprop(self, xprop=None, yprop=None,
                        mprop=None, dzprop=None,
                        truncate_le=None, zoneprop=None, zone_minmax=None,
                        coarsen=1, zone_avg=False):
        """
        Make an average map (DZ weighted) based on numpy arrays of
        properties from a 3D grid.

        The 3D arrays mush be ordinary numpies of size (nx,ny,nz). Undef
        entries must be given weights 0 by using DZ=0

        Args:
            xprop: 3D numpy of all X coordinates (also inactive cells)
            yprop: 3D numpy of all Y coordinates (also inactive cells)
            mprop: 3D numpy of requested property (e.g. porosity) all
            dzprop: 3D numpy of dz values (for weighting)
                NB zero for undef cells
            truncate_le (float): Optional. Truncate value (mask) if
                value is less
            zoneprop: 3D numpy to a zone property
            zone_minmax: a tuple with from-to zones to combine
                (e.g. (1,3))

        Returns:
            Nothing explicit, but updates the surface object.
        """

        subname = sys._getframe().f_code.co_name

        for i, myprop in enumerate([xprop, yprop, mprop, dzprop, zoneprop]):
            if isinstance(myprop, ma.MaskedArray):
                raise ValueError('Property input {} with avg {} to {} is a '
                                 'masked array, not a plain numpy ndarray'
                                 .format(i, myprop.mean(), subname))

        _regsurf_gridding.avgsum_from_3dprops_gridding(
            self,
            summing=False,
            xprop=xprop,
            yprop=yprop,
            mprop=mprop,
            dzprop=dzprop,
            truncate_le=truncate_le,
            zoneprop=zoneprop,
            zone_minmax=zone_minmax,
            coarsen=coarsen,
            zone_avg=zone_avg)

    def quickplot(self, filename='/tmp/default.png', title='QuickPlot',
                  infotext=None, minmax=(None, None), xlabelrotation=None,
                  colormap='rainbow', colortable=None,
                  faults=None, logarithmic=False):
        """Fast surface plot of maps using matplotlib.

        Args:
            filename (str): Name of plot file.
            title (str): Title of plot
            infotext (str): Additonal info on plot.
            minmax (tuple): Tuple of min and max values to be plotted. Note
                that values outside range will be set equal to range limits
            xlabelrotation (float): Rotation in degrees of X labels.
            colormap (str): Name of matplotlib or RMS file or XTGeo
                colormap. Default is matplotlib's 'rainbow'
            colortable (str): Deprecated, for backward compatibility! used
                colormap instead.
            faults (dict): If fault plot is wanted, a dictionary on the
                form => {'faults': XTGeo Polygons object, 'color': 'k'}
            logarithmic (bool): If True, a logarithmic contouring color scale
                will be used.

        """

        # This is using the more versatile Map class in XTGeo. Most kwargs
        # is just passed as is. Prefer using Map() directly in apps?

        mymap = Map()

        logger.info('Infotext is <{}>'.format(infotext))
        mymap.canvas(title=title, infotext=infotext)

        minvalue = minmax[0]
        maxvalue = minmax[1]

        if colortable is not None:
            colormap = colortable

        mymap.colormap = colormap

        mymap.plot_surface(self, minvalue=minvalue,
                           maxvalue=maxvalue, xlabelrotation=xlabelrotation,
                           logarithmic=logarithmic)
        if faults:
            mymap.plot_faults(faults['faults'])

        if filename is None:
            mymap.show()
        else:
            mymap.savefig(filename)

    def distance_from_point(self, point=(0, 0), azimuth=0.0):
        """Make map values as horizontal distance from a point with azimuth
        direction.

        Args:
            point (tuple): Point to measure from
            azimuth (float): Angle from North (clockwise) in degrees

        """

        _regsurf_oper.distance_from_point(self, point=point, azimuth=azimuth)

    def translate_coordinates(self, translate=(0, 0, 0)):
        """Translate a map in X Y VALUE space.

        Args:
            translate (tuple): Translate (shift) distance in X Y Z

        Example::

            map = RegularSurface()
            map.from_file('some_file')
            map.translate((300,500,0))
            map.to_file('some_file')

        """

        xshift, yshift, zshift = translate

        # just shift the xori and yori
        self.xori = self.xori + xshift
        self.yori = self.yori + yshift

        # note the Z coordinates are perhaps not depth
        # numpy operation:
        self.values = self.values + zshift