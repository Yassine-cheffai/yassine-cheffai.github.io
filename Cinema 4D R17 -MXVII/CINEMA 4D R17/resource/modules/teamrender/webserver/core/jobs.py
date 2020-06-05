import os
import zipfile
import shutil
import thread
import c4d
import time
import contextlib
from flask import session
from settings import RESULTDIRECTORYNAME, RESULTZIPFOLDER, PROGRESSFILE
# TODO: (seb) add thread to a list to shut them down on webserver down


def GetJobsOrJob(currentUserUuid, rdata, assets, results, log, selectedJob=None, selectedJobOnly=False, settings=c4d.BaseContainer()):
    service = c4d.modules.net.GetGlobalNetRenderService()
    if selectedJob:
        jobs = service.GetJobsList(
            triggerWatchDog=True, rdata=rdata, assets=assets, results=results,
            log=log, selectedJob=selectedJob, selectedJobOnly=selectedJobOnly, user=currentUserUuid, settings=settings)
        if selectedJobOnly and jobs and len(jobs) > 0:
            return jobs[0]
        else:
            return jobs
    else:
        jobs = service.GetJobsList(
            triggerWatchDog=True, rdata=rdata, assets=assets, results=results, log=log, user=currentUserUuid, settings=settings)
        if not jobs:
            jobs = []
        return jobs


def GetAssetPath(jobUuid, filename, currentUserUuid, lookInResults=False):
    job = GetJobsOrJob(
        currentUserUuid=currentUserUuid, rdata=False, assets=not lookInResults,
        results=lookInResults, log=False, selectedJob=jobUuid, selectedJobOnly=True)
    if not job:
        raise Exception("Job not found")
    if not lookInResults:
        if job.GetType(c4d.JOB_ASSET) != c4d.DA_CONTAINER:
            raise Exception("Job container has no asset information")
        for index, asset in job.GetContainerInstance(c4d.JOB_ASSET):
            relPath = asset.GetFilename(
                c4d.JOB_ASSET_RELDIRECTORYPATH).decode("utf-8")
            if relPath == filename:
                return asset.GetFilename(c4d.JOB_ASSET_DIRECTORYPATH).decode("utf-8")
    else:
        if job.GetType(c4d.JOB_RESULTASSET) != c4d.DA_CONTAINER:
            raise Exception("Job container has no result asset information")
        for index, asset in job.GetContainerInstance(c4d.JOB_RESULTASSET):
            if asset.GetString(c4d.JOB_RESULTASSET_NAME).decode("utf-8") == filename:
                return asset.GetFilename(c4d.JOB_RESULTASSET_DIRECTORYPATH).decode("utf-8")
    raise Exception("Asset with name %s not found" % (filename))


def GetResultAssetPath(jobUuid, filename, currentUserUuid):
    return GetAssetPath(jobUuid, filename, currentUserUuid, True)


def GetJobPathFromUuid(jobUuid, currentUserUuid):
    """
    Retrieves the path to a job with the current user using the uuid of the job.
    """
    job = GetJobsOrJob(currentUserUuid, rdata=False,
                       assets=False, results=False, log=False, selectedJob=jobUuid, selectedJobOnly=True)
    return job.GetFilename(c4d.JOB_DIRECTORY).decode("utf-8")


def StartJob(jobUuid):
    """
    Starts the specified job.
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    service.InitAndStartRenderingFullAsync(jobUuid)
    service.AddLogToJob(
        jobUuid, "Started Job by User {0}".format(session['username']), True)


def StopJob(jobUuid):
    """
    Stops the current job.
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    service.StopRendering(
        c4d.THREADMODE_ASYNC, jobUuid, c4d.RENDERRESULT_USERBREAK)


def ClearResults(jobUuid, currentUserUuid):
    """
    Clears the results from a job
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    if not service.ClearResults(jobUuid):
        raise IOError("Could not clean job")
    os.utime(GetJobPathFromUuid(jobUuid, currentUserUuid), None)
    service.AddLogToJob(
        jobUuid, "Cleared All Results by '{0}'".format(session['username']), True)


def SetDefaultSceneName(jobUuid, filename):
    service = c4d.modules.net.GetGlobalNetRenderService()
    if not service.SetDefaultSceneName(jobUuid, filename):
        raise AttributeError("Could not set default scene name")
    service.AddLogToJob(jobUuid, "Set '{0}' as Master by '{1}'".format(
        filename.encode("utf-8"), session['username']), True)


def MoveJob(jobUuid, insertafter=False, insertbefore=False):
    service = c4d.modules.net.GetGlobalNetRenderService()
    if insertafter and insertbefore:
        raise AttributeError("insertafter and insertbefore is set")
    elif insertafter is None and insertbefore is None:
        raise AttributeError("insertafter and insertbefore is None")
    else:
        if insertafter:
            service.InsertJobAfter(jobUuid, insertafter)
        else:
            service.InsertJobBefore(jobUuid, insertbefore)


def CreateRenderJob(username, jobname):
    """
	Creates a new job in the watchfolder of the current user.
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    if not service:
        raise TypeError("No service")
    path = os.path.join(
        service.GetRepository().GetRepositoryPath().decode("utf-8"), "users", username, jobname)

    if os.path.exists(path):
        raise IOError("Job %s already exists" % (path,))

    os.mkdir(path)
    return


def DeleteRenderJob(jobUuid):
    """
    Deletes the job on the server.
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    currentUserUuid = session['useruuid']
    job = GetJobsOrJob(currentUserUuid, rdata=False, assets=False,
                       results=False, log=False, selectedJob=jobUuid, selectedJobOnly=True)
    if not job:
        raise LookupError("Could not find job")
    if job.GetInt32(c4d.JOB_ID, -1) >= 0:
        raise RuntimeError("Job is still running and cannot be deleted")
    jobPath = job.GetFilename(c4d.JOB_DIRECTORY)
    if not os.path.exists(jobPath):
        raise IOError("Job directory does not exist")
    repositoryPath = service.GetRepository().GetRepositoryPath()
    if not jobPath.startswith(repositoryPath):
        return IOError("Path does not match the repository root directory")

    # first rename the directory to take it out from the actions of a concurrent
    # watchdog
    a, b = os.path.split(jobPath)
    oldJobPath = jobPath
    jobPath = os.path.join(a, "." + b)

    # Many attempts to rename the job. This ensures that a temporary and short-term filehandle
    # does not prevent deleting the job
    moved = False
    lastErrorMessage = None
    for x in xrange(0, 20):
        try:
            os.rename(oldJobPath, jobPath)
        except (OSError, IOError), e:
            time.sleep(0.01)
            lastErrorMessage = e.strerror
            continue
        except Exception, e:
            time.sleep(0.01)
            continue
        else:
            moved = True
            break
    if not moved:
        strerror = "Job could not be deleted"
        if lastErrorMessage:
            strerror += ": " + lastErrorMessage
        raise IOError(strerror)

    # Many attempts to delete the elements in a directory. This ensures that a temporary and short-term filehandle
    # does not prevent deleting the job
    for x in xrange(0, 20):
        try:
            for root, dirs, files in os.walk(jobPath):
                for name in files:
                    # First delete all files, except the job.ini. That ensures that while the job
                    # directory is deleted a concurrent watchdog does not
                    # re-create it
                    if name != "job.ini":
                        os.remove(os.path.join(root, name))
                for name in dirs:
                    shutil.rmtree(
                        os.path.join(root, name), ignore_errors=False)
            # delete entire directory now with the job.ini file
            shutil.rmtree(jobPath, ignore_errors=False)
        except:
            time.sleep(0.01)
            continue
        else:
            break
    # After a job is deleted, trigger the watch dog manually
    service.GetJobsList(
        triggerWatchDog=True, rdata=c4d.DETAILSELECTOR_NONE, assets=c4d.DETAILSELECTOR_NONE, results=c4d.DETAILSELECTOR_NONE,
        log=c4d.DETAILSELECTOR_NONE, user=currentUserUuid)


def AddAsset(jobUuid, currentUserUuid, uploaded):
    """
    Adds a file to the provided job.
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    filename = uploaded.filename
    path = GetJobPathFromUuid(jobUuid, currentUserUuid)
    if not os.path.exists(path):
        raise IOError("Job directory does not exist")
    assetPath = os.path.join(path, filename)
    if os.path.exists(assetPath):
        raise IOError("File %s already exists" % (filename.encode("utf-8")))
    uploaded.save(assetPath)
    # Handle ZIP File ...
    if zipfile.is_zipfile(assetPath):
        with contextlib.closing(zipfile.ZipFile(assetPath, allowZip64=True)) as zf:
            for member in zf.infolist():
                # if a file failed to extract, extract the other ones
                try:
                    # ignore the default mac and ds_store temp files
                    if not member.filename.startswith("__MACOSX") and not member.filename.endswith(".DS_Store"):
                        zf.extract(member, path.encode("utf-8"))
                        service.AddLogToJob(
                            jobUuid, "Extracted Asset '{0}' from '{1}'".format(member.filename, filename), True)
                except:
                    pass

        # the zip-file must always be deleted
        os.remove(assetPath)
        return "", 0
    # Or normal file ...
    else:
        identity = c4d.IDENTIFYFILE_0
        with open(assetPath) as file:
            probe = file.read(1024)
            identity = c4d.storage.GeIdentifyFile(
                filename.encode("utf-8"), probe, c4d.IDENTIFYFILE_IMAGE)[0]
        if identity == c4d.IDENTIFYFILE_IMAGE:
            texPath = os.path.join(path, 'tex')
            if not os.path.exists(texPath):
                try:
                    os.mkdir(texPath)
                except:
                    # this might occur (rarely), if the directory is already being
                    # created by another upload-thread
                    pass
            newAssetPath = os.path.join(texPath, filename)
            shutil.move(assetPath, newAssetPath)
            assetPath = newAssetPath
    service.AddLogToJob(
        jobUuid, "Added Asset '{0}' by User {1}".format(filename.encode("utf-8"), session['username']), True)
    return filename, os.stat(assetPath).st_size


def DeleteAsset(jobUuid, filename, currentUserUuid):
    """
    Removes a file from the provided job.
    """
    job = GetJobsOrJob(currentUserUuid=currentUserUuid, rdata=0, assets=0,
                       results=0, log=0, selectedJob=jobUuid, selectedJobOnly=True)
    if job.GetInt32(c4d.JOB_ID, -1) >= 0:
        raise RuntimeError("Asset cannot be deleted because job is running")
    assetPath = GetAssetPath(jobUuid, filename, currentUserUuid)
    if not os.path.exists(assetPath):
        raise IOError("File %s does not exist" % filename.encode("utf-8"))
    os.remove(assetPath)
    os.utime(os.path.dirname(assetPath), None)
    service = c4d.modules.net.GetGlobalNetRenderService()
    service.AddLogToJob(jobUuid, "Deleted Asset '{0}' by User {1}".format(
        filename.encode("utf-8"), session['username']), True)


def DeleteResultAsset(jobUuid, filename, currentUserUuid):
    """
    Removes a file from the provided job.
    """
    resultAssetPath = GetResultAssetPath(jobUuid, filename, currentUserUuid)
    if not os.path.exists(resultAssetPath):
        raise IOError("File %s does not exist" % filename.encode("utf-8"))
    os.remove(resultAssetPath)

    job = GetJobsOrJob(currentUserUuid=currentUserUuid, rdata=0, assets=0,
                       results=0, log=0, selectedJob=jobUuid, selectedJobOnly=True)
    if not job:
        raise IOError("Could not find job")
    directory = job.GetFilename(c4d.JOB_DIRECTORY).decode("utf-8")

    os.utime(os.path.join(directory, RESULTDIRECTORYNAME), None)
    service = c4d.modules.net.GetGlobalNetRenderService()
    service.AddLogToJob(jobUuid, "Deleted Result Asset '{0}' by User {1}".format(
        filename.encode("utf-8"), session['username']), True)


def GetResultAsset(jobUuid, filename, currentUserUuid, download):
    """
    Gets a result asset file.
    """
    resultAssetDir = GetResultAssetPath(jobUuid, filename, currentUserUuid)
    service = c4d.modules.net.GetGlobalNetRenderService()

    service.AddLogToJob(jobUuid, "User {0} {1} result asset '{2}'".format(
        session['username'], "downloaded" if download else "viewed", filename), True)

    if os.path.exists(resultAssetDir):
        return resultAssetDir
    raise IOError(
        "Could not find result asset '{0}'".format(filename))


def GetZipResults(jobUuid, currentUserUuid):
    """
    Zips a directory and returns it
    """
    resultpath = os.path.join(
        GetJobPathFromUuid(jobUuid, currentUserUuid), RESULTDIRECTORYNAME, RESULTZIPFOLDER)
    resultzip = None
    for path in os.listdir(resultpath):
        if path.endswith('.zip'):
            resultzip = path
            break
    if not resultzip:
        raise IOError("No zip file found in result zip directory")
    resultZipPath = os.path.join(
        path, RESULTDIRECTORYNAME, RESULTZIPFOLDER, resultpath)
    if not os.path.exists(resultZipPath):
        raise IOError("Zip file invalid")
    return resultZipPath


def DoZipResults(jobUuid, currentUserUuid):
    def ZipResults(directory, targetpath, progressfile='progress'):
        progresspath = os.path.join(directory, 'zip', progressfile)
        
        job = GetJobsOrJob(
            currentUserUuid=currentUserUuid, rdata=False, assets=False,
            results=True, log=False, selectedJob=jobUuid, selectedJobOnly=True)
            
        if not job:
            raise IOError("Could not find job")
        
        filepaths = []
        
        for index, asset in job.GetContainerInstance(c4d.JOB_RESULTASSET):
            filepaths.append(asset.GetFilename(c4d.JOB_RESULTASSET_DIRECTORYPATH).decode("utf-8"))
            
        try:
            with open(progresspath, 'w+') as progress:
                progress.write(str(0))


            with contextlib.closing(zipfile.ZipFile(os.path.join(directory, targetpath), 'w', allowZip64=True, compression=zipfile.ZIP_DEFLATED)) as resultzip:
                length = len(filepaths)
                count = 0
                for filepath in filepaths:
                    if filepath.endswith('zip'):
                        continue

                    resultzip.write(filepath, os.path.relpath(filepath, directory))
                    count += 1
                    percentage = int(count * 100 / length)
                    with open(progresspath, 'w') as progress:
                        progress.write(str(percentage))
        except:
            raise

        try:
            # wait 2 seconds to garantuee the directory time is really different to the time
            # from the result asset cache so an update is recognized
            time.sleep(2)
            os.utime(directory, None)
        except:
            pass

        try:
            os.remove(progresspath)
        except:
            pass

        service = c4d.modules.net.GetGlobalNetRenderService()
        service.AddLogToJob(jobUuid, "Zip Results Finished", True)
        return

    job = GetJobsOrJob(currentUserUuid=currentUserUuid, rdata=0, assets=0,
                       results=0, log=0, selectedJob=jobUuid, selectedJobOnly=True)
    if not job:
        raise IOError("Could not find job")
    directory = job.GetFilename(c4d.JOB_DIRECTORY).decode("utf-8")
    name = job.GetString(c4d.JOB_NAME).decode("utf-8")
    progress = int(job.GetFloat(c4d.JOB_PROGRESS) * 100)
    path = os.path.join(directory, RESULTDIRECTORYNAME, RESULTZIPFOLDER)
    if os.path.exists(path):
        shutil.rmtree(path)
    if not os.path.exists(path):
        os.mkdir(path)
    zipName = '%s_%s_percent.zip' % (name, progress)
    zipPath = os.path.join(path, zipName)
    thread.start_new_thread(
        ZipResults, (os.path.join(directory, RESULTDIRECTORYNAME), zipPath))
    return True


def GetZipProgress(jobUuid, currentUserUuid):
    path = os.path.join(
        GetJobPathFromUuid(jobUuid, currentUserUuid), RESULTDIRECTORYNAME, RESULTZIPFOLDER)
    if not os.path.exists(os.path.join(path, PROGRESSFILE)):
        return 0
    with open(os.path.join(path, PROGRESSFILE), 'r') as f:
        return int(f.read())
    raise IOError("Could not read progress")
