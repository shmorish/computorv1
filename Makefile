all: build

build:
	cargo build --release

run:
	cargo run

clean:
	cargo clean

test:
	cargo test

re: clean build