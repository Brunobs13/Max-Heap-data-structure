from __future__ import annotations

import ctypes
import os
import threading
from pathlib import Path
from typing import Any


class HeapEngineError(RuntimeError):
    pass


class MaxHeapEngine:
    _status_messages = {
        0: "ok",
        1: "null handle or pointer",
        2: "heap full",
        3: "heap empty",
        4: "invalid input",
        5: "invalid capacity",
    }

    def __init__(self, initial_capacity: int = 32, library_path: str | None = None) -> None:
        self._lock = threading.RLock()
        self._lib_path = self._resolve_library_path(library_path)
        self._lib = ctypes.CDLL(str(self._lib_path))
        self._configure_bindings()

        self._handle = self._lib.mh_create(initial_capacity)
        if not self._handle:
            raise HeapEngineError("failed to initialize max heap handle")

    def _resolve_library_path(self, provided_path: str | None) -> Path:
        repo_root = Path(__file__).resolve().parents[2]
        candidates: list[Path] = []

        env_path = os.getenv("MAX_HEAP_LIB_PATH")
        if provided_path:
            candidates.append(Path(provided_path).expanduser())
        if env_path:
            candidates.append(Path(env_path).expanduser())

        for lib_name in ("libmaxheap.dylib", "libmaxheap.so", "maxheap.dll"):
            candidates.append(repo_root / "build" / lib_name)
            candidates.append(repo_root / "build" / "Release" / lib_name)

        for candidate in candidates:
            if candidate.exists():
                return candidate

        searched = "\n".join(str(path) for path in candidates)
        raise HeapEngineError(
            "maxheap shared library not found. build the project first with scripts/build.sh.\n"
            f"searched paths:\n{searched}"
        )

    def _configure_bindings(self) -> None:
        self._lib.mh_create.argtypes = [ctypes.c_size_t]
        self._lib.mh_create.restype = ctypes.c_void_p

        self._lib.mh_destroy.argtypes = [ctypes.c_void_p]
        self._lib.mh_destroy.restype = None

        self._lib.mh_insert.argtypes = [ctypes.c_void_p, ctypes.c_int]
        self._lib.mh_insert.restype = ctypes.c_int

        self._lib.mh_delete_max.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
        self._lib.mh_delete_max.restype = ctypes.c_int

        self._lib.mh_peek.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
        self._lib.mh_peek.restype = ctypes.c_int

        self._lib.mh_size.argtypes = [ctypes.c_void_p]
        self._lib.mh_size.restype = ctypes.c_size_t

        self._lib.mh_capacity.argtypes = [ctypes.c_void_p]
        self._lib.mh_capacity.restype = ctypes.c_size_t

        self._lib.mh_resize.argtypes = [ctypes.c_void_p, ctypes.c_size_t]
        self._lib.mh_resize.restype = ctypes.c_int

        self._lib.mh_clear.argtypes = [ctypes.c_void_p]
        self._lib.mh_clear.restype = None

        self._lib.mh_build.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int), ctypes.c_size_t]
        self._lib.mh_build.restype = ctypes.c_int

        self._lib.mh_export.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int), ctypes.c_size_t]
        self._lib.mh_export.restype = ctypes.c_size_t

    def _check_status(self, status: int) -> None:
        if status == 0:
            return

        message = self._status_messages.get(status, "unknown status")
        raise HeapEngineError(message)

    def insert_many(self, values: list[int]) -> dict[str, Any]:
        inserted = 0
        rejected: list[int] = []

        with self._lock:
            for value in values:
                status = self._lib.mh_insert(self._handle, int(value))
                if status == 0:
                    inserted += 1
                elif status == 2:
                    rejected.append(value)
                else:
                    self._check_status(status)

        return {"inserted": inserted, "rejected": rejected}

    def delete_max(self) -> int:
        with self._lock:
            out_value = ctypes.c_int()
            status = self._lib.mh_delete_max(self._handle, ctypes.byref(out_value))
            self._check_status(status)
            return int(out_value.value)

    def resize(self, new_capacity: int) -> None:
        with self._lock:
            status = self._lib.mh_resize(self._handle, int(new_capacity))
            self._check_status(status)

    def clear(self) -> None:
        with self._lock:
            self._lib.mh_clear(self._handle)

    def build(self, values: list[int]) -> None:
        with self._lock:
            if not values:
                self._lib.mh_clear(self._handle)
                return

            array_type = ctypes.c_int * len(values)
            payload = array_type(*[int(item) for item in values])
            status = self._lib.mh_build(self._handle, payload, len(values))
            self._check_status(status)

    def state(self) -> dict[str, Any]:
        with self._lock:
            size = int(self._lib.mh_size(self._handle))
            capacity = int(self._lib.mh_capacity(self._handle))

            values: list[int] = []
            if size > 0:
                buffer_type = ctypes.c_int * size
                buffer = buffer_type()
                copied = int(self._lib.mh_export(self._handle, buffer, size))
                values = [int(buffer[i]) for i in range(copied)]

            top = None
            if size > 0:
                out_value = ctypes.c_int()
                status = self._lib.mh_peek(self._handle, ctypes.byref(out_value))
                if status == 0:
                    top = int(out_value.value)

        return {
            "size": size,
            "capacity": capacity,
            "max": top,
            "array": values,
            "levels": self._to_levels(values),
        }

    @staticmethod
    def _to_levels(values: list[int]) -> list[list[int]]:
        levels: list[list[int]] = []
        index = 0
        level = 0

        while index < len(values):
            level_size = 1 << level
            levels.append(values[index : index + level_size])
            index += level_size
            level += 1

        return levels

    def close(self) -> None:
        with self._lock:
            if self._handle:
                self._lib.mh_destroy(self._handle)
                self._handle = None

    def __del__(self) -> None:
        try:
            self.close()
        except Exception:
            pass
