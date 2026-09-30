#!/usr/bin/env bash

VBoxManage createhd \
    --filename ./build/img/llpc.vdi \
    --size 64 \
    --format VDI

