/* open Uq_engines; */
/* Netsmtp.Debug.enable := true;

   let build_addr = () =>
     `Socket((
       `Sock_inet_byname((Unix.SOCK_STREAM, "localhost", 25)),
       Uq_client.default_connect_options,
     ));

     let tls = Netsys_crypto.current_tls();;
   let tc = Netsys_tls.create_x509_config ~trust:[`PEM_file "/etc/ssl/certs/ca-certificates.crt" ] ~peer_auth:`None tls;;
   let c  = new Netsmtp.connect addr 300.0;;
   c#helo();;
   c#starttls tc;;
   c # auth (module Netmech_digestmd5_sasl.DIGEST_MD5) "gerd" "" [ "password", "secret", [] ] [ "digest-uri", "smtp/smtp", true];;


   Netsmtp.authenticate ~tls_config:tc ~sasl_mechs:[ (module Netmech_digestmd5_sasl.DIGEST_MD5); (module Netmech_crammd5_sasl.CRAM_MD5) ] ~user:"gerd" ~creds:["password", "secret", []] c ;;
    */