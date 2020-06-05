import sys
import os
import select
import datetime
import imp
import uuid
import mimetypes
import time
import StringIO
import re
import urllib
import socket
import threading
import gzip
import errno
import webbrowser
import traceback
import cStringIO

import c4d
from c4d import threading as c4dthreading

import functools
from functools import wraps

C4DPL_WEBSERVER_STOP = 300002208
C4DPL_WEBSERVER_SHUTDOWN = 300002212
C4DPL_WEBSERVER_OPENWEBINTERFACE = 300002220
C4DPL_WEBSERVER_CREATEANDSTART = 300002222

DEBUG = c4d.MAXON_TARGET_DEBUG or os.path.exists(
    os.path.join(c4d.storage.GeGetStartupPath().decode("utf-8"), u"c4d_net_debug.txt"))
DIRECTORY_WEBSERVER = os.path.split(__file__)[0].decode("utf-8")
TRIGGERFILEPATH = os.path.join(
    c4d.storage.GeGetC4DPath(c4d.C4D_PATH_PREFS), ".webserver.up")

customHtdocs = os.path.join(
    c4d.storage.GeGetC4DPath(c4d.C4D_PATH_PREFS), "htdocs").decode("utf-8")
if os.path.exists(customHtdocs):
    DIRECTORY_WEBSERVERHTDOCS = customHtdocs
else:
    DIRECTORY_WEBSERVERHTDOCS = os.path.join(DIRECTORY_WEBSERVER, "htdocs")

    # Create dummy _htdocs. If it's renamed to htdocs the content replaces the
    # default web-interface
    try:
        os.mkdir(
            os.path.join(c4d.storage.GeGetC4DPath(c4d.C4D_PATH_PREFS), "_htdocs"))
    except:
        pass

DIRECTORY_NETRENDER_DEFAULT = os.path.abspath(
    os.path.join(c4d.storage.GeGetC4DPath(c4d.C4D_PATH_RESOURCE), '..', 'teamrender'))
DIRECTORY_WEBSERVERLIBS = os.path.join(DIRECTORY_WEBSERVER, "libs")
DIRECTORY_STATIC = os.path.join(DIRECTORY_WEBSERVERHTDOCS, "assets")

sys.path.append(DIRECTORY_WEBSERVER.encode(sys.getfilesystemencoding()))
sys.path.append(DIRECTORY_WEBSERVERLIBS.encode(sys.getfilesystemencoding()))

from flask import Flask, json, render_template, request, session, make_response, after_this_request, send_file, send_from_directory

from werkzeug import serving
from werkzeug.datastructures import Headers
from werkzeug.exceptions import default_exceptions

try:
    from werkzeug.wsgi import wrap_file
except ImportError:
    from werkzeug.utils import wrap_file

import core.jobs as jobs
import core.network as network
import core.users as users
import core.util as util
import core.settings as settings


if not DEBUG:
    # Disable Logging
    try:
        import logging
        logging.getLogger('werkzeug').setLevel(logging.ERROR)
    except:
        pass


def jsonify(*args, **kwargs):
    return app.response_class(json.dumps(dict(*args, **kwargs), cls=CustomJsonEncoder), mimetype='application/json')


def _eintr_retry(func, *args):
    """restart a system call interrupted by EINTR"""
    while True:
        try:
            return func(*args)
        except (OSError, select.error) as e:
            if e.args[0] != errno.EINTR:
                raise


class SystemInfo():

    def __init__(self):
        _temp = c4d.GetMachineFeatures()
        self._cacheOsVersionString = _temp.GetString(
            c4d.MACHINEINFO_OSVERSION)
        self._cacheCpuSpeed = _temp.GetFloat(
            c4d.MACHINEINFO_PROCESSORSPEED_MHZ)
        self._cacheAvailableMemory = _temp.GetInt64(
            c4d.MACHINEINFO_PHYSICAL_RAM_SIZE)
        self._cacheCpuCount = _temp.GetInt32(
            c4d.MACHINEINFO_NUMBEROFPROCESSORS)
        self._cacheVersion = c4d.GetC4DVersion()
        del _temp  # important: free unused container


class Bytes():
    __slots__ = ["_bytes"]

    def __init__(self, bytes):
        self._bytes = bytes

    def GetBytes(self):
        return self._bytes

    def __ne__(self, other):
        return self._bytes != other._bytes

    def __eq__(self, other):
        return self._bytes == other._bytes

    def __lt__(self, other):
        return self._bytes < other._bytes

    def __gt__(self, other):
        return self._bytes > other._bytes

    def __ge__(self, other):
        return self._bytes >= other._bytes

    def __le__(self, other):
        return self._bytes <= other._bytes


class Kilobytes(Bytes):

    def __init__(self, gigabytes):
        Bytes.__init__(self, gigabytes * 1024)


class Megabytes(Bytes):

    def __init__(self, gigabytes):
        Bytes.__init__(self, gigabytes * 1024 * 1024)


class Gigabytes(Bytes):

    def __init__(self, gigabytes):
        Bytes.__init__(self, gigabytes * 1024 * 1024 * 1024)


class RdataDescription():

    _rdataGroups = []
    _g_rdataDescriptions = []

    def __init__(self):
        # create the dynamic description list from the render-settings. This is executed
        # in the initializer to speedup the job-request.
        rdata = c4d.documents.RenderData()
        desc = rdata.GetDescription(c4d.DESCFLAGS_DESC_0)
        for bc, id, groupid in desc:

            cycles = None
            tmpCycles = bc.GetContainerInstance(c4d.DESC_CYCLE)
            if tmpCycles:
                cycles = []
                for index, value in tmpCycles:
                    # remove the ... ellipsis at the end of a text (e.g. from
                    # "File..." to "File")
                    valueWithoutEllipsis = value.rstrip(".")
                    cycles.append((index, valueWithoutEllipsis))

            self._g_rdataDescriptions.append((bc, id, groupid, cycles))

        # create the rdata description groups from the descriptions
        for bc, id, groupid, cycles in self._g_rdataDescriptions:
            if id[-1].dtype == c4d.DTYPE_GROUP:
                tmp = bc.GetString(c4d.DESC_NAME)
                if tmp:
                    self._rdataGroups.append(
                        {"name": tmp, "id": id[-1].id, "groupid": groupid[-1].id})

    def GetRdataGroups(self):
        return self._rdataGroups

    def GetRdataDescriptions(self):
        return self._g_rdataDescriptions


class LanguagePool():

    __languages = None
    __languageDump = None

    def __init__(self):
        self.__languages = []

        # Request the default language to create the language cache to speedup
        # the first HTTP connect after start
        for language in self.GetLanguages():
            if language["default_language"]:
                self.GetLanguage(language["extensions"], True)
                break

    def GetLanguages(self):
        """
        Returns the original list of installed languages
        """
        languages = []
        index = 0
        while True:
            languageTmp = c4d.GeGetLanguage(index)
            if not languageTmp:
                break

            index += 1
            languages.append(languageTmp)
        return languages

    def ParseStringFile(self, language, pathStrings_XX, reraiseException=False):
        fp = None
        with open(pathStrings_XX, "r") as fp:
            for line in fp.readlines():
                entry = line.strip()
                try:
                    sre = re.search("\s*([\w]*)\s*(\".*\")\s*;\s*.*", entry)
                    if sre:
                        ident, strippedString = sre.groups()
                        language[ident] = strippedString.strip(
                            "\"").decode("unicode_escape")
                        # .encode("utf-8") The default encoding for JSON is utf-8. The strings in JSO are represented
                        # by a unicode escape sequence
                        # See http://www.ietf.org/rfc/rfc4627.txt --> 2.5
                        # String Representation
                except:
                    if reraiseException:
                        raise

    def GetLanguagesDump(self):
        """
        Returns the installed language packs.
        @return:    A language dict with the keys 'lang_id', 'language', 'modified_date' and 'hash_str'
        """

        if self.__languageDump:
            return self.__languageDump

        languages = self.GetLanguages()
        languages_hash_str = str(hash(str(languages)))
        json_dump = json.dumps({"languages": languages}, cls=json.JSONEncoder)
        self.__languageDump = {
            "json_dump": json_dump, "modified_date": g_ApplicationUptime, "hash_str": languages_hash_str}

        return self.__languageDump

    def GetLanguage(self, lang_id, us_fallback=True):
        """
        Returns the requested language pack
        @return:    A language dict with the keys 'lang_id', 'language', 'modified_date' and 'hash_str'
        """
        lang_id = lang_id.lower()

        language_elements = {}
        language_modified = None

        if lang_id:
            strings_XX = "strings_" + lang_id

            # Check the language cache if the language has already been parsed
            for language in self.__languages:
                if language["lang_id"] == lang_id:
                    return language

            try:
                pathStrings_XX = os.path.join(
                    DIRECTORY_NETRENDER_DEFAULT, strings_XX, "c4d_strings.str")
                if os.path.exists(pathStrings_XX):
                    self.ParseStringFile(language_elements, pathStrings_XX)

                    try:
                        # use the modification date of the string file
                        language_modified = os.path.getmtime(pathStrings_XX)
                    except:
                        pass
            except:
                if DEBUG:
                    assert(False)

            try:
                # Add the string files from the xtensions string resource (for
                # the render settings tab)
                pathStrings_XX = os.path.join(c4d.storage.GeGetC4DPath(
                    c4d.C4D_PATH_RESOURCE), "..", "xtensions", strings_XX, "description", "drendersettings.str")
                if os.path.exists(pathStrings_XX):
                    self.ParseStringFile(language_elements, pathStrings_XX)
            except:
                if DEBUG:
                    assert(False)

            language_hash_str = str(hash(frozenset(language_elements)))
            json_dump = json.dumps(
                {"labels": language_elements}, cls=json.JSONEncoder)
            language_pack = {"lang_id": lang_id, "elements": language_elements, "json_dump":
                             json_dump, "modified_date": language_modified, "hash_str": language_hash_str}
            self.__languages.append(language_pack)

            if language_elements:
                return language_pack

        # fallback to US language if not already requested
        return self.GetLanguage("us", False)


class Connection(c4dthreading.C4DThread):

    __slots__ = ["request", "client_address", "app", "ssl_context",
                 "multithread", "multiprocess", "server_address", "passthrough_errors"]

    def log(self, log, a, b):
        print(log, a, b)

    def __init__(self, request, client_address, app):
        self.request = request
        self.client_address = client_address
        self.app = app
        self.server_address = app.server_address
        self.passthrough_errors = app.passthrough_errors
        self.ssl_context = None
        self.multithread = True
        self.multiprocess = False

    def Start(self, *args, **kwargs):
        return c4dthreading.C4DThread.Start(self, *args, **kwargs)

    def End(self, wait):
        return c4dthreading.C4DThread.End(self, wait)

    def GetThreadName(self):
        return "TR Browser Conn"

    def Main(self):
        server = self.app._server
        request = self.request
        client_address = self.client_address

        try:
            server.finish_request(request, client_address)
            server.close_request(request)
        except:
            server.handle_error(request, client_address)
            server.close_request(request)

        self.app.TriggerCleanup()


class FlaskServer(Flask):

    __triggerCleanup = None
    __connections = None

    def TriggerCleanup(self):
        self.__triggerCleanup = True

    def __init__(self, *args, **kw):
        super(FlaskServer, self).__init__(*args, **kw)
        self.__connections = []
        self.__triggerCleanup = False

    def ServeForever(self, server, bt, poll_interval=1.0):

        def CleanupThreads():
            if self.__triggerCleanup:
                self.__triggerCleanup = False
                self.__connectionsLock.acquire()
                self.__connections = filter(
                    lambda x: x.IsRunning(), self.__connections)
                self.__connectionsLock.release()

        def StopThreads():
            for thr in self.__connections:
                thr.End(False)

            for thr in self.__connections:
                thr.Wait(False)

        server.__serving = True
        while not bt.TestBreak():
            r, w, e = _eintr_retry(
                select.select, [server], [], [], poll_interval)
            if r:
                try:
                    server._handle_request_noblock()
                except:
                    pass

            CleanupThreads()
        self.shutdown_signal = True
        StopThreads()

    def ProcessRequest(self, request, client_address):
        conn = Connection(request, client_address, self)
        conn.Start(c4d.THREADMODE_ASYNC, c4d.THREADPRIORITY_ABOVE)

        self.__connectionsLock.acquire()
        self.__connections.append(conn)
        self.__connectionsLock.release()

    def MakeServer(self, host, port, app=None,
                   request_handler=None, passthrough_errors=False,
                   ssl_context=None):
        """
        Create a new server instance that is either threaded or not
        """
        server = None

        def TestPortOnLoopback(port):
            """
            Check if a socket listener can be opened on a loopwith with the given port.
            """

            def DoTest():
                # Open a socket on all interfaces under the given port
                def OpenSocket(local):
                    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                    s.bind(("127.0.0.1" if local else "0.0.0.0", port))
                    s.listen(5)
                    s.close()

                openedOnLocalInterfaces = False
                openedOnAllInterfaces = False
                localInterfaceException = None
                try:
                    OpenSocket(True)
                    openedOnLocalInterfaces = True
                except Exception as e:
                    # store the exception for later usage
                    localInterfaceException = e
                else:
                    OpenSocket(False)
                    openedOnAllInterfaces = True

                return openedOnAllInterfaces, openedOnLocalInterfaces, localInterfaceException

            boundToAllInterfaces = False
            openedOnLocalInterfaces = False
            # if the trigger file still exists, the application was not
            # shutdown properly
            nongracefulShutdown = os.path.exists(TRIGGERFILEPATH)
            if nongracefulShutdown:
                t = c4d.GeGetTimer()
                maxSeconds = 15
                curSeconds = 0
                while c4d.GeGetTimer() - t < maxSeconds * 1000:
                    info = "Restoring Webserver. Please wait... %1.0f%%" % (
                        curSeconds / float(maxSeconds) * 100.0)
                    c4d.StatusSetText(info)
                    print(info)
                    curSeconds += 1

                    boundToAllInterfaces, openedOnLocalInterfaces, localInterfaceException = DoTest()
                    if openedOnLocalInterfaces:
                        print("Restoring Webserver. Please wait... %1.0f%%" % (100.0))
                        break
                    else:
                        time.sleep(1)
                        continue

                c4d.StatusClear()
            else:
                boundToAllInterfaces, openedOnLocalInterfaces, localInterfaceException = DoTest()

            if not openedOnLocalInterfaces:
                raise localInterfaceException
            elif not boundToAllInterfaces:
                print(
                    "WARNING: Webserver is just reachable via 127.0.0.1:{0}. Change the webserver port and try again.".format(port))

            # Create the trigger file which is available after a non-graceful
            # shutdown of c4d
            try:
                open(TRIGGERFILEPATH, 'a').close()
            except:
                pass

            # the webserver must run at least on 127.0.0.1:port
            return openedOnLocalInterfaces

        success = TestPortOnLoopback(port)
        if not success:
            return None

        try:
            service = c4d.modules.net.GetGlobalNetRenderService()
            service.NetConsoleOutput(
                1, "Webserver started on port {0}\n".format(port))
        except:
            pass

        self.server_address = (host, port)
        self.passthrough_errors = passthrough_errors

        server = serving.ThreadedWSGIServer(
            host, port, app, request_handler, passthrough_errors, ssl_context)
        if server and g_useC4DThreadForRequests:
            self.__connectionsLock = threading.Lock()

            # Use self.ProcessRequest because FlaskServer does not inherit from
            # ThreadedWSGIServer
            server.process_request = self.ProcessRequest
        return server

    def RunSimple(self, bt, host, port, application, use_reloader=False,
                  use_debugger=False, use_evalex=True,
                  extra_files=None, threaded=False, request_handler=None, static_files=None,
                  passthrough_errors=False, ssl_context=None):

        if os.environ.get('WERKZEUG_RUN_MAIN') != 'true':
            display_hostname = host != '*' and host or 'localhost'
            if ':' in display_hostname:
                display_hostname = '[%s]' % display_hostname

        self._server = self.MakeServer(host, port, application,
                                       request_handler, passthrough_errors, ssl_context)
        try:
            if self._server:
                self.ServeForever(self._server, bt)
        except:
            raise
        finally:
            if self._server:
                self._server.server_close()

            # remove the trigger file
            try:
                os.remove(TRIGGERFILEPATH)
            except:
                pass

    def Run(self, bt, host=None, port=None, debug=None, **options):

        if host is None:
            host = '0.0.0.0'

        if port is None or port == 0:
            port = 8080

        self.RunSimple(bt, host, port, self, **options)


class CustomJsonEncoder(json.JSONEncoder):

    _jobstate_status_statusshort = {
        c4d.JOBSTATE_IDLE: ('jobstate_idle', 'idle', None),
        c4d.JOBSTATE_PREPARING_RUNNING: ('jobstate_preparing_running', 'prepare', None),
        c4d.JOBSTATE_PREPARING_FAILED: ('jobstate_preparing_failed', 'failed', 1.0),
        c4d.JOBSTATE_PREPARING_OK: ('jobstate_preparing_ok', 'prepare', None),
        c4d.JOBSTATE_RENDER_RUNNING: ('jobstate_render_running', 'progress', None),
        c4d.JOBSTATE_RENDER_OK: ('jobstate_render_ok', 'complete', None),
        c4d.JOBSTATE_RENDER_FAILED: ('jobstate_render_failed', 'failed', 1.0),
        c4d.JOBSTATE_ALLOCATESPACE_RUNNING: ('jobstate_allocatespace_running', 'progress', None),
        c4d.JOBSTATE_ALLOCATESPACE_OK: ('jobstate_allocatespace_ok', '', None),
        c4d.JOBSTATE_ALLOCATESPACE_FAILED: ('jobstate_allocatespace_failed', 'failed', 1.0),
        c4d.JOBSTATE_DOWNLOAD_RUNNING: ('jobstate_download_running', 'progress', None),
        c4d.JOBSTATE_DOWNLOAD_OK: ('jobstate_download_ok', '', None),
        c4d.JOBSTATE_DOWNLOAD_FAILED: ('jobstate_download_failed', 'failed', 1.0),
        c4d.JOBSTATE_ASSEMBLE_RUNNING: ('jobstate_assemble_running', 'assemble', None),
        c4d.JOBSTATE_ASSEMBLE_OK: ('jobstate_assemble_ok', 'complete', None),
        c4d.JOBSTATE_ASSEMBLE_FAILED: ('jobstate_assemble_failed', 'failed', 1.0),
        c4d.JOBSTATE_STOPPED: ('jobstate_stopped', 'stopped', None),
        c4d.JOBSTATE_QUEUED: ('jobstate_queued', 'prepare', None),
        c4d.JOBSTATE_PENDING: ('jobstate_pending', 'pending', None)
    }

    def EnhanceJobList(self, job):
        """Converts C4D variables to String values.
        """
        status, statusshort, progressvalue = self._jobstate_status_statusshort.get(
            job['state'], ('undefined', '', None))
        job['status'] = status
        job['statusshort'] = statusshort
        job['progressgui'] = (
            progressvalue if progressvalue else job['progress']) * 100

    def ConvertBaseContainerToList(self, obj):

        def SerializeAssets(bc):
            asset = {}
            asset["size"] = bc.GetInt64(c4d.JOB_ASSET_SIZE)
            asset["relpath"] = bc.GetFilename(c4d.JOB_ASSET_RELDIRECTORYPATH)

            if bc.GetBool(c4d.JOB_ASSET_IMAGESEQUENCE):
                asset["imagesequence"] = True
                asset["imagesequenceprefix"] = bc.GetString(
                    c4d.JOB_ASSET_IMAGESEQUENCE_PREFIX)
                asset["imagesequenceappendix"] = bc.GetString(
                    c4d.JOB_ASSET_IMAGESEQUENCE_APPENDIX)
                asset["imagesequencefrom"] = bc.GetInt64(
                    c4d.JOB_ASSET_IMAGESEQUENCE_FROM)
                asset["imagesequenceto"] = bc.GetInt64(
                    c4d.JOB_ASSET_IMAGESEQUENCE_TO)
            else:
                asset["name"] = bc.GetString(c4d.JOB_ASSET_NAME)

            # dont make public, it's not needed right now
            # asset["directory"] = bc.GetFilename(c4d.JOB_ASSET_DIRECTORYPATH)
            return asset

        def SerializeResultAssets(bc):
            resultasset = {}
            resultasset["name"] = bc.GetString(c4d.JOB_RESULTASSET_NAME)
            resultasset["size"] = bc.GetInt64(c4d.JOB_RESULTASSET_SIZE)
            resultasset["directory"] = bc.GetFilename(c4d.JOB_RESULTASSET_DIRECTORYPATH)
            return resultasset

        def SerializeContainerElement(value, rdata):
            # rdata is used if a specific type needs additional context
            # information from another render settings entry
            if isinstance(value, c4d.BaseTime):
                return str(value.GetFrame(int(rdata.GetFloat(c4d.RDATA_FRAMERATE))))
            elif isinstance(value, (int, long, str, float, c4d.BaseTime)):
                return value
            else:
                return False

        if obj.GetId() == c4d.MACHINE_CONTAINER:
            machine = {}

            machine["id"] = obj.GetInt32(c4d.MACHINE_ID)
            machine["uuid"] = obj.GetUuid(c4d.MACHINE_UUID)
            machine["name"] = obj.GetString(c4d.MACHINE_NAME)
            machine["description"] = obj.GetString(c4d.MACHINE_DESC)
            machine["status"] = "machinestate_" + \
                obj.GetString(c4d.MACHINE_STATE)
            machine["statusshort"] = obj.GetString(c4d.MACHINE_STATE)
            machine["driverversion"] = obj.GetString(c4d.MACHINE_DRIVERVERSION)
            machine["selected"] = obj.GetBool(c4d.MACHINE_SELECTED)
            machine["verificationbits"] = obj.GetInt32(
                c4d.MACHINE_VERIFICATIONBITS)
            machine["islocal"] = obj.GetBool(c4d.MACHINE_ISLOCAL)
            machine["os"] = obj.GetString(c4d.MACHINE_OS)
            machine["cinemaversion"] = obj.GetString(c4d.MACHINE_CINEMAVERSION)
            machine["graphiccard"] = obj.GetString(c4d.MACHINE_GRAPHICCARD)
            machine["processor"] = obj.GetString(c4d.MACHINE_PROCESSOR)
            machine["plugins"] = obj.GetString(c4d.MACHINE_PLUGINS)
            machine["address"] = obj.GetString(c4d.MACHINE_ADDRESS)
            machine["current_job"] = obj.GetString(c4d.MACHINE_CURRENTJOB)
            machine["current_framestart"] = obj.GetInt64(c4d.MACHINE_FRAMESTART)
            machine["current_frameend"] = obj.GetInt64(c4d.MACHINE_FRAMEEND)

            if obj.GetType(c4d.MACHINE_CURRENTLOG) == c4d.DA_CONTAINER:
                current_logs = []
                for index, line in obj.GetContainerInstance(c4d.MACHINE_CURRENTLOG):
                    current_logs.append(line)
                machine["current_logs"] = current_logs

            return machine

        elif obj.GetId() == c4d.JOB_CONTAINER:
            job = {}

            # only deliver the ID if its set
            if obj.GetType(c4d.JOB_ID) != c4d.DA_NIL:
                job["id"] = obj.GetInt32(c4d.JOB_ID)

            job["name"] = obj.GetString(c4d.JOB_NAME)
            job["uuid"] = str(obj.GetUuid(c4d.JOB_UUID))
            job["state"] = obj.GetInt32(c4d.JOB_STATE)
            job["framerange"] = obj.GetInt32(c4d.JOB_FRAMERANGE)
            job["user"] = obj.GetString(c4d.JOB_USER)
            job["framesrel"] = obj.GetInt64(c4d.JOB_FRAMES_REL)
            job["framesabs"] = obj.GetInt64(c4d.JOB_FRAMES_ABS)
            job["datetime_create"] = obj.GetString(c4d.JOB_DATETIME_CREATE)
            job["datetime_start"] = obj.GetString(c4d.JOB_DATETIME_START)
            job["datetime_rendertime"] = obj.GetInt64(
                c4d.JOB_DATETIME_RENDERTIME)
            job["progress"] = obj.GetFloat(c4d.JOB_PROGRESS)
            job["defaultscenename"] = obj.GetString(c4d.JOB_DEFAULTSCENENAME)
            job["errorcount"] = obj.GetInt64(c4d.JOB_ERRORCOUNT)
            job["involvedmachines"] = obj.GetInt64(c4d.JOB_INVOLVEDMACHINES)
            job["assets_modified"] = obj.GetBool(c4d.JOB_ASSET_MODIFIED)
            job["resultassets_modified"] = obj.GetBool(
                c4d.JOB_RESULTASSET_MODIFIED)

            # Render Settings
            if obj.GetType(c4d.JOB_RDATA) == c4d.DA_CONTAINER:
                rdataList = []
                rdata = obj.GetContainer(c4d.JOB_RDATA)
                for index, value in rdata:
                    tmp = SerializeContainerElement(value, rdata)
                    if tmp:
                        for bc, id, groupid, cycles in g_rdataDesc.GetRdataDescriptions():
                            if id[-1].id == index:
                                ident = bc.GetString(c4d.DESC_IDENT)
                                translation = None

                                # if the rendersetting element has cycles,
                                # check for a corresponding translation
                                if cycles:
                                    # find the first element in the cycle and
                                    # return the name, otherwise just the value
                                    translation = next(
                                        (name for index, name in cycles if tmp == index), tmp)

                                # on some values idents might not be set
                                if ident:
                                    data = {
                                        ident: tmp, "groupid": groupid[-1].id, "dtype": id[-1].dtype}
                                    if translation:
                                        data["translation"] = translation
                                    rdataList.append(data)
                                break
                job["rdata"] = rdataList

            if obj.GetType(c4d.JOB_CURRENTLOG) == c4d.DA_CONTAINER:
                current_logs = []
                for index, line in obj.GetContainerInstance(c4d.JOB_CURRENTLOG):
                    current_logs.append(line)
                job["current_logs"] = current_logs

            # Assets
            if obj.GetType(c4d.JOB_ASSET) == c4d.DA_CONTAINER:
                assets = []
                for index, asset in obj.GetContainerInstance(c4d.JOB_ASSET):
                    assets.append(SerializeAssets(asset))
                job["assets"] = assets

            # Result Assets
            if obj.GetType(c4d.JOB_RESULTASSET) == c4d.DA_CONTAINER:
                resultassets = []
                
                directory = obj.GetFilename(c4d.JOB_DIRECTORY).decode("utf-8")
                relpath = os.path.join(directory, settings.RESULTDIRECTORYNAME)
                
                for index, resultasset in obj.GetContainerInstance(c4d.JOB_RESULTASSET):
                    resultasset = SerializeResultAssets(resultasset)
                    resultasset['directory'] = os.path.relpath(resultasset['directory'], relpath)
                    resultassets.append(resultasset)
                job["resultassets"] = resultassets

            self.EnhanceJobList(job)

            return job
        else:
            raise TypeError("Unknown container")

    def default(self, obj):
        if isinstance(obj, c4d.BaseContainer):
            return self.ConvertBaseContainerToList(obj)
        elif isinstance(obj, uuid.UUID):
            return str(obj)
        else:
            return json.JSONEncoder.default(self, obj)


g_useC4DThreadForRequests = True
g_suggestedTimePerEndpoint = 2 if DEBUG else None
g_userLock = threading.Lock()
g_ApplicationUptime = int(time.time())
g_webserver = None
g_systemInfo = SystemInfo()
g_rdataDesc = RdataDescription()
g_languagePool = LanguagePool()
g_endPoints = []

# Currently HTTP/1.1 is not supported. If a HTTP request has a keep-alive connection
# we need to shutdown the 2nd self.handle_one_request properly (see BaseHTTPRequestHandler.handle)
# serving.BaseHTTPRequestHandler.protocol_version = "HTTP/1.1"
app = FlaskServer(__name__, template_folder=DIRECTORY_WEBSERVERHTDOCS.encode(
    sys.getfilesystemencoding()), static_folder=DIRECTORY_STATIC.encode(sys.getfilesystemencoding()))


def EnableTimeCheck(suggestedTimePerEndpoint):
    global g_suggestedTimePerEndpoint
    g_suggestedTimePerEndpoint = suggestedTimePerEndpoint


class EndpointHelper():

    @staticmethod
    def Register(endpoint):
        """
        Registers a flask endpoint and lists it in the /help endpoint
        """
        g_endPoints.append(endpoint)
        return endpoint

    @staticmethod
    def ErrorHandling(fn):
        @wraps(fn)
        def DecoratorWithTimeCheck(*args, **kwargs):
            try:
                if g_suggestedTimePerEndpoint:
                    debugTime = time.time()

                # Actual End Point Call
                res = fn(*args, **kwargs)

                if g_suggestedTimePerEndpoint:
                    endpointTime = time.time() - debugTime
                    if endpointTime > g_suggestedTimePerEndpoint:
                        print(
                            "WARNING: %s(...) took longer than the suggested %ims: %ims\n    ARGS: %s\n    KWARGS: %s\n    PARAMS: %s\n    POST: %s" %
                            (fn.__name__, g_suggestedTimePerEndpoint * 1000, endpointTime * 1000, repr(args), repr(kwargs), repr(request.args), repr(request.form)))
                return res
            # except None, e:
            except Exception as e:
                if DEBUG:
                    raise

                status_code = 500

                # find a more proper error code
                for key, value in default_exceptions.iteritems():
                    if isinstance(e, value):
                        status_code = key
                        break

                return make_response(str(e), status_code)

        return DecoratorWithTimeCheck

    @staticmethod
    def RequireLogin(fn):
        """
        Decorator to enforce logged in users.
        """
        @wraps(fn)
        def decorator(*args, **kwargs):
            validSession = session.get('useruuid', None)
            if not ViewFunctionHelper.GetUserManagementEnabled():
                session['autologin'] = True

                if validSession is None:
                    # if the usermanagement is not enabled yet set the default session members
                    session['username'], session['useruuid'], session['isadmin'], session['language'] = users.GetUserDetails('admin')

                return fn(*args, **kwargs)
            elif session.get('autologin', False):
                return make_response('Forbidden. Not logged in', 401)

            if validSession is None:
                username, useruuid, isadmin, language = ViewFunctionHelper.CheckUserCredentials()
                validSession = useruuid is not None

            if validSession:
                return fn(*args, **kwargs)
            else:
                return make_response('Forbidden. Not logged in', 401)
        return decorator

    @staticmethod
    def RequiredAdmin(fn):
        """
        Decorator to allow only admins.
        """
        @wraps(fn)
        def decorator(*args, **kwargs):
            if session.get('isadmin', False) is True:
                return fn(*args, **kwargs)
            else:
                return make_response('Forbidden. User has no admin rights.', 403)
        return decorator

    @staticmethod
    def Gzipped(f):
        @functools.wraps(f)
        def view_func(*args, **kwargs):
            @after_this_request
            def compress(response):
                if ((not settings.USE_GZIP_COMPRESSION) or
                        ("gzip" not in request.headers.get("Accept-Encoding", "").lower()) or
                        (response.status_code < 200) or
                        (response.status_code >= 300) or
                        ("Content-Encoding" in response.headers)):
                    return response
                response.direct_passthrough = False
                buffer = cStringIO.StringIO()
                tmpf = gzip.GzipFile(mode="wb", fileobj=buffer)
                tmpf.write(response.data)
                tmpf.close()
                response.data = buffer.getvalue()
                response.headers["Content-Encoding"] = "gzip"
                response.headers["Content-Length"] = len(response.data)
                response.headers["Vary"] = "Accept-Encoding"
                return response

            return f(*args, **kwargs)

        return view_func


class ViewFunctionHelper():

    @staticmethod
    def GetUserManagementEnabled():
        service = c4d.modules.net.GetGlobalNetRenderService()
        return service.GetNetPreferences().GetBool(c4d.WPREF_NET_USERMANAGEMENTENABLED)

    @staticmethod
    def CheckUserCredentials():
        """
        Login Endpoint.
        """
        session['username'] = None

        if request.method == 'POST':
            username = request.form['username']
            password = request.form['password']
        else:
            username = request.args.get('username', '')
            password = request.args.get('password', '')

        username, useruuid, isadmin, language = users.CheckUserCredentials(
            username, password)
        if not useruuid:
            return None, None, None, None

        session['username'] = username
        session['useruuid'] = useruuid
        session['isadmin'] = isadmin

        with g_userLock:
            activeUser = app.jinja_env.globals.get('active_users', {})
            activeUser[useruuid] = datetime.datetime.now()
            app.jinja_env.globals['active_users'] = activeUser

        return username, useruuid, isadmin, language


class FlaskView():

    @staticmethod
    @app.route(EndpointHelper.Register("/login"), methods=['POST', 'GET'])
    @EndpointHelper.Gzipped
    @EndpointHelper.ErrorHandling
    def CheckLogin():
        # NOTE: use POST especially on special chars
        username, useruuid, isadmin, language = ViewFunctionHelper.CheckUserCredentials()
        if not useruuid:
            return make_response('Forbidden. Not logged in', 401)
        # return also language labels to limit calls to server
        languages = g_languagePool.GetLanguage(language)
        session['autologin'] = False
        return jsonify(login=True, username=username, useruuid=useruuid, isadmin=isadmin, language=language, labels=languages["elements"])

    @staticmethod
    @app.route('/', defaults={'path': ''})
    @app.route('/<path:path>')
    @EndpointHelper.Gzipped
    @EndpointHelper.ErrorHandling
    def index(path):
        """
        loading the static files
        """
        # offer render argument to render file, especially for speedups
        render = bool(request.args.get('render', 1, type=int))

        if path == '':
            path = "index.html"

        if render and path.endswith('html'):
            return render_template(path, usermgt_enabled=ViewFunctionHelper.GetUserManagementEnabled())
        else:
            # http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.3.5
            return send_from_directory(DIRECTORY_WEBSERVERHTDOCS, path, conditional=True)

    @staticmethod
    @app.route(EndpointHelper.Register("/logout"), methods=['GET'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def Logout():
        """
        Logs the current user out.
        """

        useruuid = session['useruuid']
        with g_userLock:
            activeUser = app.jinja_env.globals.get('active_users', {})
            if useruuid in activeUser:
                del activeUser[useruuid]

        session.clear()

        return jsonify(logout=True)

    @staticmethod
    @app.route(EndpointHelper.Register("/network"))
    @EndpointHelper.Gzipped
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def GetMachines():
        """
        Retrieves the current available machines as JSON.
        """
        return jsonify(network=network.GetMachines(logCount=1000))

    @staticmethod
    @app.route(EndpointHelper.Register("/network/restart"))
    @EndpointHelper.RequireLogin
    @EndpointHelper.RequiredAdmin
    @EndpointHelper.ErrorHandling
    def NetworkRestartServer():
        """
        Restarts the server
        """
        network.NetworkRestartServer()
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register("/network/restart/clients"))
    @EndpointHelper.RequireLogin
    @EndpointHelper.RequiredAdmin
    @EndpointHelper.ErrorHandling
    def NetworkRestartAllClients():
        """
        Restarts all clients
        """
        network.NetworkRestartAllClients()
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register("/network/<machineId>/restart"))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def NetworkRestartClient(machineId):
        """
        Restarts the specified machine
        """
        network.NetworkRestartClient(uuid.UUID(machineId))
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register("/network/<machineid>/log"))
    @EndpointHelper.RequireLogin
    @EndpointHelper.RequiredAdmin
    @EndpointHelper.ErrorHandling
    def GetMachineLog(machineid):
        logcount = request.args.get('logcount', 0, type=int)

        log, name = network.GetMachineLog(
            logCount=logcount, machineUuid=uuid.UUID(machineid))
        result = make_response(log)
        current = datetime.datetime.now()
        name = name.replace(' ', '_') + '_' + \
            current.strftime('%Y-%m-%d_%H%M%S')
        result.headers['Content-Type'] = 'plain/txt'
        result.headers[
            'Content-Disposition'] = 'attachment; filename=' + name + '.log'
        return result

    @staticmethod
    @app.route(EndpointHelper.Register("/language"))
    @EndpointHelper.Gzipped
    @EndpointHelper.ErrorHandling
    def GetLanguage():
        """
        Retrieves the language string for a specific language as JSON.
        """
        lang_id = request.args.get('id', '')
        language = g_languagePool.GetLanguage(lang_id)

        expires = 60 * 60 * 24  # 1 day

        response = app.response_class(
            language["json_dump"], mimetype='application/json')
        response.set_etag(language["hash_str"])
        response.cache_control.public = True
        response.cache_control.max_age = 30  # seconds
        response.expires = int(time.time() + expires)

        last_modified = language["modified_date"]
        if last_modified is not None:
            response.last_modified = last_modified

        return response.make_conditional(request)

    @staticmethod
    @app.route(EndpointHelper.Register("/languages"))
    @EndpointHelper.ErrorHandling
    def GetLanguages():
        """
        Retrieves the available languages as JSON.
        """
        languages = g_languagePool.GetLanguagesDump()

        expires = 60 * 60 * 24  # 1 day

        response = app.response_class(
            languages["json_dump"], mimetype='application/json')
        response.set_etag(languages["hash_str"])
        response.cache_control.public = True
        response.cache_control.max_age = 30  # seconds
        response.expires = int(time.time() + expires)

        last_modified = languages["modified_date"]
        if last_modified is not None:
            response.last_modified = last_modified

        return response.make_conditional(request)

    @staticmethod
    @app.route(EndpointHelper.Register("/monitor"))
    @EndpointHelper.Gzipped
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def Monitor():
        """
        Retrieves the jobs for the current user as JSON.
        """

        rdata = request.args.get('rdata', True, type=int)
        assetscache = request.args.get('assetscache', False, type=int)
        resultassetscache = request.args.get('resultassetscache', False, type=int)

        settings = c4d.BaseContainer()
        settings.SetBool(1, assetscache)
        settings.SetBool(2, resultassetscache)

        result = {}
        result['jobs'] = jobs.GetJobsOrJob(session['useruuid'], rdata=rdata, assets=False,
                                           results=False, log=False, settings=settings)
        result['network'] = network.GetMachines(logCount=0)
        return jsonify(result)

    @staticmethod
    @app.route(EndpointHelper.Register("/jobs"))
    @EndpointHelper.Gzipped
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def GetJobsOrJob():
        """
        Retrieves the jobs for the current user as JSON.
        """
        rdata = request.args.get('rdata', True, type=int)
        assets = request.args.get('assets', True, type=int)
        results = request.args.get('results', True, type=int)
        log = request.args.get('log', True, type=int)
        jobid = request.args.get('jobid', None)
        rgroups = request.args.get('rgroups', True, type=int)
        assetscache = request.args.get('assetscache', False, type=int)
        resultassetscache = request.args.get(
            'resultassetscache', False, type=int)
        selectedjobonly = request.args.get('selectedjobonly', False, type=int)

        settings = c4d.BaseContainer()
        settings.SetBool(1, assetscache)
        settings.SetBool(2, resultassetscache)

        result = {}
        if jobid:
            ret = jobs.GetJobsOrJob(
                session['useruuid'], rdata=rdata, assets=assets,
                results=results, log=log, selectedJob=uuid.UUID(jobid), selectedJobOnly=selectedjobonly,
                settings=settings)
            if selectedjobonly:
                result['jobs'] = (ret, )
            else:
                result['jobs'] = ret
        else:
            result[
                'jobs'] = jobs.GetJobsOrJob(session['useruuid'], rdata=rdata, assets=assets,
                                            results=results, log=log, settings=settings)

        if rgroups:
            result['rgroups'] = g_rdataDesc.GetRdataGroups()

        return jsonify(result)

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/start'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def StartJob(jobid):
        """
        Starts the specified job.
        """
        return jsonify(result=jobs.StartJob(uuid.UUID(jobid)))

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/clear'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def ClearResults(jobid):
        """
        Clears the current job.
        """
        jobs.ClearResults(uuid.UUID(jobid), session['useruuid'])
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/stop'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def StopJob(jobid):
        """
        Stops the current job.
        """
        jobs.StopJob(uuid.UUID(jobid))
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/setdefaultscenename'), methods=['POST'])
    @EndpointHelper.ErrorHandling
    def SetDefaultSceneName(jobid):
        filename = request.form.get('filename', None)
        if not filename:
            raise AttributeError("Missing scene name")

        filename = urllib.unquote(filename)

        jobs.SetDefaultSceneName(uuid.UUID(jobid), filename)
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/move'))
    def MoveRenderJob(jobid):
        insertafter = request.args.get('insertafter', None)
        if insertafter:
            insertafter = uuid.UUID(insertafter)

        insertbefore = request.args.get('insertbefore', None)
        if insertbefore:
            insertbefore = uuid.UUID(insertbefore)

        jobs.MoveJob(
            uuid.UUID(jobid), insertafter=insertafter, insertbefore=insertbefore)
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/create'), methods=['POST'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def CreateRenderJob():
        """
        Creates a new job in the watchfolder of the current user.
        """
        jobname = request.form.get('jobname', None)
        if not jobname:
            raise AttributeError("Missing jobname")

        if not re.match("^[a-zA-Z0-9_. -]+$", jobname):
            raise ValueError(
                "Only the following chars are allowed: 'A-Z', 'a-z', '0-9', '.', '_', '-' and ' '")

        # OPTIONAL: Set True to return the job after its created
        getjobs = request.form.get('getjobs', False)

        jobs.CreateRenderJob(session['username'], jobname)
        if getjobs:
            for job in jobs.GetJobsOrJob(currentUserUuid=session['useruuid'], rdata=False, assets=False, results=False, log=False):
                if job.GetString(c4d.JOB_NAME) == jobname:
                    return jsonify(job=job)

            raise IOError("Could not find job")

        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/delete'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def DeleteRenderJob(jobid):
        """
        Creates a new job in the watchfolder of the current user.
        """
        jobs.DeleteRenderJob(uuid.UUID(jobid))
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/asset/add'), methods=['POST'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def AddAsset(jobid):
        """
        Adds a file to the provided job.
        """
        uploaded = request.files['file']
        if not uploaded:
            raise AttributeError("No files found in the attachments")

        filename, size = jobs.AddAsset(
            uuid.UUID(jobid), session['useruuid'], uploaded)
        return jsonify(name=filename, size=size)

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/asset/delete'), methods=['POST'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def DeleteAsset(jobid):
        """
        Removes a file from the provided job.
        """
        filename = request.form.get('filename', None)
        if not filename:
            raise Exception("argument filename is missing")
        filename = urllib.unquote(filename)

        return jsonify(result=jobs.DeleteAsset(uuid.UUID(jobid), filename, session['useruuid']))

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/result/delete'), methods=['POST'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def DeleteResultAsset(jobid):
        """
        Removes a file from the provided job.
        """
        filename = request.form.get('filename', None)
        if not filename:
            raise Exception("argument filename is missing")
        filename = urllib.unquote(filename)

        jobs.DeleteResultAsset(uuid.UUID(jobid), filename, session['useruuid'])
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/result/get/<filename>'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def GetResultAsset(jobid, filename):
        """
        Gets a result file from the provided job.
        """
        returnjpg = bool(request.args.get('returnjpg', 0, type=int))

        resultAssetName = urllib.unquote(filename).encode("utf8")

        resultAssetPath = jobs.GetResultAsset(
            uuid.UUID(jobid), resultAssetName, session['useruuid'], download=not returnjpg)

        resultAssetModificationTime = os.path.getmtime(resultAssetPath)

        headers = Headers()
        if not returnjpg:
            headers.add(
                'Content-Disposition', 'attachment', filename=resultAssetName)

        if not os.path.exists(resultAssetPath):
            return make_response('Could not find requested file.', 404)

        if resultAssetPath.endswith("jpeg") or resultAssetPath.endswith("jpg") or not returnjpg:
            fp = open(resultAssetPath, "rb")
            data = wrap_file(request.environ, fp)
            headers['Content-Length'] = os.path.getsize(resultAssetPath)
            mimetype, encoding = mimetypes.guess_type(resultAssetName)
            if mimetype is None:
                mimetype = "application/octet-stream"
        else:
            bmp = c4d.bitmaps.BaseBitmap()
            result, ismovie = bmp.InitWith(resultAssetPath.encode("utf-8"))
            if result != c4d.IMAGERESULT_OK:
                return make_response('Could not load requested file (%i).' % result, 404)

            data = StringIO.StringIO()
            bmpJpg = util.BaseBitmap2Jpg(bmp)
            data.write(bmpJpg)
            data.seek(0)
            headers['Content-Length'] = len(bmpJpg)
            mimetype = "image/jpeg"

        rv = app.response_class(
            data, mimetype=mimetype, headers=headers,
            direct_passthrough=True)

        # if we know the file modification date, we can store it as the
        # the time of the last modification.
        if resultAssetModificationTime is not None:
            rv.last_modified = int(resultAssetModificationTime)

        rv.cache_control.public = True
        return rv

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/result'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def GetZipResults(jobid):
        """
        Zips all results and send them back.
        """
        zipPath = jobs.GetZipResults(uuid.UUID(jobid), session['useruuid'])
        zipDirectory, zipFile = os.path.split(zipPath)
        return send_from_directory(zipDirectory, zipFile, as_attachment=True)

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/result/zip'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def DoZipResults(jobid):
        jobs.DoZipResults(uuid.UUID(jobid), session['useruuid'])
        return jsonify(result=True)

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/result/zip/progress'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def GetZipProgress(jobid):
        progressValue = jobs.GetZipProgress(
            uuid.UUID(jobid), session['useruuid'])
        return jsonify(progress=progressValue)

    @staticmethod
    @app.route(EndpointHelper.Register('/info'))
    @EndpointHelper.Gzipped
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def GetServerInfo():

        def StripPath(path):
            """
            Return volume path of OSX
            """
            os = c4d.GeGetCurrentOS()
            if os == c4d.OPERATINGSYSTEM_OSX:
                volumePrefix = "/Volumes"
                if path.startswith(volumePrefix):
                    volumePrefixLen = len(volumePrefix)
                    return path[: path[volumePrefixLen + 1:].find("/") + volumePrefixLen + 1]
                else:
                    return path[0]
            elif os == c4d.OPERATINGSYSTEM_WIN:
                return path[0]
            else:
                return path

        service = c4d.modules.net.GetGlobalNetRenderService()
        path = service.GetRepository().GetRepositoryPath()
        name = service.GetName()

        mem = c4d.storage.GeGetMemoryStat()

        info = c4d.storage.GetFreeVolumeSpace(path)
        availableBytes, totalBytes = info if info else [0, 0]

        applicationuptime = (int(time.time()) - g_ApplicationUptime) * 1000.0

        if Bytes(availableBytes) < Gigabytes(2):
            usage = 'critical'
        elif Bytes(availableBytes) < Gigabytes(3):
            usage = 'warning'
        else:
            usage = 'normal'

        drive = {"drive": StripPath(path), "availableBytes":
                 availableBytes, "totalBytes": totalBytes, "status": usage}
        memory = {"totalBytes": g_systemInfo._cacheAvailableMemory,
                  "applicationTotalBytes": mem.GetInt64(c4d.C4D_MEMORY_STAT_MEMORY_INUSE)}

        return jsonify(servicename=service.GetName(), drive=drive, name=name, memory=memory, applicationUptime=applicationuptime, version=g_systemInfo._cacheVersion, os=g_systemInfo._cacheOsVersionString, cpucount=g_systemInfo._cacheCpuCount, cpuspeed=g_systemInfo._cacheCpuSpeed)

    @staticmethod
    @app.route(EndpointHelper.Register('/jobs/<jobid>/documentpreview'))
    @EndpointHelper.RequireLogin
    def GetDocumentPreview(jobid):
        try:
            job = jobs.GetJobsOrJob(
                session['useruuid'], rdata=0, assets=0, results=0, log=0, selectedJob=uuid.UUID(jobid), selectedJobOnly=True)
            if not job:
                raise AttributeError("Job could not be found")

            jobDirectory = job.GetFilename(c4d.JOB_DIRECTORY).decode("utf-8")
            defaultSceneName = job.GetString(
                c4d.JOB_DEFAULTSCENENAME).decode("utf-8")
            defaultScenePath = os.path.join(jobDirectory, defaultSceneName)
            if defaultSceneName == "" or not os.path.exists(defaultScenePath):
                def GetDefaultScenePath(jobDirectory):
                    for root, dirs, files in os.walk(jobDirectory):
                        for name in files:
                            if name.endswith(u"c4d") and not name.startswith("."):
                                return os.path.join(root, name)
                    return None

                defaultScenePath = GetDefaultScenePath(jobDirectory)
                if not defaultScenePath:
                    raise LookupError("No document preview available")

            # dont load any objects or materials, we just need the preview
            # picture
            doc = c4d.documents.LoadDocument(
                defaultScenePath.encode("utf-8"), c4d.SCENEFILTER_0 | c4d.SCENEFILTER_IGNOREXREFS)
            if not doc:
                raise IOError("Job could not be read")

            bmp = doc.GetDocPreviewBitmap()
            strIO = StringIO.StringIO()
            strIO.write(util.BaseBitmap2Jpg(bmp))
            strIO.seek(0)
            return send_file(strIO, attachment_filename="preview.jpg", as_attachment=False, add_etags=False)
        except Exception:
            response = jsonify()
            response.status_code = 500
            return response

    @staticmethod
    @app.route(EndpointHelper.Register('/users'))
    @EndpointHelper.Gzipped
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def GetAllUsers():
        """
        Retrieves all availabe users.
        """
        def GetActiveUser():
            with g_userLock:
                activeUser = app.jinja_env.globals.get('active_users', {})
                now = datetime.datetime.now()
                remove_user = []
                for user in activeUser:
                    if now - activeUser[user] > datetime.timedelta(minutes=settings.ACTIVE_USER_TIMEOUT):
                        remove_user.append(user)
                if len(remove_user) > 0:
                    for to_remove in remove_user:
                        del activeUser[to_remove]
                    app.jinja_env.globals.get('active_users', activeUser)

                return activeUser.keys()

        tmpUserlist = users.GetUsers()
        activeUsers = GetActiveUser()
        currentUsername = session['username']

        username, useruuid, isadmin, language = users.GetUserDetails(
            currentUsername)

        userList = []
        for u in tmpUserlist:
            if isadmin or (not isadmin and u['username'] == currentUsername):
                userList.append(u)

        for u in userList:
            if u['uuid'] in activeUsers:
                u['logged_in'] = True

        return jsonify(users=userList)

    @staticmethod
    @app.route(EndpointHelper.Register('/ping'))
    @EndpointHelper.ErrorHandling
    def Ping():
        """
        Ping the web-server
        """
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/verify'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def Verify():
        """
        Verifies the users session
        """
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/help'))
    @EndpointHelper.ErrorHandling
    def GetHelp():
        """
        Returns all default possible json commands
        """
        return jsonify(commands=g_endPoints)

    @staticmethod
    @app.route(EndpointHelper.Register('/users/create'), methods=['POST'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.RequiredAdmin
    @EndpointHelper.ErrorHandling
    def AddUser():
        """
        Creates a new user.
        """
        username = request.form.get('username', None)
        password = request.form.get('password', None)
        isadmin = request.form.get('isadmin', None)
        description = request.form.get('description', '')

        if not username:
            raise AttributeError("Missing username")

        if not re.match("^[a-zA-Z0-9_.-]+$", username):
            raise ValueError(
                "Only the following chars are allowed: 'A-Z', 'a-z', '0-9', '.', '_' and '-'")

        if username == "admins":
            raise ValueError("Username is not allowed")

        users.AddUser(username, password, description, isadmin)
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/users/<userid>/delete'))
    @EndpointHelper.RequireLogin
    @EndpointHelper.RequiredAdmin
    @EndpointHelper.ErrorHandling
    def DeleteUser(userid):
        """
        Deletes a user.
        """
        userUuid = uuid.UUID(userid)
        currentUserUuid = session['useruuid']
        if currentUserUuid == userUuid:
            raise AttributeError("You cannot delete yourself.")

        return jsonify(result=users.DeleteUser(userUuid))

    @staticmethod
    @app.route(EndpointHelper.Register('/users/edit'), methods=['POST'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.RequiredAdmin
    @EndpointHelper.ErrorHandling
    def EditUser():
        """
        Editing a user.
        """
        useruuid = request.form.get('useruuid', None)
        newpassword = request.form.get('newpassword', None)
        newdescription = request.form.get('newdescription', None)
        newisadmin = request.form.get('newisadmin', None)

        return jsonify(result=users.EditUser(uuid.UUID(useruuid), newpassword, newdescription, newisadmin))

    @staticmethod
    @app.route(EndpointHelper.Register('/settings/password/verify'), methods=['POST'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def VerifySettingsPassword():
        username = session['username']
        password = request.form['password']

        username = users.CheckUserCredentials(username, password)[0]
        return jsonify(result=bool(username))

    @staticmethod
    @app.route(EndpointHelper.Register('/settings/password/set'), methods=['POST'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def ChangeSettingsPassword():
        """
        Changes the password for the current logged in user.
        """
        userUuid = request.form['useruuid']
        oldPassword = request.form['oldpassword']
        newPassword = request.form['newpassword']

        users.ChangeSettingsPassword(
            uuid.UUID(userUuid), oldPassword, newPassword)
        return jsonify()

    @staticmethod
    @app.route(EndpointHelper.Register('/settings/language/set'), methods=['GET', 'POST'])
    @EndpointHelper.RequireLogin
    @EndpointHelper.ErrorHandling
    def ChangeSettingsLanguage():
        """Changes the language for the current logged in user.
        """
        useruuid = session['useruuid']
        if request.method == 'POST':
            language = request.form['language']
        else:
            language = request.args.get('language', '')

        users.ChangeSettingsLanguage(useruuid, language)
        # return labels immediately to get rid of additional request.
        languages = g_languagePool.GetLanguage(language)
        return jsonify(labels=languages["elements"])


class FlaskThread(c4dthreading.C4DThread):

    __lastErrorMessage = ""

    def GetLastErrorMessage(self):
        return self.__lastErrorMessage

    __errorCallback = None

    def __init__(self, app):
        self._app = app
        app.secret_key = os.urandom(24)
        app.debug = DEBUG

        # make the c4d module available in jinja
        app.jinja_env.globals.update(c4d=c4d)

    def SetErrorCallback(self, cb):
        self.__errorCallback = cb

    def GetThreadName(self):
        return "TR Webserver"

    def Main(self):
        c4d.GePluginMessage(c4d.C4DPL_WEBSERVER_START, None)

        service = c4d.modules.net.GetGlobalNetRenderService()
        port = service.GetNetPreferences().GetInt32(
            c4d.WPREF_NET_WEBSERVERPORT, 8080)

        try:
            self.__lastErrorMessage = ""
            c4d.StatusSetNetLoad(c4d.STATUSNETSTATE_IDLE)
            self._app.Run(self, host='0.0.0.0', port=port)
        except socket.error as e:
            self.__lastErrorMessage = "ERROR {0}: Could not open Webserver on port '{1}'\n    Reason: {2}".format(
                e[0], port, e[1])
            print("  \n" + self.__lastErrorMessage + "\n ")
            if self.__errorCallback:
                self.__errorCallback(self.__lastErrorMessage)
            c4d.GePluginMessage(c4d.C4DPL_WEBSERVER_ERROR, None)
        except Exception as e:
            self.__lastErrorMessage = "ERROR: Could not open Webserver on port '{0}'\n    Reason: {1}".format(
                port, str(e))
            if DEBUG:
                traceback.print_exc(file=sys.stdout)
            print("  \n" + self.__lastErrorMessage + "\n ")
            if self.__errorCallback:
                self.__errorCallback(self.__lastErrorMessage)
            c4d.GePluginMessage(c4d.C4DPL_WEBSERVER_ERROR, None)
        else:
            c4d.GePluginMessage(C4DPL_WEBSERVER_STOP, None)


def RestartWebserver(showError):
    global g_webserver
    if g_webserver:
        if showError:
            g_webserver.SetErrorCallback(lambda message: c4d.gui.MessageDialog(
                message, c4d.GEMB_OK | c4d.GEMB_FORCEDIALOG))
        g_webserver.End(wait=True)
        g_webserver.Start(c4d.THREADMODE_ASYNC, c4d.THREADPRIORITY_ABOVE)


def CreateAndStartWebserver(app):
    global g_webserver
    g_webserver = FlaskThread(app)
    g_webserver.Start(c4d.THREADMODE_ASYNC, c4d.THREADPRIORITY_ABOVE)


def CloseAndDeleteWebserver():
    global g_webserver
    if g_webserver:
        g_webserver.End(wait=True)
    g_webserver = None


if __name__ == '__main__':

    def CleanupZipResultDirectory():
        """
        Checks all zip directories for orphaned zip and progress files and deletes them
        """

        service = c4d.modules.net.GetGlobalNetRenderService()
        jobs = service.GetJobsList(
            triggerWatchDog=False, rdata=False, assets=False, results=False, log=False)
        for job in jobs:
            path = job.GetFilename(c4d.JOB_DIRECTORY).decode("utf-8")
            zipDirectoryPath = os.path.join(
                path, settings.RESULTDIRECTORYNAME, settings.RESULTZIPFOLDER)
            try:
                # If the zip directory contains a progress file the zip file is
                # corrupt
                progressFileDeleted = False
                for name in os.listdir(zipDirectoryPath):
                    if name.lower() == 'progress':
                        os.remove(os.path.join(zipDirectoryPath, name))
                        progressFileDeleted = True
                        break

                if progressFileDeleted:
                    for name in os.listdir(zipDirectoryPath):
                        if name.lower().endswith(".zip"):
                            zipFilePath = os.path.join(zipDirectoryPath, name)
                            print(
                                "Found possibly corrupted result zip file: %s" % zipFilePath)
            except:
                pass

    def RegisterWebserverModule():
        # Make the Flask Server available via a dynamic module which will be
        # c4d.modules.net.webserver
        webserver = imp.new_module(__name__)
        webserver.__dict__.update(globals())
        c4d.modules.net.webserver = webserver

    CleanupZipResultDirectory()
    CreateAndStartWebserver(app)
    RegisterWebserverModule()


def OpenBrowser():
    service = c4d.modules.net.GetGlobalNetRenderService()
    if service and g_webserver:
        msg = g_webserver.GetLastErrorMessage()
        if not msg:
            prefs = service.GetNetPreferences()
            webbrowser.open("http://127.0.0.1:%i" %
                            prefs.GetInt32(c4d.WPREF_NET_WEBSERVERPORT, 8080))
        else:
            c4d.gui.MessageDialog(msg, c4d.GEMB_OK | c4d.GEMB_FORCEDIALOG)


def PluginMessage(id, data):
    if id == C4DPL_WEBSERVER_SHUTDOWN:
        CloseAndDeleteWebserver()
    elif id == c4d.C4DPL_WEBSERVER_PORTCHANGED:
        RestartWebserver(True)
    elif id == C4DPL_WEBSERVER_OPENWEBINTERFACE:
        OpenBrowser()
    elif id == C4DPL_WEBSERVER_CREATEANDSTART:
        CreateAndStartWebserver(app)
    return True
