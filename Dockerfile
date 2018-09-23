FROM ocaml/opam2

RUN sudo apt-get update && \
  sudo apt-get install -y --no-install-recommends apt-utils && \
  sudo apt-get install m4 -y

RUN opam-sandbox-enable && \
  opam update && \
  opam switch create 4.06.1 && \
  eval $(opam config env) && \
  opam install opium core redis-lwt dune reason

WORKDIR /worker
ADD . /worker/

USER root

RUN eval $(opam config env) && \
  cd /worker && \
  cd src && \
  dune build rserver.exe

ENTRYPOINT [ "echo" ]