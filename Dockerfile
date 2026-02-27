FROM python:3.11-slim AS build
WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY . .
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --config Release

FROM python:3.11-slim AS runtime
WORKDIR /app

COPY --from=build /usr/local /usr/local
COPY --from=build /app/src /app/src
COPY --from=build /app/web /app/web
COPY --from=build /app/build /app/build
COPY --from=build /app/requirements.txt /app/requirements.txt

ENV MAX_HEAP_LIB_PATH=/app/build/libmaxheap.so
ENV MAX_HEAP_INITIAL_CAPACITY=32
ENV LOG_LEVEL=INFO

EXPOSE 8080
CMD ["uvicorn", "src.api.app:app", "--host", "0.0.0.0", "--port", "8080"]
