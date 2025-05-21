#!/bin/bash
set -e


#!/bin/bash
set -e

PLUGIN=$(pwd)/node_modules/.bin/protoc-gen-ts_proto

mkdir -p src/generated

npx protoc \
  --plugin=protoc-gen-ts_proto=$PLUGIN \
  --ts_proto_out=src/generated \
  --ts_proto_opt=esModuleInterop=true,outputServices=none,outputEncodeMethods=true \
  -I ../proto \
  ../proto/keyevent.proto

echo "✅ Protobuf TypeScript generated!"
