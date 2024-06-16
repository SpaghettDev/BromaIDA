from subprocess import (
    STARTUPINFO, call,
    DEVNULL, STDOUT, STARTF_USESHOWWINDOW,
    CalledProcessError
)

g_has_pybroma: bool = False


def install_pybroma() -> bool:
    """Installs PyBroma if not already installed.
    Not in utils to avoid cyclic dependencies."""
    global g_has_pybroma
    if g_has_pybroma:
        return False

    try:
        si = STARTUPINFO()
        si.dwFlags |= STARTF_USESHOWWINDOW
        ret = call([
            "python", "-m", "pip", "show", "pybroma"
        ], startupinfo=si, stdout=DEVNULL, stderr=STDOUT)

        if ret != 0:
            raise CalledProcessError(ret, "")

        g_has_pybroma = True

        print("[+] BromaIDA: PyBroma found")

        return True
    except CalledProcessError:
        try:
            # print("[!] BromaIDA: PyBroma not installed! Installing...")

            # si = STARTUPINFO()
            # si.dwFlags |= STARTF_USESHOWWINDOW
            # ret = call([
            #     "python", "-m", "pip", "install",
            #     "https://github.com/CallocGD/PyBroma/archive/refs/heads/main.zip"
            # ], startupinfo=si, stdout=DEVNULL, stderr=STDOUT)

            # if ret != 0:
            #     raise CalledProcessError(ret, "")

            # g_has_pybroma = True

            # print("[+] BromaIDA: PyBroma successfully installed")

            # return ret
            print("[!] BromaIDA: PyBroma not installed! Please install it!")
            return False
        except CalledProcessError:
            print("[!] BromaIDA: Couldn't install PyBroma!")

            return False
