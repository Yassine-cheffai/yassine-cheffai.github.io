import c4d


def RestartMachine(service, machineUuid):
    c4d.modules.net.NetSpecialEventAdd(service, machineUuid, c4d.BaseContainer(c4d.MSG_NETRENDER_RESTART))


def GetMachines(logCount, machineUuid=None):
    result = None
    service = c4d.modules.net.GetGlobalNetRenderService()
    if service:
        if machineUuid:
            result = service.GetMachinesList(logCount, machineUuid)
            if result:
                result = result[0]
        else:
            result = service.GetMachinesList(logCount)
    return result


def NetworkRestartServer():
    """
    Restarts the server
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    serviceUuid = service.GetUuid()
    RestartMachine(service, serviceUuid)


def NetworkRestartAllClients():
    """
    Restarts all clients
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    serviceUuid = service.GetUuid()
    for machine in GetMachines(logCount=0):
        machineUuid = machine.GetUuid(c4d.MACHINE_UUID)
        if machineUuid != serviceUuid:
            RestartMachine(service, machineUuid)


def NetworkRestartClient(machineUuid):
    """Restarts the specified machine
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    RestartMachine(service, machineUuid)


def GetMachineLog(logCount, machineUuid):
    machine = GetMachines(logCount, machineUuid)
    if not machine:
        raise LookupError("Could not find machine")

    if machine.GetType(c4d.MACHINE_CURRENTLOG) != c4d.DA_CONTAINER:
        raise KeyError("Could not find current logs")

    current_logs = ""
    for index, line in machine.GetContainerInstance(c4d.MACHINE_CURRENTLOG):
        current_logs += line + '\r\n'

    return current_logs, machine.GetString(c4d.MACHINE_NAME)
