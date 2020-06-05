import c4d


def GetUserPool():
    """
    Gets the user pool
    """
    service = c4d.modules.net.GetGlobalNetRenderService()
    if not service:
        raise AttributeError("No global render service found")

    pool = service.GetUserPool()
    if not pool:
        raise AttributeError("No user pool found")

    return pool


def GetUsers():
    pool = GetUserPool()
    if not pool:
        return None

    return pool.GetUsers()


def GetUserDetails(username):
    users = GetUsers()
    for user in users:
        if user['username'].lower() == username.lower():
            return user['username'], user['uuid'], user['isadmin'], user['language']

    return None, None, None, None


def CheckUserCredentials(username, password):
    userPool = GetUserPool()
    if not userPool.CheckUserCredentials(username, password):
        return None, None, None, None

    return GetUserDetails(username)


def AddUser(username, password, description, isadmin):
    """
    Create a new user.
    """
    if not username:
        raise AttributeError("username is missing")

    if not password:
        raise AttributeError("password is missing")

    if isadmin is None:
        raise AttributeError("isadmin info is missing")

    userPool = GetUserPool()
    if not userPool.AddUser(username, password, description, 1 if isadmin.lower() == 'true' else 0):
        raise LookupError("Could not add user")

    return True


def DeleteUser(userUuid):
    """
    Deletes a user.
    """
    pool = GetUserPool()
    if not pool.DeleteUser(userUuid):
        raise LookupError("Could not delete user")

    return True


def EditUser(useruuid, newPassword, newDescription, newIsAdmin):
    """
    Editing a user.
    """
    if not useruuid:
        raise AttributeError("useruuid is missing")

    pool = GetUserPool()

    if newPassword:
        if not pool.EditUserPassword(useruuid, newPassword):
            raise ValueError("Could not edit user password")

    if newDescription:
        if not pool.EditUserInfo(useruuid, newDescription):
            raise ValueError("Could not edit user info")

    if newIsAdmin:
        if not pool.ChangeUserAccountType(useruuid, 1 if newIsAdmin.lower() == 'true' else 0):
            raise ValueError("Could not edit user account type")

    return True


def ChangeSettingsPassword(useruuid, oldpassword, newpassword):
    """
    Changes the password for the current logged in user.
    """

    if not useruuid:
        raise AttributeError("useruuid is not set")

    if not oldpassword:
        raise AttributeError("oldpassword is not set")

    if not newpassword:
        raise AttributeError("newpassword is not set")

    pool = GetUserPool()
    if not pool.ChangePassword(useruuid, oldpassword, newpassword):
        raise ValueError("Could not set the password")

    return


def ChangeSettingsLanguage(useruuid, language):
    """
    Changes the language for the current logged in user.
    """

    if not useruuid:
        raise AttributeError("<useruuid> is empty")

    if not language:
        raise AttributeError("<language> is not set")

    pool = GetUserPool()
    if not pool.ChangeLanguage(useruuid, language):
        raise AttributeError("Could not change language")

    return
