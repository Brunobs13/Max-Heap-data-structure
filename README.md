# Max Heap Engine Platform

A production-ready transformation of an academic Max Heap implementation into a modular C++ engine, API service, and interactive web application.

## Project Overview
This repository provides a complete Max Heap platform with:
- A robust C++ heap core
- A C-compatible API bridge for integrations
- A FastAPI service exposing operational endpoints
- An interactive web dashboard to inspect and control heap behavior in real time
- CI, Docker, tests, and professional repository hygiene

## Business Problem
Academic projects often demonstrate algorithms but miss production qualities such as modularity, observability, secure configuration, and deployment readiness. This project solves that gap by turning a simple data-structure assignment into a business-grade technical artifact suitable for interviews and portfolio reviews.

## Architecture Diagram (Text)
```
[Interactive Web Dashboard]
          |
          v
     [FastAPI Service]
          |
          v
 [Python ctypes Integration Layer]
          |
          v
 [C API Wrapper - Shared Library]
          |
          v
      [C++ MaxHeap Core]
```

## Tech Stack
- C++17 (core data structure engine)
- C ABI wrapper (`ctypes` integration)
- Python 3.11 + FastAPI + Uvicorn
- HTML/CSS/JavaScript dashboard
- CMake + CTest
- Docker + Docker Compose
- GitHub Actions CI

## Project Structure
```
.
├── src/
│   ├── core/
│   │   ├── include/
│   │   │   ├── max_heap.hpp
│   │   │   └── max_heap_c_api.h
│   │   ├── max_heap.cpp
│   │   └── c_api.cpp
│   ├── apps/
│   │   └── heap_cli.cpp
│   └── api/
│       ├── engine.py
│       └── app.py
├── tests/
│   └── cpp/
│       └── test_max_heap.cpp
├── web/
│   ├── index.html
│   ├── styles.css
│   └── app.js
├── configs/
├── docs/
├── scripts/
├── legacy/
├── CMakeLists.txt
├── Dockerfile
├── docker-compose.yml
└── Makefile
```

## Setup Instructions (Step-by-Step)
1. Clone the repository:
```bash
git clone https://github.com/Brunobs13/Max-Heap-data-structure.git
cd Max-Heap-data-structure
```

2. Build the C++ engine and tools:
```bash
./scripts/build.sh
```

3. Run tests:
```bash
./scripts/test.sh
```

4. Install API dependencies:
```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

5. Run API + dashboard:
```bash
./scripts/run_api.sh
```

6. Open the app:
- Dashboard: `http://localhost:8080`
- Health: `http://localhost:8080/health`
- State API: `http://localhost:8080/api/heap/state`

## CI/CD Overview
The workflow in `.github/workflows/ci.yml` performs:
1. Dependency setup (Python + CMake + g++)
2. C++ build
3. C++ test execution via CTest
4. API import validation

This guarantees merge quality gates and reproducible builds.

## Data Versioning Strategy
This project is algorithmic and does not currently rely on external datasets. If telemetry data is introduced, the recommended approach is:
- DVC for versioning large datasets
- Immutable snapshot tags for performance benchmarks
- Dataset lineage metadata under `docs/`

## Model Tracking Strategy
No ML model training is currently present. If predictive tuning (e.g., workload-aware heap strategies) is added:
- Track experiments with MLflow
- Version benchmark datasets with DVC
- Store model registry metadata for reproducibility

## Deployment Strategy
### Local Docker Deployment
```bash
docker compose up --build
```

### Cloud Deployment
Deploy container to Render/Fly.io/Railway with:
- Port `8080`
- Environment variables from `.env.example`
- Health endpoint `/health`

## Security Considerations
- No hardcoded credentials in source files
- Environment-based runtime configuration (`.env.example`)
- Legacy academic files isolated under `legacy/`
- Professional `.gitignore` blocks common secret/artifact leaks

## Lessons Learned
- Wrapping C++ with a C ABI enables safe language interoperability
- Clear module boundaries make testing and maintenance easier
- Dashboard visibility improves understanding of internal heap behavior during debugging/interviews

## Future Improvements
- Multi-heap session support with persistent storage
- WebSocket streaming for near real-time updates
- Prometheus/OpenTelemetry instrumentation
- Role-based access for admin actions
- Benchmark harness for large-scale heap operations
