# root/pymulti_network/tests.py
import mpi4py

def is_mpi4py_v2_or_newer(ver: str) -> bool:
    # Prefer packaging.version if installed
    try:
        from packaging.version import Version
        return Version(ver) > Version("1.3.1")
    except Exception:
        # Fallback: tuple-compare numbers (good enough for mpi4py x.y.z)
        parts = []
        for p in ver.split("."):
            try:
                parts.append(int(p))
            except ValueError:
                # strip things like 'rc1' crudely
                num = ""
                for ch in p:
                    if ch.isdigit():
                        num += ch
                    else:
                        break
                parts.append(int(num) if num else 0)
        return tuple(parts) > (1, 3, 1)

is_v2 = is_mpi4py_v2_or_newer(mpi4py.__version__)

with open("pyconfig.pxi", "w") as f:
    f.write(f"DEF MPI4V2 = {int(is_v2)}\n")