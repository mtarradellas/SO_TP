FROM debian:jessie

RUN apt-get update && apt-get install -y python-pip && pip install cpplint

CMD cd sources && cpplint --filter=-legal/copyright,-readability/casting,-build/header_guard,-runtime/int --exclude=src/Bootloader/BMFS/*.c --exclude=src/Kernel/charFont.c --exclude=src/Kernel/include/charFont.h --recursive src/ 