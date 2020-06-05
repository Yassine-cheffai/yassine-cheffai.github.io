import c4d
from c4d import storage, bitmaps, utils

USE_PROFILING = True
if USE_PROFILING:
    try:
        import cProfile
        import pstats
        import StringIO
    except:
        pass


class Profiler():

    def __init__(self):
        self._pr = None

    def Start(self):
        if not USE_PROFILING:
            raise Exception("Profiling is not enabled")

        self._pr = cProfile.Profile()
        self._pr.enable()

    def Stop(self, doPrint=False):
        self._pr.disable()
        s = StringIO.StringIO()
        ps = pstats.Stats(self._pr, stream=s).sort_stats('cumulative')
        if doPrint:
            ps.print_stats()

        return s.getvalue()


def BaseBitmap2Jpg(bmp, quality=90, width=None, height=None):
    if width or height:
        if width == 0:
            width = bmp.GetBw()

        if height == 0:
            height = bmp.GetBh()

        # if only width is given, keep aspect ratio
        if not width:
            width = int(bmp.GetBw() / (bmp.GetBh() / float(height)))

        # if only height is given, keep aspect ratio
        if not height:
            height = int(bmp.GetBh() / (bmp.GetBw() / float(width)))

        bmpSmall = bitmaps.BaseBitmap()
        bmpSmall.Init(width, height, 24)
        bmp.ScaleBicubic(bmpSmall, 0, 0, bmp.GetBw() - 1, bmp.GetBh()
                         - 1, 0, 0, bmpSmall.GetBw() - 1, bmpSmall.GetBh() - 1)
        bmp = bmpSmall

    mfs = storage.MemoryFileStruct()
    mfs.SetMemoryWriteMode()

    bc = c4d.BaseContainer()
    bc.SetLong(c4d.JPGSAVER_QUALITY, int(utils.Clamp(0, quality, 100.0)))
    bmp.Save(mfs, c4d.FILTER_JPG, bc, c4d.SAVEBIT_0)

    data = mfs.GetData()
    if not data:
        return None

    return data[0]
