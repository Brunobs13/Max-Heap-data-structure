from __future__ import annotations

import logging
import os
from datetime import datetime, timezone
from pathlib import Path
from typing import Any

from fastapi import FastAPI, HTTPException
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel, Field

from src.api.engine import HeapEngineError, MaxHeapEngine


class InsertRequest(BaseModel):
    values: list[int] = Field(..., min_length=1, max_length=10_000)


class BuildRequest(BaseModel):
    values: list[int] = Field(default_factory=list, max_length=10_000)


class ResizeRequest(BaseModel):
    capacity: int = Field(..., ge=1, le=1_000_000)


def utc_now() -> str:
    return datetime.now(tz=timezone.utc).isoformat()


logging.basicConfig(
    level=os.getenv("LOG_LEVEL", "INFO"),
    format="%(asctime)s %(levelname)s %(name)s %(message)s",
)
logger = logging.getLogger("maxheap.api")

initial_capacity = int(os.getenv("MAX_HEAP_INITIAL_CAPACITY", "32"))
engine = MaxHeapEngine(initial_capacity=initial_capacity)
metrics: dict[str, Any] = {
    "started_at": utc_now(),
    "requests_total": 0,
    "insert_ops": 0,
    "delete_ops": 0,
    "build_ops": 0,
    "resize_ops": 0,
    "clear_ops": 0,
    "errors_total": 0,
}

app = FastAPI(title="Max Heap Engine API", version="1.0.0")

REPO_ROOT = Path(__file__).resolve().parents[2]
WEB_ROOT = REPO_ROOT / "web"

if WEB_ROOT.exists():
    app.mount("/assets", StaticFiles(directory=WEB_ROOT), name="assets")


@app.get("/health")
def health() -> dict[str, Any]:
    metrics["requests_total"] += 1
    return {
        "status": "ok",
        "service": "max-heap-engine",
        "timestamp_utc": utc_now(),
        "library": str(engine._lib_path),
    }


@app.get("/api/heap/state")
def heap_state() -> dict[str, Any]:
    metrics["requests_total"] += 1
    return engine.state()


@app.post("/api/heap/insert")
def heap_insert(payload: InsertRequest) -> dict[str, Any]:
    metrics["requests_total"] += 1
    metrics["insert_ops"] += 1

    try:
        result = engine.insert_many(payload.values)
    except HeapEngineError as exc:
        metrics["errors_total"] += 1
        raise HTTPException(status_code=400, detail=str(exc)) from exc

    return {
        "message": f"inserted {result['inserted']} values",
        "result": result,
        "state": engine.state(),
    }


@app.post("/api/heap/delete-max")
def heap_delete_max() -> dict[str, Any]:
    metrics["requests_total"] += 1
    metrics["delete_ops"] += 1

    try:
        value = engine.delete_max()
    except HeapEngineError as exc:
        metrics["errors_total"] += 1
        raise HTTPException(status_code=400, detail=str(exc)) from exc

    return {
        "message": "max element removed",
        "removed": value,
        "state": engine.state(),
    }


@app.post("/api/heap/build")
def heap_build(payload: BuildRequest) -> dict[str, Any]:
    metrics["requests_total"] += 1
    metrics["build_ops"] += 1

    try:
        engine.build(payload.values)
    except HeapEngineError as exc:
        metrics["errors_total"] += 1
        raise HTTPException(status_code=400, detail=str(exc)) from exc

    return {
        "message": "heap rebuilt",
        "state": engine.state(),
    }


@app.post("/api/heap/resize")
def heap_resize(payload: ResizeRequest) -> dict[str, Any]:
    metrics["requests_total"] += 1
    metrics["resize_ops"] += 1

    try:
        engine.resize(payload.capacity)
    except HeapEngineError as exc:
        metrics["errors_total"] += 1
        raise HTTPException(status_code=400, detail=str(exc)) from exc

    return {
        "message": f"capacity updated to {payload.capacity}",
        "state": engine.state(),
    }


@app.post("/api/heap/clear")
def heap_clear() -> dict[str, Any]:
    metrics["requests_total"] += 1
    metrics["clear_ops"] += 1
    engine.clear()
    return {
        "message": "heap cleared",
        "state": engine.state(),
    }


@app.get("/api/metrics")
def api_metrics() -> dict[str, Any]:
    metrics["requests_total"] += 1
    snapshot = dict(metrics)
    snapshot["timestamp_utc"] = utc_now()
    snapshot["heap_state"] = engine.state()
    return snapshot


@app.get("/")
def web_root() -> FileResponse:
    index_path = WEB_ROOT / "index.html"
    if not index_path.exists():
        raise HTTPException(status_code=404, detail="web dashboard not available")
    return FileResponse(index_path)
