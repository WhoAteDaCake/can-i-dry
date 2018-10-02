FROM ocaml/opam2

RUN sudo apt-get update && \
  sudo apt-get install -y --no-install-recommends apt-utils pkg-config libgnutls28-dev nettle-dev  && \
  sudo apt-get install m4 -y

RUN opam-sandbox-enable && \
  opam update && \
  opam switch create 4.06.1 && \
  eval $(opam config env) && \
  opam install reason core dune cohttp cohttp-lwt-unix lwt ezjsonm conf-gnutls ocamlnet

WORKDIR /worker
ADD . /worker/

USER root

RUN eval $(opam config env) && \
  cd /worker/server && \
  dune build rserver.exe && \
  mv ../_build/default/server/rserver.exe ../ && \
  cd ../

ENTRYPOINT [ "./rserver.exe" ]