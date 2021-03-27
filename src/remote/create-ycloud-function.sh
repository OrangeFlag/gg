#!/bin/bash -e

FUNCTION_NAME=gg-ycloud-function
FUNCTION_PATH=ycloud_function
MEMORY=128m
TIMEOUT=300s

cp common/{common,ggpaths}.py ${FUNCTION_PATH}
cp "$(which gg-execute-static)" ${FUNCTION_PATH}

zip ${FUNCTION_PATH}/source.zip ${FUNCTION_PATH}/*

yc serverless function create --name=${FUNCTION_NAME}
yc serverless function version create --function-name=${FUNCTION_NAME} --runtime python37 --entrypoint main.handler \
  --memory ${MEMORY} --execution-timeout ${TIMEOUT} --source-path ${FUNCTION_PATH}/source.zip

rm ${FUNCTION_PATH}/{common,ggpaths}.py
