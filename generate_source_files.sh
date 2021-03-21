# Generate the embedded source code.
mkdir -p ./nucleo-f767zi/generated

cd EmbeddedProto
protoc --plugin=protoc-gen-eams=protoc-gen-eams -I../proto --eams_out=../nucleo-f767zi/generated ../proto/eth_messages.proto
cd -

# Generate the desktop source code.
mkdir -p ./desktop/generated
protoc -I./proto --python_out=./desktop/generated ./proto/eth_messages.proto