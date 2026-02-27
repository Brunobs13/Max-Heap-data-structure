.PHONY: build test run-cli run-api docker-up docker-down

build:
	./scripts/build.sh

test:
	./scripts/test.sh

run-cli:
	./scripts/run_cli.sh

run-api:
	./scripts/run_api.sh

docker-up:
	docker compose up --build

docker-down:
	docker compose down
