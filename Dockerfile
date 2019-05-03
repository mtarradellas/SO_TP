FROM debian:jessie

RUN apt-get update && apt-get install -y python-pip && pip install cpplint

CMD cd sources && cpplint --recursive src/