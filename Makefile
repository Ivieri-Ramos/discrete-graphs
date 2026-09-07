ARGS = $(filter-out $@,$(MAKECMDGOALS))

run-build:
	mkdir -p output
	chmod 777 output
	docker compose build

run-app:
	docker compose run --rm -v "./output:/app/output" app main.py $(ARGS)

%:
	@: